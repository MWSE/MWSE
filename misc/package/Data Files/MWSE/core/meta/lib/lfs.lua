--- @meta

--- LuaFileSystem is a Lua library developed to complement the set of functions related to file systems offered by the standard Lua distribution. This library has been further extended by MWSE.
--- @class lfslib
lfs = {}

--- Returns a table with the file attributes corresponding to filepath (or nil followed by an error message and a system-dependent error code in case of error). If the second optional argument is given and is a string, then only the value of the named attribute is returned (this use is equivalent to lfs.attributes(filepath)[request_name], but the table is not created and only one attribute is retrieved from the O.S.). if a table is passed as the second argument, it (result_table) is filled with attributes and returned instead of a new table. The attributes are described as follows; attribute mode is a string, all the others are numbers, and the time related attributes use the same time reference of os.time: dev, mode, nlink, access, modification, change, size, permissions.
--- @param path string The path, relative to the current directory, to get attributes for.
--- @param request string The specific attribute to look for. If provided, the return value becomes the requested value (or nil) instead of a table.
--- @param resultTable table If provided, this table will be reused as the result table. If not provided, a new table is created.
--- @return table|string|number|nil attributes All attributes in a table, the value of a specific attribute if one was requested, or nil on an error.
--- @return string|nil error In the case of an error, a string describing the issue.
function lfs.attributes(path, request, resultTable) end

--- Changes the current working directory to the given path. Note that this is global. Do not use this function without restoring the previous directory at the end of your logic. Failure to do so can cause unrecoverable errors.
--- @param path string The path to change to.
--- @return boolean|nil success true in case of success, or nil on an error.
--- @return string|nil error In the case of an error, a string describing the issue.
function lfs.chdir(path) end

--- Changes the current working directory to the given path.
--- @return string|nil path The current working directory, or nil on an error.
--- @return string|nil error In the case of an error, a string describing the issue.
function lfs.currentdir() end

--- Creates a Lua iterator over the entries of a given directory. This can be used to create a for loop of directories. Be wary that current and parent directory markers may be available in the results.
--- @param path string The path to iterate over.
function lfs.dir(path) end

--- Simple check to determine if a directory exists.
--- @param path string The path to determine existance of.
--- @return any result No description yet available.
function lfs.directoryexists(path) end

--- Simple check to determine if a file exists.
--- @param path string The path to determine existance of.
--- @return any result No description yet available.
function lfs.fileexists(path) end

--- Creates a link. This is a hard link by default.
--- @param old string The object to link to.
--- @param new string The name of the link.
--- @param symlink boolean If true, the link will be symbolic instead.
--- @return boolean|nil success true in case of success, or nil on an error.
--- @return string|nil error In the case of an error, a string describing the issue.
function lfs.link(old, new, symlink) end

--- Locks a file or a part of it. This function works on open files; the file handle should be specified as the first argument.
--- @param filehandle ioFile The file handle to lock.
--- @param mode string "r" for a read/shared lock, or "w" for a write/exclusive lock.
--- @param start number The starting point of the file to lock.
--- @param length number The length of the file, relative to start, to lock.
--- @return boolean|nil success true if the operation was successful, or nil in the case of an error.
--- @return string|nil error In the case of an error, a string describing the issue. In particular, if the lock exists and is not stale it returns the "File exists" message.
function lfs.lock(filehandle, mode, start, length) end

--- Creates a lockfile (called lockfile.lfs) in path if it does not exist and returns the lock. If the lock already exists checks if it's stale, using the second parameter (default for the second parameter is INT_MAX, which in practice means the lock will never be stale. The lock object has a method to free it.
--- @param path string The path to lock.
--- @return lfsLock|nil success The lock object to manage, or nil in the case of an error.
--- @return string|nil error In the case of an error, a string describing the issue. In particular, if the lock exists and is not stale it returns the "File exists" message.
function lfs.lock_dir(path) end

--- Creates a new directory.
--- @param dirname string The name of the new directory.
--- @return boolean|nil success true in case of success, or nil on an error.
--- @return string|nil error In the case of an error, a string describing the issue.
function lfs.mkdir(dirname) end

--- Removes an existing directory. Unless using the recursive parameter, only empty directories can be removed.
--- @param dirname string The name of the directory to remove.
--- @param recursive boolean If true, all contents of the directory will be removed as well.
--- @return boolean|nil success true in case of success, or nil on an error.
--- @return string|nil error In the case of an error, a string describing the issue.
function lfs.rmdir(dirname, recursive) end

--- Sets the writing mode for a file.
--- @param file ioFile The file to set mode for.
--- @param mode string The file's new mode. Can be either "binary" or "text".
--- @return boolean|nil success true in case of success, or nil on an error.
--- @return string errorOrOldMode If succesful, this will be the previous file mode used. In the case of an error, a string describing the issue.
function lfs.setmode(file, mode) end

--- Set access and modification times of a file. Both access and modified times are measured in seconds, matching the OS time function.
--- @param path string The path to touch.
--- @param accessTime number The new access time to set.
--- @param modifiedTime number The new last modified time to set
--- @return boolean|nil success true in case of success, or nil on an error.
--- @return string|nil error In the case of an error, a string describing the issue.
function lfs.touch(path, accessTime, modifiedTime) end

--- Unlocks a file or a part of it. This function works on open files; the file handle should be specified as the first argument.
--- @param filehandle ioFile The file handle to unlock.
--- @param mode string "r" for a read/shared lock, or "w" for a write/exclusive unlock.
--- @param start number The starting point of the file to unlock.
--- @param length number The length of the file, relative to start, to unlock.
--- @return boolean|nil success true if the operation was successful, or nil in the case of an error.
--- @return string|nil error In the case of an error, a string describing the issue. In particular, if the lock exists and is not stale it returns the "File exists" message.
function lfs.unlock(filehandle, mode, start, length) end

