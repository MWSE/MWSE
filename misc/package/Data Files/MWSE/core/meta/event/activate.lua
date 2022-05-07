--- @meta
--- @diagnostic disable:undefined-doc-name

--- This event is invoked whenever something is activated, typically by the player. Activation is usually done with the associated activate/use key, but may also be forced by scripts.
--- 
--- Non-player actors may also activate objects, such as when opening doors, or via MWSE functions like [`tes3.activate()`](https://mwse.github.io/MWSE/apis/tes3/#tes3activate).
--- 
--- Some examples of when the activate event fires includes:
--- 
--- - When a door is used.
--- - When an item is picked up.
--- - When someone attempts to open a container.
---
--- [Examples available in online documentation](https://mwse.github.io/MWSE/events/activate).
--- @class activateEventData
--- @field block boolean If set to `true`, vanilla logic will be suppressed. Returning `false` will set this to `true`.
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field activator tes3reference *Read-only*. The actor attempting to trigger the event.
--- @field target tes3reference *Read-only*. The reference that is being activated.