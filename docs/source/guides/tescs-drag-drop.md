# TESCS Drag-and-Drop Protocol

This guide describes the drag-and-drop protocol that lets external tools, desktop applications, or web pages drag object definitions directly into the Construction Set Render Window to spawn placed references.

The protocol is intentionally tool-neutral: identifiers carry no vendor prefix, so any Construction Set implementation (CSSE today, others such as OpenCS in the future) can accept the same payloads.

---

## Overview

The Render Window acts as an OLE `IDropTarget`. An external tool starts a drag offering either Unicode text (`CF_UNICODETEXT`) or rich HTML (`HTML Format`).

On drop, the editor resolves the object IDs against the active record database, filters them to placeable types, stages them into the selection, and hands positioning and cell resolution off to the native engine placement handler.

---

## 1. Drag Formats

Offer either or both of the following formats during the drag.

### Format A: Unicode Text (`CF_UNICODETEXT`)

The simplest format to produce from desktop UI toolkits or web browsers. The payload is one or more lines:

```text
cs-object:<editorId>
```

- Each line is a separate object to spawn.
- Leading and trailing whitespace on each line is trimmed.
- Lines that do not start with the `cs-object:` prefix are ignored.
- The total payload must not exceed **64 KB**.
- At most **256 objects** are accepted in a single drop.

#### Example Payload (Single Object)
```text
cs-object:furn_active_de_bed_01
```

#### Example Payload (Multiple Objects)
```text
cs-object:furn_active_de_bed_01
cs-object:light_com_lantern_02_128
cs-object:active_de_bedroll
```

### Format B: HTML Format (`"HTML Format"`)

For rich web pages, drag elements that represent objects. The dropped HTML is scanned for elements carrying a `data-cs-object` attribute.

- The attribute value is the object's editor ID: `data-cs-object="<editorId>"`.
- Only content between the `StartFragment` and `EndFragment` markers of the Win32 HTML clipboard format is scanned.
- The total clipboard payload must not exceed **64 KB**.
- At most **256 objects** are extracted.

The current implementation does **not** run a full HTML parser; it scans for the
`data-cs-object` attribute directly. The following are tolerated:

- Either single quotes (`data-cs-object='<editorId>'`) or double quotes.
- Whitespace around the `=` (`data-cs-object = "<editorId>"`).

Unquoted attribute values (`data-cs-object=furn_active_de_bed_01`) are **not**
supported. Always quote the value.

#### Example HTML Element
```html
<div class="spawnable-card" data-cs-object="furn_active_de_bed_01">
  <img src="bed.png" />
  <span>Dunmer Bed</span>
</div>
```

#### Extension Attributes

The protocol is designed to grow without breaking existing tools.

- **The `data-cs-` prefix is reserved.** Future versions of this protocol may
  define additional keys under it (for example, relative position or rotation
  for spawning whole object palettes). Today only `data-cs-object` is read; any
  other `data-cs-*` attribute is ignored.
- **Tools may attach their own attributes.** Any other attribute on the element
  (your own `data-*` keys, `id`, `class`, etc.) is ignored by the editor, so
  external tools can carry their own metadata without conflicting with the
  protocol.

Because unrecognized keys are dropped silently, a payload written for a future
version of the protocol still works against an older editor.

```html
<div data-cs-object="furn_active_de_bed_01"
     data-cs-rotation="0,0,90"
     data-myeditor-palette="bedroom">
  Dunmer Bed
</div>
```

---

## 2. Placeable Object Types

Only object types that can natively be dragged from the Object Window into the Render Window are accepted. Unsupported types are silently ignored.

### Allowed Types
- Activators
- Apparatuses
- Armor
- Bodyparts
- Books
- Clothing
- Containers
- Doors
- Ingredients
- Lights
- Lockpicks
- Miscellaneous items
- Probes
- Repair tools
- Statics
- Weapons
- NPCs
- Creatures
- Leveled Creatures
- Alchemy (Potions)

### Rejected Types
- Spells (cannot be placed)
- Enchantments (cannot be placed)
- Leveled Items (the native Object Window excludes these from placement)
- Existing References (cannot be duplicated/dragged as base objects)
- Deleted objects (filtered out to avoid warning popups)

---

## 3. Web Browser Implementation Example

In a web application, target the Construction Set by setting data on the `dragstart` event:

```javascript
element.addEventListener('dragstart', (event) => {
    // Format A: Unicode text
    event.dataTransfer.setData("text/plain", "cs-object:furn_active_de_bed_01");

    // Format B: HTML
    event.dataTransfer.setData("text/html",
        `<div data-cs-object="furn_active_de_bed_01">Dunmer Bed</div>`);

    event.dataTransfer.effectAllowed = "copy";
});
```
