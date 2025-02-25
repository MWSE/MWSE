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

---@type table<Logger.LEVEL, string>
local LOG_LEVEL_STRINGS = table.invert(LOG_LEVEL)

-- Note: \x1B is the escape character.

-- Tiny optimization: store the terminal escape sequences for each log level, so they don't 
-- have to be recomputed every log message.
---@type table<Logger.LEVEL, string>
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
---@type table<Logger.LEVEL, string>
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
---@alias Logger.formatter fun(self: Logger, record: Logger.Record, ...: string|any|fun(...): ...): string


--- A logging record. Contains information about the log level and the line number.
---@class Logger.Record
---@field stackLevel integer how far up the stack we're being called
---@field level Logger.LEVEL logging level
---@field lineNumber integer|false the line number, if enabled for this logger
---@field timestamp number|false the timestamp of this message

--- Parameters used to create a new `Logger`. 
-- All parameters are optional, as they will be retrieved automatically if not provided.
-- 
--- In addition to displayed parameters, you can also pass a `modName`, a `filepath`, a `level` and a `filepath`.
-- Although for the latter options, it's recommended you use the relevant methods instead.
---@class Logger.newParams
---@field modName string? the name of the mod this logger is for. will be automatically retrieved if not provided
---@field modDir string? The directory that this mod operates in. Used to retrieve the `Mod_Info`.
---@field moduleName string|false? The name of a module that this logger belongs to.
-- This is useful in cases where one file has several distinct parts. 
-- The `moduleName` will be displayed next to the name of the mod, in parentheses.
---@field filePath string? path to the file. will be retrieved if not provided
---@field level Logger.LEVEL? the log level to set this object to. Default: "LEVEL.INFO"
---@field writeToFile string|boolean|nil whether to write the log messages to a file, or the name of the file to write to. if `false` or `nil`, messages will be written to `MWSE.log`
---@field includeLineNumber boolean? Should the current line number be printed when writing log messages? Default: `true`
---@field includeTimestamp boolean? should timestamps be included in logging messages? Default: `false`
---@field abbreviateHeader boolean? Should the header messages be abbreviated? Default: `false`.
---@field formatter Logger.formatter? A way to specify how logging messages should be formatted. A default one will be used if not provided.


---@alias Logger.LEVEL
---|0                       NONE: Nothing will be printed
---|1                       ERROR: Error messages will be printed
---|2                       WARN: Warning messages will be printed
---|3                       INFO: Only crucial information will be printed
---|4                       DEBUG: Debug messages will be printed
---|5                       TRACE: Many debug messages will be printed
---|`Logger.LEVEL.NONE`     Nothing will be printed
---|`Logger.LEVEL.ERROR`    Error messages will be printed
---|`Logger.LEVEL.WARN`     Warning messages will be printed
---|`Logger.LEVEL.INFO`     Crucial information will be printed
---|`Logger.LEVEL.DEBUG`    Debug messages will be printed
---|`Logger.LEVEL.TRACE`    Many debug messages will be printed


--- Stores all the mod-level information for a logger. 
--- This allows a mod to have several different loggers that are all sychronized with each other.
---@class Logger.SharedData
---@field level Logger.LEVEL The logging level for this logger
---@field formatter Logger.formatter
---@field modName string name of the mod
---@field modDir string
---@field includeTimestamp boolean should the current time be printed when writing log messages? Default: `false`
---@field includeLineNumber boolean should the current time be printed when writing log messages? Default: `false`
---@field outputFile file*|false? The file the log is being written to, or `nil`.
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
	local filePath, newFilePath

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

		filePath = newFilePath
    end
	if not filePath then return end


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
	local _, mwseFolderEndIndex = filePath:find("data files\\mwse\\", 1, true)
	if mwseFolderEndIndex then
		filePath = filePath:sub(mwseFolderEndIndex + 1)
	end
	
	---@type string[]
	local pathParts = filePath:split("\\")

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
		else -- no author directory
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
			filePath, rootDir
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
---@class Logger : Logger.SharedData
---@operator call (Logger.newParams?): Logger
---@field level Logger.LEVEL
---@field filePath string? the relative path to the file this logger was defined in.
---@field protected sharedData Logger.SharedData stores communal data
---@field moduleName string|false? the module this logger belongs to, or `nil` if it's a general purpose logger
local Logger = {LOG_LEVEL = LOG_LEVEL}

--- Indexed by `modDir`. Keeps track of all the loggers associated with a given mod.
---@type table<string, Logger[]>
local registeredLoggers = {}

-- A set containing all the "communal keys", i.e., the keys stored in SharedData.
local COMMUNAL_KEYS = {
	level = true,
	formatter = true,
	modName = true,
	modDir = true,
	includeTimestamp = true,
	includeLineNumber = true,
	outputFile = true,
	abbreviateHeader = true,
	defaultOutputPath = true,
}


-- The default value for each of the shared values, except for `modName` and `modDir`, which MUST
-- be provided when creating a new `SharedData`.
-- This includes, in particular, defining the default formatter.
---@type Logger.SharedData
local SHARED_DEFAULT_VALUES = {
	---@diagnostic disable-next-line: assign-type-mismatch
	modDir = nil,
	---@diagnostic disable-next-line: assign-type-mismatch
	modName = nil,
    abbreviateHeader = false,
    includeLineNumber = true,
    includeTimestamp = false,
    level = LOG_LEVEL.INFO,
    outputFile = false,

    ---@param self Logger
    ---@param record Logger.Record
    ---@param ... any
    formatter = function(self, record, ...)
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
			elseif aType == "table" and getmetatable(a).__tostring == nil then
				table.insert(fmtArgs, json.encode(a))
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
            str = fmt(table.unpack(fmtArgs))
        else
            str = fmtArgs[1]
        end

		---@diagnostic disable-next-line: invisible
        local header = self:makeHeader(record)

		-- If it's an error, print the traceback information.
        if record.level == LOG_LEVEL.ERROR then
            return header .. debug.traceback(str, record.stackLevel + 1)
        else
            return header .. str
        end
    end
}

---@type metatable
local SharedDataMeta = {__index = SHARED_DEFAULT_VALUES}


-- Function responsible for updating the output path of a logger.
-- This is only called in one place, but is pulled out to help with code readability.
---@param sharedData Logger.SharedData
---@param outputFile string|false|nil
local function setOutputFile(sharedData, outputFile)
		
	---@type file*|false
	local prevOutputFile = sharedData.outputFile

	if outputFile == nil or prevOutputFile == outputFile then 
		return
	end

	if prevOutputFile then
		prevOutputFile:close()
	end

	if outputFile == false then
		sharedData.outputFile = false
		return
	end

	if outputFile == true then
		outputFile = fmt("%s/%s.log", 
			LOG_FILE_PARENT_DIR, sharedData.modDir:gsub("[./\\]", "/")
		)
	elseif type(outputFile) == "string" then
		-- sanitize it
		if not outputFile:startswith(LOG_FILE_PARENT_DIR) then
			outputFile = fmt("%s/%s", LOG_FILE_PARENT_DIR, outputFile)
		end
		if not outputFile:endswith(".log") then
			outputFile = outputFile .. ".log"
		end
	end

	do -- ensure parent directory exists
		---@type integer
		local fileNameStart = assert(outputFile:find("[^/]+%.log$"), "Error: file stub could not be found!")
		local parentDir = outputFile:sub(1, fileNameStart - 1)

		if not lfs.directoryexists(parentDir) then
			local absPath
			for _, part in ipairs(parentDir:split "/") do
				absPath = absPath and fmt("%s/%s", absPath, part) or part
				if not lfs.directoryexists(absPath) then
					lfs.mkdir(absPath)
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
	- if `k` IS NOT a communal key:
		- This method returns `Logger[k]`.
		- This happens, for example, whenever `log:debug` is called. 
		- (And also happens whenever any Logger method is called.)
2) `__newindex`: Used to ensure communal keys are properly updated.
	- Whenever the user writes `logger[k] = v`, this method will check if `k` is a communal key.
	- If `k` IS a communal key:
		-
]]
---@type metatable
local LoggerMeta = {
	---@param self Logger
	__index = function (self, key)
		if COMMUNAL_KEYS[key] ~= nil then
			---@diagnostic disable-next-line: invisible
			return self.sharedData[key]
		end
		return Logger[key]
	end,

	---@param self Logger
	__newindex = function (self, k, v)
		if COMMUNAL_KEYS[k] == nil then
			rawset(self, k, v) 
		end
		-- past here, we know we need to change `self.sharedData` instead of `self`
		---@diagnostic disable-next-line: invisible
		local sharedData = self.sharedData

		if k == "level" then
			if not v then return end

			-- if `v` is a string representation of a valid log level
			if LOG_LEVEL[v] then
				sharedData.level = LOG_LEVEL[v]
				-- self("Updated log level to %s.", v)
			-- if `v` is a numeric representation of a valid log level
			elseif LOG_LEVEL_STRINGS[v] then
				sharedData.level = v
				-- self("Updated log level to %s.", LEVEL_STRINGS[v])
			end
		elseif k == "outputFile" then 
			setOutputFile(sharedData, v)
		else -- `k` is a communal key that doesn't need any special treatment
			sharedData[k] = v
		end
	end,
}


-- create a new logger by passing in a table with parameters or by passing in a string with just the `modName`
---@param params string|Logger.newParams?
---@return Logger
function Logger.new(params, params2)
	-- handle the case when this function is called as a method
	if params == Logger or params and getmetatable(params) == LoggerMeta then
		params = params2
	end


	if type(params) == "table" then
		-- update the parameters for backwards compatibility
		params.modName = params.modName or params.name
		params.level = params.level or params.logLevel
		params.filePath = params.filePath or params.filepath
	elseif type(params) == "string" then
		params = {modName = params}
	else
		params = {}
	end

	---@cast params -nil
	---@cast params -string


	if not (params.modName and params.modDir and params.filePath) then
		local modName, modDir, filePath = getModNameAndDirAndFilepath(2)
		params.modName = params.modName or modName
		params.modDir = params.modDir or modDir or params.modName
		params.filePath = params.filePath or filePath
	end

	assert(params.modName, "[Logger: ERROR] Could not create a Logger because the modName could not be found.")

	
	-- All of the loggers associated with the active mod.
	---@type Logger[]
	local siblings = table.getset(registeredLoggers, params.modDir, {})

	-- Check if this Logger has already been constructed.
	for _, sibling in pairs(siblings) do
		if sibling.filePath == params.filePath and sibling.moduleName == params.moduleName then
			return sibling
		end
	end

	local sharedData = siblings[1] and siblings[1].sharedData
	if not sharedData then
		---@diagnostic disable-next-line: missing-fields
		sharedData = setmetatable({ 
			modDir = params.modDir,
			modName = params.modName,
			
			-- Tiny optimization: store a copy of the logging level so that we don't trigger the
			-- `__index` metamethod of `SharedData` whenever we check the logging level.
			level = SHARED_DEFAULT_VALUES.level,
		}, SharedDataMeta)
	end

	---@type Logger
	---@diagnostic disable-next-line: missing-fields
	local self = {
		moduleName = params.moduleName, 
		filePath = params.filePath, 
		sharedData = sharedData,
	}
	setmetatable(self, LoggerMeta)

	-- initialize the created object
	
	table.insert(siblings, self)
	-- Update the values from the passed parameters.
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

-- define dummy methods that will get overriden soon 
-- these are just to help with linting until the documentation is finalized
-- these won't be present in the final release.
do 
	--- Determines whether the logging output should be written to a specific file, or to `MWSE.log`.
	---@param filePath string|boolean 
	--- 1) If `false`, output will be written to `MWSE.log`.
	--- 2) If `true`, a default filepath will be generated, in the form "Data Files/MWSE/logs/<MOD_DIR>.log".
	---  - e.g. "Data Files/MWSE/logs/herbert100/More QuickLoot.log".
	--- 3) If a `string`, then that string will be used as the filepath. Taken relative to `Data Files/MWSE/logs/`
	function Logger:setOutputFile(filePath) end



	--[[Change the current logging level. You can specify a string or number.
	e.g. to set the `log.level` to "DEBUG", you can write any of the following:
	1) `log:setLevel("DEBUG")`
	2) `log:setLevel(4)`
	3) `log:setLevel(Logger.LEVEL.DEBUG)`
	]]
	---@param self Logger
	---@param level Logger.LEVEL
	---@return Logger.LEVEL? the level that was set
	function Logger:setLevel(level) end
end

-- backwards compatibility / explicit setters
for key in pairs(COMMUNAL_KEYS) do   
	---@diagnostic disable-next-line: assign-type-mismatch
	Logger["set".. key:sub(1,1):upper() .. key:sub(2)] = function(self, val) 
		self[key] = val
	end
end
Logger.setLogLevel = Logger.setLevel


--[[Get a previously registered logger with the specified `modDir`. You can optionally specify a filepath.]]
---@param modDir string name of the mod
---@param filePath string? the relative filepath of this logger
---@return Logger? logger
function Logger.get(modDir, filePath)
    local arr = registeredLoggers[modDir]
    if not arr then return end
    if not filePath then return arr[1] end
    for _, logger in ipairs(arr) do
        if logger.filePath == filePath then
            return logger
        end
    end
end

---@deprecated Use Logger.get
Logger.getLogger = Logger.get


---@deprecated Use compare the `level` field directly with `Logger.LEVEL.DEBUG` (etc.)
function Logger:doLog(level)
	if LOG_LEVEL[level] then
		return self.sharedData.level >= LOG_LEVEL[level]
	elseif LOG_LEVEL_STRINGS[level] then
		return self.sharedData.level >= level
	end
end

--- Gets a string representation of a given logging level.
---@param level Logger.LEVEL? The logging level to get the string for. Default: `self.level`.
---@return string
function Logger:getLevelStr(level)
    return LOG_LEVEL_STRINGS[level or self.level]
end

-- Returns all the siblings of this logger
---@return Logger[]
function Logger:getSiblings()
    return registeredLoggers[self.sharedData.modDir]
end

--- returns all the loggers for a given mod directory (can pass a Logger as well)
---@param modDirOrLogger string|Logger
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

---@param level Logger.LEVEL
---@param offset integer? for the line number to be accurate, this method assumes it's getting called 2 levels deep (i.e.). the offset adjusts this
---@return Logger.Record record
function Logger:makeRecord(level, offset)
    return {
        level = level,
        stackLevel = 3 + (offset or 0),
        timestamp = self.sharedData.includeTimestamp   and socket.gettime(),
        lineNumber = self.sharedData.includeLineNumber and debug.getinfo(3 + (offset or 0), "l").currentline
    }
end

---@protected
---@param record Logger.Record
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
	local filePath, lineNo = self.filePath, record.lineNumber
	if filePath then
		if sharedData.abbreviateHeader then  
			local pathParts = filePath:split("/")
			local numParts = #pathParts
			for i = 1, numParts - 1 do
				pathParts[i] = pathParts[i]:sub(1, 1)
			end
			pathParts[numParts] = pathParts[numParts]:gsub(".lua$", "")
			filePath = table.concat(pathParts, "/")
		end
		if lineNo then
			table.insert(strs, fmt("%s:%-3i", filePath, lineNo))
		else
			table.insert(strs, filePath)
		end
	elseif lineNo then
		table.insert(strs, fmt("line: %-3i", lineNo))
	end

	-- add the log level string
	do
		local level = sharedData.level
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
                               		  or fmt(     "%02d:%02d.%03d",        minutes, seconds, milliseconds))
    end
	-- Notice the trailing space!
    return fmt("[%s] ", table.concat(strs, " | "))
end


-- Writes the string to a file or to the console
---@protected
function Logger:write(str)
    if self.outputFile then
        self.outputFile:write(str, "\n")
        self.outputFile:flush()
    else
        print(str)
    end
end


-- calls format on the record and writes it to the appropriate location
---@protected
---@param record Logger.Record
---@param ... any
function Logger:writeRecord(record, ...)
    self:write(self.sharedData.formatter(self, record, ...))
end

-- Make dummy methods for typehinting purposes.
-- This code will be deleted once the API has been finalized and the documentation has been made.
do 

    --- Write an `error` level debug message.
    ---@param msg string|fun(...): ...|any Message, or a function that returns all the arguments.
    ---@param ... any Additional arguments to pass when formatting.
    function Logger:error(msg, ...) end

    --- Write an `warn` level debug message.
    ---@param msg string|fun(...): ...|any Message, or a function that returns all the arguments.
    ---@param ... any Additional arguments to pass when formatting.
    function Logger:warn(msg, ...) end

    --- Write an `info` level debug message.
    ---@param msg string|fun(...): ...|any Message, or a function that returns all the arguments.
    ---@param ... any Additional arguments to pass when formatting.
    function Logger:info(msg, ...) end

    --- Write an `debug` level debug message.
    ---@param msg string|fun(...): ...|any Message, or a function that returns all the arguments.
    ---@param ... any Additional arguments to pass when formatting.
    function Logger:debug(msg, ...) end

    --- Write an `trace` level debug message.
    ---@param msg string|fun(...): ...|any Message, or a function that returns all the arguments.
    ---@param ... any Additional arguments to pass when formatting.
    function Logger:trace(msg, ...) end

end

-- Make the logging functions
---@param levelStr string
for levelStr, level in pairs(LOG_LEVEL) do
    -- e.g., "DEBUG" -> "debug"
    ---@param self Logger
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