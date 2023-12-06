local initialized = false

event.register(tes3.event.initialized, function()
	initialized = true
end, { priority = math.huge })

function tes3.isInitialized()
	return initialized
end
