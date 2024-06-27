# Timers

Timers are a way to keep track with the passage of time. They are volatile, meaning that they do not persist between saved games.

!!! warning "Timers get canceled when loading saves."
	All active timers will be canceled right before the [`loaded`](../events/loaded.md) event triggers.

## Creating a Basic Timer

To create a timer, pass a table of options to `timer.start()`.

```lua linenums="1"
-- The function we want to run when the timer completes.
local function onTimerComplete()
    tes3.messageBox("30 seconds have passed!")
end

-- Create our timer to fire the above function after 30 seconds.
timer.start({ duration = 30, callback = onTimerComplete })
```

In the above example, we create a message box after 30 seconds.


## Repeating a Timer

Timers can repeat, based on an **iterations** parameter. This can be used to define a timer that will trigger multiple times.

```lua linenums="1"
-- The function we want to run when the timer completes.
local function onTimerComplete()
    tes3.messageBox("A minute has passed!")
end

-- Create our timer to fire the above function every minute for 5 minutes.
timer.start({ duration = 60, callback = onTimerComplete, iterations = 5 })
```

Additionally, a value of `-1` will create a timer that will repeat forever.


## Controlling Timer State

A timer's state (read through the `.state` property), can be `timer.active`, `timer.paused`, or `timer.expired`. An *active* timer is running normally. A *paused* timer doesn't run, but can be resumed to pick back up where it left off. An *expired* timer has finished running (and has no more iterations).

A timer can be told to pause, resume, reset, or cancel.

```lua linenums="1"
-- Create a timer.
local myTimer = timer.start({ duration = 60, callback = onTimerComplete })

-- Pause the timer. It can be resumed again using myTimer:resume()
myTimer:pause()

-- Resume the timer. It's now active again.
myTimer:resume()

-- Reset the timer. It will take 60 seconds from right now to finish.
myTimer:reset()

-- Cancel the timer. We will no longer care about it.
myTimer:cancel()
```


## Creating Different Types of Timers

Three different types of timers are available, though an advanced user can create their own types of timers and manage their clock cycles. The default types are:

- **Real**, defined by `timer.real`. Real timers operate primarily on real time, incrementing seconds whenever the game window is active.
- **Simulate**, defined by `timer.simulate`. Simulate timers operate much like real timers, incrementing seconds but only when the game is simulating (i.e. not in menu mode). This is the default type of timer.
- **Game**, defined by `timer.game`. Game timers operate by game hour, matching game time.

```lua linenums="1"
-- Create a timer that ends after 60 seconds.
local myTimer = timer.start({ duration = 60, callback = onTimerComplete, type = timer.real })

-- Create a timer that ends after 30 unpaused seconds.
local myTimer = timer.start({ duration = 30, callback = onTimerComplete, type = timer.simulate })

-- Create a timer that ends after 4 game hours.
local myTimer = timer.start({ duration = 4, callback = onTimerComplete, type = timer.game })
```
!!! note
	After reading the Timers guide, it's recommended to proceed to the [Object Lifetimes](./object-lifetimes.md) guide. It describes how to safely use [tes3reference](../types/tes3reference.md) objects inside timer callbacks.
