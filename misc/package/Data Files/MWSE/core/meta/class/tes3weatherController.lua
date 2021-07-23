--- @meta

--- A data structure that handles the weather.
--- @class tes3weatherController
--- @field currentFogColor tes3vector3 The current fog color.
--- @field currentSkyColor tes3vector3 The current sky color.
--- @field currentWeather tes3weather The current weather.
--- @field daysRemaining number The days remaining.
--- @field hoursBetweenWeatherChange number The hours between weather changes. Stored as a float value.
--- @field hoursRemaining number The hours remaining.
--- @field lastActiveRegion tes3region Provides read-only access to last active region object.
--- @field masser tes3moon Provides read-only access to the Masser moon object.
--- @field nextWeather tes3weather The next weather.
--- @field sceneRainRoot niNode Provides read-only access to the scene rain root NiNode.
--- @field sceneSkyLight niDirectionalLight Provides read-only access to the scene sky light directional light.
--- @field sceneSkyRoot niNode Provides read-only access to the scene sky root NiNode.
--- @field sceneSnowRoot niNode Provides read-only access to the scene snow root NiNode.
--- @field sceneStormRoot niNode Provides read-only access to the scene storm root NiNode.
--- @field sceneSunBase niNode Provides read-only access to the scenesun base NiNode.
--- @field sceneSunGlare niNode Provides read-only access to the scene sun glare NiNode.
--- @field sceneSunVis niNode Provides read-only access to the scene sun vis NiNode.
--- @field secunda tes3moon Provides read-only access to the Secunda moon object.
--- @field sunglareFaderAngleMax number The sunglare fader maximum angle.
--- @field sunglareFaderColor tes3vector3 The sunglare fader color.
--- @field sunglareFaderMax number The sunglare fader maximum.
--- @field sunriseDuration number The sunrise duration.
--- @field sunriseHour number The sunrise hour.
--- @field sunsetDuration number The sunset duration.
--- @field sunsetHour number The sunset hour.
--- @field timescaleClouds string The timescale for clouds.
--- @field transitionScalar number The scalar transition.
--- @field underwaterColor tes3vector3 The underwater color, represented as a vector.
--- @field underwaterColorWeight number The underwater color weight.
--- @field underwaterDayFog number The underwater day fog value.
--- @field underwaterIndoorFog number The underwater indoor fog value.
--- @field underwaterNightFog number The underwater night fog value.
--- @field underwaterSunriseFog number The underwater sunrise fog value.
--- @field underwaterSunsetFog number The underwater sunset fog value.
--- @field weathers table Array-style table for the different weather types. Each object in the table is a tes3weather.
--- @field windVelocityCurrWeather tes3vector3 The wind velocity for the current weather.
--- @field windVelocityNextWeather tes3vector3 The wind velocity for the next weather.
tes3weatherController = {}

--- Calculates the sun damage based on the current weather.
--- @return number result No description yet available.
function tes3weatherController:calcSunDamageScalar() end

--- Immediately switches the weather to the provided weather parameter.
--- @param weatherId number No description yet available.
function tes3weatherController:switchImmediate(weatherId) end

--- Transitions the weather to the provided weather parameter based on the weather controller settings.
--- @param weatherId number No description yet available.
function tes3weatherController:switchTransition(weatherId) end

--- Updates the weather controller visuals. This should be called after any weather transitions.
function tes3weatherController:updateVisuals() end

