--- @meta

--- Provides support for interacting with json data through an extended dkjson module.
--- @class jsonlib
--- @field null number You can use this value for setting explicit null values.
--- @field version string Current version of dkjson.
json = {}

--- Decode string into a table.
--- @param s string No description yet available.
--- @param position number No description yet available.
--- @param nullValue unknown No description yet available.
--- @return table result No description yet available.
function json.decode(s, position, nullValue) end

--- Create a string representing the object. Object can be a table, a string, a number, a boolean, nil, json.null or any object with a function __tojson in its metatable. A table can only use strings and numbers as keys and its values have to be valid objects as well. It raises an error for any invalid data types or reference cycles.
--- @param object unknown No description yet available.
--- @param state table No description yet available.
--- @return string result No description yet available.
function json.encode(object, state) end

--- Loads the contents of a file through json.decode. Files loaded from Data Files\MWSE\{fileName}.json.
--- @param fileName string No description yet available.
--- @return table result No description yet available.
function json.loadfile(fileName) end

--- Quote a UTF-8 string and escape critical characters using JSON escape sequences. This function is only necessary when you build your own __tojson functions.
--- @param s string No description yet available.
--- @return string result No description yet available.
function json.quotestring(s) end

--- Saves a serializable table to Data Files\MWSE\{fileName}.json, using json.encode.
--- @param fileName string No description yet available.
--- @param object unknown No description yet available.
--- @param config table No description yet available.
--- @return table result No description yet available.
function json.savefile(fileName, object, config) end

