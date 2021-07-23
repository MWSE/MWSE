#pragma once

namespace TES3 {
	// A non-owning collection of game objects, linked together in this common collection.
	template <typename T>
	class LinkedObjectList {
	public:
		//
		// std interface functions
		//

		class iterator {
			friend class LinkedObjectList;

		private:
			T* m_Object;

			iterator(T* object) : m_Object(object) {}

		public:
			using iterator_category = std::random_access_iterator_tag;

			using value_type = T*;
			using difference_type = int;
			using pointer = T**;
			using reference = T*&;

			iterator() : m_Object(nullptr) {}

			iterator operator-(difference_type diff) const {
				iterator r = m_Object;
				r -= diff;
				return r;
			}

			iterator& operator--() {
				if (m_Object) {
					m_Object = m_Object->previousInCollection;
				}
				return *this;
			}

			iterator& operator-=(difference_type diff) {
				while (m_Object && diff-- > 0) {
					m_Object = m_Object->previousInCollection;
				}
				return *this;
			}

			iterator operator+(difference_type diff) const {
				iterator r = m_Object;
				r += diff;
				return r;
			}

			iterator& operator++() {
				if (m_Object) {
					m_Object = static_cast<value_type>(m_Object->nextInCollection);
				}
				return *this;
			}

			iterator& operator+=(difference_type diff) {
				while (m_Object && diff-- > 0) {
					m_Object = static_cast<value_type>(m_Object->nextInCollection);
				}
				return *this;
			}

			bool operator==(const iterator& itt) const {
				return itt.m_Object == m_Object;
			}

			bool operator!=(const iterator& itt) const {
				return itt.m_Object != m_Object;
			}

			reference operator->() {
				return m_Object;
			}

			reference operator*() {
				return m_Object;
			}
		};

		using value_type = T*;
		using size_type = size_t;
		using difference_type = int;
		using pointer = T**;
		using const_pointer = const T**;
		using reference = T*&;
		using const_reference = const T*&;
		using const_iterator = const iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		iterator begin() const { return head; }
		iterator end() const { return nullptr; }
		reverse_iterator rbegin() const { return tail; }
		reverse_iterator rend() const { return nullptr; }
		const_iterator cbegin() const { return head; }
		const_iterator cend() const { return nullptr; }
		const_reverse_iterator crbegin() const { return tail; }
		const_reverse_iterator crend() const { return nullptr; }
		size_type size() const noexcept { return count; }
		bool empty() const noexcept { return count == 0; }

		//
		// nextInCollection access functions.
		//

		size_type count;
		T* head;
		T* tail;

		static void* operator new(size_t size) { return reinterpret_cast<void* (__cdecl*)(size_t)>(0x727692)(size); }
		static void operator delete(void* block) { reinterpret_cast<void(__cdecl*)(void*)>(0x727530)(block); }

		const_iterator operator[](size_type index) const {
			return begin() + index;
		}

		value_type front() const { return *begin(); }

		void clear() {
			auto itt = head;
			while (itt) {
				auto toDelete = itt;
				itt = static_cast<value_type>(itt->nextInCollection);
				delete toDelete;
			}
			head = nullptr;
			tail = nullptr;
			count = 0;
		}

		iterator insert(iterator position, reference value) {
			auto beforeNode = position.m_Object;
			if (beforeNode) {
				if (beforeNode->nextInCollection) {
					beforeNode->nextInCollection->previousInCollection = value;
					value->nextInCollection = beforeNode->nextInCollection;
				}
				beforeNode->nextInCollection = value;

				count++;
			}
			else {
				if (head) {
					tail->nextInCollection = value;
					value->previousInCollection = tail;
					tail = value;
					count++;
				}
				else {
					head = value;
					tail = value;
					count++;
				}
			}

			return value;
		}

		iterator insert(size_type position, reference value) {
			return insert(begin() + position, value);
		}

		void push_front(reference value) {
			value->nextInCollection = head;

			if (head) {
				head->previousInCollection = value;
			}
			else {
				tail = value;
			}

			head = value;
		}

		void push_back(reference value) {
			insert(end(), value);
		}

		iterator erase(iterator position) {
			iterator nextInCollection = nullptr;

			auto node = position.m_Object;
			if (node) {
				if (node == head) {
					head = static_cast<value_type>(node->nextInCollection);
				}
				if (node == tail) {
					tail = static_cast<value_type>(node->previousInCollection);
				}

				if (node->previousInCollection) {
					node->previousInCollection->nextInCollection = static_cast<value_type>(node->nextInCollection);
				}
				if (node->nextInCollection) {
					nextInCollection = static_cast<value_type>(node->nextInCollection);
					node->nextInCollection->previousInCollection = static_cast<value_type>(node->previousInCollection);
				}
				node->previousInCollection = nullptr;
				node->nextInCollection = nullptr;

				count--;
			}

			return nextInCollection;
		}

		iterator erase(iterator first, iterator last) {
			auto itt = first;
			while (itt != end() && itt != last) {
				itt = erase(itt);
			}
			return itt;
		}

		iterator erase(size_type position) {
			return erase(begin() + position);
		}

		iterator erase_value(reference value) {
			const_iterator itt = std::find(cbegin(), cend(), value);
			return erase(itt);
		}

	};
	static_assert(sizeof(LinkedObjectList<void*>) == 0x0C, "TES3::LinkedObjectList failed size validation");
}
