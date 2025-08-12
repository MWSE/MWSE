local this = {}

local generalEvents = {}

local filteredEvents = {}

--- Translation table used to recover `doOnce` functions.
--- The keys are the original callbacks, and the values are the new callbacks.
---@type table<fun(e): boolean, fun(e): boolean>
local doOnceCallbacks = {}

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

local disableableEvents = mwseDisableableEventManager --- @diagnostic disable-line

local remapObjectTypeDenyList = {
	[tes3.objectType.dialogueInfo] = true,
}

local function remapFilter(options, showWarnings)
	-- We only care if we have a filter.
	local filter = options.filter
	if (not filter) then
		return
	end

	-- We also only care about userdata.
	if (type(filter) ~= "userdata") then
		return
	end

	-- Which are tes3objects...
	if (filter.objectType == nil) then
		return
	end

	-- DenyList certain object types from being filtered by ID.
	if (remapObjectTypeDenyList[filter.objectType]) then
		return
	end

	-- References get converted to the base object. Actors and containers get converted to their base object.
	local baseObject = filter.baseObject
	if (baseObject and baseObject ~= filter) then
		if (showWarnings) then
			local givenType = table.find(tes3.objectType, filter.objectType)
			local newType = table.find(tes3.objectType, baseObject.objectType)
			mwse.log("Warning: Event registered to a non-base object '%s' (%s). Switched to base object '%s' (%s). Stacktrace:\n%s", filter, givenType, baseObject, newType, debug.traceback())
		end
		filter = baseObject
	end

	-- Ignore objects that somehow don't have an ID.
	if (not filter.id) then
		return
	end

	-- Finally, objects are converted to their id.
	options.filter = filter.id:lower()
end

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

		local function newCallback(e)
			-- `isRegistered` and `unregister` will also convert the callback.
			if this.isRegistered(eventType, originalCallback, options) then
				this.unregister(eventType, originalCallback, options)
			end
			originalCallback(e)
		end

		doOnceCallbacks[originalCallback] = newCallback
		callback = newCallback
	end

	-- If 'unregisterOnLoad' was set, unregister the callback on next load event.
	if options.unregisterOnLoad then
		this.register(tes3.event.load, function()
			if this.isRegistered(eventType, callback, options) then
				this.unregister(eventType, callback, options)
			end
		end, { doOnce = true } )
	end

	-- Fix up any filters to use base object ids.
	remapFilter(options, true)

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

	-- Handle the special case where `doOnce` was used.
	if doOnceCallbacks[callback] then
		callback = doOnceCallbacks[callback] 
		doOnceCallbacks[callback] = nil -- Won't be needing this anymore.
	end

	-- Make sure options is an empty table if nothing else.
	local options = options or {}

	-- Fix up any filters to use base object ids.
	remapFilter(options, true)

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

function this.isRegistered(eventType, callback, options)
	-- Validate event type.
	if (type(eventType) ~= "string" or eventType == "") then
		return error("event.isRegistered: Event type must be a valid string.")
	end

	-- Validate callback.
	if (type(callback) ~= "function") then
		return error("event.isRegistered: Event callback must be a valid function.")
	end

	-- Handle the special case where `doOnce` was used.
	if doOnceCallbacks[callback] then
		callback = doOnceCallbacks[callback] 
	end

	-- Make sure options is an empty table if nothing else.
	local options = options or {}

	-- Fix up any filters to use base object ids.
	remapFilter(options, true)

	local callbacks = getEventTable(eventType, options.filter)
	local found = table.find(callbacks, callback)
	return found ~= nil
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

-- Custom error notifications

local errorNotifier = {
	visible_error_limit = 8,
	timeout_duration = 8.0,
	eventType = nil,
	displayed = {},
	mod_totals = {}
}

-- Expose to MWSE
this.errorNotifier = errorNotifier

function errorNotifier.createMenu()
	local menu = tes3ui.createMenu{ id = "MWSE:ErrorNotify", fixedFrame = true, modal = false, loadable = false }
	menu.autoWidth = false
	menu.autoHeight = true
	menu.absolutePosAlignX = nil
	menu.absolutePosAlignY = nil
	menu.positionX = 8 - menu.maxWidth / 2
	menu.positionY = menu.maxHeight / 2 - 8
	menu.width = 900
	menu.minWidth = 900
	menu.minHeight = 0

	local f = menu:getContentElement()
	f.contentPath = nil
	f.borderAllSides = 0
	f.paddingAllSides = 6
	f.flowDirection = tes3.flowDirection.topToBottom

	local t = f:createLabel{ text = "Lua errors" }
	t.color = { 0.9, 0.9, 0.9 }
	t.borderBottom = 4

	local m = f:createBlock{ id = "MWSE:ErrorNotify_Listing" }
	m.widthProportional = 1
	m.autoHeight = true
	m.flowDirection = tes3.flowDirection.topToBottom

	for i = 1, errorNotifier.visible_error_limit do
		local t = m:createLabel{ text = "" }
		t.color = { 0.8, 0.8, 0.65 }
		t.widthProportional = 1
		t.height = 0
		t.borderBottom = 2
		t.wrapText = true
		t.visible = false
	end

	menu:updateLayout()
	return menu
end

function errorNotifier.clearMsg()
	errorNotifier.displayed = {}
	errorNotifier.timer = nil

	local menu = tes3ui.findMenu("MWSE:ErrorNotify")
	if menu then
		local list = menu:findChild("MWSE:ErrorNotify_Listing")
		if (list) then
			for i = 1, errorNotifier.visible_error_limit do
				list.children[i].text = ""
				list.children[i].visible = false
			end
		end

		menu.visible = false
		menu:updateLayout()
	end
end

function errorNotifier.updateMenu()
	local menu = tes3ui.findMenu("MWSE:ErrorNotify")
	if not menu then
		menu = errorNotifier.createMenu()
	end

	local list = menu:findChild("MWSE:ErrorNotify_Listing")
	if (list) then
		for i, display in ipairs(errorNotifier.displayed) do
			list.children[i].text = display.msg
			list.children[i].visible = true

			if i >= errorNotifier.visible_error_limit then
				break
			end
		end
	end

	-- Double re-layout to fix wrapping text heights.
	menu.visible = true
	menu:updateLayout()
	menu:updateLayout()

	if errorNotifier.timer then
		errorNotifier.timer:reset()
	else
		errorNotifier.timer = timer.start{type = timer.real, duration = errorNotifier.timeout_duration, callback = errorNotifier.clearMsg, persist = false}
	end
end

function errorNotifier.addMsg(errSource, modName, sourceFile, lineNum, errText)
	local firstErrLine = string.match(errText, "([^\n]+)")
	local errorCount = (errorNotifier.mod_totals[modName] or 0) + 1
	errorNotifier.mod_totals[modName] = errorCount

	local s = string.format("Mod: %s            Source: %s\n%s:%d > %s", modName, errSource, sourceFile, lineNum, firstErrLine)

	-- Maintain list at fixed length
	if #errorNotifier.displayed == errorNotifier.visible_error_limit then
		table.remove(errorNotifier.displayed, 1)
	end
	table.insert(errorNotifier.displayed, { mod = modName, msg = s })

	errorNotifier.updateMenu()
end

function errorNotifier.reportError(errSource, err)
	if not mwseConfig.EnableLuaErrorNotifications then return end --- @diagnostic disable-line

	local filePath, lineNum, errText = string.match(err, "[^:]+\\mods\\([^:]+):(%d+):%s*(.+)")

	if filePath and errText then
		local modName, sourceFile = string.match(filePath, "(.+)\\([^\\]+)")
		if modName then
			errorNotifier.addMsg(errSource, modName, sourceFile, lineNum, errText)
		end
	end
end

local function onEventError(err)
	mwse.log("Error in event callback: %s\n%s", err, debug.traceback())
	errorNotifier.reportError("event " .. errorNotifier.eventType, err)
end

function this.trigger(eventType, payload, options)
	-- Make sure params are an empty table if nothing else.
	local payload = payload or {}
	local options = options or {}

	-- Convert object filtering to base object/id filtering.
	remapFilter(options, false)

	payload.eventType = eventType
	payload.eventFilter = options.filter

	local callbacks = table.copy(getEventTable(eventType, options.filter))
	for _, callback in pairs(callbacks) do
		-- Inform error notifier of current eventType.
		errorNotifier.eventType = eventType

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
		options.filter = nil
		this.trigger(eventType, payload, options)
	end

	return payload
end

return this
