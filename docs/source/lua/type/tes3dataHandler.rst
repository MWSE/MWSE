tes3dataHandler
====================================================================================================

A core game object used for storing both active and non-dynamic gameplay data.

Properties
----------------------------------------------------------------------------------------------------

`backgroundThread`_ (`number`_)
    A Windows handle to the background processing thread.

`backgroundThreadId`_ (`number`_)
    The thread ID for the background processing thread.

`backgroundThreadRunning`_ (`boolean`_)
    Access to the running state for the background processing thread.

`cellChanged`_ (`boolean`_)
    A flag set for the frame that the player has changed cells.

`centralGridX`_ (`boolean`_)
    The position of the origin horizontal grid coordinate.

`centralGridY`_ (`boolean`_)
    The position of the origin longitudinal grid coordinate.

`currentCell`_ (`tes3cell`_)
    Access to the cell that the player is currently in.

`currentInteriorCell`_ (`tes3cell`_)
    Access to the current interior cell, if the player is in an interior.

`exteriorCells`_ (`table`_)
    A table of nine tes3cellExteriorData objects for any loaded exterior cells.

`lastExteriorCell`_ (`tes3cell`_)
    Access to the last visited exterior cell.

`mainThread`_ (`number`_)
    A Windows handle to the main execution thread.

`mainThreadId`_ (`number`_)
    The thread ID for the main execution thread.

`nonDynamicData`_ (`tes3nonDynamicData`_)
    A child structure where core game objects are held.

`threadSleepTime`_ (`number`_)
    No description available.

`worldObjectRoot`_ (`niNode`_)
    One of the core parent scene graph nodes.

`worldPickLandscapeRoot`_ (`niNode`_)
    One of the core parent scene graph nodes.

`worldPickObjectRoot`_ (`niNode`_)
    One of the core parent scene graph nodes.

.. toctree::
    :hidden:

    tes3dataHandler/backgroundThread
    tes3dataHandler/backgroundThreadId
    tes3dataHandler/backgroundThreadRunning
    tes3dataHandler/cellChanged
    tes3dataHandler/centralGridX
    tes3dataHandler/centralGridY
    tes3dataHandler/currentCell
    tes3dataHandler/currentInteriorCell
    tes3dataHandler/exteriorCells
    tes3dataHandler/lastExteriorCell
    tes3dataHandler/mainThread
    tes3dataHandler/mainThreadId
    tes3dataHandler/nonDynamicData
    tes3dataHandler/threadSleepTime
    tes3dataHandler/worldObjectRoot
    tes3dataHandler/worldPickLandscapeRoot
    tes3dataHandler/worldPickObjectRoot

.. _`backgroundThread`: tes3dataHandler/backgroundThread.html
.. _`backgroundThreadId`: tes3dataHandler/backgroundThreadId.html
.. _`backgroundThreadRunning`: tes3dataHandler/backgroundThreadRunning.html
.. _`cellChanged`: tes3dataHandler/cellChanged.html
.. _`centralGridX`: tes3dataHandler/centralGridX.html
.. _`centralGridY`: tes3dataHandler/centralGridY.html
.. _`currentCell`: tes3dataHandler/currentCell.html
.. _`currentInteriorCell`: tes3dataHandler/currentInteriorCell.html
.. _`exteriorCells`: tes3dataHandler/exteriorCells.html
.. _`lastExteriorCell`: tes3dataHandler/lastExteriorCell.html
.. _`mainThread`: tes3dataHandler/mainThread.html
.. _`mainThreadId`: tes3dataHandler/mainThreadId.html
.. _`nonDynamicData`: tes3dataHandler/nonDynamicData.html
.. _`threadSleepTime`: tes3dataHandler/threadSleepTime.html
.. _`worldObjectRoot`: tes3dataHandler/worldObjectRoot.html
.. _`worldPickLandscapeRoot`: tes3dataHandler/worldPickLandscapeRoot.html
.. _`worldPickObjectRoot`: tes3dataHandler/worldPickObjectRoot.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`tes3nonDynamicData`: ../../lua/type/tes3nonDynamicData.html
.. _`table`: ../../lua/type/table.html
.. _`number`: ../../lua/type/number.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
