--- @meta

--- A Timer is a class used to keep track of callback that should be invoked at a later time.
--- @class mwseTimer
--- @field callback function The callback that will be invoked when the timer elapses.
--- @field duration number The amount of time left on the timer.
--- @field iterations number The amount of iterations left for the timer.
--- @field state number The state of the timer, matching timer.active, timer.paused, or timer.expired.
--- @field timeLeft number The amount of time left before this timer will complete.
--- @field timing number When this timer ends, or the time remaining if the timer is paused.
mwseTimer = {}

--- Cancels the timer.
function mwseTimer:cancel() end

--- Pauses the timer.
function mwseTimer:pause() end

--- Resets the timer completion time, as if it elapsed. Only works if the timer is active.
function mwseTimer:reset() end

--- Resumes a paused timer.
function mwseTimer:resume() end

