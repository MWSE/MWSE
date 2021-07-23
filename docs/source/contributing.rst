
Contributing
========================================================

The Morrowind Script Extender is open source software that welcomes contributions in the forms of ideas, bug fixes, and new features.


Visual Studio
--------------------------------------------------------

Contributing to MWSE requires a copy of `Visual Studio 2019`_, with the following features enabled:

- Workloads:

  - Desktop development with C++

- Individual components:

  - VC++ 2019 v142 toolset (x86,x64)
  
  - Windows 10 SDK for Desktop C++ [x86 and x64] (any version will do)


LuaJIT
--------------------------------------------------------

`LuaJIT`_ needs to be compiled once, and is included in the project. To compile, use **Tools > Visual Studio Command Prompt** and run the following:

.. code-block:: bat

    cd deps\LuaJIT\src
    msvcbuild.bat


.. _`LuaJIT`: https://luajit.org/
.. _`Visual Studio 2019`: https://www.visualstudio.com/downloads/
