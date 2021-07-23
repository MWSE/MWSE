tes3dataHandler
====================================================================================================

A core game object used for storing both active and non-dynamic gameplay data.

Properties
----------------------------------------------------------------------------------------------------

`backgroundThread`_ (`number`_)
    Read-only. A Windows handle to the background processing thread.

`backgroundThreadId`_ (`number`_)
    Read-only. The thread ID for the background processing thread.

`backgroundThreadRunning`_ (`boolean`_)
    Read-only. Access to the running state for the background processing thread.

`cellChanged`_ (`boolean`_)
    Read-only. A flag set for the frame that the player has changed cells.

`centralGridX`_ (`boolean`_)
    Read-only. The position of the origin horizontal grid coordinate.

`centralGridY`_ (`boolean`_)
    Read-only. The position of the origin longitudinal grid coordinate.

`currentAmbientWaterSound`_ (`tes3sound`_)
    Access to the current ambient water sound.

`currentCell`_ (`tes3cell`_)
    Read-only. Access to the cell that the player is currently in.

`currentInteriorCell`_ (`tes3cell`_)
    Read-only. Access to the current interior cell, if the player is in an interior.

`exteriorCells`_ (`table`_)
    Read-only. A table of nine tes3cellExteriorData objects for any loaded exterior cells.

`lastExteriorCell`_ (`tes3cell`_)
    Read-only. Access to the last visited exterior cell.

`mainThread`_ (`number`_)
    Read-only. A Windows handle to the main execution thread.

`mainThreadId`_ (`number`_)
    Read-only. The thread ID for the main execution thread.

`nonDynamicData`_ (`tes3nonDynamicData`_)
    Read-only. A child structure where core game objects are held.

`threadSleepTime`_ (`number`_)
    Read-only. No description available.

`useCellTransitionFader`_ (`boolean`_)
    An engine flag that controls if there is a fade in/out between cells.

`worldLandscapeRoot`_ (`niNode`_)
    Read-only. One of the core parent scene graph nodes.

`worldObjectRoot`_ (`niNode`_)
    Read-only. One of the core parent scene graph nodes.

`worldPickObjectRoot`_ (`niNode`_)
    Read-only. One of the core parent scene graph nodes.

.. toctree::
    :hidden:

    tes3dataHandler/backgroundThread
    tes3dataHandler/backgroundThreadId
    tes3dataHandler/backgroundThreadRunning
    tes3dataHandler/cellChanged
    tes3dataHandler/centralGridX
    tes3dataHandler/centralGridY
    tes3dataHandler/currentAmbientWaterSound
    tes3dataHandler/currentCell
    tes3dataHandler/currentInteriorCell
    tes3dataHandler/exteriorCells
    tes3dataHandler/lastExteriorCell
    tes3dataHandler/mainThread
    tes3dataHandler/mainThreadId
    tes3dataHandler/nonDynamicData
    tes3dataHandler/threadSleepTime
    tes3dataHandler/useCellTransitionFader
    tes3dataHandler/worldLandscapeRoot
    tes3dataHandler/worldObjectRoot
    tes3dataHandler/worldPickObjectRoot

.. _`backgroundThread`: tes3dataHandler/backgroundThread.html
.. _`backgroundThreadId`: tes3dataHandler/backgroundThreadId.html
.. _`backgroundThreadRunning`: tes3dataHandler/backgroundThreadRunning.html
.. _`cellChanged`: tes3dataHandler/cellChanged.html
.. _`centralGridX`: tes3dataHandler/centralGridX.html
.. _`centralGridY`: tes3dataHandler/centralGridY.html
.. _`currentAmbientWaterSound`: tes3dataHandler/currentAmbientWaterSound.html
.. _`currentCell`: tes3dataHandler/currentCell.html
.. _`currentInteriorCell`: tes3dataHandler/currentInteriorCell.html
.. _`exteriorCells`: tes3dataHandler/exteriorCells.html
.. _`lastExteriorCell`: tes3dataHandler/lastExteriorCell.html
.. _`mainThread`: tes3dataHandler/mainThread.html
.. _`mainThreadId`: tes3dataHandler/mainThreadId.html
.. _`nonDynamicData`: tes3dataHandler/nonDynamicData.html
.. _`threadSleepTime`: tes3dataHandler/threadSleepTime.html
.. _`useCellTransitionFader`: tes3dataHandler/useCellTransitionFader.html
.. _`worldLandscapeRoot`: tes3dataHandler/worldLandscapeRoot.html
.. _`worldObjectRoot`: tes3dataHandler/worldObjectRoot.html
.. _`worldPickObjectRoot`: tes3dataHandler/worldPickObjectRoot.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3nonDynamicData`: ../../lua/type/tes3nonDynamicData.html
.. _`tes3sound`: ../../lua/type/tes3sound.html
