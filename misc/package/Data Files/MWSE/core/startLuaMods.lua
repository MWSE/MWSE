local logger = require("logging.logger")
local dependencyManagerModule = require("dependencyManagement.DependencyManager")

local function execLuaMod(runtime)
	-- Check for dependencies if we need to.
	if (runtime.metadata and runtime.metadata.dependencies) then
		-- Generate a name from the key if we need one.
		local name = runtime.metadata.package and runtime.metadata.package.name
		if (not name) then
			name = runtime.key
		end

		-- Setup our dependencies.
		local dependencyManager = dependencyManagerModule.new({
			metadata = runtime.metadata,
			logger = logger.new{ modName = name, modDir = runtime.key },
			showFailureMessage = false,
		})

		-- Prevent loading if we don't meet dependency requirements.
		if (not dependencyManager:checkDependencies()) then
			runtime.initialized = false
			return
		end
	end

	-- Actually run the mod file.
	local prefix = runtime.legacy_mod and "mod_init" or "main"
	local result, err = pcall(dofile, runtime.key .. "." .. prefix)
	if (not result) then
		mwse.log("[LuaManager] ERROR: Failed to run mod initialization script '%s':", runtime.key .. "." .. prefix)
		if (type(err) == "table") then
			mwse.log(json.encode(err))
		else
			mwse.log(err)
		end
		runtime.initialized = false
		return
	end

	runtime.initialized = true
end

local function execLuaModWrapper(runtime)
	if (runtime.wait_until_initialize) then
		event.register(tes3.event.initialized, function()
			execLuaMod(runtime)
		end, { priority = runtime.load_priority })
	else
		execLuaMod(runtime)
	end
end

local function defaultSorter(a, b)
	-- Core mods go first.
	if (a.core_mod ~= b.core_mod) then
		return a.core_mod
	end

	-- Legacy mods go last.
	if (a.legacy_mod ~= b.legacy_mod) then
		return b.legacy_mod
	end

	-- Then we sort by explicit load order.
	if (a.load_priority ~= b.load_priority) then
		return a.load_priority > b.load_priority
	end

	-- Make sure that requirements are run first.
	if (b.lua_mod_requirements and table.find(b.lua_mod_requirements, a.key)) then
		return true
	end

	-- Finally we default to the original std::filesystem order.
	return a.load_std_order < b.load_std_order
end

-- Gather a list of mods to execute.
local runtimes = table.values(mwse.activeLuaMods) --- @diagnostic disable-line

-- Figure out some common data from the metadata.
for _, runtime in ipairs(runtimes) do
	local metadata = runtime.metadata or {}
	local metadata_tools = metadata.tools or {}
	local metadata_tools_mwse = metadata_tools.mwse or {}

	runtime.load_priority = table.get(metadata_tools_mwse, "load-priority", 0)
	runtime.wait_until_initialize = table.get(metadata_tools_mwse, "wait-until-initialize", false)

	-- Prepare a list of modules we require.
	if (metadata.dependencies and metadata.dependencies.mods) then
		local lua_mod_requirements = {}

		for _, dependency in pairs(metadata.dependencies.mods) do
			local module = dependency["mwse-module"]
			if (module) then
				table.insert(lua_mod_requirements, module)
			end
		end

		if (not table.empty(lua_mod_requirements)) then
			runtime.lua_mod_requirements = lua_mod_requirements
		end
	end
end

-- Sort them as appropriate.
table.sort(runtimes, defaultSorter)

-- And finally run them.
for _, runtime in ipairs(runtimes) do
	execLuaModWrapper(runtime)
end
