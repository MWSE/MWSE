--- @meta

--- An object that represents a file lock.
--- @class lfsLock
lfsLock = {}

--- Frees the lock, so that another lock can be made. Deletes the associated lock file.
function lfsLock:free() end

