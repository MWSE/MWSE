detectSneak
====================================================================================================

This event is invoked whenever an actor is checking if they can detect another actor sneaking.

Event Data
----------------------------------------------------------------------------------------------------

detector
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor that is doing the detecting.

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor that is possibly being detected.

isDetected
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. The previous detection calculated, starting with the base engine result.

.. _`boolean`: ../../lua/type/boolean.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
