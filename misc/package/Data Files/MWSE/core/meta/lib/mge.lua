--- @meta

--- The mge library allows MGE XE mwscript functions to be called. This is not always ideal, and this library is deprecated.
--- @class mgelib
mge = {}

--- Clears elements from the HUD. Wrapper for MGEWipeHUDElements.
function mge.clearHUD() end

--- Disables MGE XE's camera shake effect. Wrapper for mwscript's MGEDisableCameraShake.
function mge.disableCameraShake() end

--- Disables a HUD element. If a HUD ID is provided, this is a warpper for MGEDisableHUD, otherwise it is a wrapper for MGENIDDisableHUD.
--- @param hud string No description yet available.
function mge.disableHUD(hud) end

--- Disables a shader. Wrapper for mwscript's MGEDisableShader.
--- @param shader string No description yet available.
function mge.disableShader(shader) end

--- Disables the MGE XE zoom effect. Wrapper for MGEDisableZoom.
function mge.disableZoom() end

--- Enables MGE XE's camera shake effect. Wrapper for mwscript's MGEEnableCameraShake.
--- @param magnitude number No description yet available.
--- @param acceleration number No description yet available.
function mge.enableCameraShake(magnitude, acceleration) end

--- Enables a HUD element. If a HUD ID is provided, this is a warpper for MGEEnableHUD, otherwise it is a wrapper for MGENIDEnableHUD.
--- @param hud string No description yet available.
function mge.enableHUD(hud) end

--- Enables a shader. Wrapper for mwscript's MGEEnableShader.
--- @param shader string No description yet available.
function mge.enableShader(shader) end

--- Enables the MGE XE zoom effect. Wrapper for MGEEnableZoom.
function mge.enableZoom() end

--- Frees a HUD element. If a HUD ID is provided, this is a warpper for MGEFreeHUD, otherwise it is a wrapper for MGENIDFreeHUD.
--- @param hud string No description yet available.
function mge.freeHUD(hud) end

--- Makes a HUD element fullscreen. If a HUD ID is provided, this is a warpper for MGEFullscreenHUD, otherwise it is a wrapper for MGENIDFullscreenHUD.
--- @param hud string No description yet available.
function mge.fullscreenHUD(hud) end

--- Gets the window's vertical resolution. Wrapper for mwscript's MGEGetHeight.
--- @return number result No description yet available.
function mge.getScreenHeight() end

--- Wrapper for MGEGetScreenRotation.
--- @return number result No description yet available.
function mge.getScreenRotation() end

--- Gets the window's horizontal resolution. Wrapper for mwscript's MGEGetWidth.
--- @return number result No description yet available.
function mge.getScreenWidth() end

--- Returns the UI scaling used.
--- @return number result No description yet available.
function mge.getUIScale() end

--- Gets the MGE version. Wrapper for MGEGetVersion.
--- @return number result No description yet available.
function mge.getVersion() end

--- Wrapper for MGEGetZoom.
--- @return number result No description yet available.
function mge.getZoom() end

--- Loads a HUD element. This is a warpper for MGELoadHUD.
--- @param params mge.loadHUD.params This table accepts the following values:
--- 
--- ``hud``: string — No description yet available.
--- 
--- ``texture``: string — No description yet available.
--- 
--- ``enable``: boolean — No description yet available.
function mge.loadHUD(params) end

---Table parameter definitions for ``mge.loadHUD``.
--- @class mge.loadHUD.params
--- @field hud string No description yet available.
--- @field texture string No description yet available.
--- @field enable boolean No description yet available.

--- This function writes information to the mgeXE.log file in the user's installation directory. Wrapper for mwscript's MGELog function.
--- @param message string No description yet available.
function mge.log(message) end

--- Wrapper for MGERotateScreenBy.
--- @param rotation number No description yet available.
function mge.modScreenRotation(rotation) end

--- Positions a HUD element. If a HUD ID is provided, this is a warpper for MGEPositionHUD, otherwise it is a wrapper for MGENIDPositionHUD.
--- @param hud string No description yet available.
--- @param x number No description yet available.
--- @param y number No description yet available.
function mge.positionHUD(hud, x, y) end

--- Scales a HUD element. If a HUD ID is provided, this is a warpper for MGEScaleHUD, otherwise it is a wrapper for MGENIDScaleHUD.
--- @param hud string No description yet available.
--- @param x number No description yet available.
--- @param y number No description yet available.
function mge.scaleHUD(hud, x, y) end

--- Selects a HUD element, to assume for future HUD calls.
--- @param hud string No description yet available.
function mge.selectHUD(hud) end

--- Wrapper for MGECameraShakeZoom.
--- @param acceleration number No description yet available.
function mge.setCameraShakeAcceleration(acceleration) end

--- Wrapper for MGESetCameraShakeMagnitude.
--- @param magnitude number No description yet available.
function mge.setCameraShakeMagnitude(magnitude) end

--- Sets the effect for a HUD element. If a HUD ID is provided, this is a warpper for MGEChangeHUDEffect, otherwise it is a wrapper for MGENIDChangeHUDEffect.
--- @param hud string No description yet available.
--- @param effect string No description yet available.
function mge.setHUDEffect(hud, effect) end

--- Sets the effect float variable for a HUD element. This is a wrapper for MGENIDSetHUDEffectFloat.
--- @param hud string No description yet available.
--- @param variable string No description yet available.
--- @param value number No description yet available.
function mge.setHUDEffectFloat(hud, variable, value) end

--- Sets the effect long variable for a HUD element. This is a wrapper for MGENIDSetHUDEffectLong.
--- @param hud string No description yet available.
--- @param variable string No description yet available.
--- @param value number No description yet available.
function mge.setHUDEffectLong(hud, variable, value) end

--- Sets the effect vector variable for a HUD element. This is a wrapper for MGENIDSetHUDEffectVec.
--- @param hud string No description yet available.
--- @param variable string No description yet available.
--- @param value table A table of 4 values.
function mge.setHUDEffectVector4(hud, variable, value) end

--- Sets the texture for a given HUD element. If a HUD ID is provided, this is a warpper for MGEChangeHUDTexture, otherwise it is a wrapper for MGENIDChangeHUDTexture.
--- @param hud string No description yet available.
--- @param texture string No description yet available.
function mge.setHUDTexture(hud, texture) end

--- Wrapper for MGERotateScreen.
--- @param rotation number No description yet available.
function mge.setScreenRotation(rotation) end

--- Sets a shader's float variable. Wrapper for mwscript's MGEShaderSetFloat.
--- @param shader string No description yet available.
--- @param variable string No description yet available.
--- @param value number No description yet available.
function mge.setShaderFloat(shader, variable, value) end

--- Sets a shader's long variable. Wrapper for mwscript's MGEShaderSetLong.
--- @param shader string No description yet available.
--- @param variable string No description yet available.
--- @param value number No description yet available.
function mge.setShaderLong(shader, variable, value) end

--- Sets a shader's vector variable. Wrapper for mwscript's MGEShaderSetVector.
--- @param shader string No description yet available.
--- @param variable string No description yet available.
--- @param value table A table of 4 values.
function mge.setShaderVector4(shader, variable, value) end

--- Wrapper for MGEZoom, or MGESetZoom if set to animate.
--- @param amount number No description yet available.
--- @param animate boolean No description yet available.
function mge.setZoom(amount, animate) end

--- Enables MGE XE's screen spin effect. Wrapper for mwscript's MGEScreenSpin.
function mge.startScreenRotation() end

--- Disables MGE XE's screen spin effect. Wrapper for mwscript's MGEStopSpinSpin.
function mge.stopScreenRotation() end

--- Wrapper for MGEStopZoom.
function mge.stopZoom() end

--- Toggles the MGE XE zoom effect. Wrapper for MGEToggleZoom.
function mge.toggleZoom() end

--- Cancels selection for a HUD element. Wrapper for mwscript's MGECancelWithHUD.
--- @param hud string No description yet available.
function mge.unselectHUD(hud) end

--- Wrapper for MGEZoomIn, or MGEZoomInBy if an amount is provided.
--- @param amount number No description yet available.
function mge.zoomIn(amount) end

--- Wrapper for MGEZoomOut, or MGEZoomOutBy if an amount is provided.
--- @param amount number No description yet available.
function mge.zoomOut(amount) end

