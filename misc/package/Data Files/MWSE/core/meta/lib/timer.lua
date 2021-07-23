--- @meta

--- The timer library provides helper functions for creating delayed executors.
--- @class timerlib
--- @field active any Constant to represent a timer that is actively running.
--- @field expired any Constant to represent a timer that has completed.
--- @field game any Constant to represent timers that run based on in-world time. Duration measured in game-scale hours.
--- @field paused any Constant to represent a timer that is paused.
--- @field real any Constant to represent timers that run in real-time.
--- @field simulate any Constant to represent timers that run when the game isn't paused. It matches the simulate event's timing. If the game is simulating, simulate events and simulate timers progress. Duration is measured in seconds.
timer = {}

--- Creates a timer that will finish the next frame. It defaults to the next simulation frame.
--- @param callback function The callback function that will execute when the timer expires.
--- @param type number Type of the timer. This value can be timer.simulate, timer.game or timer.real.
--- @return mwseTimer timer No description yet available.
function timer.delayOneFrame(callback, type) end

--- Creates a timer.
---
--- [Examples available in online documentation](https://mwse.readthedocs.io/en/latest/lua/api/timer/start.html).
--- @param params timer.start.params This table accepts the following values:
--- 
--- ``type``: number — Type of the timer. This value can be timer.simulate, timer.game or timer.real.
--- 
--- ``duration``: number — Duration of the timer. The method of time passing depends on the timer type.
--- 
--- ``callback``: function — The callback function that will execute when the timer expires.
--- 
--- ``iterations``: number — The number of iterations to run. Use ``-1`` for infinite looping.
--- @return mwseTimer timer No description yet available.
function timer.start(params) end

---Table parameter definitions for ``timer.start``.
--- @class timer.start.params
--- @field type number Type of the timer. This value can be timer.simulate, timer.game or timer.real.
--- @field duration number Duration of the timer. The method of time passing depends on the timer type.
--- @field callback function The callback function that will execute when the timer expires.
--- @field iterations number The number of iterations to run. Use ``-1`` for infinite looping.

