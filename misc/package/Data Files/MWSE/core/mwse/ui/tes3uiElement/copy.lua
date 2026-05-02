
--
-- Specific delegates for copying element types.
--

local tes3uiElementCopyDelegates = {}

function tes3uiElementCopyDelegates.button(params)
	return params.to:createButton({
		id = params.from.id,
		text = params.from.text,
	})
end

function tes3uiElementCopyDelegates.image(params)
	return params.to:createImage({
		id = params.from.id,
		path = params.from.contentPath,
	})
end

function tes3uiElementCopyDelegates.layout(params)
	return params.to:createBlock({
		id = params.from.id,
	})
end

function tes3uiElementCopyDelegates.model(params)
	return params.to:createNif({
		id = params.from.id,
		path = params.from.contentPath,
	})
end

function tes3uiElementCopyDelegates.rect(params)
	return params.to:createRect({
		id = params.from.id,
	})
end

function tes3uiElementCopyDelegates.text(params)
	return params.to:createLabel({
		id = params.from.id,
		text = params.from.text,
	})
end


--
-- Specific delegates for duplicating element properties.
--

local tes3uiElementCopyPropertyDelegates = {}

tes3uiElementCopyPropertyDelegates[1] = function(new, prop)
	new:setPropertyInt(prop.id, prop.value)
end

tes3uiElementCopyPropertyDelegates[2] = function(new, prop)
	new:setPropertyFloat(prop.id, prop.value)
end

tes3uiElementCopyPropertyDelegates[8] = function(new, prop)
	new:setPropertyObject(prop.id, prop.value)
end

tes3uiElementCopyPropertyDelegates[16] = function(new, prop)
	local value = prop.value
	if (type(value) == "boolean") then
		new:setPropertyBool(prop.id, value)
	else
		new:setPropertyProperty(prop.id, value)
	end
end

tes3uiElementCopyPropertyDelegates[32] = function(new, prop)
	new:setPropertyCallback(prop.id, prop.value)
end

-- An array of properties to copy over.
local basicPropertiesToCopy = { "absolutePosAlignX", "absolutePosAlignY", "alpha", "autoHeight", "autoWidth", "borderAllSides", "borderBottom", "borderLeft", "borderRight", "borderTop", "childAlignX", "childAlignY", "childOffsetX", "childOffsetY", "color", "consumeMouseEvents", "contentPath", "disabled", "flowDirection", "font", "height", "heightProportional", "imageFilter", "imageScaleX", "imageScaleY", "justifyText", "maxHeight", "maxWidth", "minHeight", "minWidth", "nodeMaxX", "nodeMaxY", "nodeMinX", "nodeMinY", "nodeOffsetX", "nodeOffsetY", "paddingAllSides", "paddingBottom", "paddingLeft", "paddingRight", "paddingTop", "positionX", "positionY", "rawText", "repeatKeys", "scaleMode", "text", "texture", "visible", "width", "widthProportional", "wrapText" }

function tes3uiElement:copy(params)
	assert(type(params) == "table", "Invalid parameter. This function must be called with table parameters.")
	assert(params.to, "Invalid 'to' parameter provided.")
	params.from = self

	-- Create our copy.
	local elementType = self.type
	local delegate = tes3uiElementCopyDelegates[elementType]
	if not delegate then
		error(string.format("No copy function found for element of type %s.", elementType))
	end
	local newElement = delegate(params)

	-- Copy basic properties.
	for _, prop in ipairs(basicPropertiesToCopy) do
		local value = self[prop]
		if (value ~= nil) then
			newElement[prop] = value
		end
	end

	-- Copy over children.
	if (table.get(params, "copyChildren", true)) then
		for _, child in ipairs(params.from.children or {}) do
			local childParams = table.copy(params)
			childParams.to = newElement
			child:copy(childParams)
		end
	end

	-- Copy UI properties?
	if (table.get(params, "copyProperties", true)) then
		for _, prop in ipairs(self.properties) do
			local propDelegate = tes3uiElementCopyPropertyDelegates[prop.type]
			if not propDelegate then
				error(string.format("No copy function found for element of type %s.", prop.type))
			end
			propDelegate(newElement, prop)
		end
		mwse.copyLuaCallbacks(self, newElement) --- @diagnostic disable-line
	end

	return newElement
end
