tes3region
====================================================================================================

A structure that contains region information.

Properties
----------------------------------------------------------------------------------------------------

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`id`_ (`string`_)
    The unique identifier for the object.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The region's name. Must be a string less than 32 characters.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`sleepCreature`_ (`tes3leveledCreature`_)
    Read-only. The region's leveled creature that can be spawned while the player is sleeping.

`sounds`_ (`table`_)
    Read-only. Array-style table for the different region sounds. Each object in the table is a tes3regionSound.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`weather`_ (`tes3weather`_)
    The region's current weather.

`weatherChanceAsh`_ (`number`_)
    The chance the Ash weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceBlight`_ (`number`_)
    The chance the Blight weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceBlizzard`_ (`number`_)
    The chance the Blizzard weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceClear`_ (`number`_)
    The chance the Clear weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceCloudy`_ (`number`_)
    The chance the Cloudy weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceFoggy`_ (`number`_)
    The chance the Foggy weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceOvercast`_ (`number`_)
    The chance the Overcast weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceRain`_ (`number`_)
    The chance the Rain weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceSnow`_ (`number`_)
    The chance the Snow weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceThunder`_ (`number`_)
    The chance the Thunder weather-type will be used. Must be an integer between 0 - 100.

`weatherChances`_ (`table`_)
    Read-only. Array-style table for the different weather chances. Each object in the table is a number, between 1 - 100. The index of the table is the weather type.

.. toctree::
    :hidden:

    tes3region/deleted
    tes3region/disabled
    tes3region/id
    tes3region/modified
    tes3region/name
    tes3region/objectFlags
    tes3region/objectType
    tes3region/sleepCreature
    tes3region/sounds
    tes3region/sourceMod
    tes3region/supportsLuaData
    tes3region/weather
    tes3region/weatherChanceAsh
    tes3region/weatherChanceBlight
    tes3region/weatherChanceBlizzard
    tes3region/weatherChanceClear
    tes3region/weatherChanceCloudy
    tes3region/weatherChanceFoggy
    tes3region/weatherChanceOvercast
    tes3region/weatherChanceRain
    tes3region/weatherChanceSnow
    tes3region/weatherChanceThunder
    tes3region/weatherChances

.. _`deleted`: tes3region/deleted.html
.. _`disabled`: tes3region/disabled.html
.. _`id`: tes3region/id.html
.. _`modified`: tes3region/modified.html
.. _`name`: tes3region/name.html
.. _`objectFlags`: tes3region/objectFlags.html
.. _`objectType`: tes3region/objectType.html
.. _`sleepCreature`: tes3region/sleepCreature.html
.. _`sounds`: tes3region/sounds.html
.. _`sourceMod`: tes3region/sourceMod.html
.. _`supportsLuaData`: tes3region/supportsLuaData.html
.. _`weather`: tes3region/weather.html
.. _`weatherChanceAsh`: tes3region/weatherChanceAsh.html
.. _`weatherChanceBlight`: tes3region/weatherChanceBlight.html
.. _`weatherChanceBlizzard`: tes3region/weatherChanceBlizzard.html
.. _`weatherChanceClear`: tes3region/weatherChanceClear.html
.. _`weatherChanceCloudy`: tes3region/weatherChanceCloudy.html
.. _`weatherChanceFoggy`: tes3region/weatherChanceFoggy.html
.. _`weatherChanceOvercast`: tes3region/weatherChanceOvercast.html
.. _`weatherChanceRain`: tes3region/weatherChanceRain.html
.. _`weatherChanceSnow`: tes3region/weatherChanceSnow.html
.. _`weatherChanceThunder`: tes3region/weatherChanceThunder.html
.. _`weatherChances`: tes3region/weatherChances.html

Methods
----------------------------------------------------------------------------------------------------

`changeWeather`_
    Changes the current weather for the region to the provided weather-type parameter.

`randomizeWeather`_
    Changes the weather to a random weather type.

.. toctree::
    :hidden:

    tes3region/changeWeather
    tes3region/randomizeWeather

.. _`changeWeather`: tes3region/changeWeather.html
.. _`randomizeWeather`: tes3region/randomizeWeather.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3leveledCreature`: ../../lua/type/tes3leveledCreature.html
.. _`tes3weather`: ../../lua/type/tes3weather.html
