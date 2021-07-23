tes3weatherController
====================================================================================================

A data structure that handles the weather.

Properties
----------------------------------------------------------------------------------------------------

`currentFogColor`_ (`tes3vector3`_)
    The current fog color.

`currentSkyColor`_ (`tes3vector3`_)
    The current sky color.

`currentWeather`_ (`tes3weather`_)
    Read-only. The current weather.

`daysRemaining`_ (`number`_)
    The days remaining.

`hoursBetweenWeatherChange`_ (`number`_)
    The hours between weather changes. Stored as a float value.

`hoursRemaining`_ (`number`_)
    The hours remaining.

`lastActiveRegion`_ (`tes3region`_)
    Read-only. Provides read-only access to last active region object.

`masser`_ (`tes3moon`_)
    Read-only. Provides read-only access to the Masser moon object.

`nextWeather`_ (`tes3weather`_)
    Read-only. The next weather.

`sceneRainRoot`_ (`niNode`_)
    Read-only. Provides read-only access to the scene rain root NiNode.

`sceneSkyLight`_ (`niDirectionalLight`_)
    Read-only. Provides read-only access to the scene sky light directional light.

`sceneSkyRoot`_ (`niNode`_)
    Read-only. Provides read-only access to the scene sky root NiNode.

`sceneSnowRoot`_ (`niNode`_)
    Read-only. Provides read-only access to the scene snow root NiNode.

`sceneStormRoot`_ (`niNode`_)
    Read-only. Provides read-only access to the scene storm root NiNode.

`sceneSunBase`_ (`niNode`_)
    Read-only. Provides read-only access to the scenesun base NiNode.

`sceneSunGlare`_ (`niNode`_)
    Read-only. Provides read-only access to the scene sun glare NiNode.

`sceneSunVis`_ (`niNode`_)
    Read-only. Provides read-only access to the scene sun vis NiNode.

`secunda`_ (`tes3moon`_)
    Read-only. Provides read-only access to the Secunda moon object.

`sunglareFaderAngleMax`_ (`number`_)
    The sunglare fader maximum angle.

`sunglareFaderColor`_ (`tes3vector3`_)
    The sunglare fader color.

`sunglareFaderMax`_ (`number`_)
    The sunglare fader maximum.

`sunriseDuration`_ (`number`_)
    The sunrise duration.

`sunriseHour`_ (`number`_)
    The sunrise hour.

`sunsetDuration`_ (`number`_)
    The sunset duration.

`sunsetHour`_ (`number`_)
    The sunset hour.

`timescaleClouds`_ (`string`_)
    The timescale for clouds.

`transitionScalar`_ (`number`_)
    The scalar transition.

`underwaterColor`_ (`tes3vector3`_)
    The underwater color, represented as a vector.

`underwaterColorWeight`_ (`number`_)
    The underwater color weight.

`underwaterDayFog`_ (`number`_)
    The underwater day fog value.

`underwaterIndoorFog`_ (`number`_)
    The underwater indoor fog value.

`underwaterNightFog`_ (`number`_)
    The underwater night fog value.

`underwaterSunriseFog`_ (`number`_)
    The underwater sunrise fog value.

`underwaterSunsetFog`_ (`number`_)
    The underwater sunset fog value.

`weathers`_ (`table`_)
    Read-only. Array-style table for the different weather types. Each object in the table is a tes3weather.

`windVelocityCurrWeather`_ (`tes3vector3`_)
    The wind velocity for the current weather.

`windVelocityNextWeather`_ (`tes3vector3`_)
    The wind velocity for the next weather.

.. toctree::
    :hidden:

    tes3weatherController/currentFogColor
    tes3weatherController/currentSkyColor
    tes3weatherController/currentWeather
    tes3weatherController/daysRemaining
    tes3weatherController/hoursBetweenWeatherChange
    tes3weatherController/hoursRemaining
    tes3weatherController/lastActiveRegion
    tes3weatherController/masser
    tes3weatherController/nextWeather
    tes3weatherController/sceneRainRoot
    tes3weatherController/sceneSkyLight
    tes3weatherController/sceneSkyRoot
    tes3weatherController/sceneSnowRoot
    tes3weatherController/sceneStormRoot
    tes3weatherController/sceneSunBase
    tes3weatherController/sceneSunGlare
    tes3weatherController/sceneSunVis
    tes3weatherController/secunda
    tes3weatherController/sunglareFaderAngleMax
    tes3weatherController/sunglareFaderColor
    tes3weatherController/sunglareFaderMax
    tes3weatherController/sunriseDuration
    tes3weatherController/sunriseHour
    tes3weatherController/sunsetDuration
    tes3weatherController/sunsetHour
    tes3weatherController/timescaleClouds
    tes3weatherController/transitionScalar
    tes3weatherController/underwaterColor
    tes3weatherController/underwaterColorWeight
    tes3weatherController/underwaterDayFog
    tes3weatherController/underwaterIndoorFog
    tes3weatherController/underwaterNightFog
    tes3weatherController/underwaterSunriseFog
    tes3weatherController/underwaterSunsetFog
    tes3weatherController/weathers
    tes3weatherController/windVelocityCurrWeather
    tes3weatherController/windVelocityNextWeather

.. _`currentFogColor`: tes3weatherController/currentFogColor.html
.. _`currentSkyColor`: tes3weatherController/currentSkyColor.html
.. _`currentWeather`: tes3weatherController/currentWeather.html
.. _`daysRemaining`: tes3weatherController/daysRemaining.html
.. _`hoursBetweenWeatherChange`: tes3weatherController/hoursBetweenWeatherChange.html
.. _`hoursRemaining`: tes3weatherController/hoursRemaining.html
.. _`lastActiveRegion`: tes3weatherController/lastActiveRegion.html
.. _`masser`: tes3weatherController/masser.html
.. _`nextWeather`: tes3weatherController/nextWeather.html
.. _`sceneRainRoot`: tes3weatherController/sceneRainRoot.html
.. _`sceneSkyLight`: tes3weatherController/sceneSkyLight.html
.. _`sceneSkyRoot`: tes3weatherController/sceneSkyRoot.html
.. _`sceneSnowRoot`: tes3weatherController/sceneSnowRoot.html
.. _`sceneStormRoot`: tes3weatherController/sceneStormRoot.html
.. _`sceneSunBase`: tes3weatherController/sceneSunBase.html
.. _`sceneSunGlare`: tes3weatherController/sceneSunGlare.html
.. _`sceneSunVis`: tes3weatherController/sceneSunVis.html
.. _`secunda`: tes3weatherController/secunda.html
.. _`sunglareFaderAngleMax`: tes3weatherController/sunglareFaderAngleMax.html
.. _`sunglareFaderColor`: tes3weatherController/sunglareFaderColor.html
.. _`sunglareFaderMax`: tes3weatherController/sunglareFaderMax.html
.. _`sunriseDuration`: tes3weatherController/sunriseDuration.html
.. _`sunriseHour`: tes3weatherController/sunriseHour.html
.. _`sunsetDuration`: tes3weatherController/sunsetDuration.html
.. _`sunsetHour`: tes3weatherController/sunsetHour.html
.. _`timescaleClouds`: tes3weatherController/timescaleClouds.html
.. _`transitionScalar`: tes3weatherController/transitionScalar.html
.. _`underwaterColor`: tes3weatherController/underwaterColor.html
.. _`underwaterColorWeight`: tes3weatherController/underwaterColorWeight.html
.. _`underwaterDayFog`: tes3weatherController/underwaterDayFog.html
.. _`underwaterIndoorFog`: tes3weatherController/underwaterIndoorFog.html
.. _`underwaterNightFog`: tes3weatherController/underwaterNightFog.html
.. _`underwaterSunriseFog`: tes3weatherController/underwaterSunriseFog.html
.. _`underwaterSunsetFog`: tes3weatherController/underwaterSunsetFog.html
.. _`weathers`: tes3weatherController/weathers.html
.. _`windVelocityCurrWeather`: tes3weatherController/windVelocityCurrWeather.html
.. _`windVelocityNextWeather`: tes3weatherController/windVelocityNextWeather.html

Methods
----------------------------------------------------------------------------------------------------

`calcSunDamageScalar`_ (`number`_)
    Calculates the sun damage based on the current weather.

`switchImmediate`_
    Immediately switches the weather to the provided weather parameter.

`switchTransition`_
    Transitions the weather to the provided weather parameter based on the weather controller settings.

`updateVisuals`_
    Updates the weather controller visuals. This should be called after any weather transitions.

.. toctree::
    :hidden:

    tes3weatherController/calcSunDamageScalar
    tes3weatherController/switchImmediate
    tes3weatherController/switchTransition
    tes3weatherController/updateVisuals

.. _`calcSunDamageScalar`: tes3weatherController/calcSunDamageScalar.html
.. _`switchImmediate`: tes3weatherController/switchImmediate.html
.. _`switchTransition`: tes3weatherController/switchTransition.html
.. _`updateVisuals`: tes3weatherController/updateVisuals.html

.. _`niDirectionalLight`: ../../lua/type/niDirectionalLight.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3moon`: ../../lua/type/tes3moon.html
.. _`tes3region`: ../../lua/type/tes3region.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3weather`: ../../lua/type/tes3weather.html
