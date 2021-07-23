local results = tes3.rayTest{ tes3.getCameraPosition(), direction = tes3.getCameraVector(), findAll = true }
if results then
	for i, hit in pairs(results) do
		mwse.log("Ray hit #%d: %s", i, hit.reference or "<non-reference>");
	end
end
