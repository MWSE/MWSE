--- @meta
--- @diagnostic disable:undefined-doc-name

--- The uiShowRestMenu event is raised when the rest menu is about to be displayed. It allows the callback to change if sleeping is allowed, or to prevent the UI from showing at all.
--- @class uiShowRestMenuEventData
--- @field block boolean If set to `true`, vanilla logic will be suppressed. Returning `false` will set this to `true`.
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field allowRest boolean true if the player is allowed to sleep (determined primarily from the cell flags), or false if they must wait instead. This value can be modified.
--- @field scripted boolean *Read-only*. This value is true when the menu is shown from using the ShowRestMenu or equivalent function.