--- @meta

--- A rendering property that controls the surface properties of objects affected by the property. These surface properties include translucency, ambient reflective color, diffuse reflective color, emissive color intensity, and specular color.
--- @class niMaterialProperty : niProperty, niObjectNET, niObject
--- @field alpha number The alpha value of the material. Value should be between 0 - 1.
--- @field ambient niColor The ambient material color.
--- @field diffuse niColor The diffuse material color.
--- @field emissive niColor The emissive material color.
--- @field shininess number The shine of the material. Value should be >= 0.
--- @field specular niColor The specular material color.
niMaterialProperty = {}

