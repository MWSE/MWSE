--- @meta

--- The event library helps to instruct mwse to call a given function when a specific action is taken in the game.
--- @class eventlib
event = {}

--- Removes all callbacks registered for a given event.
--- @param eventId string No description yet available.
--- @param options table No description yet available.
function event.clear(eventId, options) end

--- Registers a function to be called when an event is raised.
--- @param eventId string No description yet available.
--- @param callback function No description yet available.
--- @param options table No description yet available.
function event.register(eventId, callback, options) end

--- Triggers an event. This can be used to trigger custom events with specific data.
--- @param eventId string No description yet available.
--- @param payload table No description yet available.
--- @param options table No description yet available.
function event.trigger(eventId, payload, options) end

--- Unregisters a function  event is raised.
--- @param eventId string No description yet available.
--- @param callback function No description yet available.
--- @param options table No description yet available.
function event.unregister(eventId, callback, options) end

