local DependencyType = require("dependencyManagement.DependencyType")
local util = require("dependencyManagement.util")
---@class MWSE.Metadata.Package
---@field name string
---@field version string Semver "MAJOR.MINOR.PATCH"
---@field plugin string
---@field description string
---@field authors string[]
---@field homepage string
---@field repository string

---@class MWSE.Metadata.Tools.MWSE
---@field lua-mod string The path to the main.lua associated with this mod
---@field load-priority number The priority for when this mod is loaded. Lower numbers are loaded first.
---@field wait-until-initialize boolean Whether to wait until the game has initialized before loading this mod.

---@class MWSE.Metadata.Tools
---@field mwse MWSE.Metadata.Tools.MWSE

---Extend this when registering your own dependency type
---@class MWSE.Metadata.Dependency
---@field url string The url to download this dependency from

---@class MWSE.Metadata
---@field package MWSE.Metadata.Package
---@field tools MWSE.Metadata.Tools
---@field dependencies MWSE.Metadata.Dependency[]

---@class DependencyManager.new.params
---@field logLevel mwseLogger.logLevel?
---@field metadata MWSE.Metadata The metadata of the mod using this dependency manager
---@field logger mwseLogger? The logger to use for this dependency manager
---@field showFailureMessage boolean? Whether to show a message box if a dependency fails to load. Defaults to true.

--[[
    This class is used to manage dependencies for a mod.
    It will check if all dependencies are met, and if not
    it will display a message box to the user.
]]
---@class DependencyManager : DependencyManager.new.params
---@field name string The name of the dependency manager
---@field failedDependencies MWSE.DependencyType.Failure[]? The list of failed dependencies
local DependencyManager = {
    ---@type DependencyManager A list of all registered dependency managers
    registeredManagers = {},
}


--Construct a new Dependency Manager
---@param e DependencyManager.new.params
---@return DependencyManager
function DependencyManager.new(e)
    local self = setmetatable({}, { __index = DependencyManager })
    self.logger = e.logger
    self.metadata = e.metadata
    self.showFailureMessage = table.get(e, "showFailureMessage", true)
    local packageName = e.metadata and e.metadata.package and e.metadata.package.name
    self.name = packageName and packageName .. ".DependencyManager"
                or "DependencyManager"
    self.logger = self.logger or require("logger").new {
        modName = "DependencyManager",
        moduleName = packageName,
        level = e.logLevel,
    }
    self.logger:assert(type(self.metadata) == "table",
        "DependencyManager.new: metadata must be a table")
    self.logger:assert(type(self.metadata.package) == "table",
        "DependencyManager.new: metadata.package must be a table")
    return self
end


---Check if all dependencies are met
---@return boolean passed #returns true if all dependencies passed, false if any failed.
function DependencyManager:checkDependencies()
    if not mwse.getConfig("EnableDependencyChecks") then
        return true
    end

    -- Don't check dependencies if a mod's plugin is not active.
    -- Log if there is metadata.toml pointing to a partially installed/uninstalled mod.
    local plugin = self.metadata.package.plugin
    local pluginExists = plugin and util.pluginExists(plugin)

    local luaMod = self.metadata.tools and self.metadata.tools.mwse and self.metadata.tools.mwse["lua-mod"]
    local luaModExists = luaMod and util.luaModExists(luaMod)

    local uncomplete = false
    if luaMod and not luaModExists then
        uncomplete = true
    elseif plugin and not pluginExists then
        uncomplete = true
    end

    if uncomplete then
        self.logger:warn("Metadata file (%s) found pointing to missing mod files:",
            self.metadata.package.name
        )
        if plugin and not pluginExists then
            self.logger:warn("Plugin: \"%s\".", plugin)
        end
        if luaMod and not luaModExists then
            self.logger:warn("MWSE lua-mod: \"%s\".", luaMod)
        end
        self.logger:warn("No dependency checking will be performed.")
        self.logger:warn("This can result from incomplete mod installation/uninstallation.")

        return true
    end

    local pluginDisabled = pluginExists and not tes3.isModActive(plugin)
    if pluginDisabled then
        self.logger:info("Plugin \"%s\" is not active, skipping dependency check.", plugin)
        return true
    end

    self.logger:debug("Checking dependencies for: %s", self.metadata.package.name)
    local failedDependencies = {} ---@type table<string, MWSE.DependencyType.Failure>
    if self.metadata.dependencies then
        for typeId, dependency in pairs(self.metadata.dependencies) do
            assert(type(typeId) == "string", "DependencyManager:checkDependencies: typeId must be a string")
            assert(type(dependency) == "table", "DependencyManager:checkDependencies: dependency must be a table")
            self.logger:debug("Checking dependency: %s", typeId)
            self.logger:debug("dependency: %s", json.encode(dependency))
            local dependencyType = DependencyType.getDependencyType(typeId)
            if dependencyType then
                self.logger:debug("Checking dependency type: %s", typeId)
                local passed, failures = dependencyType:checkDependency(dependency)
                if failures and not passed then
                    for _, failure in pairs(failures) do
                        table.insert(failedDependencies, failure)
                    end
                end
            else
                self.logger:warn("Unknown dependency type: %s", typeId)
            end
        end
    end
    if table.size(failedDependencies) > 0 then
        if self.showFailureMessage then
            self.logger:debug("Dependencies failed to load, adding to registered managers")
            self.failedDependencies = failedDependencies
            table.insert(DependencyManager.registeredManagers, self)
        end
        return false
    end
    self.logger:debug("All dependencies met")
    return true
end

return DependencyManager
