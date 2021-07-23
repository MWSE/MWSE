--- @meta

--- A core game object used for storing game settings.
--- @class tes3game
--- @field parentWindowHandle number The Windows HWND for the parent window.
--- @field playerTarget tes3reference The current activation target.
--- @field renderDistance number The game's rendering distance.
--- @field screenShotsEnabled boolean State on if screenshots are enabled.
--- @field screenX number The screen's horizontal position.
--- @field screenY number The screen's vertical position.
--- @field soundQuality number Sound quality level.
--- @field voice number Mix volumes for voices.
--- @field volumeEffect number Mix volumes for effects.
--- @field volumeFootsteps number Mix volumes for footsteps.
--- @field volumeMaster number Mix volumes for all sounds.
--- @field volumeMedia number Mix volumes for music.
--- @field windowHandle number The Windows HWND for the window.
--- @field wireframeProperty niProperty The reused wireframe property, appled when toggling wireframe rendering.
--- @field worldSceneGraphRoot niNode Access to the root of the scene graph.
tes3game = {}

