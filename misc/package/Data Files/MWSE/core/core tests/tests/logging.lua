--- @diagnostic disable: undefined-field
--- @diagnostic disable: param-type-mismatch
--- @diagnostic disable: undefined-global

local ansicolors = require("logging.colors")
local logger = require("logging.logger")
local UnitWind = require("unitwind")


local testSuite = UnitWind.new({
	enabled = true,
	highlight = true,
	exitAfter = true,
	-- To be able to mock print, we need to reroute UnitWind's output file.
	outputFile = "logging test.log"
})

testSuite:start("Testing logging API")

local loggerName = "Test"
local log = logger.new({ name = loggerName })

testSuite:test("Test logger.new", function()
	testSuite:expect(log.name).toBe(loggerName)
	testSuite:expect(log.logLevel).toBe("INFO")
end)

testSuite:failTest("Test logger.new name parameter error checking", function()
	logger.new({ name = 1 }) --- @diagnostic disable-line: assign-type-mismatch
end, "[Logger] No name provided.")

-- Test backwards compatibility:
-- * logger.getLogger
-- * logger:doLog
-- * logger:setLogLevel
-- * logger.logLevel reading

--- @diagnostic disable: deprecated
testSuite:test("Test logger.getLogger", function()
	local log = logger.getLogger(loggerName)

	testSuite:expect(log).NOT.toBe(false)
	--- @cast log mwseLogger
	testSuite:expect(log.name).toBe(loggerName)
end)

testSuite:test("Test logger:doLog", function()
	testSuite:expect(log:doLog("TRACE")).toBe(false)
	testSuite:expect(log:doLog("DEBUG")).toBe(false)
	testSuite:expect(log:doLog("INFO")).toBe(true)
	testSuite:expect(log:doLog("WARN")).toBe(true)
	testSuite:expect(log:doLog("ERROR")).toBe(true)
	testSuite:expect(log:doLog("NONE")).toBe(true)
end)

testSuite:test("Test logger:setLogLevel", function()
	log:setLogLevel("TRACE")
	testSuite:expect(log:doLog("TRACE")).toBe(true)
	testSuite:expect(log:doLog("DEBUG")).toBe(true)
	testSuite:expect(log:doLog("INFO")).toBe(true)
	testSuite:expect(log:doLog("WARN")).toBe(true)
	testSuite:expect(log:doLog("ERROR")).toBe(true)
	testSuite:expect(log:doLog("NONE")).toBe(true)

	log:setLogLevel("NONE")
	testSuite:expect(log:doLog("TRACE")).toBe(false)
	testSuite:expect(log:doLog("DEBUG")).toBe(false)
	testSuite:expect(log:doLog("INFO")).toBe(false)
	testSuite:expect(log:doLog("WARN")).toBe(false)
	testSuite:expect(log:doLog("ERROR")).toBe(false)
	testSuite:expect(log:doLog("NONE")).toBe(true)
end)

testSuite:test("Test reading logger.logLevel", function()
	log:setLevel("INFO")
	testSuite:expect(log.logLevel).toBe("INFO")

	log:setLevel("DEBUG")
	testSuite:expect(log.logLevel).toBe("DEBUG")
end)
--- @diagnostic enable: deprecated

testSuite:test("Test logger:setOutputFile", function()
	testSuite:expect(log.outputFile).toBe(nil)
	testSuite:spy(io, "open")

	local newOutputFile = "Data Files/MWSE/logs/test.log"
	log:setOutputFile(newOutputFile)
	testSuite:expect(type(log.outputFile)).toBe("userdata")
	testSuite:expect(io.open).toBeCalled()
	testSuite:expect(io.open).toBeCalledWith({ newOutputFile, "w" })
	testSuite:unspy(io, "open")

	log:setOutputFile("mwse.log")
	testSuite:expect(log.outputFile).toBe(nil)
end)


--- @param level mwseLogger.logLevel
--- @param color string
--- @param ... any?
local function defaultFormatter(level, color, ...)
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
		local a = select(i, ...)
		local aType = type(a)
		if aType == "function" then
			local s = i + 1
			local rets = (s <= n) and {a(select(s, ...))} or {a()}
			--- NOTE: return values are NOT pretty printed
			for _, v in ipairs(rets) do
				table.insert(fmtArgs, v)
			end

			local info = debug.getinfo(a, "u")
			if info.isvararg then break end
			i = i + info.nparams
		elseif type(a) == "table" or type(a) == "userdata" then
			table.insert(fmtArgs, inspect(a, INSPECT_PARAMS))
		else
			table.insert(fmtArgs, tostring(a))
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
		str = string.format(table.unpack(fmtArgs))
	else
		str = fmtArgs[1]
	end

	local header
	if  mwse.getConfig("EnableLogColors") then
		level = ansicolors(string.format("%%{%s}%s", color, level))
	end
	return string.format("[%s | %s | %s] %s", log.name, log.filePath, level, str)
end


testSuite:test("Test logging", function()
	local buffer = ""
	testSuite:mock(_G, "print", function(...)
		buffer = table.concat({ ... }, " ")
	end)

	-- This function isn't yet defined during this test's execution.
	function tes3ui.log(str, ...) --- @diagnostic disable-line: duplicate-set-field
		tes3ui.logToConsole(tostring(str):format(...), false)
	end
	testSuite:spy(tes3ui, "log")

	local EnableLogColors = mwse.getConfig("EnableLogColors")
	mwseConfig["EnableLogColors"] = false

	local args = { "INFO", "white", "Foo %s", "Bar" }
	log:info(unpack(args, 3))
	testSuite:expect(buffer).toBe(defaultFormatter(unpack(args)))


	testSuite:test("Test EnableLogColors", function()
		mwseConfig["EnableLogColors"] = true
		args = { "WARN", "bright yellow", "Hello %s", "World" }
		log:warn(unpack(args, 3))

		-- testSuite:_rawLog("buffer = %q", buffer)
		-- testSuite:_rawLog("result = %q", defaultFormatter(unpack(args)))
		testSuite:expect(buffer).toBe(defaultFormatter(unpack(args)))
		mwseConfig["EnableLogColors"] = false
	end)

	testSuite:test("Test logger.logToConsole", function()
		testSuite:expect(log.logToConsole).toBe(false)
		log.logToConsole = true
		testSuite:expect(log.logToConsole).toBe(true)
		args = { "INFO", "white", "Apples %s", "Oranges" }
		log:info(unpack(args, 3))
		testSuite:expect(tes3ui.log).toBeCalled()
		testSuite:expect(tes3ui.log).toBeCalledWith(defaultFormatter(unpack(args)))

		log.logToConsole = false
		args = { "INFO", "white", "Bananas %s", "Pineapples" }
		log:info(unpack(args, 3))
		testSuite:expect(tes3ui.log).toBeCalledTimes(1)
	end)

	testSuite:test("Test logger.includeTimestamp", function()
		testSuite:expect(log.includeTimestamp).toBe(false)
		log.includeTimestamp = true
		testSuite:expect(log.includeTimestamp).toBe(true)

		local socket = require("socket")
		testSuite:spy("socket", "gettime")
		args = { "INFO", "white", "Fruit %s", "pie" }
		log:info(unpack(args, 3))
		testSuite:expect(socket.gettime).toBeCalled()
		log.includeTimestamp = false

		args = { "INFO", "white", "Pineapples" }
		log:info(unpack(args, 3))
		testSuite:expect(socket.gettime).toBeCalledTimes(1)
	end)

	testSuite:test("Test each logging method", function()
		mwseConfig["EnableLogColors"] = true
		log:setLevel(mwse.logLevel.trace)

		args = { "TRACE", "bright white", "Fruit %s", "pie" }
		log:trace(unpack(args, 3))
		testSuite:expect(buffer).toBe(defaultFormatter(unpack(args)))

		args = { "DEBUG", "bright green", "Apple %s", "pie" }
		log:debug(unpack(args, 3))
		testSuite:expect(buffer).toBe(defaultFormatter(unpack(args)))

		args = { "INFO", "white", "Fruit %s", "pie" }
		log:info(unpack(args, 3))
		testSuite:expect(buffer).toBe(defaultFormatter(unpack(args)))

		args = { "WARN", "bright yellow", "Apple %s", "pie" }
		log:warn(unpack(args, 3))
		testSuite:expect(buffer).toBe(defaultFormatter(unpack(args)))

		args = { "ERROR", "bright red", "Fruit %s", "pie" }
		log:error(unpack(args, 3))
		testSuite:expect(buffer).toBe(defaultFormatter(unpack(args)))
	end)

	testSuite:unmock(_G, "print")
	testSuite:unspy(tes3ui, "log")
	mwseConfig["EnableLogColors"] = EnableLogColors
end)

testSuite:failTest("Test logger:assert", function()
	testSuite:spy(log, "error")
	log:assert(false, "Assert failed.")
	testSuite:expect(log.error).toBeCalled()
	testSuite:unspy(log, "error")
end, defaultFormatter("ERROR", "bright red", "Assert failed."))

testSuite:finish()
