mwse.loadConfig
====================================================================================================

Loads a config table from Data Files\MWSE\config\{fileName}.json.
	
If the default values table is passed, empty keys in the config will be filled in using its values. Additionally, if no file exists, the function will return the default table.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

fileName (`string`_)
    The non-extensioned name of the config file.

defaults (`table`_)
    Optional. A table of default values.

.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
