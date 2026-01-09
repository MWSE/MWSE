---Mod dependency - checks if a mod is installed and active
---@class MWSE.Metadata.Dependency.Mod : MWSE.Metadata.Dependency
---@field plugin string
---@field version string
---@field mwse-module string

local util = require("dependencyManagement.util")

---@type table<string, fun(dependency: MWSE.Metadata.Dependency.Mod, modVersion: string?): string>
local reasons = {
	plugin = function(dependency)
		return string.format("Plugin \"%s\" is missing or inactive", dependency.plugin)
	end,
	missingMod = function(_dependency)
		return "Mod is missing"
	end,
	versionInvalid = function(dependency)
		return string.format("version requirement is invalid: %s", dependency.version)
	end,
	versionOutdated = function(dependency, modVersion)
		return string.format("Current: %s; Required: %s", modVersion, dependency.version)
	end,
	missingModule = function(dependency)
		return string.format("Module \"%s\" is missing", dependency["mwse-module"])
	end,
}

---@param modId string
---@param dependency MWSE.Metadata.Dependency.Mod
---@param text? string
local function getDownloadButton(modId, dependency, text)
	if not dependency.url
		or not util.isValidUrl(dependency.url) then
		return false
	end
	return {
		text = string.format("%s %s", text or "Download", modId),
		tooltip = string.format('Go to "%s"', dependency.url),
		callback = function()
			os.openURL(dependency.url)
			os.exit()
		end
	}
end

local function insertReason(e)
	if not e.failures[e.modId] then
		e.failures[e.modId] = {
			title = "Mod: " .. e.modId,
			reasons = {}
		}
	end
	if e.dependency and e.dependency.url then
		e.failures[e.modId].resolveButton = getDownloadButton(e.modId, e.dependency)
	end
	table.insert(e.failures[e.modId].reasons, e.reason)
end

---@param dependencyManager DependencyManager
---@param mods table<string, MWSE.Metadata.Dependency.Mod>
---@param failures MWSE.DependencyType.Failure[]
local function doPluginCheck(dependencyManager, mods, failures)
	dependencyManager.logger:debug("Checking plugins")
	for modId, dependency in pairs(mods) do
		dependencyManager.logger:debug("mod: %s", modId)
		assert(type(modId) == "string", "Mod id must be a string")
		assert(type(dependency) == "table", "Dependency must be a table")
		--check plugin
		if dependency.plugin then
			dependencyManager.logger:debug("Checking plugin %s", dependency.plugin)
			local isActive = tes3.isModActive(dependency.plugin)
			if not isActive then
				local reason = reasons.plugin(dependency)
				dependencyManager.logger:error(reason)
				insertReason {
					modId = modId,
					dependency = dependency,
					reason = reason,
					failures = failures
				}
			end
		end
	end
	return failures
end

---@param dependencyManager DependencyManager
---@param mods table<string, MWSE.Metadata.Dependency.Mod>
---@param failures MWSE.DependencyType.Failure[]
local function doVersionCheck(dependencyManager, mods, failures)
	dependencyManager.logger:debug("Checking mod versions")
	for modId, dependency in pairs(mods) do
		dependencyManager.logger:debug("mod: %s", modId)
		--check version
		if dependency.version then
			dependencyManager.logger:debug("Dependency version %s", dependency.version)
			local metadata = toml.loadMetadata(modId)
			local modVersion = metadata and metadata.package and metadata.package.version
			if not modVersion then
				local reason = reasons.missingMod(dependency)
				dependencyManager.logger:error(reason)
				insertReason {
					modId = modId,
					dependency = dependency,
					reason = reason,
					failures = failures
				}
			else
				local operator, dependencyVersion = util.getOperator(dependency.version)
				if not operator then
					local reason = reasons.versionInvalid(dependency)
					dependencyManager.logger:error(reason)
					insertReason {
						modId = modId,
						dependency = dependency,
						reason = reason,
						failures = failures
					}
				end
				local versionMatches
				local success, err = pcall(function()
					dependencyManager.logger:debug("Checking version %s %s %s", dependencyVersion, operator.pattern,
						modVersion)
					versionMatches = operator.func(dependencyVersion, modVersion)
				end)
				if not success then
					dependencyManager.logger:error(err)
					insertReason {
						modId = modId,
						reason = err,
						failures = failures
					}
				elseif not versionMatches then
					local reason = reasons.versionOutdated(dependency, modVersion)
					dependencyManager.logger:error(reason)
					insertReason {
						modId = modId,
						dependency = dependency,
						reason = reason,
						failures = failures
					}
				end
			end
		end
	end
end

---@param dependencyManager DependencyManager
---@param mods table<string, MWSE.Metadata.Dependency.Mod>
---@param failures MWSE.DependencyType.Failure[]
local function doModuleCheck(dependencyManager, mods, failures)
	dependencyManager.logger:debug("Checking mwse modules")
	for modId, dependency in pairs(mods) do
		dependencyManager.logger:debug("mod: %s", modId)
		local module = dependency["mwse-module"]
		if module then
			dependencyManager.logger:debug("Checking mwse module %s", module)
			local found = false
			local moduleLower = module:lower()
			local package = mwse.activeLuaMods[moduleLower]
			if package then
				found = true
				dependencyManager.logger:debug("Module %s found.", module)
				-- Do we want some additional checks here?
			end
			if not found then
				local reason = reasons.missingModule(dependency)
				dependencyManager.logger:error(reason)
				insertReason {
					modId = modId,
					dependency = dependency,
					reason = reason,
					failures = failures
				}
			end
		end
	end
end

return {
	id = "mods",
	---@param dependencyManager DependencyManager
	checkDependency = function(dependencyManager, mods)
		local failures = {}
		doPluginCheck(dependencyManager, mods, failures)
		doVersionCheck(dependencyManager, mods, failures)
		doModuleCheck(dependencyManager, mods, failures)
		if table.size(failures) > 0 then
			return false, failures
		end
		dependencyManager.logger:debug("All mods are active")
		return true
	end
}
