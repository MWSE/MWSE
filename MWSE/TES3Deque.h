#pragma once

namespace TES3 {
	template <typename T>
	class Deque {
	public:
		class Node {
		public:
			Node* next;
			Node* previous;
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
				previous->next = next;
				next->previous = previous;
			}
		};

		//
		// std interface functions
		//

		class iterator {
			friend class Deque;

		private:
			Node* m_Node;

			iterator(Node* node) : m_Node(node) {}

		public:
			using iterator_category = std::bidirectional_iterator_tag;

			using value_type = T;
			using difference_type = int;
			using pointer = T*;
			using reference = T&;

			iterator() = delete;

			iterator operator-(difference_type diff) const {
				iterator r = m_Node;
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
				iterator r = m_Node;
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

		iterator begin() const { return sentinel->next; }
		iterator end() const { return sentinel; }
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

		bool unknown_0x0;
		Node* sentinel; // 0x4
		size_t count; // 0x8

#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
		static void* operator new(size_t size) { return reinterpret_cast<void* (__cdecl*)(size_t)>(0x727692)(size); }
		static void operator delete(void* block) { reinterpret_cast<void(__cdecl*)(void*)>(0x727530)(block); }
#endif

		Deque() {
			unknown_0x0 = true;
			sentinel = new Node(T());
			sentinel->previous = sentinel;
			sentinel->next = sentinel;
			count = 0;
		}

		Deque(const Deque& other) = delete;
		Deque& operator=(const Deque&) = delete;

		~Deque() { clear(); }

		reference operator[](size_type index) const { return *(begin() + index); }

		reference front() { return sentinel->next->data; }
		const_reference front() const { return sentinel->next->data; }

		reference back() { return sentinel->previous->data; }
		const_reference back() const { return sentinel->previous->data; }

		void clear() {
			while (begin() != end()) {
				erase(begin());
			}
		}

		iterator insert(const_iterator& position, const_reference value) {
			auto newNode = new Node(value);
			auto insertBeforeNode = position.m_Node;

			// Insert before a given node.
			insertBeforeNode->previous->next = newNode;
			newNode->previous = insertBeforeNode->previous;

			insertBeforeNode->previous = newNode;
			newNode->next = insertBeforeNode;

			count++;
			return newNode;
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
			iterator next = sentinel;

			auto node = position.m_Node;
			if (node != sentinel) {
				next.m_Node = node->next;

				node->previous->next = node->next;
				node->next->previous = node->previous;
				delete node;
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
	static_assert(sizeof(Deque<int>) == 0xC, "TES3::Deque failed size validation");
	static_assert(offsetof(Deque<int>, unknown_0x0) == 0x0, "TES3::Deque::unknown_0x0 failed offset validation");
	static_assert(offsetof(Deque<int>, sentinel) == 0x4, "TES3::Deque::unknown_0x0 failed offset validation");
	static_assert(offsetof(Deque<int>, count) == 0x8, "TES3::Deque::unknown_0x0 failed offset validation");
	static_assert(sizeof(Deque<int>::Node) == 0xC, "TES3::Deque::Node failed size validation");
	static_assert(offsetof(Deque<int>::Node, next) == 0x0, "TES3::Deque::Node::next failed offset validation");
	static_assert(offsetof(Deque<int>::Node, previous) == 0x4, "TES3::Deque::Node::previous failed offset validation");
	static_assert(offsetof(Deque<int>::Node, data) == 0x8, "TES3::Deque::Node::data failed offset validation");
}
