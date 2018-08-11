
tes3.loadMesh
========================================================

**Parameters:**

- ``string`` **path**: The path, relative to *Data Files\\Meshes*, of the mesh.

**Returned:**

- ``niObject`` **mesh**: The root node of the mesh that was loaded, or ``nil`` if it couldn't be.

This function loads a mesh from the Meshes folder, and returns a scene graph object. Note that this reads from disk, and can be slow.

.. _`niObject`: ../../type/ni/object.html
