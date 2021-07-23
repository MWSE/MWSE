--- @meta

--- An object that has a name, extra data, and controllers.
--- @class niObjectNET : niObject
--- @field name string The human-facing name of the given object.
niObjectNET = {}

--- Add a controller to the object as the first controller.
--- @param type niTimeController No description yet available.
function niObjectNET:prependController(type) end

--- Removes all controllers.
function niObjectNET:removeAllControllers() end

--- Removes a controller from the object.
--- @param type niTimeController No description yet available.
function niObjectNET:removeController(type) end

