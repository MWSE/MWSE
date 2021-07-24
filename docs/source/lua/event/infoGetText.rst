infoGetText
====================================================================================================

This event fires when a dialogue info object text is retrieved.

Related events: `infoLinkResolve`_, `infoResponse`_, `infoFilter`_, `postInfoResponse`_

Event Data
----------------------------------------------------------------------------------------------------

info
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3dialogueInfo`_. Read-only. The dialogue info object.

loadOriginalText
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

method. Loads the original text for the info from the game files, bypassing this event.

.. _`infoFilter`: ../../lua/event/infoFilter.html
.. _`infoLinkResolve`: ../../lua/event/infoLinkResolve.html
.. _`infoResponse`: ../../lua/event/infoResponse.html
.. _`postInfoResponse`: ../../lua/event/postInfoResponse.html
.. _`tes3dialogueInfo`: ../../lua/type/tes3dialogueInfo.html
