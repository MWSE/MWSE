--- @meta
--- @diagnostic disable:undefined-doc-name

--- A door game object. Data specific to a single door is stored on the door reference as destination and lock attachments. See ``tes3reference`` for details.
--- 
--- There is a special-case door with id ``PrisonMarker`` used for finding the nearest prison when the player is sent to jail. You should normally exclude this if you are iterating over all doors. Use the ``object.isLocationMarker`` property to detect these markers.
--- @class tes3door : tes3physicalObject, tes3object, tes3baseObject
--- @field closeSound tes3sound The sound to be played when the door closes.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field openSound tes3sound The sound to be played when the door opens.
--- @field script tes3script *Read-only*. The script that runs on the object.
tes3door = {}
