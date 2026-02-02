local scene = tes3.game.worldObjectRoot

---@param node niAVObject
---@return boolean skipThisNode
---@return boolean? skipChildren
local function filterCollisionGeometry(node)
	if node:isOfType(ni.type.RootCollisionNode) then
		-- Note that in this example, we don't even want the RootCollisionNode be yielded so we don't have to return 2 value.
		-- This way the node and its children will be skipped.
		-- The other return value can be used to yield the node but skip its children.
		return true
	end

	return false, false
end

-- Note: the function is overloaded so the filter can be passed as a second argument.
for node in table.traverse({ scene }, filterCollisionGeometry) do
	mwse.log("%s : %s", node.RTTI.name, node.name or "")
end
