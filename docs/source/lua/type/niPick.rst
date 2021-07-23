niPick
====================================================================================================

Class used in the picking intersection system within the game engine.  Using a ray, the engine performs picking operations on a scene graph or any subtree. Given a ray and a subtree, the subtree is traversed and matching nodes are added to an array.

Properties
----------------------------------------------------------------------------------------------------

`coordinateType`_ (`niPickCoordinateType`_)
    The coordinate type for the object.

`frontOnly`_ (`boolean`_)
    If true, back face triangles (those with clockwise winding order) are culled prior to intersection calculations. If false, intersections of ray with triangles are computed regardless of triangle orientation.
    

`intersectType`_ (`niPickIntersectType`_)
    The intersect type for the object.

`lastAddedRecord`_ (`niPickRecord`_)
    The pick record that was last added to the object.

`observeAppCullFlag`_ (`boolean`_)
    If true, the pick stops if it encounters an object with the 'app culled' flag set.  If false, the pick will continue over 'app culled' objects.
    

`pickType`_ (`niPickType`_)
    The pick type for the object.

`results`_ (`niPickRecordTArray`_)
    The array of pick records obtained by the last call to PickObjects.

`returnColor`_ (`boolean`_)
    If true, a vertex color value is returned for the point of intersection of triangles during picking.

`returnNormal`_ (`boolean`_)
    If true, a unit-length normal is returned for the point of intersection of triangles intersected during the picking.

`returnSmoothNormal`_ (`boolean`_)
    If true and if vertex normals exist, then a unit-length normal vector is interpolated from the vertex normals and returned during the picking operation.

`returnTexture`_ (`boolean`_)
    If true, texture coordinates are returned for the point of intersection of triangles during the picking operation.

`root`_ (`niPickType`_)
    The root pick type for the object.

`sortType`_ (`niPickSortType`_)
    The sort type for the object.

.. toctree::
    :hidden:

    niPick/coordinateType
    niPick/frontOnly
    niPick/intersectType
    niPick/lastAddedRecord
    niPick/observeAppCullFlag
    niPick/pickType
    niPick/results
    niPick/returnColor
    niPick/returnNormal
    niPick/returnSmoothNormal
    niPick/returnTexture
    niPick/root
    niPick/sortType

.. _`coordinateType`: niPick/coordinateType.html
.. _`frontOnly`: niPick/frontOnly.html
.. _`intersectType`: niPick/intersectType.html
.. _`lastAddedRecord`: niPick/lastAddedRecord.html
.. _`observeAppCullFlag`: niPick/observeAppCullFlag.html
.. _`pickType`: niPick/pickType.html
.. _`results`: niPick/results.html
.. _`returnColor`: niPick/returnColor.html
.. _`returnNormal`: niPick/returnNormal.html
.. _`returnSmoothNormal`: niPick/returnSmoothNormal.html
.. _`returnTexture`: niPick/returnTexture.html
.. _`root`: niPick/root.html
.. _`sortType`: niPick/sortType.html

Methods
----------------------------------------------------------------------------------------------------

`clearResults`_
    Clears the results of any previous picking operation.

`pickObjects`_
    Performs the picking operation.

.. toctree::
    :hidden:

    niPick/clearResults
    niPick/pickObjects

.. _`clearResults`: niPick/clearResults.html
.. _`pickObjects`: niPick/pickObjects.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niPickRecord`: ../../lua/type/niPickRecord.html
