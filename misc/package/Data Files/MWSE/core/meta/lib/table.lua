--- @meta

--- This library provides generic functions for table manipulation. It provides all its functions inside the table table.
--- @class tablelib
table = {}

--- Inserts a given value through BinaryInsert into the table sorted by [, comp].
--- 
--- If 'comp' is given, then it must be a function that receives two table elements, and returns true when the first is less than the second, e.g. comp = function(a, b) return a > b end, will give a sorted table, with the biggest value on position 1. [, comp] behaves as in table.sort(table, value [, comp]) returns the index where 'value' was inserted
--- @param t table No description yet available.
--- @param value unknown No description yet available.
--- @param comp unknown No description yet available.
--- @return number result No description yet available.
function table.bininsert(t, value, comp) end

--- Performs a binary search for a given value.
--- 
--- If the value is found: It returns a table holding all the mathing indices (e.g. { startindice,endindice } ) endindice may be the same as startindice if only one matching indice was found
--- 
--- If compval is given: then it must be a function that takes one value and returns a second value2, to be compared with the input value, e.g.: compvalue = function( value ) return value[1] end
--- 
--- If reversed is set to true: then the search assumes that the table is sorted in reverse order (largest value at position 1) note when reversed is given compval must be given as well, it can be nil/_ in this case
--- 
--- Return value: on success: a table holding matching indices (e.g. { startindice,endindice } ) on failure: nil
--- @param t table No description yet available.
--- @param value unknown No description yet available.
--- @param compval unknown No description yet available.
--- @param reversed unknown No description yet available.
--- @return table result No description yet available.
function table.binsearch(t, value, compval, reversed) end

--- Returns a random element from the given table.
--- @param t table No description yet available.
--- @return unknown result No description yet available.
function table.choice(t) end

--- This clears all keys and values from a table, but preserves the allocated array/hash sizes. This is useful when a table, which is linked from multiple places, needs to be cleared and/or when recycling a table for use by the same context. This avoids managing backlinks, saves an allocation and the overhead of incremental array/hash part growth.
--- 
--- Please note this function is meant for very specific situations. In most cases it's better to replace the (usually single) link with a new table and let the garbage collection do its work.
--- @param table table The table to clear.
function table.clear(table) end

--- Shallowly copies a table's contents to a destination table. If no destination table is provided, a new table will be created. Note that sub tables will not be copied, and will still refer to the same data.
--- @param from table No description yet available.
--- @param to table No description yet available.
--- @return table result No description yet available.
function table.copy(from, to) end

--- Copies a table's contents from one table to another, including subtitles. If a non-table key is already defined, it will not be overwritten. Metatables are not copied.
--- @param to table No description yet available.
--- @param from table No description yet available.
function table.copymissing(to, from) end

--- Copies a table's contents. All subtables will also be copied, as will any metatable.
--- @param t table No description yet available.
--- @return table result No description yet available.
function table.deepcopy(t) end

--- Returns true if the table is empty, otherwise, it returns false.
--- @param t table No description yet available.
--- @return boolean result No description yet available.
function table.empty(t) end

--- Returns the key for a given value, or nil if the table does not contain the value.
--- @param t table No description yet available.
--- @param value unknown No description yet available.
--- @return unknown result No description yet available.
function table.find(t, value) end

--- Returns an array-style table of all keys in the given table, t. Optionally, it will sort the returned table.
--- @param t table The table to get keys for.
--- @param sort boolean|function|nil If true, the returned table will be sorted. If a function is passed, the table will be sorted using the given function.
--- @return any result No description yet available.
function table.keys(t, sort) end

--- This creates a pre-sized table. This is useful for big tables if the final table size is known and automatic table resizing is too expensive.
--- @param narray number A hint for how many elements the table will have as a sequence.
--- @param nhash number A hint for how many other elements the table will have.
--- @return any result No description yet available.
function table.new(narray, nhash) end

--- Removes a value from a given table. Returns true if the value was successfully removed.
--- @param t table No description yet available.
--- @param value unknown No description yet available.
--- @return boolean result No description yet available.
function table.removevalue(t, value) end

--- Returns the number of elements inside the table. Unlike the length operator (#) this will work with any table.
--- @param t table No description yet available.
--- @return number result No description yet available.
function table.size(t) end

--- This function is used to iterate over values and its subtables in the table.
--- 
--- Each "node" is an object with a children table of other "nodes", each of which might have their own children. For example, a sceneNode is made up of niNodes, and each niNodes can have a list of niNode children. This is best used for recursive data structures like UI elements and sceneNodes etc.
---
--- [Examples available in online documentation](https://mwse.readthedocs.io/en/latest/lua/api/table/traverse.html).
--- @param t table A table to transverse.
--- @param k unknown The key of a table inside t object.
--- @return iterator result No description yet available.
function table.traverse(t, k) end

--- Returns an array-style table of all values in the given table, t. Optionally, it will sort the returned table.
--- @param t table The table to get values for.
--- @param sort boolean|function|nil If true, the returned table will be sorted. If a function is passed, the table will be sorted using the given function.
--- @return any result No description yet available.
function table.values(t, sort) end

