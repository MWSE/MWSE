#pragma once

#include "TES3Defines.h"
#include "TES3UIDefines.h"
#include "StdMap.h"

namespace TES3::UI {
	struct TreeItem {
		Property key;
		PropertyValue value;
		PropertyType valueType;

		TreeItem() = delete;

		//
		// Custom functions.
		//

		const char* getName();
		PropertyType getType() const;

		sol::object getValue_lua(sol::this_state ts) const;

	};
	static_assert(sizeof(TreeItem) == 0xC, "TES3::UI::TreeNode failed size validation");

	using TreeNode = se::StdMapNode<TreeItem>;
	static_assert(sizeof(TreeNode) == 0x1C, "TES3::UI::TreeNode failed size validation");
	static_assert(offsetof(TreeNode, item) == 0xC, "TES3::UI::TreeNode::item failed offset validation");

	struct Tree : se::StdMap<TreeItem> {
		Tree() = delete;
		~Tree() = delete;
	};
	static_assert(sizeof(Tree) == 0x10, "TES3::UI::Tree failed size validation");
}
