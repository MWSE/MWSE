local ease = {}

--[[
References:
- G7's Animation Blending: https://github.com/Greatness7/animationBlending/blob/main/Data%20Files/MWSE/mods/animationBlending/easing.lua
- https://wiki.facepunch.com/gmod/math.ease
- https://github.com/Facepunch/garrysmod/blob/master/garrysmod/lua/includes/extensions/math/ease.lua
- https://github.com/ai/easings.net / https://easings.net/

- https://github.com/EmmanuelOga/easing
- https://github.com/coronalabs/framework-easing/blob/f44f1c1f5736f8892ab1cb610560bf5e130cea13/easing.lua
- https://github.com/kikito/tween.lua
- https://stackoverflow.com/questions/41195063/general-smoothstep-equation
]]

local pi = math.pi

local cos = math.cos
local pow = math.pow
local sin = math.sin
local sqrt = math.sqrt

function ease.linear(x)
	return x
end

function ease.sineIn(x)
	return 1 - cos((x * pi) / 2)
end

function ease.sineOut(x)
	return sin((x * pi) / 2)
end

function ease.sineInOut(x)
	return -(cos(pi * x) - 1) / 2
end

function ease.quadIn(x)
	return x * x
end

function ease.quadOut(x)
	return 1 - (1 - x) * (1 - x)
end

function ease.quadInOut(x)
	if x < 0.5 then
		return 2 * x * x
	end

	return 1 - ((-2 * x + 2) ^ 2) / 2
end

function ease.cubicIn(x)
	return pow(x, 3)
end

function ease.cubicOut(x)
	return 1 - pow(1 - x, 3)
end

function ease.cubicInOut(x)
	return (x < 0.5) and (4 * x * x * x)
	    or (1 - pow(-2 * x + 2, 3) / 2)
end

function ease.quartIn(x)
	return pow(x, 4)
end

function ease.quartOut(x)
	return 1 - pow(1 - x, 4)
end

function ease.quartInOut(x)
	return (x < 0.5) and (8 * x * x * x * x)
	    or (1 - pow(-2 * x + 2, 4) / 2)
end

function ease.quintIn(x)
	return pow(x, 5)
end

function ease.quintOut(x)
	return 1 - pow(1 - x, 5)
end

function ease.quintInOut(x)
	return (x < 0.5) and (16 * x * x * x * x * x)
	    or (1 - pow(-2 * x + 2, 5) / 2)
end

function ease.expoIn(x)
	return x == 0 and 0 or (2 ^ (10 * x - 10))
end

function ease.expoOut(x)
	return x == 1 and 1 or 1 - (2 ^ (-10 * x))
end

function ease.expoInOut(x)
	return x == 0 and 0
	    or x == 1 and 1
		or x < 0.5 and (2 ^ (20 * x - 10)) / 2
		or (2 - (2 ^ (-20 * x + 10))) / 2
end

function ease.circIn(x)
	return 1 - sqrt(1 - x * x)
end

function ease.circOut(x)
	return sqrt(1 - (x - 1) * (x - 1))
end

function ease.circInOut(x)
	return x < 0.5 and (1 - sqrt(1 - ((2 * x ) ^ 2))) / 2
		or (sqrt(1 - ((-2 * x + 2) ^ 2)) + 1) / 2
end

local c1 = 1.70158
local c2 = c1 * 1.525
local c3 = c1 + 1
local c4 = ( 2 * pi ) / 3
local c5 = ( 2 * pi ) / 4.5
local n1 = 7.5625
local d1 = 2.75

function ease.backIn(x)
	return c3 * x ^ 3 - c1 * x ^ 2
end

function ease.backOut(x)
	return 1 + c3 * ((x - 1) ^ 3) + c1 * ((x - 1) ^ 2)
end

function ease.backInOut(x)
	return x < 0.5 and (((2 * x) ^ 2) * ((c2 + 1) * 2 * x - c2)) / 2
	    or (((2 * x - 2) ^ 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2
end

function ease.elasticIn(x)
	return x == 0 and 0
	    or x == 1 and 1
		or -(2 ^ (10 * x - 10)) * sin((x * 10 - 10.75) * c4)
end

function ease.elasticOut(x)
	return x == 0 and 0
	    or x == 1 and 1
		or (2 ^ (-10 * x)) * sin((x * 10 - 0.75) * c4) + 1
end

function ease.elasticInOut(x)
	return x == 0 and 0
	    or x == 1 and 1
		or x < 0.5 and -((2 ^ (20 * x - 10)) * sin((20 * x - 11.125) * c5)) / 2
		or ((2 ^ (-20 * x + 10)) * sin((20 * x - 11.125) * c5)) / 2 + 1
end

function ease.bounceIn(x)
	return 1 - ease.bounceOut(1 - x)
end

function ease.bounceOut(x)
	if (x < 1 / d1) then
		return n1 * x ^ 2
	elseif (x < 2 / d1) then
		x = x - (1.5 / d1)
		return n1 * x ^ 2 + 0.75
	elseif (x < 2.5 / d1) then
		x = x - (2.25 / d1)
		return n1 * x ^ 2 + 0.9375
	else
		x = x - (2.625 / d1)
		return n1 * x ^ 2 + 0.984375
	end
end

function ease.bounceInOut(x)
	return x < 0.5 and (1 - ease.bounceOut(1 - 2 * x)) / 2
	    or (1 + ease.bounceOut(2 * x - 1)) / 2
end

function ease.smoothstep(x)
	return x * x * (3 - 2 * x)
end

function ease.smoothstepInverse(x)
	return 0.5 - sin(math.asin(1 - 2 * x) / 3)
end

local function springOutGeneric(x, lambda, w)
	-- Higher lambda = lower swing amplitude. 1 = 150% swing amplitude.
	-- W corresponds to the amount of overswings, more = more. 4.71 = 1 overswing, 7.82 = 2
	return 1 - math.exp(-lambda * x) * math.cos(w * x)
end

function ease.springOutWeak(x)
	return springOutGeneric(x, 4, 4.71)
end

function ease.springOutMed(x)
	return springOutGeneric(x, 3, 4.71)
end

function ease.springOutStrong(x)
	return springOutGeneric(x, 2, 4.71)
end

function ease.springOutTooMuch(x)
	return springOutGeneric(x, 1, 4.71)
end


return ease
