---@param keyCode tes3.scanCode
function tes3.getKeyName(keyCode)
	if not keyCode then return end
	-- This will get the index of the appropriate GMST setting.
	-- The `%02X` performs hexadecimal formatting and makes sure numbers `< 16` have a leading 0. (e.g. `0A` instead of `A`.)
	-- For an example of how this all works, `tes3.scanCode.h` (which has a value of `35`) will become `sKeyName_23`.
	local index = tes3.gmst[string.format("sKeyName_%02X", keyCode)]
	-- If the GMST exists, return its value.
	if index then
		return tes3.findGMST(index).value
	end
end