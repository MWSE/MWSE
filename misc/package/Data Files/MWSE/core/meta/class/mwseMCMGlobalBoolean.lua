--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- A variable connected to a Morrowind Global. Treats 0 as false and >=1 as true.
--- @class mwseMCMGlobalBoolean : mwseMCMGlobal, mwseMCMVariable
--- @field converter nil Converter isn't used for this Variable type.
mwseMCMGlobalBoolean = {}

--- Returns the value stored in the Morrowind Global variable.
--- @return boolean|nil value No description yet available.
function mwseMCMGlobalBoolean:get() end

--- Creates a new variable of this type.
--- @param variable string|mwseMCMGlobalBoolean.new.variable This table accepts the following values:
--- 
--- `id`: string — The id of the Morrowind Global.
--- @return mwseMCMGlobalBoolean variable No description yet available.
function mwseMCMGlobalBoolean:new(variable) end

---Table parameter definitions for `mwseMCMGlobalBoolean.new`.
--- @class mwseMCMGlobalBoolean.new.variable
--- @field id string The id of the Morrowind Global.

--- Changes the value stored in the Global variable.
--- @param newValue boolean No description yet available.
function mwseMCMGlobalBoolean:set(newValue) end

