#pragma once

#include "CrashLogUtilities.h"

namespace CrashLogger::Client {
	std::ostream& GetLog();
	std::string GetGPU();
	std::string GetKnownClassName(void* object);
	bool DescribeObject(void* object, std::string& labelName, std::string& objectName, std::string& description);
	bool IsValidVTable(UINT32 vtable);
	void FillLabels();
}

namespace CrashLogger::Version {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::Playtime {
	void Init();
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::Exception {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::Thread {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::Calltrace {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
namespace CrashLogger::MorrowindScript {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::LuaTraceback {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::LuaMods {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}
#endif

namespace CrashLogger::Registry {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::Stack {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
	std::string GetLineForObject(void** object, UINT32 depth);
}

namespace CrashLogger::Install {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::Memory {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::Mods {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::Device {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::Warnings {
	void Process(EXCEPTION_POINTERS* info);
	std::stringstream& Get();
}

namespace CrashLogger::PDB {
	std::string GetModule(UINT32 eip, HANDLE process);
	UINT32 GetModuleBase(UINT32 eip, HANDLE process);
	std::string GetSymbol(UINT32 eip, HANDLE process);
	std::string GetLine(UINT32 eip, HANDLE process);
	std::string GetClassNameFromRTTIorPDB(void* object);
}

namespace CrashLogger {
	inline constexpr bool DEBUG_LOGGER = false;

	template<typename T>
	class Dereference {
		intptr_t pointer;
		std::size_t size;

	public:
		Dereference(intptr_t pointer, std::size_t size) : pointer(pointer), size(size) {}
		Dereference(intptr_t pointer) : pointer(pointer), size(sizeof(T)) {}
		Dereference(const void* pointer) : pointer(reinterpret_cast<intptr_t>(pointer)), size(sizeof(T)) {}

		operator bool() const {
			return IsValidPointer();
		}

		operator T* () const {
			return IsValidPointer() ? reinterpret_cast<T*>(pointer) : nullptr;
		}

		T* operator->() const {
			return IsValidPointer() ? reinterpret_cast<T*>(pointer) : nullptr;
		}

	private:
		bool IsValidAddress() const {
			MEMORY_BASIC_INFORMATION mbi = {};
			if (!VirtualQuery(reinterpret_cast<void*>(pointer), &mbi, sizeof(mbi))) {
				return false;
			}

			if (mbi.State != MEM_COMMIT) return false;

			constexpr DWORD mask = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY |
				PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
			if ((mbi.Protect & mask) == 0) return false;
			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) return false;

			const auto regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
			return static_cast<uintptr_t>(pointer) + size <= regionEnd;
		}

		bool AttemptDereference() const {
			__try {
				volatile UINT32 value = *reinterpret_cast<const volatile UINT32*>(pointer);
				(void)value;
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return false;
			}
		}

		bool IsVtableValid() const {
			__try {
				return Client::IsValidVTable(*reinterpret_cast<const UINT32*>(pointer));
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return false;
			}
		}

		bool IsValidPointer() const {
			return IsValidAddress() && AttemptDereference() && IsVtableValid();
		}
	};

	void Log(EXCEPTION_POINTERS* info);
	void AttemptLog(EXCEPTION_POINTERS* info);
	LONG WINAPI Filter(EXCEPTION_POINTERS* info);

	extern LPTOP_LEVEL_EXCEPTION_FILTER s_originalFilter;
}

namespace CrashLogger::Labels {
	class Label {
		using FormattingHandler = std::string(*)(void* ptr);

		static inline std::vector<std::unique_ptr<Label>> labels;
		static inline FormattingHandler lastHandler = nullptr;

	public:
		UINT32 address = 0;
		UINT32 size = 0;
		FormattingHandler function = nullptr;
		std::string name;

		static auto& GetAll() { return labels; }

		Label() = default;
		virtual ~Label() = default;

		Label(UINT32 address, FormattingHandler function = lastHandler, std::string name = "", UINT32 size = 4)
			: address(address), size(size), function(function), name(std::move(name)) {
			lastHandler = function;
		}

		bool Satisfies(void* ptr) const {
			__try {
				const auto value = *static_cast<UINT32*>(ptr);
				return value >= address && value <= address + size;
			}
			__except (ExceptionFilter(GetExceptionCode())) {
				return false;
			}
		}

		static std::string GetTypeName(void* ptr) {
			return PDB::GetClassNameFromRTTIorPDB(ptr);
		}

		virtual std::string GetLabelName() const { return "None"; }
		virtual std::string GetName(void* object) const { return name; }

		virtual std::string GetDescription(void* object) const {
			return function ? function(object) : "";
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

	template <class LabelType = LabelClass, class... Types>
	void Push(Types&&... args) {
		Label::GetAll().push_back(std::make_unique<LabelType>(std::forward<Types>(args)...));
	}
}
