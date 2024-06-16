-- Fetches Components and Variables by class name.
-- Intended for internal use only.
local fileUtils = {}

-- Cache all `Component` and `Variable` classes, to minimize disk usage.
local components = {} ---@type table<string, mwseMCMComponent>
local variables = {}  ---@type table<string, mwseMCMVariable>


-- Store component/variable paths so we only have to iterate the directory once.
-- Due to circular dependencies, these files cannot be `require`d immediately. So instead, the filepaths will be stored
--      ahead of time, and then `require`d as needed.
-- Files get `require`d in by the `fileUtils.get<Component|Variable>Class` functions. (They actually get `include`d for stability reasons.)
-- Once a file has been `require`d by the `fileUtils.get<Component|Variable>Class` functions, its path will be removed from these tables.
-- (Testing has shown that this approach results in a noticeable boost in performance.)

local componentPaths = {} ---@type table<string, string>
local variablePaths = {}  ---@type table<string, string>

local prefixLength = string.len("Data Files\\MWSE\\core\\")

for filePath, dir, fileName in lfs.walkdir("data files\\mwse\\core\\mcm\\components\\") do
	-- For example, when adding the path of `mwseMCMPage`:
    -- "Page" instead of "Page.lua"
    local className = fileName:sub(1, fileName:len() - 4) 

    -- "mcm.components.pages.Page" instead of "data files\\mwse\\core\\mcm\\components\\pages\\Page.lua"
    local luaPath = filePath:sub(1 + prefixLength, filePath:len() - 4):gsub("[\\/]", ".")

	componentPaths[className] = luaPath
end
-- Add backwards compatibility by redirecting old component names to new ones.
componentPaths.HyperLink = componentPaths.Hyperlink
componentPaths.SidebarPage = componentPaths.SideBarPage

for filePath, dir, fileName in lfs.walkdir("data files\\mwse\\core\\mcm\\variables\\") do
    local className = fileName:sub(1, fileName:len() - 4) 
    local luaPath = filePath:sub(1 + prefixLength, filePath:len() - 4):gsub("[\\/]", ".")

	variablePaths[className] = luaPath
end



---@protected
---@param className string The name of the class of the `mwseMCMComponent` to search for.
---@return mwseMCMComponent?
function fileUtils.getComponentClass(className)
	local class = components[className]
	if class then return class end

	local luaPath = componentPaths[className]
    if not luaPath then return end
    class = include(luaPath)
    if not class or type(class) ~= "table" then 
        mwse.log('[MCM: ERROR] Could not find the filepath of "%s"!\n%s', debug.traceback())
        return
    end

    -- Initialize the `class` field of this `class`, and cache the class in the `components` table.
    class.class = className 
    components[className] = class

    componentPaths[className] = nil -- Won't be needing this anymore.

    return class
end

---@protected
---@param className string The name of the class of the `mwseMCMVariable` to search for.
---@return mwseMCMVariable?
function fileUtils.getVariableClass(className)
	local class = variables[className]
	if class then return class end
	local luaPath = variablePaths[className]

    if not luaPath then return end
    class = include(luaPath)
    if not class or type(class) ~= "table" then 
        mwse.log('[MCM: ERROR] Could not find the filepath of "%s"!\n%s', debug.traceback())
        return
    end

    -- Initialize the `class` field of this `class`, and cache the class in the `variables` table.
    class.class = className
    variables[className] = class

    variablePaths[className] = nil -- Won't be needing this anymore.

    return class
end

return fileUtils