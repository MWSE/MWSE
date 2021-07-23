--- @meta

--- A core game object used for storing both active and non-dynamic gameplay data.
--- @class tes3dataHandler
--- @field backgroundThread number A Windows handle to the background processing thread.
--- @field backgroundThreadId number The thread ID for the background processing thread.
--- @field backgroundThreadRunning boolean Access to the running state for the background processing thread.
--- @field cellChanged boolean A flag set for the frame that the player has changed cells.
--- @field centralGridX boolean The position of the origin horizontal grid coordinate.
--- @field centralGridY boolean The position of the origin longitudinal grid coordinate.
--- @field currentAmbientWaterSound tes3sound Access to the current ambient water sound.
--- @field currentCell tes3cell Access to the cell that the player is currently in.
--- @field currentInteriorCell tes3cell Access to the current interior cell, if the player is in an interior.
--- @field exteriorCells table A table of nine tes3cellExteriorData objects for any loaded exterior cells.
--- @field lastExteriorCell tes3cell Access to the last visited exterior cell.
--- @field mainThread number A Windows handle to the main execution thread.
--- @field mainThreadId number The thread ID for the main execution thread.
--- @field nonDynamicData tes3nonDynamicData A child structure where core game objects are held.
--- @field threadSleepTime number No description yet available.
--- @field useCellTransitionFader boolean An engine flag that controls if there is a fade in/out between cells.
--- @field worldLandscapeRoot niNode One of the core parent scene graph nodes.
--- @field worldObjectRoot niNode One of the core parent scene graph nodes.
--- @field worldPickObjectRoot niNode One of the core parent scene graph nodes.
tes3dataHandler = {}

