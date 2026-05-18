local Parent = require("mcm.components.infos.MouseOverInfo")

--- @class mwseMCMActiveInfo
local ActiveInfo = Parent:new()
ActiveInfo.__index = ActiveInfo

ActiveInfo.triggerOn = "MCM:refresh"
ActiveInfo.triggerOff = "--unused--"

return ActiveInfo
