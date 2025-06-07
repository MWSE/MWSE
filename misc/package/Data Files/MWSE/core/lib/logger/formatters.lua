
---@type table<string, mwseLogger.formatter>
local formatters = {}

local fmt = string.format

local inspect = require("inspect")
local inspect_METATABLE = inspect.METATABLE
---@diagnostic disable-next-line: cast-local-type
inspect = inspect.inspect

-- Thank you G7.
local INSPECT_PARAMS = {
	newline = ' ',
	indent = '',
	process = function(item, path)
		if path[#path] == inspect_METATABLE then
			-- ignore metatables
			return
		end


		local ty, subtype = type(item)

		-- Check if it's a `table` or `userdata` with a `__tostring` metamethod
		if ty == "table" or ty == "userdata" then

			-- sol types have this magic property we can (ab)use
			if subtype then
				return fmt('%s("%s")', subtype, item)
			end

			-- Some things incorrectly define the `__tostring` method on the object instead of its metatable.
			-- But we'll play nice and support them anyway.
			local tostr = item.__tostring
			if not tostr then
				---@type metatable|nil
				local meta = getmetatable(item)

				tostr = meta and meta.__tostring
			end

			if tostr then
				-- sometimes people define their `__tostring` metamethods in a way that causes errors.
				local status, str = pcall(tostr, item)

				if status then
					return str
				end
			end
		end

		return item
	end
}

--- Recursively iterates through the `varargs` and replaces any `table`s and `userdata`s with
--- 		a prettyprinted version.
--- Other types of arguments are left unchanged.
--- This function is also written so that it performs tail recursion.
--- Credit to G7 for the idea.
---@param val any The argument currently being processed.
---@param ... any Arguments left to process.
---@return string|any ... The processed arguments.
local function prettyProcess(val, ...)
	if type(val) == "table" or type(val) == "userdata" then
		val = inspect(val, INSPECT_PARAMS)
	end
	if select("#", ...) > 0 then
		-- Only issue a recursive call if there are more arguments to process.
		-- Note that it is not sufficient to check if `...` is `nil`.
		return val, prettyProcess(...)
	else
		return val
	end
end

do -- Define the DEFAULT formatter



	--- Expands the parameters, and calls `string.format` if more than one argument was given.
	--- If `string.format` triggered an error (because invalid formatting parameters were passed),
	--- Then this function will generate a more helpful error message and display that to the user.
	local function expandAndFormat(...)
		-- If there are fewer than 2 arguments, then no string formatting is necessary.
		if select("#", ...) <= 1 then
			return (prettyProcess(...))
		end
		--Otherwise, we will try to format the string and intercept the error message on failure.
		--This is so that we can replace the error message with a more helpful one.
		--Note that `pcall(fmt, prettyProcess(...))` is basically the same thing as
		--```lua
		--pcall(function()
		--	return fmt(prettyProcess(...))
		--end)
		--```
		local statusCode, msg = pcall(fmt, prettyProcess(...))
		if statusCode then
			return msg
		end

		-- From this point onwards, we know that an error was triggered, and our job is to
		-- display a more helpful error message.

		-- The parameter that triggered the formatting error.
		local argNum = msg:match("^bad argument #(%d+)")
		argNum = tonumber(argNum)
		-- The final error message that will be returned.

		if not argNum then
			-- Something weird happened and we couldn't parse the `string.format` error message.
			-- So just use the provided error message.
			error(
				fmt("Invalid formatting argument passed to string.format:\n\t%s", msg),
				5 -- Make the error message point to the code that caused the error.
			)
		end

		-- We can find out exactly which argument caused the error.

		-- Number of arguments that were provided, after evaluating functions.
		local numProvided = select("#", ...)

		-- The type of error message. This is currently one of two things.
		local errType
		if argNum > numProvided then
			errType = fmt("Expected %i arguments, got %i.", argNum, numProvided)
		else
			errType = fmt("Argument #%i could not be formatted as specified.", argNum)
		end

		-- The prettyprinted list of arguments, that will be displayed in the form
		--		1) format_str
		--		2) param1
		--		3) param2
		--		...
		local argList = table.new(numProvided, 0)

		for i = 1, numProvided do
			local val = select(i, ...)
			-- `inspect` will wrap strings in quotes and convert `nil` to the
			-- proper string representation.
			-- This results in some wasted computation, but this is only done
			-- in the unlikely scenario where a logging message was improperly formatted.
			argList[i] = fmt("\t\t%i) %s", i, prettyProcess(val))
		end

		error(
			fmt(
				"Invalid formatting arguments were passed to string.format!\n\z
					\t%s\n\z
					\tArguments provided:\n\z
						%s\z
				",
				errType,
				table.concat(argList, "\n")
			),
			5 -- Make the error message point to the code that caused the error.
		)
	end

	--- The default formatter. This is used by all newly constructed loggers, unless they choose to use something else.
	formatters.DEFAULT = function(self, record, ...)

		---@diagnostic disable-next-line: invisible
		local header = self:makeHeader(record)

		local first, second = ...

		if type(first) == "function" then
			-- If the first argument is a function, call it and pass in the remaining parameters
			return header .. expandAndFormat(first(select(2, ...)))
		elseif type(second) == "function" then
			-- If the second argument is a function, then assume that the first parameter is the
			-- string that should be formatted.
			return header .. expandAndFormat(first, second(select(3, ...)))
		else
			-- Otherwise, don't process any functions.
			return header .. expandAndFormat(...)
		end
	end
end


do -- Define the `expandAllFunctions` formatter

	-- This is necessary because `{...}` will flatten out any `nil`s,
	-- which would lead to very confusing errors if it caused
	-- `string.format` to get fewer arguments than it was expecting.
	local function addArgs(fmtArgs, ...)
		for i = 1, select("#", ...) do
			local v = select(i, ...)
			if type(v) == "table" or type(v) == "userdata" then
				table.insert(fmtArgs, prettyProcess(v))
			elseif v == nil then
				table.insert(fmtArgs, tostring(v))
			else
				table.insert(fmtArgs, v)
			end
		end
	end

	---@param self mwseLogger
	---@param record mwseLoggerRecord
	formatters.expandAllFunctions = function (self, record, ...)
		local fmtArgs = {}




		local i, n = 1, select("#", ...)
		--[[Format each of the arguments.
			- Functions: will be called using the appropriate number of arguments.
				- E.g., if `f` is defined to accept exactly two arguments, then
				`log:debug(msg, f, a, b, c)` will reduce to `log:debug(msg, f(a,b), c)`,
				with `f(a,b)` being computed ONLY if the logging level is appropriate.
			- Tables: will be passed to `json.encode`, unless they have a `tostring` metamethod.
			- Everything else: will be sent to `tostring`.
		]]
		while i <= n do
			local v = select(i, ...)
			local vType = type(v)
			if vType == "function" then
				if i < n then
					-- adds in every argument after `i`
					addArgs(fmtArgs, v(select(i + 1, ...)))
				else
					addArgs(fmtArgs, v())
				end
				-- Find out how many arguments `v` actually wanted, then skip ahead that many arguments.
				local info = debug.getinfo(v, "u")
				if info.isvararg then break end
				-- Note that at this point, `i` stores the index of the last argument that was passed to `v`
				-- So, we still need to increment it once more at the end of this loop.
				i = i + info.nparams
			elseif vType == "table" or vType == "userdata" then
				table.insert(fmtArgs, prettyProcess(v))
			-- `nil` will mess up the table packing and unpacking, so we treat it specially
			elseif v == nil then
				table.insert(fmtArgs, "nil")
			else
				table.insert(fmtArgs, v)
			end
			i = i + 1
		end
		-- Create the return string.
		local str
		-- Only call `string.format` if there's more than one argument.
		-- This helps to avoid errors caused by users writing strings that they don't
		-- expect will be formatted.
		-- e.g., `log:debug("progress: 50%")`
		if #fmtArgs > 1 then
			str = fmt(table.unpack(fmtArgs))
		else
			str = fmtArgs[1]
		end

		---@diagnostic disable-next-line: invisible
		local header = self:makeHeader(record)

		return header .. str
	end
end

return formatters