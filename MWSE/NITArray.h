#pragma once

namespace NI {
	template <typename T>
	class TArray {
		static_assert(sizeof(T) == 0x4, "TArray types must be 4 bytes in size.");

	public:
		//
		// iterator access
		//

		class iterator {
			friend class TArray;

		private:
			T* m_Ptr;

		public:
			using iterator_category = std::random_access_iterator_tag;

			using value_type = T;
			using difference_type = int;
			using pointer = T*;
			using reference = T&;

			iterator() : m_Ptr(nullptr) {}
			iterator(T* ptr) : m_Ptr(ptr) {}

			iterator operator-(difference_type diff) const {
				return m_Ptr - diff;
			}

			iterator& operator--() {
				--m_Ptr;
				return *this;
			}

			iterator operator--(int) {
				return *this - 1;
			}

			iterator& operator-=(difference_type diff) {
				m_Ptr -= diff;
				return *this;
			}

			iterator operator+(difference_type diff) const {
				return m_Ptr + diff;
			}

			iterator& operator++() {
				++m_Ptr;
				return *this;
			}

			iterator operator++(int) {
				return *this + 1;
			}

			iterator& operator+=(difference_type diff) {
				m_Ptr += diff;
				return *this;
			}

			bool operator==(const iterator& itt) const {
				return itt.m_Ptr == m_Ptr;
			}

			bool operator!=(const iterator& itt) const {
				return itt.m_Ptr != m_Ptr;
			}

			reference operator->() const {
				return *m_Ptr;
			}

			reference operator*() const {
				return *m_Ptr;
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

		constexpr iterator begin() noexcept { return storage; }
		constexpr const_iterator begin() const noexcept { return storage; }
		constexpr const_iterator cbegin() const noexcept { return begin(); }
		constexpr iterator end() noexcept { return &storage[storageCount]; }
		constexpr const_iterator end() const noexcept { return &storage[storageCount]; }
		constexpr const_iterator cend() const noexcept { return end(); }

		constexpr reverse_iterator rbegin() noexcept { std::make_reverse_iterator(end()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(end()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return std::make_reverse_iterator(end()); }
		constexpr reverse_iterator rend() noexcept { std::make_reverse_iterator(begin()); }
		constexpr const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(begin()); }
		constexpr const_reverse_iterator crend() const noexcept { return std::make_reverse_iterator(begin()); }

		//
		// General access functions.
		//

		pointer storage; // 0x4
		size_type storageCount; // 0x8 // # of slots in the array.
		size_type endIndex; // 0xC // First empty slot in the array.
		size_type filledCount; // 0x10 // Number of filled slots.
		size_type growByCount; // 0x14 // Number of slots to increase storage by.

#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
		static void* operator new(size_type size) { return reinterpret_cast<void* (__cdecl*)(size_type)>(0x727692)(size); }
		static void operator delete(void* block) { reinterpret_cast<void(__cdecl*)(void*)>(0x727530)(block); }
#endif

		TArray(size_type size = 1) {
			storageCount = size;
			growByCount = size;
			endIndex = 0;
			filledCount = 0;
#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
			storage = reinterpret_cast<T* (__cdecl*)(size_type)>(0x727692)(size * 4);
#else
			storage = new T[size];
#endif
			memset(storage, 0, size * 4);
		}

		TArray(const TArray& other) {
			*this = other;
		}

		virtual ~TArray() {
#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
			reinterpret_cast<void(__cdecl*)(void*)>(0x727530)(storage);
#else
			delete[] storage;
#endif
		}

		TArray<T>& operator= (const TArray<T>& other) {
			// Clear current data.
			clear();

			// Change size to fit.
			growByCount = other.growByCount;
			setSize(other.size());
			
			// Copy over values.
			for (auto i = 0u; i < storageCount; ++i) {
				setAtIndex(i, other.storage[i]);
			}

			return *this;
		}

		const T operator[](size_type index) const { return at(index); }

		constexpr reference at(size_type pos) {
			if (pos >= storageCount) {
				throw std::out_of_range("TES3::TArray::at - Access out of bounds.");
			}
			return storage[pos];
		}

		constexpr reference at(size_type pos) const {
			if (pos >= storageCount) {
				throw std::out_of_range("TES3::TArray::at - Access out of bounds.");
			}
			return storage[pos];
		}

		constexpr size_type getIndexOfIterator(const_iterator& itt) {
			return itt.m_Ptr - storage;
		}

		iterator insert(const_iterator& position, const_reference value) {
			// Generic add.
			if (position == end()) {
				auto index = add(value);
				return &storage[index];
			}

			// Get our index.
			auto index = getIndexOfIterator(position);
			setAtIndex(index, value);

			// Actually set the value.
			return &storage[index];
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
			setAtIndex(position.m_Ptr - storage, 0);
			return position + 1;
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

		void clear() {
			for (auto i = 0u; i < storageCount; ++i) {
				storage[i] = T(0);
			}
			filledCount = 0;
			endIndex = 0;
		}
		
		void fill(const_reference value) {
			for (auto i = 0u; i < storageCount; ++i) {
				setAtIndex(i, value);
			}
		}

		constexpr auto size() const noexcept {
			return storageCount;
		}

		constexpr bool empty() const noexcept {
			return size() == 0;
		}

		//
		// Non-STL functions.
		//

		value_type at_legacy(size_type pos) const {
			return storage[pos];
		}

		// getIndexOfValue returns -1 if not found.
		static constexpr int INVALID_INDEX = -1;
		int getIndexOfValue(const T value) const {
			if (!value) {
				return INVALID_INDEX;
			}

			for (auto i = 0u; i < storageCount; ++i) {
				if (storage[i] == value) {
					return i;
				}
			}

			return INVALID_INDEX;
		}

		void setAtIndex(size_type index, const_reference value) {
			if (index >= storageCount) {
				return;
			}

			if (index < endIndex) {
				const auto& oldValue = storage[index];
				if (value != T(0)) {
					if (!oldValue) {
						filledCount++;
					}
				}
				else if (oldValue) {
					filledCount--;
				}
			}
			else {
				if (value) {
					filledCount++;
				}
			}

			storage[index] = value;
			recalculateEndIndex();
		}

		void setSize(size_t size) {
			if (size == storageCount) {
				return;
			}

			if (size < endIndex) {
				for (auto i = size; i < endIndex; ++i) {
					if (storage[i]) {
						storage[i] = T(0);
						filledCount--;
					}
				}
				endIndex = size;
			}
#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
			auto newStorage = reinterpret_cast<T* (__cdecl*)(size_t)>(0x727692)(size * sizeof(T));
#else
			auto newStorage = new T[size];
#endif
			memset(newStorage, 0, size * sizeof(T));
			for (auto i = 0u; i < endIndex; ++i) {
				newStorage[i] = storage[i];
			}
#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
			reinterpret_cast<void(__cdecl*)(void*)>(0x727530)(storage);
#else
			delete[] storage;
#endif
			storage = newStorage;
			storageCount = size;
		}

		bool grow() {
			return growToFit(storageCount + growByCount);
		}

		bool growToFit(size_t index) {
			if (index < storageCount) {
				return false;
			}

			setSize(std::max(storageCount + growByCount, index + 1));
			return true;
		}

		bool contains(const_reference value) const {
			return getIndexOfValue(value) >= 0;
		}

		size_type add(const_reference value) {
			size_type index = endIndex;

			if (index == storageCount) {
				setSize(storageCount + growByCount);
			}
			setAtIndex(index, value);
			return index;
		}

		int getFirstEmptyIndex() const {
			for (auto i = 0u; i < endIndex; ++i) {
				if (!storage[i]) {
					return i;
				}
			}
			return endIndex;
		}

		void addToFirstEmptyIndex(const_reference value) {
			if (!value) {
				return;
			}

			const auto index = getFirstEmptyIndex();
			growToFit(index);
			setAtIndex(index, value);
		}

		size_type getFilledCount() const { return filledCount; }
		size_type getEndIndex() const { return endIndex; }

		void compact(bool resizeStorage = false) {
			if (filledCount == endIndex) {
				return;
			}

			if (filledCount) {
				for (auto i = 0u, j = 0u; i < endIndex; ++i) {
					if (storage[i] == T(0)) continue;
					if (storage[i] != storage[j]) {
						storage[j] = storage[i];
					}
					j++;
				}
			}

			endIndex = filledCount;

			if (resizeStorage && filledCount > 0) {
				if (filledCount > 0) {
					setSize(filledCount);
				}
				else {
					setSize(1);
				}
			}
		}

		void recalculateEndIndex() {
			endIndex = storageCount;
			for (auto i = storageCount - 1; i > 0; --i) {
				if (storage[i] != T(0)) {
					break;
				}
				endIndex = i;
			}
		}

	};
	static_assert(sizeof(TArray<int>) == 0x18, "NI::TArray failed size validation");
}
