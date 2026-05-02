--- @param e keybindTestedEventData
local function noJournalMenu(e)
	--[[ A keybind can have different transition types.

	tes3.keyTransition.downThisFrame     - If keybind changed from up to down ("pressed")
	tes3.keyTransition.upThisFrame       - If keybind changed from down to up ("unpressed")
	tes3.keyTransition.changedThisFrame  - If keybind changed state ("toggled")
	tes3.keyTransition.isDown			 - If keybind is currently pressed ("while held")

	The transition type is specific per test, as there are different use cases for each transition, and there may be multiple tests per keybind. You may need to log this event to see how the game utilizes a keybind.
	]]--

	-- We only care about keybind tests that check if the key was pressed this frame.
	if (e.transition ~= tes3.keyTransition.downThisFrame) then
		return
	end

	-- If the result was false, we also don't care.
	if (not e.result) then
		return
	end

	-- Set the result to false to make the game think the key wasn't pressed.
	-- We could also block this event by using `return false`.
	tes3.messageBox("You aren't allowed to open your journal.")
	e.result = false
end
event.register(tes3.event.keybindTested, noJournalMenu, { filter = tes3.keybind.journal })
