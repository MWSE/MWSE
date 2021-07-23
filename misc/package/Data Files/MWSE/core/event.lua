local this = {}

local generalEvents = {}

local filteredEvents = {}

-- Temporary hack for event priorities.
local eventPriorities = {}

local function getEventTable(eventType, filter)
	if (filter == nil) then
		if (generalEvents[eventType] == nil) then
			generalEvents[eventType] = {}
		end
		return generalEvents[eventType]
	else
		if (filteredEvents[eventType] == nil) then
			filteredEvents[eventType] = {}
		end
		if (filteredEvents[eventType][filter] == nil) then
			filteredEvents[eventType][filter] = {}
		end
		return filteredEvents[eventType][filter]
	end
end

local function eventSorter(a, b)
	return eventPriorities[a] > eventPriorities[b]
end

local disableableEvents = mwse.disableableEvents

function this.register(eventType, callback, options)
	-- Validate event type.
	if (type(eventType) ~= "string" or eventType == "") then
		return error("event.register: Event type must be a valid string.")
	end

	-- Validate callback.
	if (type(callback) ~= "function") then
		return error("event.register: Event callback must be a function.")
	end

	-- Make sure options is an empty table if nothing else.
	local options = options or {}

	-- If 'doOnce' was set, wrap with a call to unregister.
	if options.doOnce then
		local originalCallback = callback
		callback = function (e)
			this.unregister(eventType, callback, options)
			originalCallback(e)
		end
	end

	-- Handle conversions of filters.
	if (options.filter) then
		local filterType = type(options.filter)
		if (filterType == "userdata") then
			-- References get converted to the base object.
			if (options.filter.objectType == tes3.objectType.reference) then
				options.filter = options.filter.object
				mwse.log("Warning: Event registered to reference. Reference-type filtering was deprecated on 2018-12-15, and will be removed in future versions. Please update accordingly.")
				debug.traceback()
			end

			-- Actors and containers get converted to their base object.
			local baseObject = options.filter.baseObject
			if (baseObject) then
				options.filter = baseObject
				mwse.log("Warning: Event registered to actor clone. Switched to base object.")
				debug.traceback()
			end
		end
	end

	-- Store this callback's priority.
	eventPriorities[callback] = options.priority or 0

	-- Make sure that the event isn't already registered.
	local callbacks = getEventTable(eventType, options.filter)
	local found = table.find(callbacks, callback)
	if (found == nil) then
		table.insert(callbacks, callback)
		table.sort(callbacks, eventSorter)
	else
		print("event.register: Attempted to register same '" .. eventType .. "' event callback twice.")
		print(debug.traceback())
	end

	-- If this is a disableable event, enable it.
	if (disableableEvents[eventType] == false) then
		disableableEvents[eventType] = true
	end
end

function this.unregister(eventType, callback, options)
	-- Validate event type.
	if (type(eventType) ~= "string" or eventType == "") then
		return error("event.unregister: Event type must be a valid string.")
	end

	-- Validate callback.
	if (type(callback) ~= "function") then
		return error("event.unregister: Event callback must be a valid function.")
	end

	-- Make sure options is an empty table if nothing else.
	local options = options or {}

	local callbacks = getEventTable(eventType, options.filter)
	local removed = table.removevalue(callbacks, callback)
	-- if (not removed) then
		-- print("event.register: Attempted to unregister '" .. eventType .. "' event callback that wasn't registered.")
		-- print(debug.traceback())
	-- end

	-- Do we no longer care about this event?
	if (disableableEvents[eventType] == true and generalEvents[eventType] == nil and filteredEvents[eventType] == nil) then
		disableableEvents[eventType] = false
	end
end

function this.clear(eventType, filter)
	if (filter == nil) then
		-- Clear out general events of this type.
		generalEvents[eventType] = nil
	else
		if (eventType == nil) then
			-- No event supplied, so let's clear out all events for this filter.
			for k, v in pairs(filteredEvents) do
				v[filter] = nil
			end
		elseif (filteredEvents[eventType] ~= nil) then
			-- Clear out a specific event type/filter combo.
			filteredEvents[eventType][filter] = nil
		end
	end

	-- Do we no longer care about this event?
	if (disableableEvents[eventType] == true and generalEvents[eventType] == nil and filteredEvents[eventType] == nil) then
		disableableEvents[eventType] = false
	end
end

local function onEventError(error)
	mwse.log("Error in event callback: %s\n%s", error, debug.traceback())
end

function this.trigger(eventType, payload, options)
	-- Make sure params are an empty table if nothing else.
	local payload = payload or {}
	local options = options or {}

	payload.eventType = eventType
	payload.eventFilter = options.filter

	local callbacks = table.copy(getEventTable(eventType, options.filter))
	for _, callback in pairs(callbacks) do
		local status, result = xpcall(callback, onEventError, payload)
		if (status == false) then
			result = nil
		end

		-- Returning non-nil from the callback claims/blocks the event.
		if (result ~= nil) then
			payload.claim = true
			payload.block = true
		end

		-- If the event is claimed, do not excute any further events.
		if (payload.claim) then
			return payload
		end
	end

	-- At this point if we have a filter, we've run through the filtered events.
	-- Fire off the unfiltered events too.
	if (options.filter ~= nil) then
		this.trigger(eventType, payload)
	end

	return payload
end

return this