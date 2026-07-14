--- @diagnostic disable: duplicate-set-field

-- Seed random number generator.
math.randomseed(os.time())

assert(math.approach)
assert(math.lerp)
assert(math.clamp)
assert(math.remap)
assert(math.remapclamped)
assert(math.round)
assert(math.isclose)
assert(math.nextpoweroftwo)
assert(math.normalizeangle)
assert(math.eerp)
assert(math.oscillate)
assert(math.smoothstep)
assert(math.smootherstep)
assert(math.snap)

-- Legacy naming.
math.nextPowerOfTwo = math.nextpoweroftwo

-------------------------------------------------
-- Extend base API: math.ease
-------------------------------------------------

math.ease = require("math.ease")
