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


## Behavior Refinement

### Exposing Functions from Morrowind.exe

When implementing a function from IDA, do not use freefloating calls from other classes. Instead, implement wrapper functions on the associated class.

Bad result:

```cpp
using UpdateWaterReflectionFn = void(__thiscall*)(WaterController*);
const auto updateWaterReflection = reinterpret_cast<UpdateWaterReflectionFn>(0x51E150);
updateWaterReflection(waterController);
```

Good result:

```cpp
const auto TES3_WaterController_clearWaterReflectionFlag = reinterpret_cast<void(__thiscall*)(WaterController*)>(0x51E150);
void WaterController::clearWaterReflectionFlag() {
    TES3_WaterController_clearWaterReflectionFlag(this);
}
```

```cpp
waterController->updateWaterReflection();
```

### Expose Helper Functions as Member Functions

When adding helper functions, place them as member functions instead of namespaced static functions.

Bad result:

```cpp
namespace {
    void touchThunderReflection(const WeatherThunder* weather) {
        const auto dataHandler = controller ? controller->dataHandler : nullptr;
        const auto waterController = dataHandler ? dataHandler->waterController : nullptr;
        if (!waterController) {
            return;
        }

        dataHandler->waterController->clearWaterReflectionFlag();
    }
}
```

Good result:

```cpp
void Weather::clearWaterReflectionFlag() {
    const auto dataHandler = controller ? controller->dataHandler : nullptr;
    const auto waterController = dataHandler ? dataHandler->waterController : nullptr;
    if (!waterController) {
        return;
    }

    waterController->clearWaterReflectionFlag();
}
```
