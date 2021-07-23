#pragma once

#include "TES3Defines.h"
#include "TES3UIDefines.h"

namespace TES3 {
	namespace UI {
		struct TreeItem {
			short key;
			PropertyValue value;
			PropertyType valueType;

			//
			// Custom functions.
			//

			const char* getName();
			PropertyType getType() const;

			sol::object getValue_lua(sol::this_state ts) const;

		};
		static_assert(sizeof(TreeItem) == 0xC, "TES3::UI::TreeNode failed size validation");

		struct TreeNode {
			TreeNode * branchLess;
			TreeNode * nextLeafOrRoot;
			TreeNode * branchGreaterThanOrEqual;
			TreeItem item;
			int unknown_18;
		};
		static_assert(sizeof(TreeNode) == 0x1C, "TES3::UI::TreeNode failed size validation");

		struct Tree {
			char tag;
			TreeNode * root;
			char unknown_8;
			size_t itemCount;
		};
		static_assert(sizeof(Tree) == 0x10, "TES3::UI::Tree failed size validation");
	}
}
