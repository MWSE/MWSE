string
====================================================================================================

This library provides generic functions for string manipulation, such as finding and extracting substrings, and pattern matching. When indexing a string in Lua, the first character is at position 1 (not at 0, as in C). Indices are allowed to be negative and are interpreted as indexing backwards, from the end of the string. Thus, the last character is at position -1, and so on.

Functions
----------------------------------------------------------------------------------------------------

.. toctree::
    :maxdepth: 1

    string/endswith
    string/format
    string/insert
    string/multifind
    string/startswith

