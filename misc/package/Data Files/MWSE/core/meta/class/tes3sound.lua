--- @meta

--- A sound object created in the TES3 CS.
--- @class tes3sound : tes3baseObject
--- @field filename string Path to the sound file.
--- @field id string The unique identifier for the object.
--- @field maxDistance number The maximum distance the sound will be played at. Defaults to 0. If both this and minDistance are 0, the values are ignored.
--- @field minDistance number The minimum distance the sound will be played at. Defaults to 0.
--- @field volume number Direct access to sound's volume.
tes3sound = {}

--- Returns true if the sound is actively looping.
--- @return boolean result No description yet available.
function tes3sound:isLooping() end

--- Returns true if the sound is actively playing.
--- @return boolean result No description yet available.
function tes3sound:isPlaying() end

--- This metod plays the sound.
--- @param playbackFlags number These match low-level DirectSound flags. Most mods don't need to modify this, though a value of 1 to loop may be useful. The rest of the flags can be read in the IDirectSoundBuffer8::Play documentation: https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee418074(v=vs.85).
--- @param volume number No description yet available.
--- @param pitch number No description yet available.
--- @param isNot3D boolean No description yet available.
function tes3sound:play(playbackFlags, volume, pitch, isNot3D) end

--- This stops playing the sound.
function tes3sound:stop() end

--- Serializes the object to json.
--- @return string result No description yet available.
function tes3sound:__tojson() end

