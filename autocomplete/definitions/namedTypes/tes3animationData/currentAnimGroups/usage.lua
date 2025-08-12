local ref = tes3.getPlayerTarget()
if not ref then return end
local animationData = ref.animationData
if not animationData then return end

tes3.messageBox("Current groups: lower = %s, upper = %s, leftArm = %s",
	table.find(tes3.animationGroup, ref.animationData.currentAnimGroups[tes3.animationBodySection.lower + 1]),
	table.find(tes3.animationGroup, ref.animationData.currentAnimGroups[tes3.animationBodySection.upper + 1]),
	table.find(tes3.animationGroup, ref.animationData.currentAnimGroups[tes3.animationBodySection.leftArm + 1])
)