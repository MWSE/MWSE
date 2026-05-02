-- Definition for my custom event eventData table
--- @class myCustomEventData
--- @field count integer
--- @field item tes3item
--- @field itemData tes3itemData|nil
--- @field equipped boolean

--- @param e myCustomEventData
local function doMyCustomLogic(e)
	-- We can do some logic accounting for the changes callbacks made in the eventData table.
	-- ...
end

local function somethingHappened()
	-- Assemble the eventData table
	--- @type myCustomEventData
	local e = {
		count = 1,
		item = tes3.getObject("myItemId") --[[@as tes3misc]],
		itemData = nil,
		equipped = false
	}

	-- First we trigger the pre-event that supports blocking our logic from happening.
	local eventResult = event.trigger("myMod:myCustomEvent", e, { filter = e.item.id:lower() })

	-- Was the event blocked by one of the callbacks?
	if eventResult.block then
		-- Nothing to do then :)
		return
	end

	doMyCustomLogic(eventResult)

	-- We have to reset these fields to be able to reuse
	-- eventResult table in the next call to event.trigger
	eventResult.block = nil
	eventResult.claim = nil

	-- Now we can trigger off the post-event
	event.trigger("myMod:postMyCustomEvent", eventResult, { filter = eventResult.item.id:lower() })
end
