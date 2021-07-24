infoResponse
====================================================================================================

This event fires when a dialogue response is triggered.

Related events: `infoGetText`_, `infoLinkResolve`_, `infoFilter`_, `postInfoResponse`_

Event Data
----------------------------------------------------------------------------------------------------

info
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3dialogueInfo`_. Read-only. The dialogue info object.

command
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`string`_. Read-only. The command.

dialogue
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3dialogue`_. Read-only. The dialogue object.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The reference.

variables
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tes3scriptVariables. Read-only. The script variables.

.. _`infoFilter`: ../../lua/event/infoFilter.html
.. _`infoGetText`: ../../lua/event/infoGetText.html
.. _`infoLinkResolve`: ../../lua/event/infoLinkResolve.html
.. _`postInfoResponse`: ../../lua/event/postInfoResponse.html
.. _`string`: ../../lua/type/string.html
.. _`tes3dialogue`: ../../lua/type/tes3dialogue.html
.. _`tes3dialogueInfo`: ../../lua/type/tes3dialogueInfo.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
