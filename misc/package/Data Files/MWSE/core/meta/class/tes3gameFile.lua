--- @meta

--- Represents a loaded ESM, ESP, or ESS file.
--- @class tes3gameFile
--- @field author string The file's author.
--- @field cellName string The current cell, from a save game.
--- @field currentHealth number The player's health, from a save game.
--- @field day number The current day, from a save game.
--- @field daysPassed number The number of days passed, from a save game.
--- @field description string The description of the file.
--- @field filename string The simple filename.
--- @field fileSize number The size of the file.
--- @field gameHour number The current game hour, from a save game.
--- @field masters table An array-style table of the tes3gameFiles that this is dependent upon.
--- @field maxHealth number The player's maximum health, from a save game.
--- @field modifiedTime string The timestamp that the file was modified.
--- @field month number The current month, from a save game.
--- @field path string The path to the file.
--- @field playerName string The player's name, from a save game.
--- @field year number The current year, from a save game.
tes3gameFile = {}

--- Deletes the file.
function tes3gameFile:deleteFile() end

