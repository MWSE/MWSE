--- @meta

--- A Timer Controller is a class used to sort and trigger callbacks based on an arbitrary timekeeping mechanic.
--- @class mwseTimerController
--- @field clock number The current clock time for this timer controller.
mwseTimerController = {}

--- Creates a new Timer Controller. Its initial clock is zero, unless a start time is provided.
--- @param startTime number No description yet available.
--- @return mwseTimerController result No description yet available.
function mwseTimerController.new(startTime) end

--- Creates a timer for the given Timer Controller.
--- @param params mwseTimerController.create.params This table accepts the following values:
--- 
--- ``type``: number — No description yet available.
--- 
--- ``duration``: number — No description yet available.
--- 
--- ``callback``: function — No description yet available.
--- 
--- ``iterations``: number — No description yet available.
--- @return mwseTimer timer No description yet available.
function mwseTimerController:create(params) end

---Table parameter definitions for ``mwseTimerController.create``.
--- @class mwseTimerController.create.params
--- @field type number No description yet available.
--- @field duration number No description yet available.
--- @field callback function No description yet available.
--- @field iterations number No description yet available.

