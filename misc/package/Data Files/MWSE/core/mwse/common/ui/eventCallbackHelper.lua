local eventCallbackHelper = {}

--- Takes in event data from a keyPress event and strips the last bit from it to return an ASCII code.
--- @param e tes3uiEventData
--- @return string?
function eventCallbackHelper.getKeyPressed(e)
	return bit.band(e.data0, 0x7FFFFFFF) --- @diagnostic disable-line
end

--- Takes in event data from a keyPress event and gets the character pressed.
--- @param e tes3uiEventData
--- @return string?
function eventCallbackHelper.getCharacterPressed(e)
	if (e.data0 <= 0) then --- @diagnostic disable-line
		return nil
	end
	return string.char(eventCallbackHelper.getKeyPressed(e))
end

return eventCallbackHelper
