--- @meta
--- @diagnostic disable:undefined-doc-name

--- This event is raised when a book's text is about to be displayed. By providing your own text, you can overwrite what is going to be displayed.
---
--- [Examples available in online documentation](https://mwse.github.io/MWSE/events/bookGetText).
--- @class bookGetTextEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field book tes3book *Read-only*. The book whose text is being retrieved.
--- @field text string If set, the book's text will be overwritten with this value. It needs to follow book text conventions as in the Construction Set. In essence, it uses HTML syntax. Important: every book needs to end with a `<BR>` statement to be displayed properly.