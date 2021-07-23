#pragma once

namespace TES3 {
	template <typename T>
	class StlList {
	public:
		class Node {
		public:
			T data;
			Node* previous;
			Node* next;

#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
			static void* operator new(size_t size) { return reinterpret_cast<void* (__cdecl*)(size_t)>(0x727692)(size); }
			static void operator delete(void* block) { reinterpret_cast<void(__cdecl*)(void*)>(0x727530)(block); }
#endif

			Node(const Node&) = delete;

			Node(const T& value) {
				previous = nullptr;
				next = nullptr;
				data = value;
			}

			~Node() {
				if (previous) {
					previous->next = nullptr;
				}
				previous = nullptr;

				if (next) {
					next->previous = nullptr;
				}
				next = nullptr;
			}
		};

		//
		// std interface functions
		//

		class iterator {
			friend class StlList;

		private:
			Node* m_Node;
			const StlList* m_Parent;

			iterator(Node* node, const StlList* parent) : m_Node(node), m_Parent(parent) {}

		public:
			using iterator_category = std::bidirectional_iterator_tag;

			using value_type = T;
			using difference_type = int;
			using pointer = T*;
			using reference = T&;

			iterator(const StlList* parent) : m_Node(nullptr), m_Parent(parent) {}

			iterator operator-(difference_type diff) const {
				iterator r(m_Node, m_Parent);
				r -= diff;
				return r;
			}

			iterator& operator--() {
				m_Node = m_Node->previous;
				return *this;
			}

			iterator operator--(int) {
				return *this - 1;
			}

			iterator& operator-=(difference_type diff) {
				while (diff-- > 0) {
					m_Node = m_Node->previous;
				}
				return *this;
			}

			iterator operator+(difference_type diff) const {
				iterator r(m_Node, m_Parent);
				r += diff;
				return r;
			}

			iterator& operator++() {
				m_Node = m_Node->next;
				return *this;
			}

			iterator operator++(int) {
				return *this + 1;
			}

			iterator& operator+=(difference_type diff) {
				while (diff-- > 0) {
					m_Node = m_Node->next;
				}
				return *this;
			}

			bool operator==(const iterator& itt) const {
				return itt.m_Node == m_Node || (m_Node == nullptr ? itt.isSentinel() : false);
			}

			bool operator!=(const iterator& itt) const {
				return itt.m_Node != m_Node && (m_Node == nullptr ? !itt.isSentinel() : true);
			}

			reference operator->() const {
				return m_Node->data;
			}

			reference operator*() const {
				return m_Node->data;
			}

			inline bool isSentinel() const {
				return m_Node == m_Parent->sentinel();
			}
		};

		using value_type = T;
		using size_type = size_t;
		using difference_type = int;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using const_iterator = const iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		iterator begin() const { return iterator(head, this); }
		iterator end() const { return iterator(sentinel(), this); }
		reverse_iterator rbegin() const { return std::make_reverse_iterator(end()); }
		reverse_iterator rend() const { return std::make_reverse_iterator(begin()); }
		const_iterator cbegin() const { return begin(); }
		const_iterator cend() const { return end(); }
		const_reverse_iterator crbegin() const { return rbegin(); }
		const_reverse_iterator crend() const { return rend(); }
		size_type size() const noexcept { return count; }
		bool empty() const noexcept { return count == 0; }

		//
		// General access functions.
		//

		size_type count;
		Node* head;
		Node* tail;

#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
		static void* operator new(size_t size) { return reinterpret_cast<void* (__cdecl*)(size_t)>(0x727692)(size); }
		static void operator delete(void* block) { reinterpret_cast<void(__cdecl*)(void*)>(0x727530)(block); }
#endif

		StlList() {
			count = 0;
			head = nullptr;
			tail = nullptr;
		}

		StlList(const StlList& other) = delete;
		StlList& operator=(const StlList&) = delete;

		reference operator[](size_type index) const { return *(begin() + index); }

		reference front() { return head->data; }
		const_reference front() const { return head->data; }

		reference back() { return tail->data; }
		const_reference back() const { return tail->data; }

		inline Node* sentinel() const { return reinterpret_cast<Node*>(reinterpret_cast<char*>(const_cast<Node**>(&tail)) - sizeof(T)); }

		void clear() {
			auto itt = head;
			while (itt) {
				auto toDelete = itt;
				itt = itt->next;
				delete toDelete;
			}
			head = nullptr;
			tail = nullptr;
			count = 0;
		}

		iterator insert(const_iterator& position, const_reference value) {
			auto newNode = new Node(value);
			auto insertBeforeNode = position.m_Node;

			// Insert before a given node.
			if (insertBeforeNode && insertBeforeNode != sentinel()) {
				if (insertBeforeNode->previous) {
					insertBeforeNode->previous->next = newNode;
					newNode->previous = insertBeforeNode->previous;
				}

				insertBeforeNode->previous = newNode;
				newNode->next = insertBeforeNode;
			}
			// Insert at end.
			else {
				if (tail) {
					tail->next = newNode;
				}
				newNode->previous = tail;
				tail = newNode;
			}

			// New head check.
			if (head == nullptr || insertBeforeNode == head) {
				head = newNode;
			}

			// New tail check.
			if (tail == nullptr || newNode->previous == tail) {
				tail = newNode;
			}

			count++;
			return iterator(newNode, this);
		}

		iterator insert(size_type position, const_reference value) {
			return insert(begin() + position, value);
		}

		void push_front(const_reference& value) {
			insert(begin(), value);
		}

		void push_back(const_reference& value) {
			insert(end(), value);
		}

		iterator erase(const_iterator& position) {
			iterator next(nullptr, this);

			auto node = position.m_Node;
			if (node && node != sentinel()) {
				if (node == head) {
					head = node->next;
				}
				if (node == tail) {
					tail = node->previous;
				}

				if (node->previous) {
					node->previous->next = node->next;
				}
				if (node->next) {
					next.m_Node = node->next;
					node->next->previous = node->previous;
				}
				node->previous = nullptr;
				node->next = nullptr;
				delete position.m_Node;
				count--;
			}

			return next;
		}

		iterator erase(const_iterator& first, const_iterator& last) {
			auto itt = first;
			while (itt != end() && itt != last) {
				itt = erase(itt);
			}
			return itt;
		}

		iterator erase(size_type position) {
			return erase(begin() + position);
		}

		//
		// Cached iterator access.
		//

		Node* cached_begin() {
			current = head;
			return current;
		}

		Node* cached_next() {
			current = current->next;
			return current;
		}

	};
	static_assert(sizeof(StlList<void*>) == 0xC, "TES3::StlList failed size validation");
	static_assert(offsetof(StlList<void*>, count) == 0x0, "TES3::StlList::count failed offset validation");
	static_assert(offsetof(StlList<void*>, head) == 0x4, "TES3::StlList::head failed offset validation");
	static_assert(offsetof(StlList<void*>, tail) == 0x8, "TES3::StlList::tail failed offset validation");
	static_assert(sizeof(StlList<void*>::Node) == 0xC, "TES3::StlList::Node failed size validation");
	static_assert(offsetof(StlList<void*>::Node, data) == 0x0, "TES3::StlList::Node::data failed offset validation");
	static_assert(offsetof(StlList<void*>::Node, previous) == sizeof(void*), "TES3::StlList::Node::previous failed offset validation");
	static_assert(offsetof(StlList<void*>::Node, next) == sizeof(void*) + 0x4, "TES3::StlList::Node::next failed offset validation");
}
