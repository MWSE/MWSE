--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- A rendering property that controls the use of a stencil buffer when rendering. Stencil buffering allows effects such as cutouts in a screen, decal polygons without Z-buffer "aliasing", and advanced effects such as volumetric shadows. It also includes a draw-mode setting to allow the game engine to control the culling mode of a set of geometry.
--- 
--- This table describes the actions that can be set to occur as a result of tests for niStencilProperty:
--- 
--- Value | Mode             | Description
--- ----- | ---------------- | -------------
--- 0     | ACTION_KEEP      | Keep the current value in the stencil buffer.
--- 1	  | ACTION_ZERO      | Write zero to the stencil buffer.
--- 2	  | ACTION_REPLACE   | Write the reference value to the stencil buffer.
--- 3	  | ACTION_INCREMENT | Increment the value in the stencil buffer.
--- 4     | ACTION_DECREMENT | Decrement the value in the stencil buffer.
--- 5     | ACTION_INVERT    | Bitwise invert the value in the stencil buffer.
--- @class niStencilProperty : niProperty, niObjectNET, niObject
--- @field drawMode integer The face drawing (culling) mode used to draw the object.
--- 
--- Value | Mode             | Behavior
--- ----- | ---------------- | ---------
--- 0     | DRAW_CCW_OR_BOTH | The default mode, chooses between DRAW_CCW or DRAW_BOTH.
--- 1	  | DRAW_CCW         | Draw only the triangles whose vertices are ordered counter-clockwise with respect to the viewer (Standard behavior).
--- 2	  | DRAW_CW          | Draw only the triangles whose vertices are ordered clockwise with respect to the viewer (Effectively flips the faces).
--- 3	  | DRAW_BOTH        | Do not cull back faces of any kind. Draw all triangles, regardless of orientation (Effectively force double-sided).
--- @field enabled boolean The value of the stencil buffer enable flag.
--- @field failAction integer The action that is taken in the stencil buffer when the stencil test fails. See the table at the top for available actions.
--- @field mask integer The mask value of the stencil buffer. This value is AND-ed with the `reference` and the buffer value prior to comparing and writing the buffer. The default is `0xffffffff`.
--- @field passAction integer The action that is taken in the stencil buffer when the stencil test passes and the pixel passes the Z-buffer test. See the table at the top for available actions.
--- @field reference integer The stencil reference value. It's compared against the stencil value at an individual pixel to determine the success of the stencil test.
--- @field testFunc integer The stencil buffer test function used to test the reference value against the buffer value.
--- 
--- Value | Mode               | Description
--- ----- | ------------------ | -------------
--- 0     | TEST_NEVER         | Test will allways return false. Nothing is drawn at all.
--- 1	  | TEST_LESS          | The test will only succeed if the pixel is nearer than the previous pixel.
--- 2	  | TEST_EQUAL         | Test will only succeed if the z value of the pixel to be drawn is equal to the value of the previous drawn pixel.
--- 3	  | TEST_LESS_EQUAL    | Test will succeed if the z value of the pixel to be drawn is smaller than or equal to the value in the Stencil Buffer.
--- 4     | TEST_GREATER       | Opposite of TEST_LESS
--- 5     | TEST_NOT_EQUAL     | Test will succeed if the z value of the pixel to be drawn is NOT equal to the value of the previously drawn pixel.
--- 6     | TEST_GREATER_EQUAL | Opposite of TEST_LESS_EQUAL.
--- 7	  | TEST_ALWAYS        | Test will allways succeed. The Stencil Buffer value is ignored.
--- 
--- @field zFailAction integer The action that is taken in the stencil buffer when the stencil test passes but the pixel fails the Z-buffer test. See the table at the top for available actions.
niStencilProperty = {}

--- Creates a new, niStencilProperty. By default it will use a GREATER test function, KEEP on fail, INCREMENT on pass, and use the CCW_OR_BOTH draw mode.
--- @return niStencilProperty property No description yet available.
function niStencilProperty.new() end

