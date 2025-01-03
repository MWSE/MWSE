local uw = require("unitwind").new{
    enabled = true,
    --- ... other settings ...
    highlight=false
}
local Logger = require "Logger" ---@type Logger


uw:start("Object creation (no module name)")
uw:test("passing a string", function()
    local log = Logger.new "my mod1" ---@type Logger

    uw:expect(log.modName).toBe("my mod1")
    uw:expect(log.moduleName).toBe(nil)
    uw:expect(log.level).toBe(Logger.LEVEL.INFO)
    uw:expect(log.children).toBeType("table")
    uw:expect(#log.children).toBe(0)
    uw:expect(log.file).toBe(nil)
    uw:expect(log.writeToFile).toBe(false)
    uw:expect(log.useColors).toBe(false)
    uw:expect(log.includeTimestamp).toBe(false)


end)

uw:test("passing a table", function()
    local log = Logger.new{modName="my mod2"} ---@type Logger

    uw:expect(log.modName).toBe("my mod2")
    uw:expect(log.moduleName).toBe(nil)
    uw:expect(log.level).toBe(Logger.LEVEL.INFO)
    uw:expect(log.children).toBeType("table")
    uw:expect(#log.children).toBe(0)
    uw:expect(log.file).toBe(nil)
    uw:expect(log.writeToFile).toBe(false)
    uw:expect(log.useColors).toBe(false)
    uw:expect(log.includeTimestamp).toBe(false)
end)
 
uw:test("with timestamps", function()
    local log = Logger.new{modName="my mod3",includeTimestamp=true} ---@type Logger

    uw:expect(log.modName).toBe("my mod3")
    uw:expect(log.moduleName).toBe(nil)
    uw:expect(log.level).toBe(Logger.LEVEL.INFO)
    uw:expect(log.children).toBeType("table")
    uw:expect(#log.children).toBe(0)
    uw:expect(log.file).toBe(nil)
    uw:expect(log.writeToFile).toBe(false)
    uw:expect(log.useColors).toBe(false)
    uw:expect(log.includeTimestamp).toBe(true)
end)

uw:test("with colors", function()
    local log = Logger.new{modName="my mod4", useColors=true} ---@type Logger

    uw:expect(log.modName).toBe("my mod4")
    uw:expect(log.moduleName).toBe(nil)
    uw:expect(log.level).toBe(Logger.LEVEL.INFO)
    uw:expect(log.children).toBeType("table")
    uw:expect(#log.children).toBe(0)
    uw:expect(log.file).toBe(nil)
    uw:expect(log.writeToFile).toBe(false)
    uw:expect(log.useColors).toBe(true)
    uw:expect(log.includeTimestamp).toBe(false)
end)

uw:test("with log level", function()
    local log = Logger.new{modName="my mod5", level="DEBUG"} ---@type Logger

    uw:expect(log.modName).toBe("my mod5")
    uw:expect(log.moduleName).toBe(nil)
    uw:expect(log.level).toBe(Logger.LEVEL.DEBUG)
    uw:expect(log.children).toBeType("table")
    uw:expect(#log.children).toBe(0)
    uw:expect(log.file).toBe(nil)
    uw:expect(log.writeToFile).toBe(false)
    uw:expect(log.useColors).toBe(false)
    uw:expect(log.includeTimestamp).toBe(false)
end)

uw:finish()
Logger = dofile("Logger")
uw:start("object creation (with module name)")

uw:test("passing a string", function()
    local log = Logger.new "my mod6/my module" ---@type Logger

    uw:expect(log.modName).toBe("my mod6")
    uw:expect(log.moduleName).toBe("my module")
    uw:expect(log.level).toBe(Logger.LEVEL.INFO)
    uw:expect(log.children).toBeType("table")
    uw:expect(#log.children).toBe(0)
    uw:expect(log.file).toBe(nil)
    uw:expect(log.writeToFile).toBe(false)
    uw:expect(log.useColors).toBe(false)
    uw:expect(log.includeTimestamp).toBe(false)


end)

uw:test("passing a table", function()
    local log = Logger.new{modName="my mod7", moduleName="my module"} ---@type Logger

    uw:expect(log.modName).toBe("my mod7")
    uw:expect(log.moduleName).toBe("my module")
    uw:expect(log.level).toBe(Logger.LEVEL.INFO)
    uw:expect(log.children).toBeType("table")
    uw:expect(#log.children).toBe(0)
    uw:expect(log.file).toBe(nil)
    uw:expect(log.writeToFile).toBe(false)
    uw:expect(log.useColors).toBe(false)
    uw:expect(log.includeTimestamp).toBe(false)
end)

uw:test("passing a table, but including the module name in modName", function()
    local log = Logger.new{modName="my mod8/my module"} ---@type Logger

    uw:expect(log.modName).toBe("my mod8")
    uw:expect(log.moduleName).toBe("my module")
    uw:expect(log.level).toBe(Logger.LEVEL.INFO)
    uw:expect(log.children).toBeType("table")
    uw:expect(#log.children).toBe(0)
    uw:expect(log.file).toBe(nil)
    uw:expect(log.writeToFile).toBe(false)
    uw:expect(log.useColors).toBe(false)
    uw:expect(log.includeTimestamp).toBe(false)
end)

uw:finish()

uw:start("creating a child logger")

uw:test("via makeChild", function()
    local parent = Logger.new{modName="my mod9", level="DEBUG",includeTimestamp=true,useColors=true,writeToFile=true} ---@type Logger
    local child = parent:makeChild("child module")
    uw:expect(child.modName).toBe(parent.modName)
    uw:expect(child.moduleName).toBe("child module")
    uw:expect(child.level).toBe(parent.level)
    uw:expect(child.children).toBeType("table")
    uw:expect(#child.children).toBe(0)
    uw:expect(child.writeToFile).toBe(parent.writeToFile)
    uw:expect(child.useColors).toBe(parent.useColors)
    uw:expect(child.includeTimestamp).toBe(parent.includeTimestamp)

    uw:expect(parent.children[child.moduleName]).toBe(child)
end)

uw:test("via __concat", function()
    local parent = Logger.new{modName="my mod10", level="DEBUG",includeTimestamp=true,useColors=true,writeToFile=true} ---@type Logger
    local child = parent .. ("child module")
    uw:expect(child.modName).toBe(parent.modName)
    uw:expect(child.moduleName).toBe("child module")
    uw:expect(child.level).toBe(parent.level)
    uw:expect(child.children).toBeType("table")
    uw:expect(child.writeToFile).toBe(parent.writeToFile)
    uw:expect(child.useColors).toBe(parent.useColors)
    uw:expect(child.includeTimestamp).toBe(parent.includeTimestamp)

    uw:expect(parent.children[child.moduleName]).toBe(child)

end)

uw:test("via Logger.new", function()
    local parent = Logger.new{modName="my mod12", level="DEBUG",includeTimestamp=true,useColors=true,writeToFile=true} ---@type Logger
    local child = Logger.new{modName="my mod12", moduleName="child module"}
    uw:expect(child.modName).toBe(parent.modName)
    uw:expect(child.moduleName).toBe("child module")
    uw:expect(child.level).toBe(parent.level)
    uw:expect(child.children).toBeType("table")
    uw:expect(#child.children).toBe(0)
    uw:expect(child.writeToFile).toBe(parent.writeToFile)
    uw:expect(child.useColors).toBe(parent.useColors)
    uw:expect(child.includeTimestamp).toBe(parent.includeTimestamp)

    uw:expect(parent.children[child.moduleName]).toBe(child)
end)


uw:finish()

Logger = dofile "Logger"
uw:start("properties of child loggers")

uw:test("setLevel", function()
    local parent = Logger.new{modName="my mod13", level="DEBUG",includeTimestamp=true,useColors=true,writeToFile=true} ---@type Logger
    local child1 = parent .. "child1" ---@type Logger 
    local child2 = parent .. "child2" ---@type Logger
    local child3 = parent .. "child3" ---@type Logger

    uw:expect(child1.level).toBe(parent.level)
    uw:expect(child2.level).toBe(parent.level)
    uw:expect(child3.level).toBe(parent.level)

    child1:setLevel(Logger.LEVEL.ERROR)

    uw:expect(child1.level).toBe(parent.level)
    uw:expect(child2.level).toBe(parent.level)
    uw:expect(child3.level).toBe(parent.level)

    child2:setLevel(Logger.LEVEL.INFO)

    uw:expect(child1.level).toBe(parent.level)
    uw:expect(child2.level).toBe(parent.level)
    uw:expect(child3.level).toBe(parent.level)

    child3:setLevel("TRACE")

    uw:expect(child1.level).toBe(parent.level)
    uw:expect(child2.level).toBe(parent.level)
    uw:expect(child3.level).toBe(parent.level)

    parent:setLevel("WARN")

    uw:expect(child1.level).toBe(parent.level)
    uw:expect(child2.level).toBe(parent.level)
    uw:expect(child3.level).toBe(parent.level)



end)

uw:test("includeTimestamp", function()
    local parent = Logger.new{modName="my mod14", level="DEBUG",includeTimestamp=true,useColors=true,writeToFile=true} ---@type Logger
    local child1 = parent .. "child1" ---@type Logger 
    local child2 = parent .. "child2" ---@type Logger
    local child3 = parent .. "child3" ---@type Logger

    uw:expect(child1.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child2.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child3.includeTimestamp).toBe(parent.includeTimestamp)

    child1:setIncludeTimestamp(true)

    
    uw:expect(child1.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child2.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child3.includeTimestamp).toBe(parent.includeTimestamp)

    uw:expect(child1.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child2.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child3.includeTimestamp).toBe(parent.includeTimestamp)

    child1:setIncludeTimestamp(false)

    
    uw:expect(child1.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child2.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child3.includeTimestamp).toBe(parent.includeTimestamp)

    child2:setIncludeTimestamp(true)

    
    uw:expect(child1.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child2.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child3.includeTimestamp).toBe(parent.includeTimestamp)

    child3:setIncludeTimestamp(false)

    
    uw:expect(child1.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child2.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child3.includeTimestamp).toBe(parent.includeTimestamp)

    parent:setIncludeTimestamp(true)

    
    uw:expect(child1.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child2.includeTimestamp).toBe(parent.includeTimestamp)
    uw:expect(child3.includeTimestamp).toBe(parent.includeTimestamp)


end)
uw:finish()

Logger = dofile "Logger"

uw:start("Printing: _makeHeader")
uw:test("no module name", function()
    local logger = Logger.new{modName="my mod", level="DEBUG"} ---@type Logger
    uw:expect(logger:_makeHeader("DEBUG")).toBe("my mod: DEBUG")
end)

uw:test("module name", function()
    local logger = Logger.new{modName="my mod", moduleName="my module", level="DEBUG"} ---@type Logger
    uw:expect(logger:_makeHeader("DEBUG")).toBe("my mod (my module): DEBUG")
end)
uw:finish()


Logger = dofile "Logger"

uw:start("Printing: write")

local test_str

uw:mock(_G, "print", function(s) test_str = s end)


uw:test("no module name", function()
    local logger = Logger.new{modName="my mod", level="DEBUG"} ---@type Logger
    logger:debug("this %s a %s %i", "is", "test", 23)
    uw:expect(test_str).toBe("[my mod: DEBUG] this is a test 23")
end)

uw:test("module name", function()
    local logger = Logger.new{modName="my mod", moduleName="my module", level="DEBUG"} ---@type Logger
    logger:debug("this %s a %s %i", "is", "test", 23)
    uw:expect(test_str).toBe("[my mod (my module): DEBUG] this is a test 23")
end)

uw:test("module name (again)", function()
    local logger = Logger.new{modName="my mod1", moduleName="my module", level="INFO"} ---@type Logger
    -- other stuff will try to print things, just make sure it's not the expected result
    logger:info("this %s a %s %i", "is", "test", 23)
    uw:expect(test_str).toBe("[my mod1 (my module): INFO] this is a test 23")
end)
test_str = nil


uw:test("printing only happens at appropriate log levels", function()
    local logger = Logger.new{modName="my mod1", moduleName="my module", level="INFO"} ---@type Logger
    -- other stuff will try to print things, just make sure it's not the expected result
    logger:debug("this %s a %s %i", "is", "test", 23)
    uw:expect(test_str).NOT.toBe("[my mod1 (my module): DEBUG] this is a test 23")
end)
uw:clearMocks()

uw:finish()


Logger = dofile "Logger"

uw:start("Printing: writet")

test_str = nil

uw:mock(_G, "print", function(s) test_str = s end)


uw:test("msg (no module name)", function()
    local logger = Logger.new{modName="my mod", level="DEBUG"} ---@type Logger
    logger:debugt{msg="this %s a %s %i", args={"is","test", 23}}
    uw:expect(test_str).toBe("[my mod: DEBUG] this is a test 23")
end)

uw:test("msg (w/ module name)", function()
    local logger = Logger.new{modName="my mod", moduleName="my module", level="DEBUG"} ---@type Logger
    logger:debugt{msg="this %s a %s %i", args={"is","test", 23}}
    uw:expect(test_str).toBe("[my mod (my module): DEBUG] this is a test 23")
end)


uw:test("sep (no module name)", function()
    local logger = Logger.new{modName="my mod", level="DEBUG"} ---@type Logger
    logger:debugt{sep=", ", args={"printing arguments", "second", "third", "now starting associative arguments", a="first_assoc", b="second_assoc"}}
    uw:expect(test_str).toBe("[my mod: DEBUG] printing arguments, second, third, now starting associative arguments, a=first_assoc, b=second_assoc")
end)

uw:test("sep (w/ module name)", function()
    local logger = Logger.new{modName="my mod", moduleName="my module", level="DEBUG"} ---@type Logger
    logger:debugt{sep=", ", args={"printing arguments", "second", "third", "now starting associative arguments", a="first_assoc", b="second_assoc"}}
    uw:expect(test_str).toBe("[my mod (my module): DEBUG] printing arguments, second, third, now starting associative arguments, a=first_assoc, b=second_assoc")
end)

test_str = nil


uw:test("printing only happens at appropriate log levels", function()
    local logger = Logger.new{modName="my mod1", moduleName="my module", level="INFO"} ---@type Logger
    logger:debugt{sep=", ", args={"printing arguments", "second", "third", "now starting associative arguments", a="first_assoc", b="second_assoc"}}
    -- other stuff will try to print things, just make sure it's not the expected result
    uw:expect(test_str).NOT.toBe("[my mod1 (my module): DEBUG] printing arguments, second, third, now starting associative arguments, a=first_assoc, b=second_assoc")
end)
-- need to do this so results can be printed
uw:clearMocks()

uw:finish()

uw:finish(true)