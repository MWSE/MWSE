return {
	type = "method",
	description = [[Returns the descendant element that creation functions used on this element will place children into, or the calling element if there is no specific descendant for children.

    Some widgets like ScrollPanes are built of multiple layers of elements. When an element is created in a complex widget, it is automatically placed as a child of a content element, but other functions do not access this content element directly. This function finds this content container for any element, so that changing layout and accessing children is possible. For simple elements, the calling element will be returned so that there is always a valid container element.]],
    valuetype = "tes3uiElement",
}