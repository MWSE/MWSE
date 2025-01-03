--[[ # Logger 

Based on the `mwseLogger` made by Merlord.

Current version made by herbert100.

Many suggestions and feature ideas given by C3pa.

Credit to Greatness7 as well, for the ideas of adding a `format` parameter and allowing functions to be passed to log messages.
]]

local colors = require("logger.colors")
local socket = require("socket")
local fmt = string.format

local LOG_LEVEL = {
    NONE  = 0,
    ERROR = 1,
    WARN  = 2,
    INFO  = 3,
    DEBUG = 4,
    TRACE = 5
}

local LEVEL_STRINGS = table.invert(LOG_LEVEL)

local COLORS = {
	NONE  = "white",
	WARN  = "bright yellow",
	ERROR = "bright red",
	INFO  = "white",
    DEBUG = "bright green",
    TRACE = "bright white",
}

---@type table<string, Logger[]>
local loggersByModName = {}

local communalKeys = {
    modName = true,
    modDir = true,
    includeLineNumber = true,
    writeToFile = true,
    level = true,
    includeTimestamp = true,
    format = true,
}



-- The time the game launched. Used to write timestamps.
local launchTime = socket.gettime()

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


---@class Logger.Record
---@field msg string|any|fun(...):... arguments passed to Logger:debug, Logger:info, etc
---@field args any[] arguments passed to Logger:debug, Logger:info, etc
---@field level Logger.LEVEL logging level
---@field lineNumber integer? the line number, if enabled for this logger
---@field timestamp number? the timestamp of this message


---@class Logger.new.params
---@field modName string? the name of the mod this logger is for. will be automatically retrieved if not provided
---@field modDir string? the name of the mod this logger is for. will be automatically retrieved if not provided
---@field filePath string? path to the file. will be retrieved if not provided
---@field level Logger.LEVEL? the log level to set this object to. Default: "LEVEL.INFO"
---@field moduleName string|false? the module this logger belongs to, or `nil` if it's a general purpose logger
---@field includeLineNumber boolean? should the current line be printed when writing log messages? Default: `true`
---@field includeTimestamp boolean? should the current time be printed when writing log messages? Default: `false`
---@field writeToFile string|boolean|nil whether to write the log messages to a file, or the name of the file to write to. if `false` or `nil`, messages will be written to `MWSE.log`
---@field format nil|(fun(self: Logger, record: Logger.Record): string) a way to specify how logging messages should be formatted


---@class Logger
---@operator call (Logger.new.params?): Logger
---@field modName string the name of the mod this logger is for
---@field level Logger.LEVEL
---@field filePath string the relative path to the file this logger was defined in.
---@field modDir string
---@field moduleName string? the module this logger belongs to, or `nil` if it's a general purpose logger
---@field includeTimestamp boolean? should the current time be printed when writing log messages? Default: `false`
---@field includeLineNumber boolean should the current time be printed when writing log messages? Default: `false`
---@field file file*? the file the log is being written to, if `writeToFile == true`, otherwise its nil.
---@field error fun(...): string write an error message
---@field warn fun(...): string write an warn message
---@field info fun(...): string write an info message
---@field debug fun(...): string write a debug message
---@field trace fun(...): string write a trace message
---@field new fun(paramsOrModName: Logger.new.params|string|nil): Logger
local Logger = {
    LEVEL = LOG_LEVEL,
    -- modName = nil,
    -- modDir = nil,
    moduleName = nil,
    -- filePath = nil,
    includeLineNumber = true,
    level = 3,
    includeTimestamp = false,
}
setmetatable(Logger, { __tostring = function() return "Logger" end })

-- Metatable used by log objects.
local logMetatable = {
    -- This will be overwritten later so that it's exactly equal to `Logger.debug`. This is needed for line number to be correct.
    __call = function(self, ...) self:debug(...) end,
    __index = Logger,
    __newindex = function(self, k, v)
        if k == "logLevel" then
            -- This will update the log level of every logger.
            self:setLogLevel(v)
        else
            rawset(self, k, v)
        end
    end,
    
    __tostring = function(self)
        return fmt('Logger(modName: "%s", moduleName: "%s", modDir: "%s", level: %i (%s))',
            self.modName, self.moduleName and fmt("%q", self.moduleName), self.modDir, self.level, self:getLevelStr()
        )
    end,
}

-- Updates a key for all loggers.
---@param self Logger
---@param key string|number
---@param value any
local function updateKey(self, key, value)
    for _, logger in ipairs(loggersByModName[self.modName]) do
        logger[key] = value
    end
end

-- Autogenerate methods to set communal keys. Some of these will be overwritten later on.
-- The substring stuff is to to convert the first letter to uppercase, e.g. "setLevel" instead of "setlevel"
for key in pairs(communalKeys) do
    Logger["set" .. key:sub(1,1):upper() .. key:sub(2)] = function(self, value)
        updateKey(self, key, value)
    end
end

-- If we've hit any of these, we've gone too far up the stack.
local badFilePaths = {
    [string.lower("@Data Files\\MWSE\\core\\initialize.lua")] = true,
    [string.lower("@Data Files\\MWSE\\core\\startLuaMods.lua")] = true,
    [string.lower("=[C]")] = true,
}
-- Used to cut off the irrelevant parts of file paths.
local relativePathStart = string.len("@.\\data files\\mwse\\") + 1

---@param knownInfo {modName: string, modDir: string, filePath: string}
local function updateKnownInfo(knownInfo)
    
    -- Make sure there's at least one thing we need to update.
    if knownInfo.modName and knownInfo.modDir and knownInfo.filePath then return end
    
    local info

    do -- get debug info
        local newInfo, srcLower
        local stackLevel = 2
        repeat
            newInfo = debug.getinfo(stackLevel, "S")
            
            if not newInfo then break end -- we've gone too high up the stack

            srcLower = newInfo.source:lower()

            -- If we hit a badFilePath then we've gone too far up the stack.
            -- So we should `break` before updating `info`.
            if badFilePaths[srcLower] then break end
                
            info = newInfo
            stackLevel = stackLevel + 1

            -- Stop iterating if we've gone high up enough to not hit a `mwse\\core\\` file.
            -- Remember, we start out in `mwse\\core\\lib\\logger`,
            --      so we can't bail as soon as we hit the first `mwse\\core\\` filepath.
        until not srcLower:find("^@.\\data files\\mwse\\core\\")
    end
    if not info then 
        print(fmt("[Logger WARN] Mod information for %s was not found!", knownInfo.modName))
        return
    end

    local relativePath = info.source:sub(relativePathStart) ---@type string
    
    local luaRoot, luaParts = nil, {}
    do -- Make luaParts.
        -- We're starting at index 0 so that we can provide special treatment to the first `part` returned by `gmatch`.
        -- (The first `part` will be `"mods"` or `"lib"`, which isn't really a part of the `filePath`, but it's still important.)
        local i = 0
        for part in relativePath:gmatch("[^\\]+") do
            luaParts[i] = part
            i = i + 1
        end
        luaRoot = luaParts[0]
        luaParts[0] = nil
    end
        
    ---@type MWSE.Metadata?, boolean?
    local metadata, hasAuthorName
    
    local authorModDirRoot = luaParts[1] .. "." .. luaParts[2]

    -- Using runtimes will properly detect mod author folders even when the mods dont have any metadata.
    -- Kinda hacky though.
    ---@diagnostic disable-next-line: undefined-field
    local runtime = mwse.activeLuaMods[authorModDirRoot]
    
    if runtime then
        metadata = runtime.metadata
        hasAuthorName = true
    else
        ---@diagnostic disable-next-line: undefined-field
        runtime = mwse.activeLuaMods[luaParts[1]]
        if runtime then
            metadata = runtime.metadata
            hasAuthorName = false
        end
    end

    if hasAuthorName == nil then
        local pathToCheck = table.concat({"Data Files", "MWSE", luaRoot, luaParts[1], "main.lua"}, "\\")
        hasAuthorName = not lfs.fileexists(pathToCheck)
    end

    hasAuthorName = hasAuthorName and #luaParts > 2

    knownInfo.modName = knownInfo.modName
                        or metadata and metadata.package and metadata.package.name
                        or hasAuthorName and luaParts[2] 
                        or luaParts[1]
    
    knownInfo.modDir = knownInfo.modDir
                       or metadata and metadata.tools and metadata.tools.mwse and metadata.tools.mwse["lua-mod"]
                       or hasAuthorName and authorModDirRoot 
                       or luaParts[1]
    
    
    if not knownInfo.filePath then
        local startIndex = hasAuthorName and 3 or 2
        local relevantParts = {}
        local offset = startIndex - 1
        for i=startIndex, #luaParts do
            relevantParts[i - offset] = luaParts[i]
        end
        knownInfo.filePath = table.concat(relevantParts, "/")
    end
end

-- Keys = old constructor parameters. Values = new constructor parameters.
local oldConstructorKeys = {name = "modName", logLevel = "level"}

-- Create a new logger by passing in a table with parameters or by passing in a string with just the `modName`.
---@param paramsOrModName Logger.new.params|string|nil
---@return Logger
function Logger.new(paramsOrModName)
    -- =========================================================================
    -- MAKE THE LOGGER
    -- =========================================================================
    local log, params = {}, {}
    if paramsOrModName then
        if type(paramsOrModName) == "string" then
            log.modName = paramsOrModName
            params.modName = paramsOrModName
        else
            params = paramsOrModName

            -- Support the old constructor.
            -- If this isn't done, the `logLevel` will not be set properly.
            for oldKey, newKey in pairs(oldConstructorKeys) do
                if params[oldKey] then
                    params[newKey] =  params[newKey] or params[oldKey]
                    params[oldKey] = nil
                end
            end

            log.modName = paramsOrModName.modName
            log.level = paramsOrModName.level
            log.filePath = paramsOrModName.filePath
            log.modDir = paramsOrModName.modDir
            log.moduleName = paramsOrModName.moduleName
        end
    end

    if log.modName ~= nil and log.moduleName == nil then
        local index = log.modName:find("/", 1, true)
        if index then
            log.moduleName = log.modName:sub(index+1)
            log.modName = log.modName:sub(1, index-1)
        end
    end
    
    updateKnownInfo(log)

    assert(log.modName ~= nil, "[Logger: ERROR] Could not create a Logger because modName was nil.")
    assert(type(log.modName) == "string", "[Logger: ERROR] Could not create a Logger. modName must be a string.")


    -- First, see if a Logger already exists. If it does, return it and don't do anything else.
    -- (Because we're supposed to be doing a `get` operation in this case.)
    local self = Logger.get(log)
    if self then return self end

    -- =====================================================================
    -- INITIALIZE THE LOGGER
    -- =====================================================================

    self = setmetatable(log, logMetatable)
    
    local loggerTbl = table.getset(loggersByModName, self.modName, {})
    
    -- If there are already loggers with this `modName`, get the most recent one, and then
    --      update this new loggers values to those of the most recent logger.
    -- This is so that loggers "inherit" parameters from their siblings.
    -- Note: It suffices to only fetch the values from the most recent one because Logger settings are 
    --      synchronized between Loggers.
    -- Note: If `self` has no siblings, then `loggerTbl[#loggerTbl]` will be `nil`.
    for k, siblingVal in pairs(loggerTbl[#loggerTbl] or {}) do
        if communalKeys[k] and rawget(self, k) == nil then
            rawset(self, k, siblingVal)
        end
    end

    table.insert(loggerTbl, self)


    -- Only print the warning after trying to import values from prior loggers.
    if not self.modDir then
        self.modDir = self.modName
        if self.moduleName then
            print(fmt("[Logger: WARN] modDir for %q (module %q) was nil!", self.modName, self.moduleName))
        else
            print(fmt("[Logger: WARN] modDir for %q was nil!", self.modName))
        end
    end

    -- Update the values to be those in the `params` table.
    -- This is necessary because of the inheritance code that was run in the previous `for` loop.
    
    for key, paramVal in pairs(params) do
        if communalKeys[key] then
            local newVal = rawget(self, key)
            if newVal == nil then
                newVal = paramVal
            end
            -- doing it this way so that we can respect custom set functions (i.e., `setWriteToFile`)
            Logger["set" .. key:sub(1,1):upper() .. key:sub(2)](self, newVal)
        end
    end
    return self
end


---@param writeToFile string|boolean
---@param updateAllLoggers boolean? Should we update every other `Logger`? Default: true
function Logger:setWriteToFile(writeToFile, updateAllLoggers)
    -- Don't do anything if we weren't given concrete instructions.
    if writeToFile == nil then return end

    if updateAllLoggers == nil then updateAllLoggers = true end

    -- This is kinda awkward, but it means we only have to write code for the case
    --      where we are updating a table of Loggers.
    local loggersToUpdate = updateAllLoggers and loggersByModName[self.modName] or {self}

    -- Not supposed to write to a file? Close all the files and bail.
    if writeToFile == false then
        for _, log in ipairs(loggersToUpdate) do
            if log.file then 
                log.file:close()
                log.file = nil
            end
        end
        return
    end

    for _, log in ipairs(loggersToUpdate) do
        local filename = writeToFile
        -- If it's `true` instead of a `string`, we should generate a valid filename.
        if writeToFile == true then
            -- filename = "Data Files\\MWSE\\mods\\<modDir>\\<filePath>.log"
            filename = fmt("Data Files\\MWSE\\mods\\%s\\%s.log",
                log.modDir:gsub("[%./]", "\\"), 
                log.filePath:gsub("%.lua$", ""):gsub("[%./]", "\\")
            )
        end
        -- close old file and open a new one
        if log.file then log.file:close() end
        log.file = io.open(filename, "w")
    end
end

--[[Change the current logging level. You can specify a string or number.
e.g. to set the `log.level` to "DEBUG", you can write any of the following:
1) `log:setLevel("DEBUG")`
2) `log:setLevel(4)`
3) `log:setLevel(Logger.LEVEL.DEBUG)`
]]
---@param self Logger
---@param level Logger.LEVEL
function Logger:setLevel(level)
    -- No error message if `level` is `nil`.
    if not level then return end
    -- Given a string? Convert it first
    if type(level) == "string" then
        level = LOG_LEVEL[level]
    end
    -- Note: `level` will be a valid logging level if and only if it's in the `LEVEL_STRINGS` table.
    --   (This is because `LEVEL_STRINGS = table.invert(LOG_LEVEL)`)
    if LEVEL_STRINGS[level] then
        updateKey(self, "level", level)
    end

end



--[[Get a previously registered `Logger` with the specified `modDir`.]]
---@param modDir string name of the mod
---@param filePath string? the relative filepath of this logger
---@return Logger? logger
function Logger.getByModDir(modDir, filePath)

    local loggerTbl
    for _, loggers in pairs(loggersByModName) do
        -- Make sure there's at least one logger in this table, and the `modDir`s agree.
        if loggers[1] and loggers[1].modDir == modDir then
            loggerTbl = loggers
            break
        end
    end
    if not loggerTbl then return end

    -- File path wasn't specified? Return the most recent logger.
    if not filePath then 
        return loggerTbl[#loggerTbl]
    end

    -- File path was specified? Return only return a Logger if it matches the specified file path.
    for _, log in ipairs(loggerTbl) do
        if log.filePath == filePath then
            return log
        end
    end
end

---@class Logger.get.params
---@field modName string
---@field moduleName string|false? This will only be checked if it's not `nil`. `false` means: don't don't match module names.
---@field filePath string? This will only be checked if it evaluates to `true`.


-- Get a new logger. You can pass either a `modName`, or a table containing a `modName`, `moduleName`, and/or `filePath`.
-- The `moduleName`s and `filePath`s of loggers will only be checked if the corresponding parameter evaluates to `true`.
---@param p Logger.get.params|string
---@return Logger? logger
function Logger.get(p)
    if not p then return end

    -- Is `p` is already a logger? Bail.
    ---@diagnostic disable-next-line: return-type-mismatch
    if getmetatable(p) == logMetatable then return p end

    if type(p) == "string" then
        p = {modName=p}
    elseif not p.modName then 
        return
    end

    local loggerTbl = loggersByModName[p.modName]

    if not loggerTbl then return end

    local moduleName, filePath = p.moduleName, p.filePath

    if moduleName == nil and filePath == nil then 
        return loggerTbl[1]
    end

    for _, logger in ipairs(loggerTbl) do
        -- Only check for equality if the relevant parameter was passed.
        if  (filePath == nil or filePath == logger.filePath)
        and (moduleName == nil or moduleName == logger.moduleName)
        then
            return logger
        end
    end
end


function Logger:getLevelStr()
    return LEVEL_STRINGS[self.level]
end

--- Returns all the loggers associated with this modName (can pass a Logger as well).
---@param modNameOrParams string|Logger|Logger.new.params
function Logger.getLoggers(modNameOrParams)
    local t = type(modNameOrParams)
    if t == "string" then
        return loggersByModName[modNameOrParams]
    elseif t == "table" and modNameOrParams.modName then
        return loggersByModName[modNameOrParams.modName]
    end
end



---@param level Logger.LEVEL
---@param offset integer? For the line number to be accurate, this method assumes it's getting called 2 levels deep. The `offset` parameter
-- allows this method to be called on different levels of the stack.
---@return Logger.Record record
function Logger:makeRecord(level, offset, ...)
    return {
        level = level,
        timestamp = self.includeTimestamp and socket.gettime() or nil,
        lineNumber = self.includeLineNumber and debug.getinfo(3 + (offset or 0), "l").currentline or nil
    }
end

---@param logger Logger
---@param record Logger.Record
---@return string
local function makeHeader(logger, record)
    
    local name = logger.moduleName and fmt("%s (%s)", logger.modName, logger.moduleName)
                 or logger.modName

    -- We're going to shove various things into here, and then making the string via
    --      `table.concat(headerTbl, " | ")
    local headerTbl
    if not record.lineNumber then
        headerTbl = {name, logger.filePath}
    else -- `record.lineNumber ~= nil` 
        if logger.filePath then
            headerTbl = {name, fmt("%s:%i", logger.filePath, record.lineNumber)}
        else
            headerTbl = {fmt("%s:%i", name, record.lineNumber)}
        end
    end


    local levelStr = LEVEL_STRINGS[record.level]

    if mwse.getConfig("EnableLogColors") then
        -- e.g. turn "ERROR" into "ERROR" (but written in red).
        levelStr = colors(fmt("%%{%s}%s", COLORS[levelStr], levelStr))
    end

    table.insert(headerTbl, levelStr)

    -- If a timestamp was included, then get the time since launch.
    if record.timestamp then
        local timestamp = record.timestamp - launchTime ---@type number
        local milliseconds = math.floor((timestamp % 1) * 1000)
        
        timestamp = math.floor(timestamp)
        local seconds = timestamp % 60
        local minutes = math.floor(timestamp / 60)
        local hours = math.floor(minutes / 60)
        minutes = minutes % 60

        local formattedTime

        -- Only display the hours if they're relevant. (The logger header is long enough as it is.)
        if hours ~= 0 then
            -- Format time components into H:M:S.MS string
            formattedTime = fmt("%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds)
        else
            -- Format time components into M:S.MS string
            formattedTime = fmt("%02d:%02d.%03d", minutes, seconds, milliseconds)
        end
        table.insert(headerTbl, formattedTime)
    end
    return table.concat(headerTbl, " | ")
end

-- Default formatter. Can be overridden by users.
---@param record Logger.Record
---@param msg string|table|any|(fun(...):...) Message to log.
---@param ... string|table|any|(fun(...):...) Additional arguments.
---@return string
function Logger:format(record, msg, ...)
    -- n = number of arguments
    -- arg1 = first argument
    local n, arg1 = select("#", ...), (select(1, ...))
    if n == 0 then
        -- Don't do anything.
    elseif type(msg) == "function" then
        -- Everything was passed as a function. This function can have multiple return values.
        msg = fmt(msg(...))
    elseif type(arg1) == "function" then
        -- Formatting parameters were passed as a function.
        if n == 1 then
            msg = fmt(msg, arg1())
        else
            msg = fmt(msg, arg1(select(2, ...)))
        end
    else
        -- Nothing was passed as a function, so format the message normally.
        msg = fmt(msg, ...)
    end
    return fmt("[%s] %s", makeHeader(self, record), msg)
end

-- Calls `format` on the `record`, and then writes it to the appropriate location. (i.e., MWSE.log or a custom file location)
---@param record Logger.Record
---@param ... any
function Logger:writeRecord(record, ...)
    local str = self:format(record, ...)
    if self.file then
        self.file:write(str, "\n")
        self.file:flush()
    else
        print(str)
    end
end

-- Make the logging functions.
for level_str, level in pairs(LOG_LEVEL) do
    -- e.g., "DEBUG" -> "debug"
    ---@param self Logger
    Logger[string.lower(level_str)] = function(self, msg, ...)
        if self.level >= level then 
            self:writeRecord(self:makeRecord(level), msg, ...)
        end
    end
end

-- I am a very good programmer.
Logger.none = nil

-- Update `__call` to be the same as `debug`. 
-- This is so that the line numbers are pulled correctly in in the `debug.getinfo` call.
logMetatable.__call = Logger.debug


function Logger:assert(condition, msg, ...)
    -- Condition was true? Bail.
    if condition then return end
    -- Can't call `Logger:error` because we need the call to `debug.getinfo` to produce the correct line number.
    -- We aren't using the `writeRecord` method because we also need to call `assert` directly.
    local str = self:format(self:makeRecord(LOG_LEVEL.ERROR), msg, ...)

    if self.level >= LOG_LEVEL.ERROR then
        
        if self.file then
            self.file:write(str, "\n")
            self.file:flush()
        else
            print(str)
        end
    end

    assert(condition, str)

end

-- Write a "Mod initialized" message at the "INFO" logging level.
-- If a `version` is provided (or included in your mod's metadata file), then that will be included in the log message.
---@param version string|number? The version of your mod. If not provided, it will be fetched from the mod's metadata file (if applicable).
function Logger:writeInitializedMessage(version)
    if self.level < Logger.LEVEL.INFO then return end

    if not version then
        local metadata = tes3.getLuaModMetadata(self.modDir)
        version = metadata and metadata.package and metadata.package.version
    end

    local record = self:makeRecord(LOG_LEVEL.INFO)
    -- Need to do it this way so the call to `debug.getinfo` produces the correct line number.
    if version then
        self:writeRecord(record, "Initialized version %s.", version)
    else
        self:writeRecord(record, "Mod initialized.")
    end
end



-- =============================================================================
-- BACKWARDS COMPATIBILITY
-- =============================================================================


-- Support the old Logger API.

---@deprecated Use `setLevel` instead.
---@param level_str string
function Logger:setLogLevel(level_str)
    ---@diagnostic disable-next-line: param-type-mismatch
    self:setLevel(level_str)
end


---@deprecated Use `Logger.get` instead.
Logger.getLogger = Logger.get


---@deprecated You can now write `logger.level <= Logger.LEVEL.DEBUG`. Or, you can just pass all the arguments in with a function. e.g.,
-- `logger("objectType = %s. myData = %s", function() return table.find(tes3.objectType, objType), json.encode(myData) end)`
-- The logging functions will only be evaluated at the appropriate logging level, so there's no performance hit.
function Logger:doLog(level_str)
    -- Make sure they gave us a valid logging level, and that we are at or below that logging level.
    return LOG_LEVEL[level_str] and LOG_LEVEL[level_str] <= self.level
        or false
end

return Logger ---@type Logger