infoLinkResolve
====================================================================================================

This event fires when a dialogue hyperlink is being resolved to a topic, during UI layout (not on link activation).

Related events: `infoGetText`_, `infoResponse`_, `infoFilter`_, `postInfoResponse`_

Event Data
----------------------------------------------------------------------------------------------------

topic
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`string`_. The topic that activates when this hyperlink is clicked. Initially set to the link text. Set this value to change the target topic.

.. _`infoFilter`: ../../lua/event/infoFilter.html
.. _`infoGetText`: ../../lua/event/infoGetText.html
.. _`infoResponse`: ../../lua/event/infoResponse.html
.. _`postInfoResponse`: ../../lua/event/postInfoResponse.html
.. _`string`: ../../lua/type/string.html
