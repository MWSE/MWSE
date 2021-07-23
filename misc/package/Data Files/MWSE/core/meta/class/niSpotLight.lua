--- @meta

--- Represents a spotlight in a scene. Spotlights have a specific location and direction, as well as a distance attenuation and angle attenuation functions. The light direction is handled in the same way as in `niDirectionalLight`.
--- @class niSpotLight : niPointLight, niLight, niDynamicEffect, niAVObject, niObjectNET, niObject
--- @field direction tes3vector3 The direction of the spotlight.
--- @field spotAngle number The spotlight angle, in degrees.
--- @field spotExponent number The spotlight angular attenuation exponent. This value may be ignored.
niSpotLight = {}

