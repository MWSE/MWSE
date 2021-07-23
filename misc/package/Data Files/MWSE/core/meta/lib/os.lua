--- @meta

--- Provides various OS-specific functions.
--- @class oslib
os = {}

--- Return CPU time since Lua started in seconds.
--- @return number result No description yet available.
function os.clock() end

--- Returns the current clipboard text, if any is available.
--- @return string|nil result No description yet available.
function os.getClipboardText() end

--- Sets the current clipboard text. If nil is passed, the clipboard will be cleared instead. If either operation fails, the function returns false.
--- @param text string|nil The text to put in the clipboard. If nil is provided instead, then the clipboard will be cleared.
--- @return boolean result No description yet available.
function os.setClipboardText(text) end

