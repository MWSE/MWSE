tes3gameFile
====================================================================================================

Represents a loaded ESM, ESP, or ESS file.

Properties
----------------------------------------------------------------------------------------------------

`author`_ (`string`_)
    Read-only. The file's author.

`cellName`_ (`string`_)
    Read-only. The current cell, from a save game.

`currentHealth`_ (`number`_)
    Read-only. The player's health, from a save game.

`day`_ (`number`_)
    Read-only. The current day, from a save game.

`daysPassed`_ (`number`_)
    Read-only. The number of days passed, from a save game.

`description`_ (`string`_)
    Read-only. The description of the file.

`fileSize`_ (`number`_)
    Read-only. The size of the file.

`filename`_ (`string`_)
    Read-only. The simple filename.

`gameHour`_ (`number`_)
    Read-only. The current game hour, from a save game.

`masters`_ (`table`_)
    Read-only. An array-style table of the tes3gameFiles that this is dependent upon.

`maxHealth`_ (`number`_)
    Read-only. The player's maximum health, from a save game.

`modifiedTime`_ (`string`_)
    Read-only. The timestamp that the file was modified.

`month`_ (`number`_)
    Read-only. The current month, from a save game.

`path`_ (`string`_)
    Read-only. The path to the file.

`playerName`_ (`string`_)
    Read-only. The player's name, from a save game.

`year`_ (`number`_)
    Read-only. The current year, from a save game.

.. toctree::
    :hidden:

    tes3gameFile/author
    tes3gameFile/cellName
    tes3gameFile/currentHealth
    tes3gameFile/day
    tes3gameFile/daysPassed
    tes3gameFile/description
    tes3gameFile/fileSize
    tes3gameFile/filename
    tes3gameFile/gameHour
    tes3gameFile/masters
    tes3gameFile/maxHealth
    tes3gameFile/modifiedTime
    tes3gameFile/month
    tes3gameFile/path
    tes3gameFile/playerName
    tes3gameFile/year

.. _`author`: tes3gameFile/author.html
.. _`cellName`: tes3gameFile/cellName.html
.. _`currentHealth`: tes3gameFile/currentHealth.html
.. _`day`: tes3gameFile/day.html
.. _`daysPassed`: tes3gameFile/daysPassed.html
.. _`description`: tes3gameFile/description.html
.. _`fileSize`: tes3gameFile/fileSize.html
.. _`filename`: tes3gameFile/filename.html
.. _`gameHour`: tes3gameFile/gameHour.html
.. _`masters`: tes3gameFile/masters.html
.. _`maxHealth`: tes3gameFile/maxHealth.html
.. _`modifiedTime`: tes3gameFile/modifiedTime.html
.. _`month`: tes3gameFile/month.html
.. _`path`: tes3gameFile/path.html
.. _`playerName`: tes3gameFile/playerName.html
.. _`year`: tes3gameFile/year.html

Methods
----------------------------------------------------------------------------------------------------

`deleteFile`_
    Deletes the file.

.. toctree::
    :hidden:

    tes3gameFile/deleteFile

.. _`deleteFile`: tes3gameFile/deleteFile.html

.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
