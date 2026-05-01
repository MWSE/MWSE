# Agent Instructions


## Skills

- Custom project skills can live under `.agents/skills`.
    - Use the project skill at [.agents/skills/mwse-lua-event/SKILL.md](.agents/skills/mwse-lua-event/SKILL.md) for MWSE Lua event work.
- Use the IDAPython skill for IDA Pro Python scripting and reverse-engineering tasks.

## C++ Includes

In the MWSE and CSSE projects, Forced Include Files are used for standard libraries and external dependencies through `stdafx.h` / `pch.h`.

Do not add standard-library or external-dependency `#include`s directly to `.h` or `.cpp` files in these projects.


## Code Organization

- Put helper functions on their associated controllers where appropriate.


## Workflow

- Do not bother doing git diff checks.
