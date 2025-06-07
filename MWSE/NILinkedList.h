#pragma once

#include "NIDefines.h"

namespace NI {
	template <typename T>
	struct LinkedList {
		T * data;
		LinkedList<T> * next;

		size_t size() const {
			size_t count = 0;
			for (auto node = this; node && node->data; node = node->next) {
				count++;
			}
			return count;
		}

		bool empty() const {
			return data == nullptr;
		}
	};
	static_assert(sizeof(LinkedList<void>) == 0x8, "NI::LinkedList failed size validation");

	typedef LinkedList<DynamicEffect> DynamicEffectLinkedList;
	typedef LinkedList<Node> NodeLinkedList;
	typedef LinkedList<Property> PropertyLinkedList;
}
