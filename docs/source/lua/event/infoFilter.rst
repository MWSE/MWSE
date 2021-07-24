infoFilter
====================================================================================================

This event fires when a dialogue info object is filtered.

Related events: `infoGetText`_, `infoLinkResolve`_, `infoResponse`_, `postInfoResponse`_

Event Data
----------------------------------------------------------------------------------------------------

actor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3object`_. Read-only. The actor.

info
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3dialogueInfo`_. Read-only. The dialogue info object.

dialogue
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3dialogue`_. Read-only. The dialogue object.

passes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`bool`_. Read-only. Indicates if the filter passes.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The reference.

source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The source.

.. _`infoGetText`: ../../lua/event/infoGetText.html
.. _`infoLinkResolve`: ../../lua/event/infoLinkResolve.html
.. _`infoResponse`: ../../lua/event/infoResponse.html
.. _`postInfoResponse`: ../../lua/event/postInfoResponse.html
.. _`bool`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3dialogue`: ../../lua/type/tes3dialogue.html
.. _`tes3dialogueInfo`: ../../lua/type/tes3dialogueInfo.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
