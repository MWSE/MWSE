/************************************************************************

	TES3Util.h - Copyright (c) 2008 The MWSE Project
	https://github.com/MWSE/MWSE/

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#pragma once

#include <random>
#include <queue>

#include "mwseString.h"

#include "TES3Defines.h"

#include "TES3Attachment.h"
#include "TES3DataHandler.h"
#include "TES3MagicEffect.h"
#include "TES3Collections.h"

namespace mwse {
	namespace tes3 {
		TES3::WorldController * getWorldController();
		TES3::DataHandler * getDataHandler();
		TES3::Game * getGame();

		TES3::Reference* getReference(const char* id);
		TES3::Reference* getReference(std::string& id);

		template <typename T>
		T* getObjectById(const char* id, int type = 0) {
			TES3::BaseObject* potentialResult = tes3::getDataHandler()->nonDynamicData->resolveObject(id);
			if (!potentialResult) {
				return NULL;
			}
			else if (type && potentialResult->objectType != type) {
				return NULL;
			}

			return reinterpret_cast<T*>(potentialResult);
		}

		template <typename T>
		T* getObjectById(const std::string& id, int type = 0) {
			return getObjectById<T>(id.c_str(), type);
		}

		TES3::Spell* getSpellById(const char*);
		TES3::Spell* getSpellById(std::string&);

		template <typename T>
		T* skipDeletedObjects(TES3::Object* object) {
			while (object && (object->objectFlags & TES3::ObjectFlag::Delete) == TES3::ObjectFlag::Delete)
			{
				object = object->nextInCollection;
			}
			return reinterpret_cast<T*>(object);
		}

		bool getHasBaseRecord(TES3::BaseObject* record);

		TES3::IteratorNode<TES3::ItemStack> * getFirstInventoryNode(TES3::Reference* reference);

		bool hasInventory(TES3::BaseObject* record);

		long getValue(TES3::Reference* reference, bool multiplyByCount);

		float getWeight(TES3::Reference* reference, bool multiplyByCount);

		TES3::BaseObject* getBaseRecord(TES3::BaseObject* reference);

		char* setDataString(char**, const char*);

		template <typename T>
		T* getAttachment(TES3::Reference* reference, TES3::AttachmentType::AttachmentType attachmentType) {
			if (reference == NULL || reference->attachments == NULL) {
				return NULL;
			}

			TES3::Attachment* attachment = reference->attachments;
			while (attachment && attachment->type != attachmentType) {
				attachment = attachment->next;
			}

			return reinterpret_cast<T*>(attachment);
		}

		bool insertAttachment(TES3::Reference* reference, TES3::Attachment* attachment);

		TES3::MobileActor* getAttachedMobileActor(TES3::Reference* reference);
		TES3::MobileCreature* getAttachedMobileCreature(TES3::Reference* reference);
		TES3::MobileNPC* getAttachedMobileNPC(TES3::Reference* reference);
		TES3::MobilePlayer* getAttachedMobilePlayer(TES3::Reference* reference);

		TES3::ItemData* getAttachedItemDataNode(TES3::Reference* reference);
		TES3::ItemData* getOrCreateAttachedItemDataNode(TES3::Reference* reference);

		TES3::LockAttachmentNode* getAttachedLockNode(TES3::Reference* reference);

		TES3::ItemData* createNewItemCondition(TES3::Object* object);

		unsigned int* getBaseEffectFlags();
		bool getBaseEffectFlag(int index, TES3::EffectFlag::Flag flag);
		void setBaseEffectFlag(int index, TES3::EffectFlag::Flag flag, bool set);

		size_t getEffectCount(const TES3::Effect* effectArray);

		bool setEffect(TES3::Effect * effects, long index, long effectId,
			long skillAttributeId, long range, long area, long duration,
			long minimumMagnitude, long maximumMagnitude);

		bool effectsMatch(TES3::Effect* effectsA, TES3::Effect* effectsB, long count = 8);

		float getSkillRequirement(TES3::Reference* reference, long skillId);

		void checkForLevelUp(long progress);

		void messagePlayer(const char* message);

		TES3::GameSettingInfo* getGMSTInfo(int);

		// Used in xFirstNPC/Static/Item. The last element should never be non-null.
		// The first eight elements are pointers to the first reference from the 8 surrounding cells.
		extern TES3::Reference* exteriorRefs[9];

		void clearExteriorRefs();

		void startNewGame();

		int getRestHoursInterrupted();
		void setRestHoursInterrupted(int);
		int getRestInterruptCount();
		void setRestInterruptCount(int);

		//
		// Original function calls in Morrowind.
		//

		template <typename T>
		T * _new() {
			const auto __new = reinterpret_cast<T*(__cdecl*)(size_t)>(0x727692);
			return __new(sizeof(T));
		}

		template <typename T>
		void _delete(T * address) {
			const auto __delete = reinterpret_cast<void (__cdecl*)(T *)>(0x727530);
			__delete(address);
		}

		typedef void* (__cdecl *ExternalRealloc)(void*, size_t);
		extern ExternalRealloc _realloc;
		void* realloc(void* address, size_t size);

		typedef void* (__cdecl *ExternalMalloc)(size_t);
		extern ExternalMalloc _malloc;
		void* malloc(size_t size);

		template <typename T>
		T* malloc(size_t size) {
			return reinterpret_cast<T*>(_malloc(size));
		}

		template <typename T>
		T* malloc() {
			T * ret = reinterpret_cast<T*>(_malloc(sizeof(T)));
			memset(ret, 0, sizeof(T));
			return ret;
		}

		typedef void(__cdecl *ExternalFree)(void*);
		extern ExternalFree _free;
		void free(void* address);
	}
};
