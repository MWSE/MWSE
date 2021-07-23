
Installation
========================================================

Requirements
--------------------------------------------------------

- The `Visual Studio x86 2017 Redistributable <https://aka.ms/vs/15/release/VC_redist.x86.exe>`_ is required for MWSE v2.0 on older versions of Windows. Note that you will need the x86 version, even if you already have the x64 redistributable installed.
- The `Morrowind Code Patch <https://www.nexusmods.com/morrowind/mods/19510/?>`_ modifies the game executable, and MWSE assumes you have at least the base patches performed for its own modifications.
- The `Morrowind Graphic Extender XE <https://www.nexusmods.com/morrowind/mods/41102>`_ (MGE XE) allows MWSE to be loaded. Versions prior to 0.9.10 will not function with MWSE v2.0, and version 0.10.1 or later is required for MWSE nightlies.

Installation
--------------------------------------------------------

- Head to `Morrowind Nexus <https://www.nexusmods.com/morrowind/mods/45468>`_ or `GitHub <https://github.com/MWSE/MWSE/releases>`_ and download the latest release.
- *After* installing MGE XE, copy the MWSE.dll file from the above download to your Morrowind directory. This will overwrite the DLL that came with MGE XE.

.. note:: Previous versions of MWSE came with a launcher, which is not needed if you use MGE XE. It is highly recommended that you load MWSE through MGE XE. If this is for some reason impossible, the old launcher can still be `obtained from a an older version <https://github.com/Merzasphor/MWSE/releases>`_.
