#pragma once

namespace se {
	template <typename T>
	struct StdMapNode {
		StdMapNode* branchLess; // 0x0
		StdMapNode* nextLeafOrRoot; // 0x4
		StdMapNode* branchGreaterThanOrEqual; // 0x8
		T item; // 0xC
		DWORD unknown;

		StdMapNode() = delete;
		~StdMapNode() = delete;
	};

	template <typename T>
	struct StdMap {
		using Node = StdMapNode<T>;

		char tag; // 0x0
		Node* root; // 0x4
		char unknown_0x8; // 0x8
		size_t itemCount; // 0xC

		StdMap() = delete;
		~StdMap() = delete;

		Node* begin() const {
			return root->branchLess;
		}

		Node* end() const {
			return root;
		}

		Node* treeRoot() const {
			return root->nextLeafOrRoot;
		}

		template <typename Visitor>
		void traverseInOrder(Node* sentinel, Visitor visitor) const {
			traverseInOrder(treeRoot(), sentinel, visitor);
		}

	private:
		template <typename Visitor>
		static void traverseInOrder(Node* node, Node* sentinel, Visitor& visitor) {
			if (!node || node == sentinel) {
				return;
			}

			traverseInOrder(node->branchLess, sentinel, visitor);
			visitor(node);
			traverseInOrder(node->branchGreaterThanOrEqual, sentinel, visitor);
		}
	};

	static_assert(sizeof(StdMap<void*>) == 0x10, "se::StdMap failed size validation");
	static_assert(offsetof(StdMap<void*>, root) == 0x4, "se::StdMap::root failed offset validation");
	static_assert(offsetof(StdMap<void*>, itemCount) == 0xC, "se::StdMap::itemCount failed offset validation");

	static_assert(offsetof(StdMapNode<void*>, branchLess) == 0x0, "se::StdMapNode::branchLess failed offset validation");
	static_assert(offsetof(StdMapNode<void*>, nextLeafOrRoot) == 0x4, "se::StdMapNode::nextLeafOrRoot failed offset validation");
	static_assert(offsetof(StdMapNode<void*>, branchGreaterThanOrEqual) == 0x8, "se::StdMapNode::branchGreaterThanOrEqual failed offset validation");
	static_assert(offsetof(StdMapNode<void*>, item) == 0xC, "se::StdMapNode::item failed offset validation");
}
