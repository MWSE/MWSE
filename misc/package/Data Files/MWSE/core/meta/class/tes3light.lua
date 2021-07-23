--- @meta

--- A core light object. This isn't actually a light in the rendering engine, but something like a lamp or torch.
--- @class tes3light : tes3physicalObject, tes3object, tes3baseObject
--- @field canCarry boolean Access to the light's flags, determining if the light can be carried.
--- @field color table Access to the light's base colors, in an array-style table of four values. The values can range from 0 to 255.
--- @field flickers boolean Access to the light's flags, determining if the light attenuation flickers.
--- @field flickersSlowly boolean Access to the light's flags, determining if the light attenuation flickers slowly.
--- @field icon string The path to the object's icon.
--- @field isDynamic boolean Access to the light's flags, determining if the light affects dynamically moving objects.
--- @field isFire boolean Access to the light's flags, determining if the light represents flame.
--- @field isNegative boolean Access to the light's flags, determining if the object creates darkness.
--- @field isOffByDefault boolean Access to the light's flags, determining if the light won't be active initially.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field pulses boolean Access to the light's flags, determining if the light attenuation pulses.
--- @field pulsesSlowly boolean Access to the light's flags, determining if the light attenuation pulses slowly.
--- @field radius number The base radius of the light.
--- @field script tes3script The script that runs on the object.
--- @field sound tes3sound The sound that runs on the object.
--- @field time number The amount of time that the light will last.
--- @field value number The value of the object.
--- @field weight number The weight of the object.
tes3light = {}

--- Gets the time remaining for a light, given a tes3itemData, tes3reference, or tes3equipmentStack.
--- @param data tes3reference|tes3itemData|tes3equipmentStack No description yet available.
--- @return number result No description yet available.
function tes3light:getTimeLeft(data) end

