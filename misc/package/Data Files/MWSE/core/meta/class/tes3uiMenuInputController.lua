--- @meta

--- A core came structure responsible for managing the input around menu elements.
--- @class tes3uiMenuInputController
--- @field lastInputTime number The amount of seconds that have passed since the last time the input was updated.
--- @field menuController tes3uiMenuController A handle back to the main menu controller.
--- @field textInputFocus tes3uiElement The element that is currently focused for text input.
tes3uiMenuInputController = {}

--- Clears the current text input. If no text has been passed to the current text focus, this will prevent it from doing so.
function tes3uiMenuInputController:flushBufferedTextEvents() end

