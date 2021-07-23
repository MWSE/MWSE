--- @meta

--- Object that represent a camera. Position and orientation is determined by parent properties.
--- @class niCamera : niAVObject, niObjectNET, niObject
--- @field lodAdjust number The level-of-detail adjustment value for the camera. A higher value will lower the LOD of the objects drawn by the camera.
--- @field port tes3vector4 The port on the backbuffer of the camera.
--- @field renderer niObject The renderer of the camera.
--- @field scene niObject The scene of the camera.
--- @field viewDistance number The view distance of the camera.
--- @field worldDirection tes3vector3 Defines the X-axis of the camera.
--- @field worldRight tes3vector3 Defines the Z-axis of the camera.
--- @field worldToCamera tes3matrix44 The world-to-camera transform matrix in row-major order.
--- @field worldUp tes3vector3 Defines the Y-axis of the camera.
niCamera = {}

--- Given a screen space position, calculates the world position and outlook direction. This can be useful when trying to find a reference under a UI element, such as the cusor.
--- @param point tes3vector2 The screen position to calculate a world ray for. Screen space is measured as ([-screenWidth/2, screenWidth/2], [-screenHeight/2, screenHeight/2]) with up-right being positive and an origin at the center of the screen.
--- @return tes3vector3 origin The world point that the given screen position looks out from.
--- @return tes3vector3 direction The look direction of the camera from the given origin point.
function niCamera:windowPointToRay(point) end

--- Given a world coordinate position, calculates where that point relates to the screen space.
--- @param point tes3vector3 The world position to calculate screen position for.
--- @return any result No description yet available.
function niCamera:worldPointToScreenPoint(point) end

