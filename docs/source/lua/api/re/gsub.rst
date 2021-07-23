re.gsub
====================================================================================================

Does a global substitution, replacing all occurrences of pattern in the given subject by replacement.

Returns
----------------------------------------------------------------------------------------------------

`string`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

subject (`string`_)
    The string to search.

pattern (`string`_)
    The pattern to search with.

replacement (`string`_)
    Replace all matches with this string.

Examples
----------------------------------------------------------------------------------------------------

Simple Substitution
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Use re.gsub to replace all vowels in a string with '.'.

.. code-block:: lua

    local re = require("re")

    local function onLoaded(e)

        -- returns a string with all vowels replaced by .
        mwse.log(re.gsub("hello World", "[aeiou]", "."))
        --> h.ll. W.rld

    end

    event.register("loaded", onLoaded)


.. _`string`: ../../../lua/type/string.html
