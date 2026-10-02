-- The bolt meshes to use, by spell ID.
local boltMeshes = {
	["my_spell_id"] = "my_mod\\my_bolt.nif",
}

-- The projectiles that already have the new mesh. The mobileActivated event
-- triggers again for a projectile when the player changes cells.
local replaced = setmetatable({}, { __mode = "k" })

--- @param e mobileActivatedEventData
local function onMobileActivated(e)
	if e.mobile.objectType ~= tes3.objectType.mobileSpellProjectile then return end
	if replaced[e.reference] then return end

	local meshPath = boltMeshes[e.mobile.spellInstance.source.id]
	if not meshPath then return end

	local mesh = tes3.loadMesh(meshPath, false)

	-- The engine set the always update flag on the projectile nodes at launch.
	-- This mesh did not exist at launch, so set the flag on its animation nodes.
	-- Without the flag, a particle system outside the view does not start.
	for node in mesh:traverse({ type = ni.type.NiBSAnimationNode }) do
		---@cast node niBSAnimationNode
		node.animationFlags = bit.bor(node.animationFlags, 0x10)
	end

	local sceneNode = e.reference.sceneNode
	if not sceneNode then return end

	sceneNode:detachAllChildren()
	sceneNode:attachChild(mesh)
	sceneNode:updateProperties()
	sceneNode:updateEffects()

	-- The engine updates the projectile in each frame. An update call here is not necessary.
	replaced[e.reference] = true
end
event.register(tes3.event.mobileActivated, onMobileActivated)
