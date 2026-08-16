local MY_CUSTOM_WEATHER_ID = 23

event.register(tes3.event.weatherSelect, function(e)
	if e.region.id ~= "ashlands region" then
		return
	end

	e.chances[tes3.weather.clear] = 5
	e.chances[tes3.weather.ash] = 50

	e.chances[MY_CUSTOM_WEATHER_ID] = 100
end)
