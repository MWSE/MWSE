local logging = require("logging.logger")
local logger = logging.new{
    name = "DependencyType",
    logLevel = "INFO",
    modDir = "DependencyManager",
    filePath = "DependencyType.lua",
    moduleName = "DependencyType"
}
---@class MWSE.DependencyType.ResolveButton
---@field text string The text to display on the button.
---@field tooltip string? The text to display when hovering over the button.
---@field callback fun() The action to perform when pressing the button.

---@class MWSE.DependencyType.Failure
---@field title string The title of the failure.
---@field reasons string[] A list of reasons why the dependency is not met.
---@field resolveButton MWSE.DependencyType.ResolveButton? A button to display to resolve the dependency failure.

---@class MWSE.DependencyType.params
---@field id string The unique identifier for this dependency type. All fields in the dependency will be prefixed with this value, for example, "mwse.buildDate" etc.
---@field checkDependency fun(self, dependency): boolean, MWSE.DependencyType.Failure[]? A function that checks if the given dependency is met. If the dependency is not met, the function should return false and a string describing why the dependency is not met.
---@field logLevel mwseLogger.logLevel? The log level to use for this dependency type. Defaults to "DEBUG".

---@class MWSE.DependencyType : MWSE.DependencyType.params
---@field logger mwseLogger The logger for this dependency type.
local DependencyType = {
    registeredDependencyTypes = {}
}

---@param e MWSE.DependencyType.params
function DependencyType.registerDependencyType(e)
    logger:assert(type(e.id) == "string", "DependencyType id must be a string")
    logger:assert(type(e.checkDependency) == "function", "DependencyType check must be a function")

    local dependencyType = e
    ---@cast dependencyType MWSE.DependencyType
    dependencyType.logger = logging.new{
        modName = "DependencyType",
        moduleName = e.id,
        modDir = "dependencyManagment",
        filePath = "DependencyType.lua",
        logLevel = e.logLevel or "INFO"
    }
    DependencyType.registeredDependencyTypes[e.id:lower()] = dependencyType
end

---@return MWSE.DependencyType
function DependencyType.getDependencyType(id)
    return DependencyType.registeredDependencyTypes[id:lower()]
end

return DependencyType