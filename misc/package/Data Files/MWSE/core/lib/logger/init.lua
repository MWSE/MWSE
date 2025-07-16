---@diagnostic disable: duplicate-set-field
--[[ # Logger

Based on the `mwseLogger` made by Merlord.

Current version made by herbert100.

Many suggestions and feature ideas given by C3pa.

Credit to Greatness7 as well, for the ideas of adding a `format` parameter and allowing functions to be passed to log messages.
]]

local socket = require("socket")
local FORMATTERS = require("logger.formatters")
local pathResolver = require("logger.pathResolver")

-- The time that the game was launched.
---@type number
local LAUNCH_TIME = socket.gettime()

local fmt = string.format

local logLevel = { none = 0, error = 1, warn = 2, info = 3, debug = 4, trace = 5 }

-- Holds the uppercased values of the logging levels.
-- This is used for backwards compatibility with the old logging API.
local logLevelUpper = {}

--- This table takes in a log level string and spits out the corresponding numeric log level.
--- This is defined to be the inversion of `logLevel`.
--- So, `logLevelStrings[number]` is equivalent to `table.find(logLevel, number)`.
---@type table<mwseLogger.logLevel, string>
local logLevelStrings = {}
for k, v in pairs(logLevel) do
	logLevelStrings[v] = k:upper()
	logLevelUpper[k:upper()] = v
end

-- Note: \x1B is the escape character.

-- Tiny optimization: store the terminal escape sequences for each log level, so they don't
-- have to be recomputed every log message.
---@type table<mwseLogger.logLevel, string>
local LOG_LEVEL_COLOR_STRINGS = {
	-- color doesn't matter
	[logLevel.none] = "NONE",
	-- bright yellow
	[logLevel.warn] = "\x1B[0m\x1B[1m\x1B[33mWARN\x1B[0m",
	-- bright red
	[logLevel.error] = "\x1B[0m\x1B[1m\x1B[31mERROR\x1B[0m",
	-- white
	[logLevel.info] = "\x1B[0m\x1B[37mINFO\x1B[0m",
	-- bright green
	[logLevel.debug] = "\x1B[0m\x1B[1m\x1B[32mDEBUG\x1B[0m",
	-- bright white
	[logLevel.trace] = "\x1B[0m\x1B[1m\x1B[37mTRACE\x1B[0m",
}

-- Pre-colored versions of the abbreviated log level strings.
---@type table<mwseLogger.logLevel, string>
local LOG_LEVEL_ABBREVIATED_COLOR_STRINGS = {
	-- color doesn't matter
	[logLevel.none] = "NONE",
	-- bright yellow
	[logLevel.warn] = "\x1B[0m\x1B[1m\x1B[33mW\x1B[0m",
	-- bright red
	[logLevel.error] = "\x1B[0m\x1B[1m\x1B[31mE\x1B[0m",
	-- white
	[logLevel.info] = "\x1B[0m\x1B[37mI\x1B[0m",
	-- bright green
	[logLevel.debug] = "\x1B[0m\x1B[1m\x1B[32mD\x1B[0m",
	-- bright white
	[logLevel.trace] = "\x1B[0m\x1B[1m\x1B[37mT\x1B[0m",
}

do

	--- A function that handles the formatting of log messages, given the `logger`, a `record`, and the log function parameters.
	--- The record holds information about the logging level and line number, and the rest of the parameters are the ones passed
	--- as arguments to the `Logger:debug` methods.
	--- This function should return a string, which will then be printed to the appropriate file.
	--- This function should also include the header of the log message. The default header can be accessed by
	--- using the `makeHeader` method.
	---@alias mwseLogger.formatter fun(self: mwseLogger, record: mwseLoggerRecord, ...: string|any|fun(...): ...): string

	---@alias mwseLogger.logLevel
	---|`mwse.logLevel.none`	Nothing will be printed
	---|`mwse.logLevel.error`	Error messages will be printed
	---|`mwse.logLevel.warn`	Warning messages will be printed
	---|`mwse.logLevel.info`	Crucial information will be printed
	---|`mwse.logLevel.debug`	Debug messages will be printed
	---|`mwse.logLevel.trace`	Many debug messages will be printed

	---@alias mwseLogger.logLevelString
	---|`"NONE"`	 			Nothing will be printed
	---|`"ERROR"`				Error messages will be printed
	---|`"WARN"`	 			Warning messages will be printed
	---|`"INFO"`	 			Crucial information will be printed
	---|`"DEBUG"`				Debug messages will be printed
	---|`"TRACE"`				Many debug messages will be printed

	--- Stores all the mod-level information for a logger.
	--- This allows a mod to have several different loggers that are all sychronized with each other.
	---@class mwseLogger.SharedData
	---@field level mwseLogger.logLevel The logging level for this logger
	---@field logToConsole boolean
	---@field formatter mwseLogger.formatter
	---@field modName string name of the mod
	---@field modDir string
	---@field includeTimestamp boolean should the current time be printed when writing log messages? Default: `false`
	---@field outputFile file*|nil The file the log is being written to, or `nil`.
	---@field abbreviateHeader boolean Print a shorter header?

end

local LOG_FILE_PARENT_DIR = "Data Files/MWSE/logs"


-- Logging framework.
---@class mwseLogger : mwseLogger.SharedData
---@field protected sharedData mwseLogger.SharedData Stores information that's shared between loggers. This should not be accessed directly.
local Logger = {}

-- Make it available globally.
mwse.logLevel = logLevel

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
	level = logLevel.info,
	outputFile = nil,
	logToConsole = false,

	-- This formatter could be altered to recursively process function parameters,
	-- but at the moment that does not seem particularly useful.
	-- And it would add a lot more complexity to an already pretty complex
	-- formatting function.

	formatter = FORMATTERS.DEFAULT,
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
			return logLevelStrings[self.level]
		else
			-- Return the default value.
			return SHARED_DEFAULT_VALUES[k]
		end
	end,
}

-- This function is responsible for updating the `outputFile` field of a logger.
-- This is only called in one place, but is factored out to help with code readability.
---@param sharedData mwseLogger.SharedData
---@param outputFile string|boolean|nil
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
		outputFile = fmt("%s/%s.log", LOG_FILE_PARENT_DIR, sharedData.modDir:gsub("[./\\]", "/"))
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
	--- @cast outputFile -boolean


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
	__index = function(self, key)
		-- Note: backwards compatibility is handled by `SharedData.__index`.
		if COMMUNAL_KEYS[key] ~= nil then
			---@diagnostic disable-next-line: invisible
			return self.sharedData[key]
		end
		return Logger[key]
	end,

	---@param self mwseLogger
	__newindex = function(self, k, v)
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
			if not v then
				return
			end

			-- If `v` is a string representation of a valid log level, store the numeric version.
			if logLevel[v] then
				sharedData.level = logLevel[v]
				-- It was passed in via upper case text.
			elseif logLevelUpper[v] then
				sharedData.level = logLevelUpper[v]
				-- If `v` is a numeric representation of a valid log level, store `v`.
			elseif logLevelStrings[v] then
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
---@param params string|mwseLogger.new.params?
---@return mwseLogger
function Logger.new(params)

	if type(params) == "table" then
		-- Update the names of the parameters in-place, for backwards compatibility.
		---@diagnostic disable-next-line: undefined-field
		params.modName = params.modName or params.name
		---@diagnostic disable-next-line: undefined-field
		params.level = params.level or params.logLevel
	elseif type(params) == "string" then
		params = { modName = params }
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
	---@diagnostic disable-next-line: undefined-field
	local modDir = params.modDir
	-- This is `params.filepath`, or the autogenerated `filepath`.
	---@diagnostic disable-next-line: undefined-field
	local filepath = params.filepath

	if not (modName and modDir and filepath) then
		local autoModName, autoModDir, autofilepath = pathResolver.getModNameAndDirAndFilepath(2)
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
		---@diagnostic disable-next-line: assign-type-mismatch
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
	Logger["set" .. key:sub(1, 1):upper() .. key:sub(2)] = function(self, val)
		self[key] = val
	end
end

function Logger:setModuleName(newModuleName)
	self.moduleName = newModuleName
end

---@param filepath string?
---@return mwseLogger|nil logger
function Logger.get(modDir, filepath)
	local arr = registeredLoggers[modDir]
	if not arr then
		return
	end
	if not filepath then
		return arr[1]
	end
	for _, logger in ipairs(arr) do
		if logger.filepath == filepath then
			return logger
		end
	end
end

---@deprecated
function Logger.getLogger(modName)
	for _, arr in pairs(registeredLoggers) do
		if arr[1] and arr[1].sharedData.modName == modName then
			return arr[1]
		end
	end
	return false
end

---@deprecated
function Logger:doLog(level)
	if logLevel[level] then
		return self.sharedData.level >= logLevel[level]
	elseif logLevelUpper[level] then
		return self.sharedData.level >= logLevelUpper[level]
	elseif logLevelStrings[level] then
		return self.sharedData.level >= level
	end
end

function Logger:getLevelString(level)
	return logLevelStrings[level or self.level]
end

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
---@param level mwseLogger.logLevel
---@param offset integer? for the line number to be accurate, this method assumes it's getting called 2 levels deep (i.e.). the offset adjusts this
---@return mwseLoggerRecord record
function Logger:makeRecord(level, offset)
	return {
		level = level,
		stackLevel = 3 + (offset or 0),
		timestamp = self.sharedData.includeTimestamp and socket.gettime(),
		lineNumber = mwse.getConfig("EnableLogLineNumbers") and debug.getinfo(3 + (offset or 0), "l").currentline,
	}
end

---@protected
---@param record mwseLoggerRecord
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
			levelStr = logLevelStrings[level]
			if sharedData.abbreviateHeader then
				levelStr = levelStr:sub(1, 1)
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
		table.insert(strs, hours > 0 and fmt("%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds) or
		             fmt("%02d:%02d.%03d", minutes, seconds, milliseconds))
	end
	-- Notice the trailing space!
	return fmt("[%s] ", table.concat(strs, " | "))
end

-- Writes the string to a file and possibly also to the console.
---@protected
function Logger:write(str)
	local outputFile = self.sharedData.outputFile
	if outputFile then
		outputFile:write(str, "\n")
		outputFile:flush()
	else
		print(str)
	end
	if self.logToConsole then
		tes3ui.log(str)
	end
end

-- calls format on the record and writes it to the appropriate location
---@protected
---@param record mwseLoggerRecord
---@param ... any
function Logger:writeRecord(record, ...)
	self:write(self.sharedData.formatter(self, record, ...))

end

-- Make the logging functions
---@param levelStr string
for levelStr, level in pairs(logLevel) do
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

Logger.none = nil

-- Update `call` to be the same as `debug`.
-- This is so that the line numbers are pulled correctly when using the metamethod.
LoggerMeta.__call = Logger.debug

--- @generic T
--- @param v? T
--- @param message string|fun(...): ...
--- @param ...? any
--- @return T v
--- @return any ...
function Logger:assert(v, message, ...)
	if v then
		return v, message, ...
	end

	-- cant call `Logger:error` because we need the call to `debug.getinfo` to produce the correct line number. super hacky :/
	local str = self:formatter(self:makeRecord(logLevel.error), message, ...)

	if self.sharedData.level >= logLevel.error then
		self:write(str)
	end

	return assert(v, str, ...)
end

function Logger:writeInitMessage(version)
	if self.sharedData.level < logLevel.info then
		return
	end

	local record = self:makeRecord(logLevel.info)

	if not version then
		local m = tes3.getLuaModMetadata(self.modDir)
		version = m and m.package and m.package.version
	end
	if version then
		self:writeRecord(record, "Initialized version %s.", version)
	else
		self:writeRecord(record, "Mod initialized.")
	end
end

return Logger
