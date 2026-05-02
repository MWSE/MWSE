local root = tes3.player.sceneNode

for node in root:traverse({ type = ni.type.NiTriShape, prefix = "tri c_m_shoe" }) do
	assert(node.scale == 1)
end
