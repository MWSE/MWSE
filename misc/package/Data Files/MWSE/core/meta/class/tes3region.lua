--- @meta

--- A structure that contains region information.
--- @class tes3region : tes3baseObject
--- @field name string The region's name. Must be a string less than 32 characters.
--- @field sleepCreature tes3leveledCreature The region's leveled creature that can be spawned while the player is sleeping.
--- @field sounds table Array-style table for the different region sounds. Each object in the table is a tes3regionSound.
--- @field weather tes3weather The region's current weather.
--- @field weatherChanceAsh number The chance the Ash weather-type will be used. Must be an integer between 0 - 100.
--- @field weatherChanceBlight number The chance the Blight weather-type will be used. Must be an integer between 0 - 100.
--- @field weatherChanceBlizzard number The chance the Blizzard weather-type will be used. Must be an integer between 0 - 100.
--- @field weatherChanceClear number The chance the Clear weather-type will be used. Must be an integer between 0 - 100.
--- @field weatherChanceCloudy number The chance the Cloudy weather-type will be used. Must be an integer between 0 - 100.
--- @field weatherChanceFoggy number The chance the Foggy weather-type will be used. Must be an integer between 0 - 100.
--- @field weatherChanceOvercast number The chance the Overcast weather-type will be used. Must be an integer between 0 - 100.
--- @field weatherChanceRain number The chance the Rain weather-type will be used. Must be an integer between 0 - 100.
--- @field weatherChances table Array-style table for the different weather chances. Each object in the table is a number, between 1 - 100. The index of the table is the weather type.
--- @field weatherChanceSnow number The chance the Snow weather-type will be used. Must be an integer between 0 - 100.
--- @field weatherChanceThunder number The chance the Thunder weather-type will be used. Must be an integer between 0 - 100.
tes3region = {}

--- Changes the current weather for the region to the provided weather-type parameter.
--- @param weatherId number No description yet available.
function tes3region:changeWeather(weatherId) end

--- Changes the weather to a random weather type.
function tes3region:randomizeWeather() end

