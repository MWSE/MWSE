#pragma once

namespace TES3 {
	template <typename T>
	class IteratedList {
	public:
		struct VirtualTable {
			void(__thiscall* destructor)(IteratedList<T>*, bool); // 0x0
		};

		class Node {
		public:
			Node* previous;
			Node* next;
			T data;

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

		class IteratorPreserver {
			friend class IteratedList;
		public:
			IteratorPreserver(IteratedList<T>* parent) :
				m_Parent(parent),
				m_PreviousCurrent(parent->current)
			{

			}

			IteratorPreserver(const IteratorPreserver& other) {
				m_Parent = other.m_Parent;
				m_PreviousCurrent = other.m_PreviousCurrent;
			}

			~IteratorPreserver() {
				if (m_Parent) {
					m_Parent->current = m_PreviousCurrent;
				}
			}

		protected:
			IteratedList<T>* m_Parent;
			Node* m_PreviousCurrent;
		};

		//
		// std interface functions
		//

		class iterator {
			friend class IteratedList;

		private:
			Node* m_Node;
			const IteratedList* m_Parent;

			iterator(Node* node, const IteratedList* parent) : m_Node(node), m_Parent(parent) {}

		public:
			using iterator_category = std::bidirectional_iterator_tag;

			using value_type = T;
			using difference_type = int;
			using pointer = T*;
			using reference = T&;

			iterator(const IteratedList* parent) : m_Node(nullptr), m_Parent(parent) {}

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
				if (m_Node == m_Parent->tail) {
					m_Node = m_Parent->sentinel();
				}
				else {
					m_Node = m_Node->next;
				}
				return *this;
			}

			iterator operator++(int) {
				return *this + 1;
			}

			iterator& operator+=(difference_type diff) {
				while (diff-- > 0) {
					if (m_Node == m_Parent->tail) {
						m_Node = m_Parent->sentinel();
						return *this;
					}
					else {
						m_Node = m_Node->next;
					}
				}
				return *this;
			}

			bool operator==(const iterator& itt) const {
				return itt.m_Node == m_Node;
			}

			bool operator!=(const iterator& itt) const {
				return itt.m_Node != m_Node;
			}

			reference operator->() const {
				return m_Node->data;
			}

			reference operator*() const {
				return m_Node->data;
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

		iterator begin() const { return iterator(head ? head : sentinel(), this); }
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

		VirtualTable* virtualTable;
		size_type count;
		Node* head;
		Node* tail;
		Node* current;

#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
		static void* operator new(size_t size) { return reinterpret_cast<void* (__cdecl*)(size_t)>(0x727692)(size); }
		static void operator delete(void* block) { reinterpret_cast<void(__cdecl*)(void*)>(0x727530)(block); }
#endif

		IteratedList() {
#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
			virtualTable = reinterpret_cast<VirtualTable*>(0x7477AC);
#else
			virtualTable = nullptr;
#endif
			count = 0;
			head = nullptr;
			tail = nullptr;
			current = nullptr;
		}

		IteratedList(const IteratedList& other) = delete;
		IteratedList& operator=(const IteratedList&) = delete;

		~IteratedList() {
			if (virtualTable) {
				virtualTable->destructor(this, false);
			}
		}

		reference operator[](size_type index) const { return *(begin() + index); }

		reference front() { return head->data; }
		const_reference front() const { return head->data; }

		reference back() { return tail->data; }
		const_reference back() const { return tail->data; }

		inline Node* sentinel() const  { return reinterpret_cast<Node*>(const_cast<Node**>(&tail)); }

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

		// Creates a class that stores the iterator's current state.
		// When the preserver destructs, the "current" iterator state is restored.
		// This should only really be used if doing a managed/cached loop on the list that is otherwise const (no elements added/removed).
		IteratorPreserver makeIteratorPreserver() {
			return IteratorPreserver(this);
		}

	};
	static_assert(sizeof(IteratedList<void*>) == 0x14, "TES3::IteratedList failed size validation");
	static_assert(offsetof(IteratedList<void*>, count) == 0x4, "TES3::IteratedList::count failed offset validation");
	static_assert(offsetof(IteratedList<void*>, head) == 0x8, "TES3::IteratedList::head failed offset validation");
	static_assert(offsetof(IteratedList<void*>, tail) == 0xC, "TES3::IteratedList::tail failed offset validation");
	static_assert(offsetof(IteratedList<void*>, current) == 0x10, "TES3::IteratedList::current failed offset validation");
	static_assert(sizeof(IteratedList<void*>::Node) == 0xC, "TES3::IteratedList::Node failed size validation");
	static_assert(offsetof(IteratedList<void*>::Node, previous) == 0x0, "TES3::IteratedList::Node::previous failed offset validation");
	static_assert(offsetof(IteratedList<void*>::Node, next) == 0x4, "TES3::IteratedList::Node::next failed offset validation");
	static_assert(offsetof(IteratedList<void*>::Node, data) == 0x8, "TES3::IteratedList::Node::data failed offset validation");
}
