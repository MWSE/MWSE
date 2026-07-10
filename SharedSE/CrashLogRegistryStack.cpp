#include "CrashLogger.h"

#include "StringUtil.h"

namespace CrashLogger {
	static bool GetStringForClassLabel(void* object, std::string& labelName, std::string& objectName, std::string& description) {
		try {
			if (Client::DescribeObject(object, labelName, objectName, description)) {
				return true;
			}

			static bool labelsFilled = false;
			if (!labelsFilled) {
				Client::FillLabels();
				labelsFilled = true;
			}

			for (const auto& label : Labels::Label::GetAll()) {
				if (label && label->Satisfies(object)) {
					labelName = label->GetLabelName();
					objectName = label->GetName(object);
					description = label->GetDescription(object);
					return true;
				}
			}
		}
		catch (...) {
		}
		return false;
	}

	static bool CopyPrintableString(const char* source, char (&buffer)[MAX_PATH], std::size_t& length) {
		__try {
			for (length = 0; length < MAX_PATH - 1; ++length) {
				const auto value = source[length];
				if (value == '\0') {
					buffer[length] = '\0';
					return true;
				}
				if (!se::string::is_printable(value)) {
					return false;
				}
				buffer[length] = value;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return false;
		}
		return false;
	}

	static bool GetAsString(const void* object, std::string& labelName, std::string& string) {
		if (!object) return false;

		char buffer[MAX_PATH] = {};
		std::size_t length = 0;
		if (!CopyPrintableString(static_cast<const char*>(object), buffer, length) || length < 3) {
			return false;
		}

		labelName = "String";
		string = SanitizeString(buffer);
		return true;
	}
}

namespace CrashLogger::Registry {
	static std::stringstream output;

	void Process(EXCEPTION_POINTERS* info) {
		try {
			output << fmt::format("REG | {:^10} | DEREFERENCE INFO", "Value") << '\n';

			const std::map<std::string, UINT32> registers{
				{ "eax", info->ContextRecord->Eax },
				{ "ebx", info->ContextRecord->Ebx },
				{ "ecx", info->ContextRecord->Ecx },
				{ "edx", info->ContextRecord->Edx },
				{ "edi", info->ContextRecord->Edi },
				{ "esi", info->ContextRecord->Esi },
				{ "ebp", info->ContextRecord->Ebp },
				{ "esp", info->ContextRecord->Esp },
				{ "eip", info->ContextRecord->Eip },
			};

			for (const auto& [name, value] : registers) {
				output << fmt::format("{} | 0x{:08X} | ", name, value);
				if (const auto description = Stack::GetLineForObject(reinterpret_cast<void**>(value), 5); !description.empty()) {
					output << description;
				}
				output << '\n';
			}
		}
		catch (...) {
			output << "Failed to log registry.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::Stack {
	static std::map<UINT32, UINT8> memoize;
	static std::stringstream output;

	static bool GetStringForRTTIorPDB(void** object, std::string& buffer) {
		try {
			if (const auto name = PDB::GetClassNameFromRTTIorPDB(object); !name.empty()) {
				buffer += fmt::format("0x{:08X} ==> RTTI: {}", *reinterpret_cast<UINT32*>(object), name);
				return true;
			}
		}
		catch (...) {
		}
		return false;
	}

	static bool GetStringForLabel(void** object, std::string& buffer) {
		try {
			std::string labelName;
			std::string objectName;
			std::string description;
			if (GetStringForClassLabel(object, labelName, objectName, description)) {
				buffer += fmt::format("0x{:08X} ==> {}: {}: {}", *reinterpret_cast<UINT32*>(object), labelName, objectName, description);
				return true;
			}
			if (GetStringForRTTIorPDB(object, buffer)) {
				return true;
			}
			if (GetAsString(object, labelName, description)) {
				buffer += fmt::format("0x{:08X} ==> {}: \"{}\"", *reinterpret_cast<UINT32*>(object), labelName, description);
				return true;
			}
		}
		catch (...) {
		}
		return false;
	}

	static bool GetStringForLabelSEH(void** object, std::string& buffer) {
		__try {
			return GetStringForLabel(object, buffer);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return false;
		}
	}

	static bool ReadPointer(void** object, UINT32& value) {
		__try {
			value = *reinterpret_cast<UINT32*>(object);
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return false;
		}
	}

	std::string GetLineForObject(void** object, UINT32 depth) {
		if (!object) return "";

		std::string buffer;
		do {
			if (GetStringForLabelSEH(object, buffer)) {
				return buffer;
			}

			UINT32 dereference = 0;
			if (!ReadPointer(object, dereference) || !dereference) {
				return "";
			}
			buffer += fmt::format("0x{:08X} ==> ", dereference);
			object = reinterpret_cast<void**>(dereference);
			--depth;
		} while (depth);

		return "";
	}

	static UINT32 GetStackValue(UINT32* esp, UINT32 index) {
		__try {
			return esp[index];
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return 0;
		}
	}

	void Process(EXCEPTION_POINTERS* info) {
		try {
			output << fmt::format("  # | {:^10} | DEREFERENCE INFO", "Value") << '\n';
			const auto esp = reinterpret_cast<UINT32*>(info->ContextRecord->Esp);
			for (UINT32 index = 0; index < 0x100; ++index) {
				const auto value = GetStackValue(esp, index);
				const auto description = GetLineForObject(reinterpret_cast<void**>(value), 5);
				const bool memoized = memoize.contains(value);
				if (index <= 0x8 || (!description.empty() && !memoized)) {
					output << fmt::format(" {:2X} | 0x{:08X} | ", index, value);
					if (!memoized) {
						output << description;
						memoize.emplace(value, static_cast<UINT8>(index));
					}
					else {
						output << fmt::format("Identical to {:2X}", memoize[value]);
					}
					output << '\n';
				}
			}
		}
		catch (...) {
			output << "Failed to log stack.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}
