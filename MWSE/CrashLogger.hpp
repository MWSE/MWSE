#pragma once
#include "CrashLogUtilities.hpp"

#include "TES3Actor.h"
#include "TES3ActorAnimationController.h"
#include "TES3AudioController.h"
#include "TES3BodyPartManager.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3CombatSession.h"
#include "TES3Creature.h"
#include "TES3CutscenePlayer.h"
#include "TES3DataHandler.h"
#include "TES3Dialogue.h"
#include "TES3Game.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3InputController.h"
#include "TES3ItemData.h"
#include "TES3Light.h"
#include "TES3LoadScreenManager.h"
#include "TES3MagicEffectInstance.h"
#include "TES3Misc.h"
#include "TES3MobilePlayer.h"
#include "TES3MobManager.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3Sound.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIMenuController.h"
#include "TES3VFXManager.h"
#include "TES3Weapon.h"
#include "TES3Weather.h"
#include "TES3WorldController.h"

#include "NICollisionSwitch.h"
#include "NIFlipController.h"
#include "NILinesData.h"
#include "NIPick.h"
#include "NISortAdjustNode.h"
#include "NITriShape.h"
#include "NITriShapeData.h"
#include "NIUVController.h"

#include "BitUtil.h"
#include "ScriptUtil.h"
#include "TES3Util.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "BuildDate.h"
#include "CodePatchUtil.h"
#include "MWSEConfig.h"
#include "MWSEDefs.h"

namespace CrashLogger::Version { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Playtime { inline void Init(); inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Exception { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Thread { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Calltrace { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::MorrowindScript { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::LuaTraceback { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Registry { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Stack { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
//namespace CrashLogger::Modules { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Install { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Memory { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Mods { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::LuaMods { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Device { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
//namespace CrashLogger::AssetTracker { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Warnings { inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }

namespace CrashLogger::Stack {
	inline std::string GetLineForObject(void** object, UINT32 depth);
}

namespace CrashLogger::PDB {
	inline std::string GetModule(UINT32 eip, HANDLE process);
	inline UINT32 GetModuleBase(UINT32 eip, HANDLE process);
	inline std::string GetSymbol(UINT32 eip, HANDLE process);
	inline std::string GetLine(UINT32 eip, HANDLE process);
	inline std::string GetClassNameFromRTTIorPDB(void* object);
}

namespace CrashLogger {
	template<typename T>
	class Dereference {
		intptr_t pointer;
		std::size_t size;

	public:
		Dereference(intptr_t pointer, std::size_t size) : pointer(pointer), size(size) {}

		Dereference(intptr_t pointer) : pointer(pointer), size(sizeof(T)) {}
		Dereference(const void* pointer) : pointer((intptr_t)pointer), size(sizeof(T)) {}

		operator bool() {
			return IsValidPointer();
		}

		operator T* () {
			if (IsValidPointer()) {
				return reinterpret_cast<T*>(pointer);
			}

			return nullptr;
			//throw std::runtime_error("Bad dereference");
		}

		T* operator->() {
			if (IsValidPointer()) {
				return reinterpret_cast<T*>(pointer);
			}

			return nullptr;
			//throw std::runtime_error("Bad dereference");
		}

	private:
		bool IsValidAddress() const {
			MEMORY_BASIC_INFORMATION mbi;
			if (::VirtualQuery((void*)pointer, &mbi, sizeof(mbi)))
			{
				if (mbi.State != MEM_COMMIT) return false;

				DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
				if ((mbi.Protect & mask) == 0) return false;

				if (mbi.Protect & PAGE_GUARD) return false;
				if (mbi.Protect & PAGE_NOACCESS) return false;

				if (size_t(mbi.RegionSize) < size) return false;

				return true;
			}
			return false;
		}

		bool AttemptDereference() const {
			try {
				// Attempt to read the address as a UINT32
				volatile UINT32 temp = *reinterpret_cast<const volatile UINT32*>(pointer);
				return true;
			}
			catch (...) {
				return false;
			}
		}

		bool IsVtableValid() const {
			//			if (vtables_.find(vtable) == vtables_.end()) return false;

			UINT32 vtable = *reinterpret_cast<UINT32*>(pointer);
			if (vtable > 0x740000 && vtable < 0x750000)
				return true;

			return false;
		}

		bool IsValidPointer() const {
			try {
				if (!IsValidAddress()) return false;
				if (!AttemptDereference()) return false;
				if (!IsVtableValid()) return false;
				return true;
			}
			catch (...) {
				return false;
			}
		}
	};
}

namespace CrashLogger::Labels {
	inline std::string AsUINT32(void* ptr) { return fmt::format("{:#08X}", **static_cast<UINT32**>(ptr)); }

	template<typename T> std::string As(void* ptr) {
		try {
			if (auto sanitized = Dereference<T>((UINT32)ptr)) {
				return LogClassLineByLine(*sanitized);
			}
			else {
				return "Unable to dereference";
			}
		}
		catch (...) {
			return "Failed to format";
		}
	}

	class Label {
		static inline std::vector<std::unique_ptr<Label>> labels;
		typedef std::string(*FormattingHandler)(void* ptr);
		static inline FormattingHandler lastHandler = nullptr;

	public:

		UINT32 address;
		UINT32 size;
		FormattingHandler function = lastHandler;
		std::string name;

		static auto& GetAll() { return labels; }

		Label() : address(0), size(0), function(nullptr) {}
		virtual ~Label() = default;

		Label(UINT32 aAddress, FormattingHandler aFunction = lastHandler, std::string aName = "", UINT32 aSize = 4)
			: Label()
		{
			address = aAddress;
			size = aSize;
			function = aFunction;
			name = std::move(aName);

			lastHandler = aFunction;
		};


		bool Satisfies(void* ptr) const {
			__try {
				return *static_cast<UINT32*>(ptr) >= address && *static_cast<UINT32*>(ptr) <= address + size;
			}
			__except (ExceptionFilter(GetExceptionCode()))
			{
				return false;
			}
		}

		static std::string GetTypeName(void* ptr) {
			return PDB::GetClassNameFromRTTIorPDB(ptr);
		}

		virtual std::string GetLabelName() const { return "None"; }

		virtual std::string GetName(void* object) const { return name; }

		virtual std::string GetDescription(void* object) const {
			if (function) return function(object);
			return "";
		}
	};

	class LabelClass : public Label {
	public:
		using Label::Label;

		std::string GetLabelName() const override { return "Class"; }

		std::string GetName(void* object) const override { return name.empty() ? GetTypeName(object) : name; }
	};

	class LabelGlobal : public Label {
	public:
		using Label::Label;

		std::string GetLabelName() const override { return "Global"; }
	};

	class LabelEmpty : public Label {
	public:
		using Label::Label;
	};

	template <class LabelType = LabelClass, class... _Types> void Push(_Types... args) {
		Label::GetAll().push_back(std::make_unique<LabelType>(std::forward<_Types>(args)...));
	}

	void FillMWSELabels();

	inline void FillLabels() {
		FillMWSELabels();
	}
}