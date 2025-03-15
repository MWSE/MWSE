--[[ # Logger 

Based on the `mwseLogger` made by Merlord.

Current version made by herbert100.

Many suggestions and feature ideas given by C3pa.

Credit to Greatness7 as well, for the ideas of adding a `format` parameter and allowing functions to be passed to log messages.
]]

local socket = require("socket")

-- The time that the game was launched.
---@type number
local LAUNCH_TIME = socket.gettime()


local fmt = string.format




local LOG_LEVEL = {
	NONE  = 0,
	ERROR = 1,
	WARN  = 2,
	INFO  = 3,
	DEBUG = 4,
	TRACE = 5
}

--- This table takes in a log level string and spits out the corresponding numeric log level.
--- This is defined to be the inversion of `LOG_LEVEL`.
--- So, `LOG_LEVEL_STRINGS[number]` is equivalent to `table.find(LOG_LEVEL, number)`.
---@type table<mwseLogger.LOG_LEVEL, string>
local LOG_LEVEL_STRINGS = table.invert(LOG_LEVEL)

-- Note: \x1B is the escape character.

-- Tiny optimization: store the terminal escape sequences for each log level, so they don't 
-- have to be recomputed every log message.
---@type table<mwseLogger.LOG_LEVEL, string>
local LOG_LEVEL_COLOR_STRINGS = {
	-- color doesn't matter
	[LOG_LEVEL.NONE]  = "NONE",
	-- bright yellow
	[LOG_LEVEL.WARN]  = "\x1B[0m\x1B[1m\x1B[33mWARN\x1B[0m",
	-- bright red
	[LOG_LEVEL.ERROR] = "\x1B[0m\x1B[1m\x1B[31mERROR\x1B[0m",
	-- white
	[LOG_LEVEL.INFO]  = "\x1B[0m\x1B[37mINFO\x1B[0m",
	-- bright green
	[LOG_LEVEL.DEBUG] = "\x1B[0m\x1B[1m\x1B[32mDEBUG\x1B[0m",
	-- bright white
	[LOG_LEVEL.TRACE] = "\x1B[0m\x1B[1m\x1B[37mTRACE\x1B[0m",
}

-- Pre-colored versions of the abbreviated log level strings.
---@type table<mwseLogger.LOG_LEVEL, string>
local LOG_LEVEL_ABBREVIATED_COLOR_STRINGS = {
	-- color doesn't matter
	[LOG_LEVEL.NONE]  = "NONE",
	-- bright yellow
	[LOG_LEVEL.WARN]  = "\x1B[0m\x1B[1m\x1B[33mW\x1B[0m",
	-- bright red
	[LOG_LEVEL.ERROR] = "\x1B[0m\x1B[1m\x1B[31mE\x1B[0m",
	-- white
	[LOG_LEVEL.INFO]  = "\x1B[0m\x1B[37mI\x1B[0m",
	-- bright green
	[LOG_LEVEL.DEBUG] = "\x1B[0m\x1B[1m\x1B[32mD\x1B[0m",
	-- bright white
	[LOG_LEVEL.TRACE] = "\x1B[0m\x1B[1m\x1B[37mT\x1B[0m",
}

-- Define various things that will be moved into the official documtation system
-- once the API has been finalized.
do 

--- A function that handles the formatting of log messages, given the `logger`, a `record`, and the log function parameters.
--- The record holds information about the logging level and line number, and the rest of the parameters are the ones passed
--- as arguments to the `Logger:debug` methods.
--- This function should return a string, which will then be printed to the appropriate file.
--- This function should also include the header of the log message. The default header can be accessed by 
--- using the `makeHeader` method.
---@alias mwseLogger.formatter fun(self: mwseLogger, record: mwseLogger.Record, ...: string|any|fun(...): ...): string



---@alias mwseLogger.LOG_LEVEL
---|0					   NONE: Nothing will be printed
---|1					   ERROR: Error messages will be printed
---|2					   WARN: Warning messages will be printed
---|3					   INFO: Only crucial information will be printed
---|4					   DEBUG: Debug messages will be printed
---|5					   TRACE: Many debug messages will be printed
---|`mwseLogger.LOG_LEVEL.NONE`	 Nothing will be printed
---|`mwseLogger.LOG_LEVEL.ERROR`	Error messages will be printed
---|`mwseLogger.LOG_LEVEL.WARN`	 Warning messages will be printed
---|`mwseLogger.LOG_LEVEL.INFO`	 Crucial information will be printed
---|`mwseLogger.LOG_LEVEL.DEBUG`	Debug messages will be printed
---|`mwseLogger.LOG_LEVEL.TRACE`	Many debug messages will be printed


--- Stores all the mod-level information for a logger. 
--- This allows a mod to have several different loggers that are all sychronized with each other.
---@class mwseLogger.SharedData
---@field level mwseLogger.LOG_LEVEL The logging level for this logger
---@field logToConsole boolean
---@field formatter mwseLogger.formatter
---@field modName string name of the mod
---@field modDir string
---@field includeTimestamp boolean should the current time be printed when writing log messages? Default: `false`
---@field outputFile file*|nil The file the log is being written to, or `nil`.
---@field abbreviateHeader boolean Print a shorter header?


end

local LOG_FILE_PARENT_DIR = "Data Files/MWSE/logs"
local GET_MOD_INFO_MAX_ITERS = 15
local BAD_FILEPATHS = {
	[string.lower("@Data Files\\MWSE\\core\\initialize.lua")] = true,
	[string.lower("@Data Files\\MWSE\\core\\startLuaMods.lua")] = true,
	[string.lower("=[C]")] = true,
}

---Returns the `modName`, `modDir`, and `filepath` of the currently executing file. 
---@param offset integer? The stack offset to use when calling `debug.getinfo`. DEFAULT: `0`.
---@return string? modName
---@return string? modDir
---@return string? filepath
local function getModNameAndDirAndFilepath(offset)

	-- =========================================================================
	-- STEP 1: Get the filepath of the file that wants to construct a Logger.
	-- =========================================================================
	-- This is done by repeatedly calling `debug.getinfo`,
	-- 	with increasing `stackLevel`s, until we hit a "bad filepath".
	--	(i.e., until we hit the core functionality that's responsible for executing the mods.)
	-- The desired filepath will be the highest `stackLevel` that is not a "bad filepath".
	-- We don't pick the first valid filepath because we want to play nicely with 
	-- "Logger factories" that exist in some mods. (For example, Seph's library and 
	-- the "Skills Module" by Merlord.)

	---@type string
	local filepath, newFilePath

	local startingOffset = 1 + (offset or 0)
	local newDebugInfo

	-- max 10 iterations, but in reality we will only need to do at most like 4
	for i = startingOffset, startingOffset + GET_MOD_INFO_MAX_ITERS do
		newDebugInfo = debug.getinfo(i, "S")
		
		if not newDebugInfo then 
			-- we've gone too high up the stack
			break
		end
		newFilePath = newDebugInfo.source:lower()


		if BAD_FILEPATHS[newFilePath] then 
			-- we've gone too high up the stack
			break
		end

		filepath = newFilePath
	end
	if not filepath then return end


	-- =========================================================================
	-- STEP 2: Use the filepath to decipher the mods name and directory.
	-- =========================================================================
	--[[We need to successfully parse the path in order to extract
		the modName, modDir, and relativeFilePath.
		We also need to do this in a way that respects:
		1) The fact that some mods might have author folders.
			(e.g., "herbert100/more quickloot" instad of "more quickloot")
		2) Some mods might be given names via a corresponding `metadata.toml` file.
	]]


	-- Kill off the part of the path that contains "data files\\mwse\\"
	local _, mwseFolderEndIndex = filepath:find("data files\\mwse\\", 1, true)
	if mwseFolderEndIndex then
		filepath = filepath:sub(mwseFolderEndIndex + 1)
	end
	
	---@type string[]
	local pathParts = filepath:split("\\")

	-- The root directory. This will be one of "mods", "lib", or "core".
	local rootDir = table.remove(pathParts, 1)



	-- now we will initialize the modName and modDir
	-- we start off by assuming there is no mod author folder, and that the modName and modDir are the same.
	local modDir = pathParts[1]
	local modName = modDir

	
	-- If it's a mod, initialize it using the appropriate runtime.
	-- Also check if there's a mod author folder.
	-- By using runtimes, we can determine if a mod has an author folder or not.
	if rootDir == "mods" then
		--[[ If there's a mod author folder, then:
			- `pathParts[1]` is the mod author name
			- `pathParts[2]` is the `modName`
			- `pathParts[1].."."..pathParts[2]` is the `modDir`.
		Otherwise, pathParts[1]` is the `modName` and `modDir`
		]]
		local modDirWithAuthorName = pathParts[1].."."..pathParts[2]

		---@type {key: string, path: string, parent_path: string, legacy_mod: boolean, core_mod: boolean, metadata: MWSE.Metadata?}
		---@diagnostic disable-next-line: undefined-field
		local runtime = mwse.activeLuaMods[modDirWithAuthorName]

		
		if runtime then
			modDir = modDirWithAuthorName
			modName = pathParts[2]
		else -- No author directory.
			---@diagnostic disable-next-line: undefined-field
			runtime = mwse.activeLuaMods[pathParts[1]]
		end

		-- Unless we have made an error, the runtime will always exist.
		-- We'll use it to update the mod name using the mods metadata, if appropriate.
		if runtime then
			local pkg = runtime.metadata and runtime.metadata.package
			modName = pkg and pkg.name or modName
		else
			mwse.log(
				"[Logger | ERROR]: Could not find the runtime information for \"%s\" or \"%s\". \z
					This should not happen!", 
				pathParts[1], modDirWithAuthorName
			)
		end
	elseif rootDir == "lib" then
		-- do nothing
	elseif rootDir == "core" then
		-- do nothing
	else
		mwse.log(
			'\t[Logger | ERROR]: Filepath "%s" was not correctly deduced. "%s" is an invalid root direcotry', 
			filepath, rootDir
		)
	end
	-- This is path of the filepath that does not include the `modDir`.
	-- NOTE: `modDir` could be either `pathParts[1]` or `pathParts[1] .. "." .. pathParts[2]`.
	-- Thus, `modDir` is NOT a substring of `table.concat(pathParts, "/")`, 
	-- as the directory delimters may differ.
	local relativeFilePath = table.concat(pathParts, "/"):sub(modDir:len() + 2)

	return modName, modDir, relativeFilePath
end



-- Logging framework.
---@class mwseLogger : mwseLogger.SharedData
---@field protected sharedData mwseLogger.SharedData Stores information that's shared between loggers. This should not be accessed directly.
local Logger = {
	LOG_LEVEL = LOG_LEVEL,
	--- A table containing all the provided logging formatters. 
	--- But of course, it is also possible to define your own custom formatter.
	FORMATTERS = require("logger.formatters")
}

-- Make it available globally.
mwse.Logger = Logger
mwse.LOG_LEVEL = LOG_LEVEL

--- Indexed by `modDir`. Keeps track of all the loggers associated with a given mod.
---@type table<string, mwseLogger[]>
local registeredLoggers = {}

--- A set containing all the "communal keys", i.e., the keys stored in SharedData.
local COMMUNAL_KEYS = {
	level = true,
	--- Backwards compatibility: this was the old name of the `level` field.
	--- This field will always be redirected to `SharedData.level`.
	--- Note: This field should, from the users perspective, always be the string 
	--- representation of the current logging level.
	logLevel = true, 
	formatter = true,
	--- Backwards compatibility: this was the old name of the `modName` field.
	--- This field will always be redirected to `SharedData.modName`.
	name = true, 
	modName = true,
	modDir = true,
	logToConsole = true,
	includeTimestamp = true,
	outputFile = true,
	abbreviateHeader = true,
	defaultOutputPath = true,
}


-- The default value for each of the shared values, except for `modName` and `modDir`, which MUST
-- be provided when creating a new `SharedData`.
-- This includes, in particular, defining the default formatter.
---@type mwseLogger.SharedData
local SHARED_DEFAULT_VALUES = {
	---@diagnostic disable-next-line: assign-type-mismatch
	modDir = nil,
	---@diagnostic disable-next-line: assign-type-mismatch
	modName = nil,
	abbreviateHeader = false,
	includeTimestamp = false,
	level = LOG_LEVEL.INFO,
	outputFile = nil,
	logToConsole = false,

	-- This formatter could be altered to recursively process function parameters,
	-- but at the moment that does not seem particularly useful.
	-- And it would add a lot more complexity to an already pretty complex
	-- formatting function.

	formatter = require("logger.formatters").DEFAULT
}

--- This is the metatable used by `SharedData` instances.
--- There is only one metamethod that's implemented.
---@type metatable
local SharedDataMeta = {
	--- This is responsible for looking up missing values in the `SHARED_DEFAULT_VALUES` table.
	--- We also convert the older versions of field names to ensure backwards compatibility.
	--- The general control flow structure is as follows:
	--- 1) The user tries to index, for example, `logger.formatter`.
	--- 2) `LoggerMeta.__index(logger, "formatter")` is called. 
	--- 	- This metamethod realizes that `formatter` is a communal key, so it executes the following code:
	--- 		```lua
	--- 		logger.sharedData["formatter"]
	--- 		```
	--- 3) If no custom formatter was found (i.e., `logger.sharedData["formatter"] == nil)`, 
	--- 	then this metamethod is called, and it returns the default formatter.
	---@param self mwseLogger.SharedData
	---@param k string
	__index = function(self, k)
		if k == "name" then
			-- Backwards compatibility: return the `modName`.
			return rawget(self, "modName")
		elseif k == "logLevel" then
			-- Backwards compatibility: return the current logging level as a string.
			return LOG_LEVEL_STRINGS[self.level]
		else
			-- Return the default value.
			return SHARED_DEFAULT_VALUES[k]
		end
	end
}


-- This function is responsible for updating the `outputFile` field of a logger.
-- This is only called in one place, but is factored out to help with code readability.
---@param sharedData mwseLogger.SharedData
---@param outputFile string|false|nil
local function setOutputFile(sharedData, outputFile)
	---@type file*|false
	local prevOutputFile = sharedData.outputFile

	if prevOutputFile == outputFile then 
		return
	end

	if prevOutputFile then
		prevOutputFile:close()
	end

	if outputFile == false or outputFile == nil then
		sharedData.outputFile = nil
		return
	end

	if outputFile == true then
		outputFile = fmt("%s/%s.log", 
			LOG_FILE_PARENT_DIR, sharedData.modDir:gsub("[./\\]", "/")
		)
	-- sanitize the input string, 
	elseif type(outputFile) == "string" then
		-- If we're passed a string, we should ensure it's of the form `Data Files/MWSE/logs/%s.log`
		-- And we should also make sure it's not equal to Data Files/MWSE/logs/MWSE.log
		if not outputFile:endswith(".log") then
			outputFile = outputFile .. ".log"
		end
		-- Invalid input, so do nothing and return.
		if outputFile:lower() == "mwse.log" then
			sharedData.outputFile = nil
			return
		end
		if not outputFile:startswith(LOG_FILE_PARENT_DIR) then
			outputFile = fmt("%s/%s", LOG_FILE_PARENT_DIR, outputFile)
		end
		
	end

	do -- Ensure parent directory exists.
	
		---@type integer
		local fileNameStart = assert(outputFile:find("[^/]+%.log$"), "Error: file stub could not be found!")
		local parentDir = outputFile:sub(1, fileNameStart - 1)

		if not lfs.directoryexists(parentDir) then
			local path
			for _, pathPart in ipairs(parentDir:split "/") do
				path = path and fmt("%s/%s", path, pathPart) or pathPart
				if not lfs.directoryexists(path) then
					lfs.mkdir(path)
				end
			end
		end
	end

	sharedData.outputFile = io.open(outputFile, "w")
end

--[[ Metatable used by all Loggers.
There are currently three supported metamethods:
1) `__index`: Used to look up `SharedData` and `Logger` methods. 
	- When `logger` is indexed by a key `k`, this method will first check if `k` is a communal key.
	- If `k` IS a communal key: 
		- This method returns `logger.sharedData[k]`.
		- Note: `SharedData` implements its own `__index` metamethod, which is used to fetch default values.
	- If `k` IS NOT a communal key:
		- This method returns `Logger[k]`.
		- This happens, for example, whenever `log:debug` is called. 
		- (And also happens whenever any Logger method is called.)

2) `__newindex`: Used to ensure communal keys are properly updated.
	- Note that `__newindex` is triggered only if the `k` in question does not currently exist in the table.
	- This okay for this implementation because we are only interesting in specifying custom behavior for communal keys,
		and we take measures to ensure that communal values never get written directly to logger objects.
	- Instead, communal values are stored in the `sharedData` field.
	- Whenever the user writes `logger[k] = v`, this method will check if `k` is a communal key.
	- If `k` IS a communal key:
		- write the new value inside of `self.sharedData`.
	- If `k` IS NOT a communal key:
		- set the value normally.

3) `__call`: This is shorthand for `log:debug`. This will be set later, once the `debug` method is defined.
]]
---@type metatable
local LoggerMeta = {
	---@param self mwseLogger
	__index = function (self, key)
		-- Note: backwards compatibility is handled by `SharedData.__index`.
		if COMMUNAL_KEYS[key] ~= nil then
			---@diagnostic disable-next-line: invisible
			return self.sharedData[key]
		end
		return Logger[key]
	end,

	---@param self mwseLogger
	__newindex = function (self, k, v)
		if COMMUNAL_KEYS[k] == nil then
			rawset(self, k, v)
			return
		end
		-- Past here, we know we need to change `self.sharedData` instead of `self`.
		---@diagnostic disable-next-line: invisible
		local sharedData = self.sharedData

		-- We also allow users to set the `logLevel` directly, for backwards compatibility.
		-- In either case, we will update `sharedData.level.
		if k == "level" or k == "logLevel" then
			if not v then return end

			-- If `v` is a string representation of a valid log level, store the numeric version.
			if LOG_LEVEL[v] then
				sharedData.level = LOG_LEVEL[v]
			-- If `v` is a numeric representation of a valid log level, store `v`.
			elseif LOG_LEVEL_STRINGS[v] then
				sharedData.level = v
			end

		elseif k == "outputFile" then 
			setOutputFile(sharedData, v)
		-- Backwards compatibility: convert to `name` to `modName`.
		elseif k == "name" then
			sharedData.modName = v

		-- Make it harder for the `modDir` to be changed after loggers are initialized.
		-- Note: This only affects the behavior when writing `log.modDir = "foo"`. It does not 
		-- 	change change what happens when writing `log.sharedData.modDir = "foo"`.
		-- But this is about the best we can do in Lua.
		-- The reason we are doing this is that some weird bugs can happen if users decide to 
		-- change the `modDir` after initializing a logger, as it could potentially
		-- result in loggers from different mods having the same `sharedData`.
		elseif k == "modDir" then
			-- Do nothing.

		else -- `k` is a communal key that doesn't need any special treatment.
			sharedData[k] = v
		end
	end,
}


-- create a new logger by passing in a table with parameters or by passing in a string with just the `modName`
---@param params string|mwseLogger.new.data?
---@return mwseLogger
function Logger.new(params)


	if type(params) == "table" then
		-- Update the names of the parameters in-place, for backwards compatibility.
		---@diagnostic disable-next-line: undefined-field
		params.modName = params.modName or params.name
		---@diagnostic disable-next-line: undefined-field
		params.level = params.level or params.logLevel
	elseif type(params) == "string" then
		params = {modName = params}
	else
		params = {}
	end
	if params.modName and type(params.modName) ~= "string" then
		error("[Logger] No name provided.")
	end



	---@cast params -nil
	---@cast params -string

	-- We will temporarily store separate copies locally. 
	-- This is so that, for example, an autogenerated `modName` does not replace 
	-- a `modName` that was manually set in a sibling logger.
	-- We will only set the `modName` and `modDir` to be the autogenerated values
	-- 	if they could not be obtained from a sibling.
	-- However, if `params.modName` is not nil, then we will update all the siblings using that value.

	-- Note that we have to do this in a bit of a convulated order because we need to have
	-- 	a `modDir` (either from `params` or from `getModNameAndDirAndFilepath`) in order to 
	-- retrieve the table of sibling loggers.

	-- This is `params.modName`, or the autogenerated `modName`.
	local modName = params.modName
	-- This is `params.modDir`, or the autogenerated `modDir`.
	local modDir = params.modDir
	-- This is `params.filepath`, or the autogenerated `filepath`.
	local filepath = params.filepath
	
	if not (modName and modDir and filepath) then
		local autoModName, autoModDir, autofilepath = getModNameAndDirAndFilepath(2)
		modName = modName or autoModName
		modDir = modDir or autoModDir or modName
		filepath = filepath or autofilepath
	end

	assert(modName, "[Logger: ERROR] Could not create a Logger because the modName could not be found.")

	
	-- All of the loggers associated with the active mod.
	---@type mwseLogger[]
	local siblings = table.getset(registeredLoggers, modDir, {})

	-- Check if this Logger has already been constructed.
	-- If it has, update its communal values and then return it.
	for _, sibling in pairs(siblings) do
		if sibling.filepath == filepath and sibling.moduleName == params.moduleName then
			-- If we reach this point, we will update the shared settings and then return.

			for k in pairs(COMMUNAL_KEYS) do
				local paramVal = params[k]
				if paramVal ~= nil then
					-- Note: This triggers the `__newindex` metamethod.
					sibling[k] = paramVal
				end
			end
			-- No need to create a new logger in this case.
			return sibling

		end
	end
	
	-- The shared data used by our new logger.
	-- If there are any sibling loggers, we'll fetch the `SharedData` from one of the siblings.
	-- Otherwise (i.e. if this is the logger made for a certain mod), we'll make a new `SharedData`.
	-- This approach ensures all loggers use the same `SharedData` table.
	local sharedData

	if siblings[1] then
		sharedData = siblings[1].sharedData
	else
		-- We can't use the `modName` and `modDir` of any siblings, so use the autogenerated copies.
		---@diagnostic disable-next-line: missing-fields
		sharedData = setmetatable({ 
			modDir = modDir,
			modName = modName,
			
			-- Tiny optimization: store a copy of the logging level so that we don't trigger the
			-- `__index` metamethod of `SharedData` whenever we check the logging level.
			level = params.level or SHARED_DEFAULT_VALUES.level,
		}, SharedDataMeta)
	end

	

	---@type mwseLogger
	---@diagnostic disable-next-line: missing-fields
	local self = {
		moduleName = params.moduleName, 
		filepath = filepath, 
		sharedData = sharedData,
	}
	setmetatable(self, LoggerMeta)

	table.insert(siblings, self)

	-- Update the values from the passed parameters.
	-- This is where we get the payoff of storing the autogenerated `modName` and `modDir` locally,
	-- as this loop will not overwrite the `modName` or `modDir` of existing loggers with the 
	-- autogenerated values.
	for k in pairs(COMMUNAL_KEYS) do

		local paramVal = params[k]
		if paramVal ~= nil then
			-- Note: This triggers the `__newindex` metamethod.
			self[k] = paramVal
		end
	end

	return self
end

-- =============================================================================
-- METHODS
-- =============================================================================



-- backwards compatibility / explicit setters
for key in pairs(COMMUNAL_KEYS) do   
	---@diagnostic disable-next-line: assign-type-mismatch
	Logger["set".. key:sub(1,1):upper() .. key:sub(2)] = function(self, val) 
		self[key] = val
	end
end

function Logger:setModuleName(newModuleName)
	self.moduleName = newModuleName
end


--[[Get a previously registered logger with the specified `modDir`. You can optionally specify a filepath.]]
---@param modDir string name of the mod
---@param filepath string? the relative filepath of this logger
---@return mwseLogger? logger
function Logger.get(modDir, filepath)
	local arr = registeredLoggers[modDir]
	if not arr then return end
	if not filepath then return arr[1] end
	for _, logger in ipairs(arr) do
		if logger.filepath == filepath then
			return logger
		end
	end
end

---@deprecated Use Logger.get
function Logger.getLogger(modName)
	for _, arr in pairs(registeredLoggers) do
		if arr[1] and arr[1].sharedData.modName == modName then
			return arr[1]
		end
	end
	return false
end




---@deprecated Use compare the `level` field directly with `mwseLogger.LOG_LEVEL.DEBUG` (etc.)
function Logger:doLog(level)
	if LOG_LEVEL[level] then
		return self.sharedData.level >= LOG_LEVEL[level]
	elseif LOG_LEVEL_STRINGS[level] then
		return self.sharedData.level >= level
	end
end

--- Gets a string representation of a given logging level.
---@param level mwseLogger.LOG_LEVEL? The logging level to get the string for. Default: `self.level`.
---@return string
function Logger:getLevelStr(level)
	return LOG_LEVEL_STRINGS[level or self.level]
end


--- Returns all the loggers for a given mod directory (can pass a Logger as well).
---@param modDirOrLogger string|mwseLogger
function Logger.getLoggers(modDirOrLogger)
	if type(modDirOrLogger) == "string" then
		return registeredLoggers[modDirOrLogger]
	elseif type(modDirOrLogger) == "table" and modDirOrLogger.modDir ~= nil then
		return registeredLoggers[modDirOrLogger.modDir]
	end
end

-- =============================================================================
-- FUNCTIONS THAT HAVE TO DO WITH WRITING THINGS TO A FILE
-- =============================================================================

---@protected
---@param level mwseLogger.LOG_LEVEL
---@param offset integer? for the line number to be accurate, this method assumes it's getting called 2 levels deep (i.e.). the offset adjusts this
---@return mwseLogger.Record record
function Logger:makeRecord(level, offset)
	return {
		level = level,
		stackLevel = 3 + (offset or 0),
		timestamp = self.sharedData.includeTimestamp and socket.gettime(),
		lineNumber = mwse.getConfig("EnableLogLineNumbers") and debug.getinfo(3 + (offset or 0), "l").currentline
	}
end

---@protected
---@param record mwseLogger.Record
---@return string
function Logger:makeHeader(record)
	-- We're going to shove various things into here, and then call `table.concat`.
	local strs = {}

	local sharedData = self.sharedData
	local moduleName = self.moduleName
	
	if moduleName then
		table.insert(strs, fmt("%s (%s)", sharedData.modName, moduleName))
	else
		table.insert(strs, sharedData.modName)
	end

	-- insert filepath and line number
	local filepath, lineNo = self.filepath, record.lineNumber
	if filepath then
		if sharedData.abbreviateHeader then  
			local pathParts = filepath:split("/")
			local numParts = #pathParts
			for i = 1, numParts - 1 do
				pathParts[i] = pathParts[i]:sub(1, 1)
			end
			pathParts[numParts] = pathParts[numParts]:gsub(".lua$", "")
			filepath = table.concat(pathParts, "/")
		end
		if lineNo then
			table.insert(strs, fmt("%s:%-3i", filepath, lineNo))
		else
			table.insert(strs, filepath)
		end
	elseif lineNo then
		table.insert(strs, fmt("line: %-3i", lineNo))
	end

	-- add the log level string
	do
		local level = record.level
		local levelStr
		if mwse.getConfig("EnableLogColors") then
			if sharedData.abbreviateHeader then
				levelStr = LOG_LEVEL_ABBREVIATED_COLOR_STRINGS[level]
			else
				levelStr = LOG_LEVEL_COLOR_STRINGS[level]
			end
		else
			levelStr = LOG_LEVEL_STRINGS[level]
			if sharedData.abbreviateHeader then
				levelStr = levelStr:sub(1,1)
			end
		end
		table.insert(strs, levelStr)
	end


	if record.timestamp then
		local floor = math.floor
		local ts = record.timestamp - LAUNCH_TIME
		local milliseconds = floor(1000 * (ts % 1))
		local hours, minutes, seconds = floor(ts / 3600), floor(ts / 60) % 60, floor(ts) % 60

		-- Only show the number of hours if it's `> 1`.
		table.insert(strs, hours > 0 and fmt("%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds)
							   		  or fmt(	 "%02d:%02d.%03d",		minutes, seconds, milliseconds))
	end
	-- Notice the trailing space!
	return fmt("[%s] ", table.concat(strs, " | "))
end


-- Writes the string to a file and possibly also to the console.
---@protected
function Logger:write(str)
	if self.outputFile then
		self.outputFile:write(str, "\n")
		self.outputFile:flush()
	else
		print(str)
	end
	if self.logToConsole then
		tes3ui.log(str)
	end
end


-- calls format on the record and writes it to the appropriate location
---@protected
---@param record mwseLogger.Record
---@param ... any
function Logger:writeRecord(record, ...)
	self:write(self.sharedData.formatter(self, record, ...))
	
end


-- Make the logging functions
---@param levelStr string
for levelStr, level in pairs(LOG_LEVEL) do
	-- e.g., "DEBUG" -> "debug"
	---@param self mwseLogger
	---@diagnostic disable-next-line: assign-type-mismatch
	Logger[string.lower(levelStr)] = function(self, ...)
		---@diagnostic disable-next-line: invisible
		if self.sharedData.level >= level then
			---@diagnostic disable-next-line: invisible
			self:writeRecord(self:makeRecord(level), ...)
		end
	end
end

-- I am a very good programmer.
Logger.none = nil

-- Update `call` to be the same as `debug`. 
--This is so that the line numbers are pulled correctly when using the metamethod.
LoggerMeta.__call = Logger.debug

---@generic T, S, A
--- Wrapper for `assert`
---@param v T 
---@param msg string|any
---@param ... A
---@return T condition
---@return string|S str
---@return A? ...
function Logger:assert(v, msg, ...)
	if v then return v, msg, ... end

	-- cant call `Logger:error` because we need the call to `debug.getinfo` to produce the correct line number. super hacky :/
	local str = self:formatter(self:makeRecord(LOG_LEVEL.ERROR), msg, ...)

	if self.sharedData.level >= LOG_LEVEL.ERROR then
		self:write(str)
	end

	return assert(v, str, ...)
end

function Logger:writeInitMessage(version)
	if self.sharedData.level < LOG_LEVEL.INFO then return end

	local record = self:makeRecord(LOG_LEVEL.INFO)
	
	if not version then	
		local m = tes3.getLuaModMetadata(self.modDir)
		version = m and m.package and m.package.version
	end
	if version then
		self:write(self:formatter(record, "Initialized version %s.", version))
	else
		self:write(self:formatter(record, "Mod initialized."))
	end
end


return Logger