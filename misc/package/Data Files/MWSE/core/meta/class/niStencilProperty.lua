--- @meta

--- A rendering property that controls the use of a stencil buffer when rendering. It also includes a draw-mode setting to allows the game engine to control the culling mode of a set of geometry.
--- @class niStencilProperty : niProperty, niObjectNET, niObject
--- @field drawMode number The drawing mode used to draw the object.
--- @field enabled boolean The value of the stencil buffer enable flag.
--- @field failAction number The action that is taken in the stencil buffer when the stencil test fails.
--- @field mask number The mask value of the stencil buffer.
--- @field passAction number The action that is taken in the stencil buffer when the stencil test passes and the pixel passes the Z-buffer test.
--- @field reference number The stencil reference.
--- @field testFunc number The stencil buffer test function used to test the reference value against the buffer value.
--- @field zFailAction number The action that is taken in the stencil buffer when the stencil test passes but the pixel fails the Z-buffer test.
niStencilProperty = {}

