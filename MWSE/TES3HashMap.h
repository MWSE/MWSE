#pragma once

#include "MemoryUtil.h"

namespace TES3 {

	template <typename K, typename V>
	struct HashMap {
		struct KeyValuePair {
			K key; // 0x0
			V value; // 0x0 + sizeof(K)
		};
		struct Node {
			// Allow convenient access to the key/value.
			union {
				KeyValuePair keyValuePair; // 0x0
				struct {
					K key; // 0x0
					V value; // 0x0 + sizeof(K) + alignment
				};
			};
			Node* nextNode; // 0x0 + sizeof(K) + sizeof(V) + alignment
		};
		struct VirtualTable {
			void(__thiscall* destructor)(HashMap<K, V>*, bool); // 0x0
			unsigned int(__thiscall* hashKey)(const HashMap<K, V>*, K); // 0x4
			bool(__thiscall* compareKey)(const HashMap<K, V>*, K, K); // 0x8
			void(__thiscall* makeKeyValuePair)(const HashMap<K, V>*, KeyValuePair*, K, V); // 0xC
			void(__thiscall* deleteKeyValuePair)(HashMap<K, V>*, KeyValuePair*); // 0x10
		};
		VirtualTable* vTable; // 0x0
		size_t count; // 0x4
		size_t bucketCount; // 0x8
		Node** buckets; // 0xC

		unsigned int hashKey(K& key) const { return vTable->hashKey(this, key); }
		bool compareKey(K& first, K& second) const { return vTable->compareKey(this, first, second); }
		void makeKeyValuePair(KeyValuePair* kvp, K& key, V& value) const { vTable->makeKeyValuePair(this, kvp, key, value); }
		void deleteKeyValuePair(KeyValuePair* kvp) { vTable->deleteKeyValuePair(this, kvp); }

		void addKey(K& key, V& value) {
			auto index = hashKey(key);
			for (auto itt = buckets[index]; itt; itt = itt->nextNode) {
				if (compareKey(key, itt->key)) {
					itt->value = value;
					return;
				}
			}

			auto node = mwse::tes3::_new<Node>();
			makeKeyValuePair(&node->keyValuePair, key, value);
			node->nextNode = buckets[index];
			buckets[index] = node;
		}

		bool eraseKey(K& key) {
			auto index = hashKey(key);
			for (auto itt = &buckets[index]; *itt; itt = &(*itt)->nextNode) {
				if (compareKey(key, (*itt)->key)) {
					auto next = (*itt)->nextNode;
					deleteKeyValuePair(&(*itt)->keyValuePair);
					reinterpret_cast<void(__cdecl*)(void*)>(0x727530)(*itt);
					*itt = next;
					return true;
				}
			}
			return false;
		}

		bool containsKey(K& key) const {
			auto index = hashKey(key);
			for (auto itt = buckets[index]; itt; itt = itt->nextNode) {
				if (compareKey(key, itt->key)) {
					return true;
				}
			}
			return false;
		}

		V& getValue(K& key) const {
			auto index = hashKey(key);
			for (auto itt = buckets[index]; itt; itt = itt->nextNode) {
				if (compareKey(key, itt->key)) {
					return itt->value;
				}
			}
			throw std::runtime_error("Value doesn't exist for given key.");
		}

		Node* getNode(K& key) const {
			auto index = hashKey(key);
			for (auto itt = buckets[index]; itt; itt = itt->nextNode) {
				if (compareKey(key, itt->key)) {
					return itt;
				}
			}
			return nullptr;
		}
	};
	static_assert(sizeof(HashMap<const char*, void*>) == 0x10, "TES3::HashMap failed size validation");
}
