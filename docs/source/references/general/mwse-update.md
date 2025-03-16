# MWSE-Update

MWSE comes with an automatic updater. Running `MWSE-Update.exe` will check if a newer build of MWSE is available than the one currently installed. It will automatically download and unpack the update. During the process, a console window will log what the updater is currently doing. If the newest build is installed the updater console window will automatically close.

## Flags

### `-startAfter`

This flag will make the updater run Morrowind automatically after it's done.

### `-overwriteResources`

This flag will make the updater overwrite resource files during the update. Resources files are files MWSE ships with that are outside of the `Data Files\\MWSE` folder. Those are `Data Files\\meshes` and `Data Files\\textures`. Typically, if you have a texture pack that changes one of MWSE's textures don't add this flag so the updater won't overwrite your texture pack.
