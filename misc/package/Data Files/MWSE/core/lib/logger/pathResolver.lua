local GET_MOD_INFO_MAX_ITERS = 15
local BAD_FILEPATHS = {
	[string.lower("@Data Files\\MWSE\\core\\initialize.lua")] = true,
	[string.lower("@Data Files\\MWSE\\core\\startLuaMods.lua")] = true,
	[string.lower("@.\\Data Files\\MWSE\\core\\lib\\event.lua")] = true,
	[string.lower("@Data Files\\MWSE\\core\\lib\\event.lua")] = true,
	[string.lower("=[C]")] = true,
}

local pathResolver = {}

---Returns the `modName`, `modDir`, and `filepath` of the currently executing file.
---@param offset integer? The stack offset to use when calling `debug.getinfo`. DEFAULT: `0`.
---@return string? modName
---@return string? modDir
---@return string? filepath
function pathResolver.getModNameAndDirAndFilepath(offset)

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
	if not filepath then
		return
	end

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
		local modDirWithAuthorName = pathParts[1] .. "." .. pathParts[2]

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
			mwse.log("[Logger | ERROR]: Could not find the runtime information for \"%s\" or \"%s\". \z
					This should not happen!", pathParts[1], modDirWithAuthorName)
		end
	elseif rootDir == "lib" then
		-- do nothing
	elseif rootDir == "core" then
		-- do nothing
	else
		mwse.log('\t[Logger | ERROR]: Filepath "%s" was not correctly deduced. "%s" is an invalid root direcotry', filepath,
		         rootDir)
	end
	-- This is path of the filepath that does not include the `modDir`.
	-- NOTE: `modDir` could be either `pathParts[1]` or `pathParts[1] .. "." .. pathParts[2]`.
	-- Thus, `modDir` is NOT a substring of `table.concat(pathParts, "/")`,
	-- as the directory delimters may differ.
	local relativeFilePath = table.concat(pathParts, "/"):sub(modDir:len() + 2)

	return modName, modDir, relativeFilePath
end

return pathResolver
