return {
	type = "function",
	description = [[Creates a top-level menu. It will be styled like a regular menu, with the configurable background alpha and a frame. There are two types of menu: `dragFrame` (movable and resizeable with titlebar) or `fixedFrame` (fixed with simple border). A type must be specified to create a menu.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "string|number", description = "The menu’s ID. The menu can be later accessed by tes3ui.findMenu(id)." },
			{ name = "dragFrame", type = "boolean", optional = true, default = false, description = "Constructs a draggable and resizeable frame and background for the menu. It is similar to the stats, inventory, magic and map menus in the standard UI. Its title bar text can be set with the .text property. After construction, position and minimum dimensions should be set." },
			{ name = "fixedFrame", type = "boolean", optional = true, default = false, description = "Constructs a fixed (non-draggable) frame and background for the menu. The layout system should automatically centre and size it to fit whatever is added to the menu. This type of menu is modal by default, preventing interaction with other menus while the menu is active." },
			{ name = "modal", type = "boolean", optional = true, default = true, description = "Only applies to fixedFrame menus. Modal menus prevent interaction with other menus while the menu is active. This behavior can be disabled with this flag." },
			{ name = "loadable", type = "boolean", optional = true, default = true, description = "Only applies to dragFrame menus. Remembers the position and size of the menu (by id) when the user moves it. Calling loadMenuPosition after menu creation will restore it to the last set size and position. If set to false, calls to loadMenuPosition will fail." },
		},
	}},
	valuetype = "tes3uiElement",
}
