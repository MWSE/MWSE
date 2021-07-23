bookGetText
====================================================================================================

This event is raised when a book's text is about to be displayed. By providing your own text, you can overwrite what is going to be displayed.

Event Data
----------------------------------------------------------------------------------------------------

text
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`string`_. If set, the book's text will be overwritten with this value.

book
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3book`_. Read-only. The book whose text is being retrieved.

.. _`string`: ../../lua/type/string.html
.. _`tes3book`: ../../lua/type/tes3book.html
