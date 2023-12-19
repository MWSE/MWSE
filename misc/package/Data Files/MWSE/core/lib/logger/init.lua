--[[ # Logger 

Based on the `mwseLogger` made by Merlord.

Current version made by herbert100.
]]


local colors = require("logger.colors")



local LoggerMetatable = {
    -- make new loggers by calling `Logger`
    __call=function (cls, ...) return cls.new(...) end,
    __tostring = function(self) return ("Logger") end
}




---@type table<string, Logger>
local loggers = {}


--[[## Logger

### Creating a new `Logger`

To create a new logger for a mod with the specified `modName`, you can write one of the following four things:
1) `log = Logger("MODNAME")`
2) `log = Logger{modName="MODNAME", level=LEVEL?, ...}`
3) `log = Logger.new("MODNAME")`
4) `log = Logger.new{modName="MODNAME", level=LEVEL?, ...}`

tring representing a numerical code (ie "NONE", "PROBLEMS", ..., "TRACE")

### Writing log messages

- you can log things by writing `log:info`, `log:warn`, `log:error`, `log:debug`, etc.
- additionally, you can write debug messages by typing `log(str)`

**Passing multiple parameters:**
If multiple parameters are passed, then `string.format` will be called on the first parameter. You have two options for formatting strings:
1) pass the formatting options as regular arguments.
    - e.g., `log:trace("The %s today is %i %s", "weather", 20, "degrees")`
    - So, `log:trace(s, ...)` and `log:trace(string.format(s, ...))` will print the same things.
    - the only difference is that in the first case, `string.format` will be called AFTER the `log.level` is checked.
2) pass the formatting options as a `function`.
    - e.g., `log:trace("The %s today is %i, %s", function() return "weather", 20, "degrees" end)
    - So, `log:trace(s, func)` and `log:trace(string.format(s, func() )) will print the same things.
    - The key difference is that both `string.format` AND `func` will only be called after the `log.level` is checked.
    - This could be very nice for performance reasons, if you're printing complicated debugging messages.


**Passing a single parameter:**
If only a single parameter is passed, `string.format` will NOT be called. This is to avoid unexpected errors from writing strings 
that inadvertently contain formatting specifiers.



### Updating the `log.level`

Once a `log` has been created, the log level can be changed by using the `log:setLevel(Logger.LEVEL)` method.
You can specify a string or number.
e.g. to set the `log.level` to "DEBUG", you can write any of the following:
1) `log:setLevel("DEBUG")`
2) `log:setLevel(2)`
3) `log:setLevel(Logger.LEVEL.DEBUG)`

### Accessing the `log.level`

The current logging level can be accessed by writing `log.level` or `#log`.

The `log.level` is stored internally as an `integer`, so that you can easily check to see if the `log.level` is above a certain value.
The log levels are:
NONE = 0
ERROR = 1
WARN = 2
INFO = 3
DEBUG = 4
TRACE = 5

So, to check if the logging level is "DEBUG" or higher, you can write
- `#log >= 4`

Additionally, you can write
- `log >= 4`

**NOTE:** The `log` **must** be on the `>=` side of the comparison if using this syntax. Writing `log <= 4` will result in an error. 
]]
---@class Logger
---@field modName string the name of the mod this logger is for
---@field level Logger.LEVEL
---@field moduleName string? the module this logger belongs to, or `nil` if it's a general purpose logger
---@field children table<string, Logger> all the child logggers, indexed by their `moduleName`
---@field useColors boolean should colors be used when writing log statements? Default: `false`
---@field writeToFile boolean if true, we will write the log contents to a file with the same name as this mod.
---@field includeTimestamp boolean should the current time be printed when writing log messages? Default: `false`
---@field file file*? the file the log is being written to, if `writeToFile == true`, otherwise its nil.
local Logger = {
    ---@type table<Logger.LEVEL_STRING, Logger.LEVEL>
    LEVEL = {
        NONE = 0,
        ERROR = 1,
        WARN = 2,
        INFO = 3,
        DEBUG = 4,
        TRACE = 5
    }
}
setmetatable(Logger, LoggerMetatable)


---@alias Logger.LEVEL
---|`Logger.LEVEL.NONE`     Nothing will be printed
---|`Logger.LEVEL.ERROR`    Error messages will be printed
---|`Logger.LEVEL.WARN`     Warning messages will be printed
---|`Logger.LEVEL.INFO`     Crucial information will be printed
---|`Logger.LEVEL.DEBUG`    Debug messages will be printed
---|`Logger.LEVEL.TRACE`    Many debug messages will be printed
---|0                       NONE: Nothing will be printed
---|1                       ERROR: Error messages will be printed
---|2                       WARN: Warning messages will be printed
---|3                       INFO: Only crucial information will be printed
---|4                       DEBUG: Debug messages will be printed
---|5                       TRACE: Many debug messages will be printed

---@alias Logger.LEVEL_STRING
---|"NONE"      Nothing will be printed
---|"ERROR"     Error messages will be printed
---|"WARN"      Warning messages will be printed
---|"INFO"      Crucial information will be printed
---|"DEBUG"     Debug messages will be printed
---|"TRACE"     Many debug messages will be printed




-- metatable used by log objects
local logMetatable = {
    __call = function(self, ...) self:debug(...) end,
    __lt = function(num, self) return num < self.level end,
    __le = function(num, self) return num <= self.level end,
    __len = function(self) return self.level end,
    __index = Logger,
    ---@param self Logger
    ---@param str string
    __concat = function(self, str)
        return self:makeChild(str)
    end,
    __tostring = function(self)
        if self.moduleName then 
            return string.format(
                "Logger(modName=\"%s\", moduleName=\"%s\", level=%i, levelStr=%s)",
                self.modName, self.moduleName, self.level, table.find(Logger.LEVEL, self.level)
            )
        else
            return string.format(
                    "Logger(modName=\"%s\", level=%i, levelStr=%s)",
                    self.modName, self.level, table.find(Logger.LEVEL, self.level)
                )
        end
    end,
}



---@class Logger.newParams
---@field modName string the name of the mod this logger is for
---@field level Logger.LEVEL|Logger.LEVEL_STRING|nil the log level to set this object to. Default: "INFO"
---@field moduleName string? the module this logger belongs to, or `nil` if it's a general purpose logger
---@field useColors boolean? should colors be used when writing log statements? Default: `false`
---@field includeTimestamp boolean? should the current time be printed when writing log messages? Default: `false`
---@field writeToFile string|boolean|nil whether to write the log messages to a file, or the name of the file to write to. if `false` or `nil`, messages will be written to `MWSE.log`


--[[##Create a new logger for a mod with the specified `modName`. 

New objects can be created in the following ways:
- `log = Logger("MODNAME")`
- `log = Logger{modName="MODNAME", level=LEVEL?, ...}`
- `log = Logger.new("MODNAME")`
- `log = Logger.new{modName="MODNAME", level=LEVEL?, ...}`

In addition to `modName`, you may specify
- `useColors`: whether to use colors when printing log messages.
- `level`: the logging level to start this logger at. This is the same as making a new logger and then calling `log:setLevel(level)`
- `writeToFile`: either a boolean saying we should write to a file, or the name of a file to write to. If false (the default), then log messages will be written to `MWSE.log`
]]
---@param params Logger.newParams
function Logger.new(params)
    -- if it's just a string, treat it as the `modName`
    if type(params) == "string" then
        params = {modName=params} 
    end
    -- do some error checking to make sure `params` are correct
    assert(params.modName ~= nil, "Error: Could not create a Logger because modName was nil.")
    assert(type(params.modName) == "string", "Error: Could not create a Logger. modName must be a string.")

    if params.moduleName == nil then
        params.modName, params.moduleName = unpack(params.modName:split("/"))
    end

    local parentLog
    -- if a logger for the given mod has already been made
    if loggers[params.modName] then 
        local log = loggers[params.modName]
        
        if params.moduleName ~= nil then
            -- we were given a `moduleName`, so we should check if that given child log exists
            if log.children[params.moduleName] then
                -- return the child if it exists
                return log.children[params.moduleName]
            else
                -- otherwise, mark that the log we're making is a child of this log.
                parentLog = log
                -- copy over information from parentLog into `params`, taking care to not overwrite `params`
                for _,k in ipairs{"useColors", "writeToFile", "level", "includeTimestamp"} do
                    if params[k] == nil then
                        params[k] = parentLog[k]
                    end
                end

            end
        else
            return log
        end
    end

    ---@type Logger
    local log = {
        modName = params.modName,
        moduleName = params.moduleName,
        useColors=params.useColors or false,
        writeToFile = params.writeToFile or false,
        level = Logger.LEVEL.INFO, -- we'll set it with the dedicated function so we can do fancy stuff
        includeTimestamp = params.includeTimestamp or false,
        children = {},
    }

    if log.writeToFile then
        local fileName
        if type(log.writeToFile) == "string" then 
            fileName = log.writeToFile
        else
            fileName = log.modName .. ".log"
        end
        log.file = io.open(fileName, "w")
    end
    setmetatable(log, logMetatable)
    
    if parentLog == nil then
        loggers[params.modName] = log
    else
        parentLog.children[params.modName] = log
    end

    log:setLevel(params.level) -- checks if it was `nil`

    return log -- this gets sent right into the `init` method below, but it's metatable gets set first.
end









local COLORS = {
	NONE =  "white",
	WARN =  "bright yellow",
	ERROR =  "bright red",
	INFO =  "white",
    DEBUG = "bright green",
    TRACE =  "bright white",
}

--[[Get a previously registered logger with the specified `modName`.

**Note:** Calling `Logger.new(modName)` will also return a previously registered logger if it exists.
The difference between `.get` and `.new` is that if a logger does not exist, `.new` will create one, while `.get` will not.
]]
---@param modName string name of the mod
---@param moduleName string the name of the module
---@return Logger? logger
function Logger.get(modName, moduleName)
    if moduleName == nil then
        modName, moduleName = unpack(modName:split("/"))
    end
    local log = loggers[modName]
    if log then
        if moduleName ~= nil then 
            return log.children[moduleName]
        end
        return log
    end
end


function Logger:getLevelStr()
    return table.find(Logger.LEVEL, self.level)
end


function Logger:makeChild(moduleName)
    return self.new{
        modName=self.modName,
        level=self.level,
        moduleName=moduleName, 
        useColors=self.useColors,
        writeToFile=self.writeToFile,
        includeTimestamp=self.includeTimestamp
    }
end
local LogLevel = Logger.LEVEL


--- get the parent of this logger. only really used internally
---@return Logger
function Logger:getParent()
    return loggers[self.modName]
end


--[[Change the current logging level. You can specify a string or number.
e.g. to set the `log.level` to "DEBUG", you can write any of the following:
1) `log:setLevel("DEBUG")`
2) `log:setLevel(4)`
3) `log:setLevel(Logger.LEVEL.DEBUG)`
]]
---@param self Logger
---@param level Logger.LEVEL|Logger.LEVEL_STRING
function Logger:setLevel(level)
    

    local lvl -- the actual level we should use, instead of a string or something
    if LogLevel[level] then 
        lvl = LogLevel[level]
    elseif type(level) == "number" then 
        if LogLevel.NONE <= level and level <= LogLevel.TRACE then
            ---@diagnostic disable-next-line: assign-type-mismatch
            lvl = level
        end
    elseif type(level) == "string" and LogLevel[level:upper()] then 
        lvl = LogLevel[level:upper()]
    end
    if lvl then
        -- make sure we updated all registered loggers for the mod, not just this one
        local parent = self:getParent()
        parent.level = lvl
        for _, child in pairs(parent.children) do
            child.level = lvl
        end
    end
end


---@param includeTimestamp boolean Whether logs should use timestamps
function Logger:setIncludeTimestamp(includeTimestamp)
    -- we need to know what to do
    if includeTimestamp == nil then return end
    local parent = self:getParent()

    parent.includeTimestamp = includeTimestamp
    for _, logger in pairs(parent.children) do
        logger.includeTimestamp = includeTimestamp
    end

end

--- for internal use only. it generates the header message that will be enclosed in square brackets when printing log messages.
function Logger:_makeHeader(LogStr)
    local header_t = {}
    
    if self.moduleName ~= nil then
        header_t[1] = string.format("%s (%s):", self.modName, self.moduleName)
    else
        header_t[1] = string.format("%s:", self.modName)
    end
    if self.useColors then
        -- e.g. turn "ERROR" into "ERROR" (but written in red)
        LogStr = colors(string.format("%%{%s}%s", COLORS[LogStr], LogStr))
    end
    header_t[#header_t+1] = LogStr
    if self.includeTimestamp then
        local socket = require("socket")
        local timestamp = socket.gettime()
        local milliseconds = math.floor((timestamp % 1) * 1000)
        timestamp = math.floor(timestamp)

        -- convert timestamp to a table containing time components
        local timeTable = os.date("*t", timestamp)

        -- format time components into H:M:S:MS string
        local formattedTime = string.format(": %02d:%02d:%02d.%03d", timeTable.hour, timeTable.min, timeTable.sec, milliseconds)
        header_t[#header_t+1] = formattedTime
    end
    return table.concat(header_t," ")
end

--- write to log. only used internally
---@param LogStr Logger.LEVEL_STRING
function Logger:write(LogStr,...)
    local s, s1, s2
    local n, header = select("#",...), self:_makeHeader(LogStr)
    
    if n == 1 then
        s = string.format("[%s] %s", header, ...)
    else
        s1, s2 = ...
        if n == 2 and type(s2) == "function" then 
            s = string.format("[%s] %s", header, string.format(s1, s2()))
        else
            s = string.format("[%s] %s", header, string.format(s1, select(2,...)))
        end
    end

    
    if self.writeToFile ~= false then
        self.file:write(s .. "\n"); self.file:flush()
    else
       print(s)
    end
end





--[[ Write an error message, if the current `log.level` permits it. 

**Passing multiple parameters:**
If multiple parameters are passed, then `string.format` will be called on the first parameter. You have two options for formatting strings:
1) pass the formatting options as regular arguments.
    - e.g., `log:trace("The %s today is %i %s", "weather", 20, "degrees")`
    - So, `log:trace(s, ...)` and `log:trace(string.format(s, ...))` will print the same things.
    - the only difference is that in the first case, `string.format` will be called AFTER the `log.level` is checked.
2) pass the formatting options as a `function`.
    - e.g., `log:trace("The %s today is %i, %s", function() return "weather", 20, "degrees" end)
    - So, `log:trace(s, func)` and `log:trace(string.format(s, func() )) will print the same things.
    - The key difference is that both `string.format` AND `func` will only be called after the `log.level` is checked.
    - This could be very nice for performance reasons, if you're printing complicated debugging messages.


**Passing a single parameter:**
If only a single parameter is passed, `string.format` will NOT be called. This is to avoid unexpected errors from writing strings 
that inadvertently contain formatting specifiers.
]]
---@param ... string the strings to write the log
function Logger:error(...)
    if self.level >= LogLevel.ERROR then self:write("ERROR", ...) end
end


--[[ Write a warning message, if the current `log.level` permits it. 

**Passing multiple parameters:**
If multiple parameters are passed, then `string.format` will be called on the first parameter. You have two options for formatting strings:
1) pass the formatting options as regular arguments.
    - e.g., `log:trace("The %s today is %i %s", "weather", 20, "degrees")`
    - So, `log:trace(s, ...)` and `log:trace(string.format(s, ...))` will print the same things.
    - the only difference is that in the first case, `string.format` will be called AFTER the `log.level` is checked.
2) pass the formatting options as a `function`.
    - e.g., `log:trace("The %s today is %i, %s", function() return "weather", 20, "degrees" end)
    - So, `log:trace(s, func)` and `log:trace(string.format(s, func() )) will print the same things.
    - The key difference is that both `string.format` AND `func` will only be called after the `log.level` is checked.
    - This could be very nice for performance reasons, if you're printing complicated debugging messages.


**Passing a single parameter:**
If only a single parameter is passed, `string.format` will NOT be called. This is to avoid unexpected errors from writing strings 
that inadvertently contain formatting specifiers.
]]
---@param ... string the strings to write the log
function Logger:warn(...)
    if self.level >= LogLevel.WARN then self:write("WARN", ...) end
end

--[[ Write an info message, if the current `log.level` permits it. 

**Passing multiple parameters:**
If multiple parameters are passed, then `string.format` will be called on the first parameter. You have two options for formatting strings:
1) pass the formatting options as regular arguments.
    - e.g., `log:trace("The %s today is %i %s", "weather", 20, "degrees")`
    - So, `log:trace(s, ...)` and `log:trace(string.format(s, ...))` will print the same things.
    - the only difference is that in the first case, `string.format` will be called AFTER the `log.level` is checked.
2) pass the formatting options as a `function`.
    - e.g., `log:trace("The %s today is %i, %s", function() return "weather", 20, "degrees" end)
    - So, `log:trace(s, func)` and `log:trace(string.format(s, func() )) will print the same things.
    - The key difference is that both `string.format` AND `func` will only be called after the `log.level` is checked.
    - This could be very nice for performance reasons, if you're printing complicated debugging messages.


**Passing a single parameter:**
If only a single parameter is passed, `string.format` will NOT be called. This is to avoid unexpected errors from writing strings 
that inadvertently contain formatting specifiers.
]]
---@param ... string the strings to write the log
function Logger:info(...)
    if self.level >= LogLevel.INFO then self:write("INFO", ...) end
end


--[[ Write a debug message, if the current `log.level` permits it. 

**Passing multiple parameters:**
If multiple parameters are passed, then `string.format` will be called on the first parameter. You have two options for formatting strings:
1) pass the formatting options as regular arguments.
    - e.g., `log:trace("The %s today is %i %s", "weather", 20, "degrees")`
    - So, `log:trace(s, ...)` and `log:trace(string.format(s, ...))` will print the same things.
    - the only difference is that in the first case, `string.format` will be called AFTER the `log.level` is checked.
2) pass the formatting options as a `function`.
    - e.g., `log:trace("The %s today is %i, %s", function() return "weather", 20, "degrees" end)
    - So, `log:trace(s, func)` and `log:trace(string.format(s, func() )) will print the same things.
    - The key difference is that both `string.format` AND `func` will only be called after the `log.level` is checked.
    - This could be very nice for performance reasons, if you're printing complicated debugging messages.


**Passing a single parameter:**
If only a single parameter is passed, `string.format` will NOT be called. This is to avoid unexpected errors from writing strings 
that inadvertently contain formatting specifiers.
]]
---@param ... string the strings to write the log
function Logger:debug(...)
    if self.level >= LogLevel.DEBUG then self:write("DEBUG", ...) end
end

--[[ Write a trace message, if the current `log.level` permits it. 

**Passing multiple parameters:**
If multiple parameters are passed, then `string.format` will be called on the first parameter. You have two options for formatting strings:
1) pass the formatting options as regular arguments.
    - e.g., `log:trace("The %s today is %i %s", "weather", 20, "degrees")`
    - So, `log:trace(s, ...)` and `log:trace(string.format(s, ...))` will print the same things.
    - the only difference is that in the first case, `string.format` will be called AFTER the `log.level` is checked.
2) pass the formatting options as a `function`.
    - e.g., `log:trace("The %s today is %i, %s", function() return "weather", 20, "degrees" end)
    - So, `log:trace(s, func)` and `log:trace(string.format(s, func() )) will print the same things.
    - The key difference is that both `string.format` AND `func` will only be called after the `log.level` is checked.
    - This could be very nice for performance reasons, if you're printing complicated debugging messages.


**Passing a single parameter:**
If only a single parameter is passed, `string.format` will NOT be called. This is to avoid unexpected errors from writing strings 
that inadvertently contain formatting specifiers.
]]
---@param ... string the strings to write the log
function Logger:trace(...)
    if self.level >= LogLevel.TRACE then self:write("TRACE", ...) end
end


do

    -- =========================================================================
    -- table log formatting
    -- =========================================================================
---@class Logger.writeParams
---@field sep string|boolean|nil if `true` or a `string`, then multiline printing will be used with this as the separator. (if `true`, then `\n\t` will be used).
---@field msg string? if passed, this is the message that will be formatted using `args`. otherwise, only `args` will be printed
---@field args table|(fun(): table) this is what will be passed to `string.format` or `table.concat`. if it's a `table`, it will be unpacked. if it's a `function`, it will be called and then unpacked. using a function is more efficient in the case that the message may not always be printed (e.g. debug messages)

---@param LogStr Logger.LEVEL_STRING
---@param t Logger.writeParams
function Logger:writet(LogStr, t)
    local header = self:_makeHeader(LogStr)
    local s
    
    -- args is going to be what we're actually printing if `msg == nil`. it will support associative tables.
    -- _args is just to deal with the case that `t.args` is a function
    local args, _args = {}, (type(t.args) == "function" and t.args()) or t.args

    local i,n = 1, #_args

    if t.msg then
        s = string.format("[%s] %s", header, t.msg:format(unpack(_args)))
    else
        local sep = (type(t.sep) == "string" and t.sep) or "\n\t"

        for k,v in pairs(_args) do
            if i <= n then      
                table.insert(args,v)
            else
                table.insert(args,string.format("%s=%s",k,v))
            end
            i = i + 1
        end
        s = string.format("[%s] %s", header, table.concat(args, sep))
    end

    
    if self.writeToFile ~= false then
        self.file:write(s .. "\n"); self.file:flush()
    else
       print(s)
    end
end






--[[## Write a warning message by passing in options as a table. 

The two main syntaxes are:
1) `sep`: defaults to "\n\t". The separator to use when printing multiple strings/numbers.
2) `msg`: If passed, then this message will be formatted with `string.format`, using `args` as the format parameters.

There are two ways to pass `args`:
1) As a table, e.g.,  `log:debug{args={"string1", "string2"}}`
2) As a function that returns a table, e.g., `log:debug{args=function() return {"string1", "string2"} end}`

Here are some examples:
- `log:warnt{msg="This %s message", args={"is a"}}` 
    --> "This is a message"
- `log:warnt{msg="The date is %s and the weather is %i degrees", args=function() return {"Tuesday", 20} end}`
    --> "The date is Tuesday and the weather is 20 degrees"
- `log:warnt{args={"Hello", "World", "It's lovely outside"}}`
    --> "Hello\n\tWorld\n\tIt's lovely outside"
- `log:warnt{sep=", ", args={"number1 = 10", "number2 = 15", "number3 = 20"}}`
    --> "number1 = 10, number2 = 15, number3 = 20"
]]
---@param self Logger
---@param writeParams Logger.writeParams
function Logger:warnt(writeParams)
    if self.level >= LogLevel.WARN then 
        self:writet("WARN", writeParams)
    end
end


--[[## Write an error message by passing in options as a table. 

The two main syntaxes are:
1) `sep`: defaults to "\n\t". The separator to use when printing multiple strings/numbers.
2) `msg`: If passed, then this message will be formatted with `string.format`, using `args` as the format parameters.

There are two ways to pass `args`:
1) As a table, e.g.,  `log:debug{args={"string1", "string2"}}`
2) As a function that returns a table, e.g., `log:debug{args=function() return {"string1", "string2"} end}`

Here are some examples:
- `log:warnt{msg="This %s message", args={"is a"}}` 
    --> "This is a message"
- `log:warnt{msg="The date is %s and the weather is %i degrees", args=function() return {"Tuesday", 20} end}`
    --> "The date is Tuesday and the weather is 20 degrees"
- `log:warnt{args={"Hello", "World", "It's lovely outside"}}`
    --> "Hello\n\tWorld\n\tIt's lovely outside"
- `log:warnt{sep=", ", args={"number1 = 10", "number2 = 15", "number3 = 20"}}`
    --> "number1 = 10, number2 = 15, number3 = 20"
]]
---@param writeParams Logger.writeParams
function Logger:errort(writeParams)
    if self.level >= LogLevel.ERROR then 
        self:writet("ERROR", writeParams)
    end
end

--[[## Write an info message by passing in options as a table. 

The two main syntaxes are:
1) `sep`: defaults to "\n\t". The separator to use when printing multiple strings/numbers.
2) `msg`: If passed, then this message will be formatted with `string.format`, using `args` as the format parameters.

There are two ways to pass `args`:
1) As a table, e.g.,  `log:debug{args={"string1", "string2"}}`
2) As a function that returns a table, e.g., `log:debug{args=function() return {"string1", "string2"} end}`

Here are some examples:
- `log:warnt{msg="This %s message", args={"is a"}}` 
    --> "This is a message"
- `log:warnt{msg="The date is %s and the weather is %i degrees", args=function() return {"Tuesday", 20} end}`
    --> "The date is Tuesday and the weather is 20 degrees"
- `log:warnt{args={"Hello", "World", "It's lovely outside"}}`
    --> "Hello\n\tWorld\n\tIt's lovely outside"
- `log:warnt{sep=", ", args={"number1 = 10", "number2 = 15", "number3 = 20"}}`
    --> "number1 = 10, number2 = 15, number3 = 20"
]]
---@param self Logger
---@param writeParams Logger.writeParams
function Logger:infot(writeParams)
    if self.level >= LogLevel.INFO then 
        self:writet("INFO", writeParams)
    end
end

--[[## Write a debug message by passing in options as a table. 

The two main syntaxes are:
1) `sep`: defaults to "\n\t". The separator to use when printing multiple strings/numbers.
2) `msg`: If passed, then this message will be formatted with `string.format`, using `args` as the format parameters.

There are two ways to pass `args`:
1) As a table, e.g.,  `log:debug{args={"string1", "string2"}}`
2) As a function that returns a table, e.g., `log:debug{args=function() return {"string1", "string2"} end}`

Here are some examples:
- `log:warnt{msg="This %s message", args={"is a"}}` 
    --> "This is a message"
- `log:warnt{msg="The date is %s and the weather is %i degrees", args=function() return {"Tuesday", 20} end}`
    --> "The date is Tuesday and the weather is 20 degrees"
- `log:warnt{args={"Hello", "World", "It's lovely outside"}}`
    --> "Hello\n\tWorld\n\tIt's lovely outside"
- `log:warnt{sep=", ", args={"number1 = 10", "number2 = 15", "number3 = 20"}}`
    --> "number1 = 10, number2 = 15, number3 = 20"
]]
---@param self Logger
---@param writeParams Logger.writeParams
function Logger:debugt(writeParams)
    if self.level >= LogLevel.DEBUG then 
        self:writet("DEBUG", writeParams)
    end
end

--[[## Write a trace message by passing in options as a table. 

The two main syntaxes are:
1) `sep`: defaults to "\n\t". The separator to use when printing multiple strings/numbers.
2) `msg`: If passed, then this message will be formatted with `string.format`, using `args` as the format parameters.

There are two ways to pass `args`:
1) As a table, e.g.,  `log:debug{args={"string1", "string2"}}`
2) As a function that returns a table, e.g., `log:debug{args=function() return {"string1", "string2"} end}`

Here are some examples:
- `log:warnt{msg="This %s message", args={"is a"}}` 
    --> "This is a message"
- `log:warnt{msg="The date is %s and the weather is %i degrees", args=function() return {"Tuesday", 20} end}`
    --> "The date is Tuesday and the weather is 20 degrees"
- `log:warnt{args={"Hello", "World", "It's lovely outside"}}`
    --> "Hello\n\tWorld\n\tIt's lovely outside"
- `log:warnt{sep=", ", args={"number1 = 10", "number2 = 15", "number3 = 20"}}`
    --> "number1 = 10, number2 = 15, number3 = 20"
]]
---@param self Logger
---@param writeParams Logger.writeParams
function Logger:tracet(writeParams)
    if self.level >= LogLevel.TRACE then 
        self:writet("TRACE", writeParams)
    end
end

end




---@class Logger.addToMCMParams
---@field component mwseMCMPage|mwseMCMSideBarPage|mwseMCMCategory The Page/Category to which this setting will be added.
---@field config table? the config to store the logLevel in. Recommended. If not provided, the `logLevel` will reset to "INFO" each time the mod is launched.
---@field createCategory boolean? should a subcategory be made for the log settings? Default: a new category will be created, so long as `component` is not a `mwseMCMCategory`
---@field label string? the label to be shown for the setting. Default: "Log Settings"
---@field description string? The description to show for the log settings. Usually not necesssary. If not provided, a default one will be provided.

--- Add this logger to the passed MCM category/page. You can pass arguments in a table or directly as function parameters.
---@param componentOrParams Logger.addToMCMParams|table The parameters, or the Page/Category to which this setting will be added.
---@param config table? the config to store the logLevel in 
---@param createCategory boolean? should a subcategory be made for the log settings? makes it easier to read the description. default: true
function Logger:addToMCM(componentOrParams, config, createCategory)

    local label, description, component

    -- if it's not a page or category
    if type(componentOrParams) == "table" and not componentOrParams.componentType then 
        component = componentOrParams.component
        config = config or componentOrParams.config
        createCategory = createCategory or componentOrParams.createCategory

        label = componentOrParams.label
        description = componentOrParams.description
    end

    if component == nil then component = componentOrParams end

    if label == nil then label = "Logging Level" end

    if description == nil then 
        description = "\z
            Change the current logging settings. You can probably ignore this setting. A value of 'PROBLEMS' or 'INFO' is recommended, \n\z
            unless you're troubleshooting something. Each setting includes all the log messages of the previous setting. Here is an \z
            explanation of the options:\n\n\t\z
            \z
            NONE: Absolutely nothing will be printed to the log.\n\n\t\z
            \z
            PROBLEMS: If the mod has any problems, those will be written to the log. Nothing else will be written to the log.\n\n\t\z
            \z
            INFO: Some basic behavior of the mod will be logged, but nothing extreme.\n\n\t\z
            \z
            DEBUG: A lot of the inner workings will be logged. You may notice a decrease in performance.\n\n\t\z
            \z
            TRACE: Even more internal workings will be logged. The log file may be hard to read, unless you have a specific thing you're looking for.\z
        \z
    "
    elseif description == false then 
        description = nil
    end

    


    -- `setLevel` makes sure the value passed is a number, so it's chill
    if config then self:setLevel(config.logLevel) end

    
        

    local logSettings -- this is where the new setting will be added.
    
    -- if `createCategory == true` or `createCategory` wasn't specified, and we aren't creating this component inside a category
    if createCategory == true or (createCategory == nil and component.componentType ~= "Category") then
        logSettings = component:createCategory{label="Log Settings", description = description}
    else
        logSettings = component
    end

    local logOptions = {}
    local i
    for str, num in pairs(LogLevel) do
        i = num + 1 -- `Logger.LEVEL` starts at 0, and we want `logOptions` to start at 1.
        logOptions[i] = {label = str, value = num}
    end
    
    logSettings:createDropdown{label =label, description = description, options = logOptions,
        variable = (config and mwse.mcm.createTableVariable{ id = "logLevel", table = config}) or nil,
        callback = function (dropdown)
            self:setLevel(dropdown.variable.value)
            self("updated log level to %i (%s)", self.level, self:getLevelStr())
        end
    }
end

return Logger