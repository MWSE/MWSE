---
name: mwse-lua-event
description: "Add new events to the MWSE Lua API. Covers the full end-to-end process: C++ event class (header + source), memory hook registration in LuaManager, autocomplete definition file, example scripts, and documentation generation. Use when the user wants to hook a new Morrowind engine call and expose it to Lua mod authors."
---

# MWSE Lua Event Development

This skill covers the complete workflow for adding a new event to MWSE's Lua API.

---

## Overview

Every MWSE Lua event follows the same pattern:

1. **C++ header** – declare the event class in `MWSE/Lua<Name>Event.h`
2. **C++ source** – implement constructor and `createEventTable()` in `MWSE/Lua<Name>Event.cpp`
3. **Hook callback** – add a static callback function and memory patch in `MWSE/LuaManager.cpp`
4. **Registration** – `#include` the header and call `genCallEnforced` inside `LuaManager::hook()`
5. **Disableable event registration** – register the event in `MWSE/LuaDisableableEventManager.cpp`
6. **Event string constant** – add the string constant to `misc/package/Data Files/MWSE/core/lib/tes3/event.lua`
7. **Autocomplete definition** – create `autocomplete/definitions/events/standard/<eventName>.lua`
8. **Examples** – add Lua example scripts in `autocomplete/definitions/events/standard/<eventName>/`
9. **Rebuild docs** – run the autocomplete builder; docs are auto-generated

---

## Step 1 – C++ Header (`MWSE/Lua<Name>Event.h`)

Most events inherit from **`ObjectFilteredEvent`** (which gives per-object filtering) and the **`DisableableEvent<T>`** mixin (which adds an enabled/disabled flag so the hook only runs when there is at least one Lua listener).

If no per-object filter is needed, inherit from `GenericEvent` + `DisableableEvent<T>` instead.

### Header template

```cpp
#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
    class MyEvent : public ObjectFilteredEvent, public DisableableEvent<MyEvent> {
    public:
        MyEvent(TES3::Reference* reference, /* other args */);
        sol::table createEventTable();

    protected:
        TES3::Reference* m_Reference;
        // Store any other data needed in createEventTable()
    };
}
```

**Class hierarchy reference:**

| Class | Purpose |
|---|---|
| `BaseEvent` | Pure virtual root – `getEventName()`, `createEventTable()`, `getEventOptions()` |
| `GenericEvent` | Stores event name string (`m_EventName`) |
| `ObjectFilteredEvent` | Adds `m_EventFilter` (`TES3::BaseObject*`) and implements `getEventOptions()` |
| `DisableableEvent<T>` | Template mixin – static `m_EventEnabled` bool; gates the event so it only fires when a listener is registered |

The `DisableableEvent` mixin adds:
- `static bool getEventEnabled()` – the C++ hook checks this before firing
- `static void setEventEnabled(bool)` – called by the Lua event system when a listener is registered/removed

### When to use `GenericEvent` only (no filter)

If there is no meaningful object to filter on (e.g. a frame event, a weather event), inherit from `GenericEvent` directly:

```cpp
class MyEvent : public GenericEvent, public DisableableEvent<MyEvent> {
public:
    MyEvent(/* args */);
    sol::table createEventTable();
};
```

---

## Step 2 – C++ Source (`MWSE/Lua<Name>Event.cpp`)

```cpp
#include "LuaMyEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

// Include headers for every TES3 type you reference
#include "TES3Reference.h"

namespace mwse::lua::event {
    MyEvent::MyEvent(TES3::Reference* reference, /* other args */) :
        ObjectFilteredEvent("myEvent", reference),  // "myEvent" is the Lua event name string
        m_Reference(reference)
        // initialise other members
    {
    }

    sol::table MyEvent::createEventTable() {
        const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
        auto& state = stateHandle.getState();
        auto eventData = state.create_table();

        eventData["reference"] = m_Reference;
        // eventData["someValue"] = m_SomeValue;    // mutable – Lua can write to it
        // eventData["readOnlyThing"] = m_Thing;    // readOnly in autocomplete, but C++ doesn't enforce this

        return eventData;
    }
}
```

**Key rules for `createEventTable()`:**
- Always get the state via `LuaManager::getInstance().getThreadSafeStateHandle()` – this acquires the recursive mutex.
- Fields with no `readOnly = true` in the autocomplete definition are expected to be writable by Lua. Read them back after `triggerEvent` returns.
- Store all data you need as protected member variables in the constructor; don't read engine memory inside `createEventTable()` if those pointers might have become stale.

---

## Step 3 – Hook Callback Patterns (in `LuaManager.cpp`)

There are four main patterns for hook callbacks. All are static functions defined in `LuaManager.cpp`'s anonymous scope, typically near related hooks.

### Pattern A – `__fastcall` delegate (thiscall override)

Use when replacing a virtual/non-virtual thiscall method with a wrapper that calls the engine method that was already wrapped to fire the event internally.

```cpp
void __fastcall OnMyEvent(TES3::SomeClass* obj, DWORD _UNUSED_, /* other args */) {
    // Call the wrapped engine method which internally fires the event.
    obj->myMethod(/* args */);
}
```

The `DWORD _UNUSED_` is required by `__fastcall`: MSVC passes the first argument in `ecx` (= `this`) and the second in `edx` (unused for thiscall). The dummy parameter absorbs `edx`.

### Pattern B – Fire event after calling original

Use when the event observes what the engine *did*, rather than gating it.

```cpp
void __fastcall OnMyEvent(TES3::SomeClass* obj, DWORD _UNUSED_) {
    // Call original function first.
    const auto TES3_OriginalFunction =
        reinterpret_cast<void(__thiscall*)(TES3::SomeClass*)>(0x12345678);
    TES3_OriginalFunction(obj);

    // Then fire our event.
    if (event::MyEvent::getEventEnabled()) {
        LuaManager::getInstance().getThreadSafeStateHandle()
            .triggerEvent(new event::MyEvent(obj->reference));
    }
}
```

### Pattern C – Fire event before calling original (blockable)

Use when Lua should be able to prevent the engine action from happening.

```cpp
signed char __cdecl OnMyEvent(/* engine args */) {
    if (event::MyEvent::getEventEnabled()) {
        const auto stateHandle =
            mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
        sol::object response = stateHandle.triggerEvent(new event::MyEvent(/* args */));
        if (response.get_type() == sol::type::table) {
            sol::table eventData = response;
            if (eventData.get_or("block", false)) {
                return 0;  // blocked
            }
        }
    }

    // Call original function.
    return reinterpret_cast<signed char(__cdecl*)(/* args */)>(0xDEADBEEF)(/* args */);
}
```

### Pattern D – `__declspec(naked)` + `__stdcall` handler

Use when the hook site is in the middle of a function and registers need to be saved/restored, or when arguments are in unusual registers.

```cpp
__declspec(naked) void patchMyHook() {
    __asm {
        push eax    // save/push relevant registers as args
        push esi
        push ebp
    }
}
const size_t patchMyHook_size = 0x3;  // size of the above asm bytes

void __stdcall OnMyHookHandler(
    TES3::SomeClass* target, TES3::MobileActor* attacker, float value)
{
    if (event::MyEvent::getEventEnabled()) {
        auto& luaManager = mwse::lua::LuaManager::getInstance();
        const auto stateHandle = luaManager.getThreadSafeStateHandle();
        sol::table eventData =
            stateHandle.triggerEvent(new event::MyEvent(target, attacker, value));
        if (eventData.valid()) {
            value = eventData["value"];
        }
    }

    // Call original function.
    const auto TES3_OriginalFn =
        reinterpret_cast<void(__thiscall*)(TES3::SomeClass*, float)>(0x12345678);
    TES3_OriginalFn(target, value);
}
```

Then in `hook()`, install it as:
```cpp
writePatchCodeUnprotected(ADDRESS, (BYTE*)&patchMyHook, patchMyHook_size);
genCallUnprotected(ADDRESS + offsetInPatch, reinterpret_cast<DWORD>(OnMyHookHandler));
```

### Reading back mutable fields (calc events)

For "calc" events where Lua may change a value:

```cpp
sol::table eventData = stateHandle.triggerEvent(new event::MyCalcEvent(value));
if (eventData.valid()) {
    value = eventData.get_or("value", value);
}
// Use the (possibly modified) value
```

---

## Step 4 – Registration in `LuaManager.cpp`

### 4a – Add the `#include`

Find the block of event header includes (~line 200 in `LuaManager.cpp`, all `#include "Lua*Event.h"` lines) and add yours **in lexicographic order** with the others:

```cpp
#include "LuaMyEvent.h"
```

### 4b – Install the hook in `LuaManager::hook()`

Inside the `hook()` function (near the end of `LuaManager.cpp`), add a `genCallEnforced` call:

```cpp
// Hook: My descriptive comment
genCallEnforced(0xCALLSITE, 0xORIGINAL_FN, reinterpret_cast<DWORD>(OnMyEvent));
```

**`genCallEnforced` signature:**
```cpp
bool genCallEnforced(DWORD address, DWORD previousTo, DWORD to);
```
- `address` – the address of the `E8` CALL instruction to replace
- `previousTo` – the address the original CALL pointed to (safety check; the patch is skipped if it doesn't match)
- `to` – the address of your new callback

Multiple call sites pointing to the same original function each need their own `genCallEnforced` line. The same hook callback function can be reused.

**Other patching utilities** (from `SharedSE/MemoryUtil.h`):

| Function | Use |
|---|---|
| `genCallEnforced(addr, prev, to)` | Replace a CALL instruction |
| `genJumpEnforced(addr, prev, to)` | Replace a JMP instruction |
| `genNOPUnprotected(addr, size)` | NOP out bytes |
| `overrideVirtualTableEnforced(vtable, offset, prev, to)` | Patch a vtable slot |
| `writePatchCodeUnprotected(addr, bytes, size)` | Write raw bytes (for naked patches) |
| `genCallUnprotected(addr, to)` | Write a CALL without the safety check |

### Finding hook addresses

Hook addresses (`0xCALLSITE`, `0xORIGINAL_FN`) come from reverse-engineering the Morrowind engine binary. Tools:
- **IDA Pro / Ghidra** – disassemble `Morrowind.exe` to find call sites
- **x32dbg** – dynamic analysis; set breakpoints to confirm execution paths
- Look at existing `genCallEnforced` calls in `LuaManager::hook()` for nearby addresses as orientation points

---

## Step 5 – Disableable Event Registration (`MWSE/LuaDisableableEventManager.cpp`)

The event needs to be registered in `MWSE/LuaDisableableEventManager.cpp`.

---

## Step 6 – Event String Constant (`misc/package/Data Files/MWSE/core/lib/tes3/event.lua`)

The string constant of the event needs to be added to `misc/package/Data Files/MWSE/core/lib/tes3/event.lua`.

---

## Step 7 – Autocomplete Definition (`autocomplete/definitions/events/standard/<eventName>.lua`)

Create the file at `autocomplete/definitions/events/standard/myEvent.lua`. The filename (minus `.lua`) becomes the Lua event name string used in `event.register("myEvent", callback)`.

### Full format

```lua
return {
    type = "event",
    description = [[
Multi-line description of when and why this event fires.
Supports Markdown and wiki-style links.
    ]],
    related = { "otherRelatedEvent" },   -- optional: names of related events
    eventData = {
        ["reference"] = {
            type = "tes3reference",
            readOnly = true,
            description = "The reference involved in the event.",
        },
        ["value"] = {
            type = "number",
            description = "A value that Lua scripts can modify.",
            -- no readOnly = true means Lua can write to this field
        },
        ["source"] = {
            type = "string",
            description = "Where this event originated.",
            readOnly = true,
        },
    },
    filter = "reference",   -- which eventData key is the filterable object
    blockable = true,       -- set to true if Lua can return false to block the action
    examples = {
        ["BasicUsage"] = {
            title = "Listen for the event and print a message",
        },
    },
    links = {
        ["xSomeFunction"] = "mwscript/functions/actor/xSomeFunction",
    },
}
```

**Field reference:**

| Field | Required | Notes |
|---|---|---|
| `type` | yes | Always `"event"` |
| `description` | yes | Markdown description |
| `eventData` | yes | Map of field name → `{type, readOnly?, description}` |
| `filter` | no | Key in `eventData` used as the per-object filter |
| `blockable` | no | `true` if returning `false` from a callback blocks the engine action |
| `related` | no | Array of other event name strings |
| `examples` | no | Map of example key → `{title}` |
| `links` | no | Map of link name → doc path |

**Type strings** use MWSE named types: `tes3reference`, `tes3mobileActor`, `tes3mobilePlayer`, `tes3spell`, `number`, `boolean`, `string`, `nil`, etc. Browse `autocomplete/definitions/namedTypes/` for available types.

---

## Step 8 – Example Scripts

For each key in `examples`, create a Lua file at:

```
autocomplete/definitions/events/standard/<eventName>/<ExampleKey>.lua
```

The file is pure Lua – no wrapper, just code. Example:

```lua
-- autocomplete/definitions/events/standard/myEvent/BasicUsage.lua
local event = require("event")

local function onMyEvent(e)
    mwse.log("myEvent fired! Reference: %s", e.reference.id)
end

event.register(tes3.event.myEvent, onMyEvent)
```

---

## Step 9 – Rebuild Documentation

Docs are **auto-generated** — never edit `docs/source/events/*.md` by hand.

After editing the `.lua` definition files, regenerate:

```bash
cd autocomplete
lua builders/emmy.lua
lua builders/mkdocs.lua
```

Output: `docs/source/events/<eventName>.md` is created/updated automatically.

---

## Complete Checklist

When adding a new event, verify all of the following:

- [ ] `MWSE/Lua<Name>Event.h` – header with class declaration
- [ ] `MWSE/Lua<Name>Event.cpp` – constructor initializes `ObjectFilteredEvent("eventName", filterRef)` and member variables; `createEventTable()` builds the `sol::table`
- [ ] `LuaManager.cpp` – `#include "Lua<Name>Event.h"` added to the event include block
- [ ] `LuaManager.cpp` – Static hook callback function(s) added with correct calling convention
- [ ] `LuaManager.cpp` – `genCallEnforced(...)` call(s) in `LuaManager::hook()` for each call site
- [ ] `LuaDisableableEventManager.cpp` – event registered
- [ ] Hook checks `event::<Name>Event::getEventEnabled()` before calling `triggerEvent`
- [ ] `misc/package/Data Files/MWSE/core/lib/tes3/event.lua` – string constant added
- [ ] `autocomplete/definitions/events/standard/<eventName>.lua` – definition created with all fields
- [ ] Example Lua files created for each entry in `examples`
- [ ] Rebuilt docs with autocomplete builder
- [ ] MWSE project builds without errors (x86 Debug or Release)

---

## Common Mistakes

- **Missing `DWORD _UNUSED_`** in `__fastcall` callbacks: the second parameter in `__fastcall` maps to `edx`, which thiscall doesn't use. Omitting the dummy causes argument misalignment.
- **Calling `triggerEvent` without checking `getEventEnabled()`**: this fires the event every frame even if nobody is listening, causing a performance regression.
- **Forgetting the `#include`** in `LuaManager.cpp`: the linker won't know about the event class and you'll get a link error or ODR problem.
- **Editing generated docs directly**: always edit the `.lua` definition file, then rebuild.
- **Storing raw TES3 pointers past the hook return**: TES3 objects can be freed by the game at any time. Only store them in the event class long enough to populate the event table. Do not cache them in static variables between frames.
- **Not including required TES3 headers in the `.cpp`**: include every TES3 type you use (e.g. `TES3Reference.h`, `TES3MobileActor.h`) to avoid incomplete-type errors.
- **Wrong filter object**: the `filter` field in the autocomplete definition should match the `m_EventFilter` passed to `ObjectFilteredEvent(...)` in the constructor. These must be consistent so per-object filtering works correctly.
