--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- This class contains all the information needed to deform vertices by a single bone.
--- @class niSkinDataBoneData
--- @field bounds niBound This bounding volume is used internally by the skinning system to calculate bounding volumes for skinned objects without actually having to calculate the positions of the vertices in the skinned object.
--- @field transform tes3transform Defines the transform of the bone in the bind pose from the skinned object's coordinate system to the bone's coordinate system.
--- @field weights niSkinDataBoneDataVertexWeight[] *Read-only*. Contains all the per-vertex weight coefficients used when deforming the verticies influenced by this bone.
