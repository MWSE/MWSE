--- @diagnostic disable: undefined-field
--- @diagnostic disable: param-type-mismatch
--- @diagnostic disable: undefined-global

local UnitWind = require("unitwind")




local TEST_EVENT_ID = "testEventId"

--- Wrapper that makes sure we always use the special event ID and set `doOnce = true`.
--- @param callback fun(e): (nil|boolean)
--- @param filter any
--- @param priority integer?
local function register(callback, filter, priority)
	return event.register(TEST_EVENT_ID, callback, {doOnce = true, priority = priority, filter = filter})
end

--- @param filter any
local function trigger(filter)
	return event.trigger(TEST_EVENT_ID, {}, {filter = filter})
end


local function isRegistered(callback, filter, priority)
	return event.isRegistered(TEST_EVENT_ID, callback, {doOnce = true, priority = priority, filter = filter})
end





-- use the debug library to spy on the `generalEvents` and `filteredEvents` tables.
---@type {[tes3.event]: (fun(e): boolean)[]}
local generalEvents
---@type {[tes3.event]: (fun(e): boolean)[]}
local filteredEvents




---@type {[string]: fun()}
local innerCallbacks = {
	callback1 = function () end,
	callback2 = function () end,
	callback3 = function () end,
}


local testSuite = UnitWind.new({
	enabled = true,
	highlight = true,
	exitAfter = true,
	beforeAll = function()
		-- Use the debug library to gain access to the internal variables
		-- used by the event API
		for i = 1, 5000 do
			local name, value = debug.getupvalue(event.clear, i)
			if not name then break end
			if name == "generalEvents" then
				generalEvents = value
			elseif name == "filteredEvents" then
				filteredEvents = value
			end
		end

	end,
	-- To be able to mock print, we need to reroute UnitWind's output file.
	outputFile = "events test.log",
	afterEach = function()
		event.clear(TEST_EVENT_ID)
		for _, k in ipairs(table.keys(innerCallbacks)) do
			-- reset the number of mock calls
			-- this is done because I can't get `unspy` to work
			table.clear(innerCallbacks[k]._mockCalls)
		end
	end
})

for _, k in ipairs(table.keys(innerCallbacks)) do
	testSuite:spy(innerCallbacks, k)
end

testSuite:start("Testing event API")

testSuite:test("Load generalEvents and filteredEvents", function ()
	testSuite:expect(generalEvents).toBeType("table")
	testSuite:expect(filteredEvents).toBeType("table")
end)



-- Make sure `doOnce` results in a callback being executed exactly once.
testSuite:test("doOnce: basic functionality", function()

	--- This bit of indirection is necessary because `unitwind:spy` 
	--- results in the stuff stored in `innerCallbacks` no longer being functions
	--- and the event API needs the callbacks to be functions
	local callback = function()
		innerCallbacks.callback1()
	end

	register(callback)

	-- Make sure the function got registers properly
	testSuite:expect(generalEvents[TEST_EVENT_ID]).toBeType("table")
	-- There should only be one thing registered
	testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(1)
	
	-- The callback itself should not be registered, but instead a thin wrapper should be registered.
	testSuite:expect(generalEvents[TEST_EVENT_ID][1]).NOT.toBe(callback)

	trigger()
	
	testSuite:expect(innerCallbacks.callback1).toBeCalled()
	testSuite:expect(innerCallbacks.callback1).toBeCalledTimes(1)
	
	trigger()


	-- The event should have been unregistered by now
	testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(0)
	testSuite:expect(innerCallbacks.callback1).NOT.toBeCalledTimes(2)
	
	testSuite:expect(isRegistered(callback)).toBe(false)
end)

-- Make sure that `isRegistered` properly detects when a `doOnce` function is registered
testSuite:test("doOnce: compatibility with isRegistered", function()

	-- make sure the event table got cleared since the last test
	testSuite:expect(generalEvents[TEST_EVENT_ID]).toBeType("nil")

	local callback = function()
		innerCallbacks.callback1()
	end

	register(callback)


	testSuite:expect(isRegistered(callback)).toBe(true)
	trigger()
	-- Function should be unregistered after the event triggers
	testSuite:expect(isRegistered(callback)).toBe(false)
	
end)


testSuite:test("doOnce: priority works", function()

	testSuite:expect(generalEvents[TEST_EVENT_ID]).toBeType("nil")

	local callback1 = function(e)
		innerCallbacks.callback1()
		e.block = true
		e.claim = true
		return false
	end
	local callback2 = function()
		innerCallbacks.callback2()
	end

	register(callback1, nil, 10)
	register(callback2, nil, 5)
	
	testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(2)
	
	trigger()
	
	-- The first event should be deregistered since the event fired.
	testSuite:expect(isRegistered(callback1)).toBe(false)
	testSuite:expect(innerCallbacks.callback1).toBeCalled()

	-- The second event should still be registered because a higher priority callback
	-- claimed the event.
	testSuite:expect(isRegistered(callback2)).toBe(true)
	testSuite:expect(innerCallbacks.callback2).NOT.toBeCalled()
	
	testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(1)
	

	trigger()

	testSuite:expect(isRegistered(callback2)).toBe(false)
	testSuite:expect(innerCallbacks.callback2).toBeCalled()
	
	testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(0)
	
end)

testSuite:test("doOnce: priority works with regular callbacks", function()

	testSuite:expect(innerCallbacks.callback1).toBeCalledTimes(0)


	local callback1 = function(e)
		innerCallbacks.callback1()
		e.block = true
		e.claim = true
		return false
	end
	local callback2 = function(e)
		innerCallbacks.callback2()
		e.block = true
		e.claim = true
		return false
	end

	local callback3 = function(e)
		innerCallbacks.callback3()
	end


	event.register(TEST_EVENT_ID, callback1, {priority = 30})
	event.register(TEST_EVENT_ID, callback2, {priority = 20, doOnce = true})
	event.register(TEST_EVENT_ID, callback3, {priority = 10, doOnce = true})
	
	testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(3)

	do -- make sure the higher priority event blocks the others from firing
		
		trigger()

		testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(3)
		
		testSuite:expect(isRegistered(callback1)).toBe(true)
		testSuite:expect(isRegistered(callback2)).toBe(true)
		testSuite:expect(isRegistered(callback3)).toBe(true)

		testSuite:expect(innerCallbacks.callback1).toBeCalledTimes(1)

	end

	do -- Make sure it happens again, since `callback1` was not registered with `doOnce`
	
		trigger()

		testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(3)
		
		testSuite:expect(isRegistered(callback1)).toBe(true)
		testSuite:expect(isRegistered(callback2)).toBe(true)
		testSuite:expect(isRegistered(callback3)).toBe(true)

		testSuite:expect(innerCallbacks.callback1).toBeCalledTimes(2)

	end
	
	-- unregister the first callback and see if the others behave normally.
	event.unregister(TEST_EVENT_ID, callback1)
	
	testSuite:expect(isRegistered(callback1)).toBe(false)
	testSuite:expect(isRegistered(callback2)).toBe(true)
	testSuite:expect(isRegistered(callback3)).toBe(true)
	
	-- this should result in callback2 firing, but not callback 3
	trigger()

	
	testSuite:expect(isRegistered(callback1)).toBe(false)
	testSuite:expect(isRegistered(callback2)).toBe(false)
	testSuite:expect(isRegistered(callback3)).toBe(true)

	testSuite:expect(innerCallbacks.callback2).toBeCalledTimes(1)


	trigger()
	
	testSuite:expect(isRegistered(callback1)).toBe(false)
	testSuite:expect(isRegistered(callback2)).toBe(false)
	testSuite:expect(isRegistered(callback3)).toBe(false)

	
	testSuite:expect(innerCallbacks.callback3).toBeCalledTimes(1)
end)


testSuite:test("doOnce: unregister", function()

	testSuite:expect(innerCallbacks.callback1).toBeCalledTimes(0)


	local callback1 = function(e)
		innerCallbacks.callback1()
	end

	event.register(TEST_EVENT_ID, callback1, {priority = 30, doOnce = true})
	
	testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(1)
	
	event.unregister(TEST_EVENT_ID, callback1, {priority = 30})

	testSuite:expect(#generalEvents[TEST_EVENT_ID]).toBe(0)
	
	trigger()

	testSuite:expect(innerCallbacks.callback1).toBeCalledTimes(0)
end)


testSuite:finish()

