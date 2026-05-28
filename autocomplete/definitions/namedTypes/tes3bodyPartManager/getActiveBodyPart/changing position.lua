local dagger = tes3.getObject("iron dagger") --[[@as tes3weapon]]
local slot = tes3.player1stPerson.bodyPartManager:getActiveBodyPartForItem(dagger)

slot.node.translation.z = slot.node.translation.z - 20
slot.node:update()
