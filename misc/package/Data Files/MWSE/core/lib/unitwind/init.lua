local inspect = require("inspect")
local mwseLogger = require("logging.logger")

---@class UnitWind.new.params
---@field enabled boolean? Whether to enable UnitWind. Defaults to true.
---@field highlight boolean? Whether to highlight output. Defaults to true.
---@field beforeAll fun(self: UnitWind)? A function to run before all tests are run.
---@field afterAll fun(self: UnitWind)? A function to run after all tests have been run.
---@field beforeEach fun(self: UnitWind)? A function to run before each test.
---@field afterEach fun(self: UnitWind)? A function to run after each test.
---@field exitAfter boolean? Whether to exit the game after all tests have been run. Defaults to false.
---@field outputFile string? The path to the file to write output to. If not defined, output will be written to mwse.log.

--- UnitWind - A unit testing framework for Morrowind Script Extender (MWSE)
---
--- See `unitwind.tests.lua` for examples of how to use UnitWind.
---@class UnitWind : UnitWind.new.params
---@field totalTests number The total number of tests that have been run.
---@field testsPassed number The total number of tests that have passed.
---@field testsFailed number The total number of tests that have failed.
---@field completedTests table<string, boolean> A table of tests that have been run, with the test name as the key and a boolean indicating whether the test passed as the value.
---@field logger mwseLogger The logger to use for output.
---@field outputFile file*? The file to write output to. If not defined, output will be written to mwse.log.
local UnitWind = {
    schema = {
        name = "UnitWind",
        fields = {
            enabled = { type = "boolean", default = true, required = false },
            highlight = { type = "boolean", default = true, required = false },
            beforeAll = { type = "function", required = false },
            afterAll = { type = "function", required = false },
            beforeEach = { type = "function", required = false },
            afterEach = { type = "function", required = false },
            exitAfter = { type = "boolean", default = false, required = false },
            outputFile = { type = "string", required = false },
            --internal
            totalTests = { type = "number", required = false },
            testsPassed = { type = "number", required = false },
            testsFailed = { type = "number", required = false },
            completedTests = { type = "table", required = false },
        }
    },
}
local ansicolors = require("logging.colors")
local validator = require "unitwind.validator"

--- Construct a new UnitWind instance
---@param data UnitWind.new.params
---@return UnitWind
function UnitWind.new(data)
    ---@type UnitWind
    local unitwind = table.deepcopy(data)
    unitwind.totalTests = 0
    unitwind.testsPassed = 0
    unitwind.testsFailed = 0
    unitwind.completedTests = {}
    unitwind.logger = mwseLogger.new{
        name = "UnitWind",
        logLevel = "INFO",
    }
    --Validate and set defaults
    validator.validate(unitwind, UnitWind.schema)
    setmetatable(unitwind, UnitWind)
    UnitWind.__index = UnitWind
    unitwind:setOutputFile(data.outputFile)
    return unitwind
end

--- Use in place of an expected arguemt when using expect().toBeCalledWith() to check for any value.
---
--- Pass in the expected type as a string to check for a specific type.
---
--- Example:
---
---    expect(myFunction).toBeCalledWith(UnitWind.any("number"), UnitWind.any("string"))
---
---@param expectedType type? #The expected type of the argument. If no type is provided, any value will be accepted.
function UnitWind.any(expectedType)
    return {
        _unitwind_type = expectedType ~= nil and expectedType or "any"
    }
end

--- Log results to a file other than mwse.log
---@param outputFile string #The file to write output to.
function UnitWind:setOutputFile(outputFile)
    if outputFile == nil or string.lower(outputFile) == "mwse.log" then
        self.outputFile = nil
    else
        local errMsg = "[ERROR] Logger:setLogLevel() - Not a valid outputFile (must be a string)"
        assert(type(outputFile) == "string", errMsg)
        self.outputFile = io.open(outputFile, "w")
    end
end

--- Expect a result to be a certain value or type
---
--- Example:
---
---    expect(1).toBeType("number")
---
---@param result any #The result to check
---@return UnitWind.expects #An object with functions to perform expectations on the result
function UnitWind:expect(result)
    --Example usage: expect(1).toBe(1)
    local expectTypes = {
        toBe = function(expectedResult, isNot)
            if not self.enabled then return false end
            if (result == expectedResult) == isNot then
                error(string.format("Expected value to %sbe %s, got: %s.", isNot and "not " or "", expectedResult, result))
            end
            return true
        end,

        --Example usage: expect(1).toBeType("number")
        toBeType = function(expectedType, isNot)
            if not self.enabled then return false end
            local thisType = type(result)
            if (thisType == expectedType) == isNot then
                error(string.format("Expected type to %sbe %s, got: %s.", isNot and "not " or "", expectedType, thisType))
            end
            return true
        end,

        toFail = function(_, isNot)
            if not self.enabled then return false end
            local status, res = pcall(result)
            if not isNot then
                if status then
                    error(string.format("Expected function to fail, but instead returned %s", res or "nil"))
                end
            else
                if not status then
                    error(string.format("Expected function to succeed, but it failed with error: %s", res))
                end
            end
            return true
        end,

        toFailWithError = function(expectedError, isNot)
            if not self.enabled then return false end
            local status, res = pcall(result)
            local errorMessage = string.match(res, ":%d+: (.*)") or res
            if not isNot then
                if status then
                    error(string.format("Expected function to fail, but instead returned %s", res))
                end
                if errorMessage ~= expectedError then
                    error(string.format("Expected function to fail with error %s, but instead failed with error: %s", expectedError, errorMessage))
                end
            else
                if not status then
                    error(string.format("Expected function to succeed, but it failed with error: %s", errorMessage))
                end
            end
            return true
        end,

        toBeCalled = function(_, isNot)
            if not self.enabled then return false end
            if not result._mockCalls then
                error("Expected function to be called, but it was not a mock function.")
            end
            local called = #result._mockCalls > 0
            if (called == true) == isNot then
                error(string.format("Expected function to %sbe called, but it was %s.", isNot and "not " or "", called and "called" or "not called"))
            end
            return true
        end,
        toBeCalledTimes = function(expectedTimes, isNot)
            if not self.enabled then return false end
            if not result._mockCalls then
                error("Expected function to be called, but it was not a mock function.")
            end
            local called = #result._mockCalls
            if (called == expectedTimes) == isNot then
                error(string.format("Expected function to %sbe called %s times, but it was called %s times.", isNot and "not " or "", expectedTimes, called))
            end
            return true
        end,
        toBeCalledWith = function(expectedArgs, isNot)
            if not self.enabled then return false end
            if not result._mockCalls then
                error("Expected function to be called, but it was not a mock function.")
            end
            local called = #result._mockCalls > 0
            if not called then
                error("Expected function to be called, but it was not called.")
            end
            --if args are not a table, make them a table
            if type(expectedArgs) ~= "table" then
                expectedArgs = { expectedArgs }
            end
            local allArgsMatch = true
            --Loop through all calls to the mock function
            for _, call in ipairs(result._mockCalls) do
                --Make sure the number of arguments matches
                if #call == #expectedArgs then
                    --Loop through each argument and check if it matches
                    for i, arg in ipairs(call) do
                        --Check for any() args and compare types
                        if expectedArgs[i]._unitwind_type then
                            if expectedArgs[i]._unitwind_type == "any" then
                                --Any param is allowed
                                break
                            elseif type(arg) ~= expectedArgs[i]._unitwind_type then
                                --type mismatch
                                allArgsMatch = false
                            end
                            break
                        end
                        if arg ~= expectedArgs[i] then
                            allArgsMatch = false
                            break
                        end
                    end
                end
            end

            if (allArgsMatch == true) == isNot then
                --Replace any() with a unique placeholder that can be string replaced later, like `'''type'''`
                for i, arg in ipairs(expectedArgs) do
                    if arg._unitwind_type then
                        expectedArgs[i] = string.format("'''%s'''", arg._unitwind_type)
                    end
                end
                --inspect each arg into a comma separated string
                local expectedArgsString
                for i, arg in ipairs(expectedArgs) do
                    if i == 1 then
                        expectedArgsString = inspect(arg)
                    else
                        expectedArgsString = string.format("%s, %s", expectedArgsString, inspect(arg))
                    end
                end
                --do tehg same for the _mockCalls[1]
                local resultArgsString
                for i, arg in ipairs(result._mockCalls[1]) do
                    if i == 1 then
                        resultArgsString = inspect(arg)
                    else
                        resultArgsString = string.format("%s, %s", resultArgsString, inspect(arg))
                    end
                end

                local errorMsg = string.format("Expected function to %sbe called with %s, but it was called with %s.",
                    isNot and "not " or "", expectedArgsString, resultArgsString)
                --Replace the unique placeholder with any(<type)
                errorMsg = string.gsub(errorMsg, "\"'''(%w+)'''\"", "any(%1)")
                error(errorMsg)
            end
            return true
        end,
    }

    ---@class UnitWind.expects
    ---@field toBe fun(expectedResult: any, isNot: boolean?): boolean #Check if the result is equal to the expected value
    ---@field toBeType fun(expectedType: string, isNot: boolean?): boolean #Check if the result is of the expected type
    ---@field toFail fun(isNot: boolean?): boolean #Check if the result is a function that fails
    ---@field toFailWithError fun(expectedError: string, isNot: boolean?): boolean #Check if the result is a function that fails with the expected error
    ---@field toBeCalled fun(isNot: boolean?): boolean #Check if the result is a mock function that has been called
    ---@field toBeCalledTimes fun(expectedTimes: number, isNot: boolean?): boolean #Check if the result is a mock function that has been called the expected number of times
    ---@field toBeCalledWith fun(expectedArgs: any, isNot: boolean?): boolean #Check if the result is a mock function that has been called with the expected arguments
    ---@field NOT UnitWind.expects.NOT #A table with the same functions as the parent, except they perform the reverse check
    local expects = {}

    ---@class UnitWind.expects.NOT
    ---@field toBe fun(expectedResult: any, isNot: boolean?): boolean #Check if the result is not equal to the expected value
    ---@field toBeType fun(expectedType: string, isNot: boolean?): boolean #Check if the result is not of the expected type
    ---@field toFail fun(isNot: boolean?): boolean #Check if the result is a function that succeeds
    ---@field toFailWithError fun(expectedError: string, isNot: boolean?): boolean #Check if the result is a function that succeeds or fails with a different error
    ---@field toBeCalled fun(isNot: boolean?): boolean #Check if the result is a mock function that has not been called
    ---@field toBeCalledTimes fun(expectedTimes: number, isNot: boolean?): boolean #Check if the result is a mock function that has not been called the expected number of times
    ---@field toBeCalledWith fun(expectedArgs: any, isNot: boolean?): boolean #Check if the result is a mock function that has not been called with the expected arguments
    expects.NOT = {}
    for expectType, func in pairs(expectTypes) do
        expects[expectType] = function(...)
            return func(..., false)
        end
        expects.NOT[expectType] = function(...)
            return func(..., true)
        end
    end
    return expects
end


--- Mock out a value on a table or module
---
--- - Replaces a field on an object with a mock value
--- - If the mock value is a function, it will be spied on
---
--- Example:
---
---    local mockTable = { value = "original" }
---    UnitWind:mock(mockTable, "value", "mock")
---    unitwind:expect(mockTable.value).toBe("mock")
---
---@param object table|string The table or module which contains the key to mock
---@param key string The function or value to mock
function UnitWind:mock(object, key, mockvalue)
    if type(object) == "string" then
        object = include(object)
        if not object then
            error(string.format("Could not find module to mock %s", object))
        end
    end
    local originalValue = object[key]
    self.mocks = self.mocks or {}
    self.mocks[object] = self.mocks[object] or {}
    self.mocks[object][key] = originalValue
    object[key] = mockvalue
    --spy on mock if its a function
    if type(mockvalue) == "function" then
        self:spy(object, key)
    end
end

--- Restore a mock to its original state
---@param object table|string The table or module which contains the key to unmock
---@param key string The function or value to unmock
function UnitWind:unmock(object, key)
    self.logger:debug("unmock: %s: %s", object, key)
    if type(object) == "string" then
        object = include(object)
        if not object then
            error(string.format("Could not find module to unmock %s", object))
        end
    end
    if not self:_hasMock(object) then
        error(string.format("Could not find mock for %s.%s", object, key))
    end
    --restore original value
    local original = self.mocks[object][key]
    object[key] = original
    --remove from self.mocks
    self.mocks[object][key] = nil
    if not next(self.mocks[object]) then
        self.mocks[object] = nil
    end
    --remove from self.spies if its a function
    if type(original) == "function" then
        self:unspy(object, key)
    end
end

--- Restore all mocked functions to their original state
function UnitWind:clearMocks()
    self.logger:debug("clearMocks()")
    if not self.mocks then return end
    for object, mocks in pairs(self.mocks) do
        for key, originalValue in pairs(mocks) do
            self.logger:debug("Reseting mock %s.%s to %s", object, key, inspect(originalValue))
            object[key] = originalValue
        end
        --remove from self.spies if its a function
        --self.spies[object] = nil
    end
    self.mocks = nil
end

--- Spy on a function
---
--- - The number of calls and the arguments passed to the spied function will be recorded
--- - Unlike mock(), the original function will still be called
--- - To test a spy, use `expect.toBeCalledWith`
---
--- Example:
---
---     local myModule = include("myModule")
---     UnitWind:spy(myModule, "myFunction")
---     myModule.myFunction("hello", "world")
---     UnitWind:expect(myModule.myFunction).toBeCalledWith({"hello", "world"})
---     UnitWind:unspy(myModule, "myFunction")
---
---@param object table|string The table or module which contains the key to spy on
---@param key string The function to spy on
function UnitWind:spy(object, key)
    if type(object) == "string" then
        object = include(object)
        if not object then
            error(string.format("Could not find module to spy on %s", object))
        end
    end
    if type(key) ~= "string" then
        error(string.format("Key must be a string, got %s", type(key)))
    end
    if type(object[key]) ~= "function" then
        error(string.format("Key %s is not a function, got %s", key, type(object[key])))
    end
    if not object[key] then
        error(string.format("Could not find key %s on object %s", key, object))
    end
    local originalValue = object[key]
    self.spies = self.spies or {}
    self.spies[object] = self.spies[object] or {}
    self.spies[object][key] = originalValue
    local spy = {
        _mockCalls = {},
        original = originalValue,
    }
    setmetatable(spy, {
        __index = function(t, k)
            if k == "_mockCalls" then
                return t._mockCalls
            end
            return t.original[k]
        end,
        __call = function(t, ...)
            table.insert(t._mockCalls, {...})
            return t.original(...)
        end
    })
    object[key] = spy
end

--- Remove a spy from a function
---@param object table|string The table or module which contains the key to unspy
---@param key string The function to unspy
function UnitWind:unspy(object, key)
    if type(object) == "string" then
        object = include(object)
        if not object then
            error(string.format("Could not find module to unspy on %s", object))
        end
    end
    if not self:_hasSpy(object, key) then
        error(string.format("Could not find spy for %s.%s", object, key))
    end
    --restore original value
    local spy = self.spies[object][key]
    if type(spy) == "table" and spy._mockCalls then
        local originalFunction = spy.original
        object[key] = originalFunction
    end
    --object[key] = originalFunction
    --remove from self.spies
    self.spies[object][key] = nil
    if not next(self.spies[object]) then
        self.spies[object] = nil
    end
end

--- Restore all spied functions to their original state
function UnitWind:clearSpies()
    if not self.spies then return end
    for object, spies in pairs(self.spies) do
        for key, spy in pairs(spies) do
            if type(spy) == "table" and spy._mockCalls then
                object[key] = spy.original
            end
        end
    end
    self.spies = nil
end


--- Begin a test suite
---@param testsName string #The name of the test suite
function UnitWind:start(testsName)
    self.testsName = testsName or ""
    self:_logHeader("-----------------------------------------------------")
    self:_logHeader("Starting: %s", self.testsName)
    self:_logHeader("-----------------------------------------------------")
    if self.beforeAll then
        self:beforeAll()
    end
end

--Perform a test
---@param testName string #The name of the test
---@param callback function #The function to test
function UnitWind:test(testName, callback)
    if not self.enabled then return end
    if self.beforeEach then
        self:beforeEach()
    end
    self.totalTests = self.totalTests + 1
    local status, error = pcall(callback)
    table.insert(self.completedTests, {
        name = testName,
        passed = status,
    })
    if status == true then
        self.testsPassed = self.testsPassed + 1
    else
        self.testsFailed = self.testsFailed + 1
        self:error("Error Message: %s\n%s", error, debug.traceback())
    end
    if self.afterEach then
        self:afterEach()
    end
end

--[[
    Perform a test where the expected result is a failure.
    When the test "fails", the error message will still log
    but the test will be marked as passed.
]]
---@param testName string #The name of the test
---@param callback function #The function to test
function UnitWind:failTest(testName, callback, expectedError)
    if not self.enabled then return end
    if self.beforeEach then
        self:beforeEach()
    end
    self.totalTests = self.totalTests + 1
    local status, error = pcall(callback)

    local didPass = false
    if status == false then
        local errorMessage = string.match(error, ":%d+: (.*)") or error
        if expectedError and (errorMessage ~= expectedError) then
            self.testsFailed = self.testsFailed + 1
            self:error("Expected error: %s\nActual error: %s",
                expectedError, errorMessage)
        else
            self.testsPassed = self.testsPassed + 1
            didPass = true
        end
    else
        self.testsFailed = self.testsFailed + 1
        self:_log("Error Message: %s", error)
    end
    table.insert(self.completedTests, {
        name = testName,
        passed = didPass,
    })
    if self.afterEach then
        self:afterEach()
    end
end

--Reset the test count, restore mocks and print the results
function UnitWind:reset()
    self.logger:debug("Resetting UnitWind")
    self.testsName = ""
    self.totalTests = 0
    self.testsPassed = 0
    self.testsFailed = 0
    self.completedTests = {}
    self:clearSpies()
    self:clearMocks()
end

--Finish all tests and print the results
---@param exitAfter boolean? #Exit the program after printing the results
function UnitWind:finish(exitAfter)
    if not self.enabled then return end
    self:_logHeader("-----------------------------------------------------")
    self:_logHeader("Finished: %s", self.testsName or "")
    self:_logHeader("-----------------------------------------------------")
    for _, test in ipairs(self.completedTests) do
        if test.passed then
            self:_passLog(test.name)
        else
            self:_failLog(test.name)
        end
    end
    local passed = self:_color(string.format("%d passed", self.testsPassed), 'green')
    local failed = self:_color(string.format("%d failed", self.testsFailed), self.testsFailed > 0 and 'red' or 'white')
    local total = string.format("%d total", self.totalTests)

    if self.testsPassed == self.totalTests then
        self:_logSuccess("✔️ %s PASSED ✔️", self.testsName:upper())
    else
        self:_logFailure("❌ %s FAILED ❌", self.testsName:upper())
    end
    self:_logHeader("-----------------------------------------------------")
    if self.afterAll then
        self:afterAll()
    end
    self:reset()
    if exitAfter or self.exitAfter then
        os.exit(0)
    end
end

------------------------------------------
-- Private functions
------------------------------------------

---@private
function UnitWind:_hasSpy(object, key)
    return (self.spies and self.spies[object] and self.spies[object][key]) ~= nil
end

---@private
function UnitWind:_hasMock(object)
    return (self.mocks and self.mocks[object]) ~= nil
end

---@private
function UnitWind:_color(message, color)
    if self.highlight then
        message = ansicolors('%' .. string.format('{%s}%s', color, message))
    end
    return message
end

---@private
function UnitWind:_rawLog(message, ...)
    if not self.enabled then return end
    local prefix = self:_color("[UnitWind] ", 'bright magenta')
    local output = tostring(prefix .. message):format(...)
    --Prints to custom file if defined
    if self.outputFile then
        self.outputFile:write(output .. "\n")
        self.outputFile:flush()
    else
        --otherwise straight to mwse.log
        print(output)
    end
end

---@private
function UnitWind:_log(message, ...)
    self:_rawLog(message, ...)
end

---@private
function UnitWind:_logSuccess(message, ...)
    self:_rawLog(self:_color(message, 'greenbg black'), ...)
end

---@private
function UnitWind:_logFailure(message, ...)
    self:_rawLog(self:_color(message, 'redbg bright white'), ...)
end

---@private
function UnitWind:_logWhite(message, ...)
    self:_rawLog(self:_color(message, 'white'), ...)
end

---@private
function UnitWind:error(message, ...)
    self:_rawLog(self:_color(message, 'red'), ...)
end

---@private
function UnitWind:_logHeader(message, ...)
    self:_rawLog(self:_color(message, 'bright magenta'), ...)
end

---@private
function UnitWind:_passLog(message, ...)
    local pass = self:_color('✔️', 'green')
    self:_rawLog(pass .. " " .. message, ...)
end

---@private
function UnitWind:_failLog(message, ...)
    local fail = self:_color('❌', 'red')
    self:_rawLog(fail .. " " .. message, ...)
end

return UnitWind
