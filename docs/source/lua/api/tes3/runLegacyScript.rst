tes3.runLegacyScript
====================================================================================================

This function will compile and run a mwscript chunk of code. This is not ideal to use, but can be used for features not yet exposed to lua.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

script (`tes3script`_)
    Default: ``tes3.worldController.scriptGlobals``. The base script to base the execution from.

source (`number`_)
    The compilation source to use. Defaults to tes3.scriptSource.default

command (`string`_)
    The script text to compile and run.

variables (`tes3scriptVariables`_)
    Optional. If a reference is provided, the reference's variables will be used.

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    The reference to target for execution.

dialogue (`tes3dialogue`_, `string`_)
    Optional. If compiling for dialogue context, the dialogue associated with the script.

info (`tes3dialogueInfo`_)
    Optional. The info associated with the dialogue.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3dialogue`: ../../../lua/type/tes3dialogue.html
.. _`tes3dialogueInfo`: ../../../lua/type/tes3dialogueInfo.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
