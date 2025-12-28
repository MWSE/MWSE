local DependencyType = require("dependencyManagement.DependencyType")
local DependencyNotifier = require("dependencyManagement.DependencyNotifier")
local DependencyManager = require("dependencyManagement.DependencyManager")
local LOG_LEVEL = "TRACE"
local logger = require("logging.logger").new{
    name = "Dependencies",
    logLevel = LOG_LEVEL
}
---@param file string
---@return boolean
local function isLuaFile(file)
    return file:lower():endswith(".lua")
end
---@param file string
---@return boolean
local function isInitFile(file)
    return file:lower() == "init.lua"
end
---@param file string
---@return boolean
local function isMetadataFile(file)
    return file:lower():endswith("-metadata.toml")
end

--[[
    Register all dependency types in the dependencyManagement.types folder.
]]
local modulePath = "dependencyManagement.types.?"
local path = "Data Files/MWSE/core/lib/dependencyManagement/types"
logger:debug("Registering dependency types from: %s", path)
for file in lfs.dir(path) do
    logger:debug("Registering dependency type: %s", file)
    if isLuaFile(file) and not isInitFile(file) then
        logger:debug("Is lua file: %s", file)
        local module = modulePath:gsub("%?", file:sub(1, -5))
        logger:debug("Module: %s", module)
        local dependencyConfig = require(module)
        dependencyConfig.logLevel = LOG_LEVEL
        logger:debug("Registering dependency type: %s", dependencyConfig.id)
        DependencyType.registerDependencyType(dependencyConfig)
    end
end

local function checkDependencies()
    logger:debug("Checking dependencies")
    --[[
        For each `-metadata.toml` file in Data Files,
        that do not have a `tools.mwse.lua-mod` field,
        create a dependency manager for it and check dependencies
    ]]
    local path = "Data Files"
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            if isMetadataFile(file) then
                logger:debug("Found metadata file: %s", file)
                local filePath = path .. "\\" .. file
                local metadata = toml.loadFile(filePath)
                if not metadata then
                    logger:error("Could not load metadata file: %s", file)
                elseif not metadata.package then
                    logger:error("Metadata file does not contain a package table: %s", file)
                else
                    logger:debug("Checking dependencies for: %s", metadata.package.name or "[unknown]")
                    local manager = DependencyManager.new{
                        metadata = metadata,
                        logLevel = LOG_LEVEL
                    }
                    manager:checkDependencies()
                end
            end
        end
    end
end
checkDependencies()

local function showNextMessage()
    local manager = table.remove(DependencyManager.registeredManagers, 1)
    if manager and manager.failedDependencies then
        local notifier = DependencyNotifier:new {
            logger = manager.logger,
            failedDependencies = manager.failedDependencies,
            packageName = manager.metadata
                and manager.metadata.package
                and manager.metadata.package.name
        }
        notifier:dependenciesFailMessage(showNextMessage)
    end
end

--[[
    Once all mods have been initialized, this function will be called
    to display a message box for each dependency manager that has failed dependencies.
]]
event.register(tes3.event.initialized, function()
    logger:debug("Displaying failed dependencies. Number of dependency managers with failures: %s", #DependencyManager.registeredManagers)
    showNextMessage()

end, { priority = -math.huge })


