---@class DependencyNotifier
local DependencyNotifier = {
    ---@type mwseLogger
    logger = nil,
    failedDependencies = nil,
    packageName = nil
}

function DependencyNotifier.new(cls, e)
    cls.__index = cls
    local obj = {
        packageName = e.packageName,
        failedDependencies = e.failedDependencies,
        logger = e.logger or require("logger").new {
            modName = "DependencyNotifier",
            moduleName = e.packageName
        }
    }
    return setmetatable(obj, cls)
end

---Display the list of failed dependencies to the player
---@param callback function The callback to call when the message box is closed
function DependencyNotifier:dependenciesFailMessage(callback)
    timer.frame.delayOneFrame(function()
        local function customBlock(parentBlock)
            parentBlock.widthProportional = 1.0
            parentBlock.borderTop = 6
            parentBlock.minWidth = 400
            --parentBlock.childAlignX = 0.5
            parentBlock:getTopLevelMenu():getContentElement().maxWidth = nil
            local scrollPane = parentBlock:createVerticalScrollPane()
            scrollPane.widthProportional = 1.0
            scrollPane.heightProportional = 1.0
            scrollPane.paddingAllSides = 2
            scrollPane.minHeight = 500

			self.logger:error("A list of dependencies not met:")
            for _, failure in pairs(self.failedDependencies) do
                local dependencyBlock = scrollPane:createThinBorder()
                dependencyBlock.flowDirection = "top_to_bottom"
                dependencyBlock.autoHeight = true
                dependencyBlock.widthProportional = 1.0
                dependencyBlock.paddingAllSides = 8
                dependencyBlock.paddingBottom = 12
                dependencyBlock.borderAllSides = 2
                --dependencyBlock.childAlignX = 0.5

                local title = failure.title or "NO TITLE"
                local titleLabel = dependencyBlock:createLabel { text = title }
                titleLabel.color = tes3ui.getPalette(tes3.palette.headerColor)
                titleLabel.wrapText = true
                titleLabel.justifyText = "center"
                titleLabel.widthProportional = 1.0
                titleLabel.borderBottom = 4
                titleLabel.color = tes3ui.getPalette(tes3.palette.headerColor)
				self.logger:error(title:gsub("\n", ""))

                for _, reason in pairs(failure.reasons) do
					local text = "- " .. reason
                    local depLabel = dependencyBlock:createLabel { text = text }
                    --depLabel.color = tes3ui.getPalette(tes3.palette.negativeColor)
                    depLabel.wrapText = true
                    --depLabel.justifyText = "center"
                    depLabel.widthProportional = 1.0
					self.logger:error(text)
                end
                if failure.resolveButton then
                    self.logger:assert(type(failure.resolveButton.text) == "string",
                        "DependencyNotifier:dependenciesFailMessage: resolveButton.text must be a string")
                    self.logger:assert(type(failure.resolveButton.callback) == "function",
                        "DependencyNotifier:dependenciesFailMessage: resolveButton.callback must be a function")
                    local button = dependencyBlock:createButton { text = failure.resolveButton.text }
                    button.widthProportional = 1.0
                    button.borderTop = 10
                    button:register("mouseClick", failure.resolveButton.callback)
                    if failure.resolveButton.tooltip then
                        button:register("help", function()
                            local tooltip = tes3ui.createTooltipMenu()
                            tooltip:createLabel{ text = failure.resolveButton.tooltip}
                        end)
                    end
                end
            end
            scrollPane.widget:contentsChanged()
            parentBlock:getTopLevelParent():updateLayout()
            parentBlock:updateLayout()
        end
        local buttons = {
            {
                text = tes3.findGMST(tes3.gmst.sOK).value,
                callback = function()
                    if callback then
                        callback()
                    end
                end,
            },
        }
        tes3ui.showMessageMenu {
            header = string.format("%s - Failed Dependencies", self.packageName),
            customBlock = customBlock,
            buttons = buttons,
        }
    end)
end


return DependencyNotifier