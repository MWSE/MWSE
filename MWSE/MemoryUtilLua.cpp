#include "MemoryUtilLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "MemAccess.h"
#include "MemoryUtil.h"
#include "Log.h"

#include "TES3GameFile.h"
#include "TES3Inventory.h"
#include "TES3ItemData.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MobileObject.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"

namespace mwse {
	namespace lua {
		sol::table convertFrom;
		sol::table convertTo;

		// Structure that keeps track of an overwritten C function, telling how we convert arguments before sending them to lua.
		class FunctionDefinition {
		public:
			bool thisCall = false;
			sol::protected_function thisCallConverter = sol::nil;
			bool fastCall = false;
			sol::protected_function fastCallConverter = sol::nil;
			std::vector<sol::protected_function> argumentConverters;
			sol::protected_function returnConverter = sol::nil;
		};

		// Utility map that matches a Morrowind.exe function (or calling address) to its FunctionDefinition.
		std::unordered_map<DWORD, FunctionDefinition> functionDefinitions;

		// Utility map that matches a calling address to the previous Morrowind.exe calling address.
		std::unordered_map<DWORD, DWORD> existingFunctionCalls;

		// Utility map that matches a calling address to the lua function that will be called instead.
		std::unordered_map<DWORD, sol::protected_function> luaFunctionOverrides;

		// Temporary container that holds the current argument list to be sent to the called lua function.
		std::vector<sol::object> luaFunctionArguments;

		// Fake types for when we can't bind the normal types.
		template <typename T>
		struct LegacyIteratedList {
			struct Node {
				Node* previous;
				Node* next;
				T data;
			};
			void* virtualTable;
			size_t count;
			Node* head;
			Node* tail;
			Node* current;
		};

		// Fills luaFunctionArguments based on given argX parameters and what converters it expects.
		FunctionDefinition * fillLuaCallArguments(DWORD callingAddress, DWORD functionAt, DWORD ecx, DWORD edx, DWORD arg0 = 0, DWORD arg1 = 0, DWORD arg2 = 0, DWORD arg3 = 0, DWORD arg4 = 0, DWORD arg5 = 0) {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			luaFunctionArguments.clear();

			// Figure out the definition based on the previous function address or calling address.
			auto definitionItt = functionDefinitions.find(functionAt);
			if (definitionItt == functionDefinitions.end()) {
				definitionItt = functionDefinitions.find(callingAddress);
				if (definitionItt == functionDefinitions.end()) {
					return nullptr;
				}
			}

			FunctionDefinition& definition = definitionItt->second;

			if (definition.thisCall) {
				luaFunctionArguments.push_back(definition.thisCallConverter(ecx));
			}

			if (definition.fastCall) {
				luaFunctionArguments.push_back(definition.fastCallConverter(edx));
			}

			auto argCount = definition.argumentConverters.size();
			if (argCount > 0) {
				luaFunctionArguments.push_back(definition.argumentConverters[0](arg0));
				if (argCount > 1) {
					luaFunctionArguments.push_back(definition.argumentConverters[1](arg1));
					if (argCount > 2) {
						luaFunctionArguments.push_back(definition.argumentConverters[2](arg2));
						if (argCount > 3) {
							luaFunctionArguments.push_back(definition.argumentConverters[3](arg3));
							if (argCount > 4) {
								luaFunctionArguments.push_back(definition.argumentConverters[4](arg4));
								if (argCount > 5) {
									luaFunctionArguments.push_back(definition.argumentConverters[5](arg5));
								}
							}
						}
					}
				}
			}

			return &definition;
		}

		// Actual dispatching function
		DWORD callGenericLuaFunctionFinal(DWORD callingAddress, FunctionDefinition * definition = nullptr) {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			auto function = luaFunctionOverrides[callingAddress];
			sol::protected_function_result result = function(sol::as_args(luaFunctionArguments));
			if (!result.valid()) {
				sol::error error = result;
				log::getLog() << "Lua error encountered in injected lua function:" << std::endl << error.what() << std::endl;
				return 0;
			}

			sol::object returnValue = result;
			if (definition && definition->returnConverter != sol::nil) {
				returnValue = definition->returnConverter(returnValue);
				return returnValue.as<DWORD>();
			}
			// Guess at the return type.
			else {
				if (returnValue.is<double>()) {
					// Fix this! Need some way of defining the return type.
					return (DWORD)returnValue.as<double>();
				}
				else if (returnValue.is<bool>()) {
					return (DWORD)returnValue.as<bool>();
				}
				else if (returnValue.is<void*>()) {
					return (DWORD)returnValue.as<void*>();
				}
				else {
					return 0;
				}
			}
		}

		// ret __stdcall/__thiscall/__fastcall(this, edx)
		DWORD __fastcall callGenericLuaFunction_fastcall_0arg(DWORD ecx, DWORD edx) {
			// Get the calling address.
			byte ** asmEBP;
			__asm { mov asmEBP, ebp };
			DWORD callingAddress = DWORD(asmEBP[1] - 0x5);

			auto definition = fillLuaCallArguments(callingAddress, existingFunctionCalls[callingAddress], ecx, edx);
			return callGenericLuaFunctionFinal(callingAddress, definition);
		}

		// ret __stdcall/__thiscall/__fastcall(this, edx, arg0)
		DWORD __fastcall callGenericLuaFunction_fastcall_1arg(DWORD ecx, DWORD edx, DWORD arg0) {
			// Get the calling address.
			byte ** asmEBP;
			__asm { mov asmEBP, ebp };
			DWORD callingAddress = DWORD(asmEBP[1] - 0x5);

			auto definition = fillLuaCallArguments(callingAddress, existingFunctionCalls[callingAddress], ecx, edx, arg0);
			return callGenericLuaFunctionFinal(callingAddress, definition);
		}

		// ret __stdcall/__thiscall/__fastcall(this, edx, arg0, arg1)
		DWORD __fastcall callGenericLuaFunction_fastcall_2arg(DWORD ecx, DWORD edx, DWORD arg0, DWORD arg1) {
			// Get the calling address.
			byte ** asmEBP;
			__asm { mov asmEBP, ebp };
			DWORD callingAddress = DWORD(asmEBP[1] - 0x5);

			auto definition = fillLuaCallArguments(callingAddress, existingFunctionCalls[callingAddress], ecx, edx, arg0, arg1);
			return callGenericLuaFunctionFinal(callingAddress, definition);
		}

		// ret __stdcall/__thiscall/__fastcall(this, edx, arg0, arg1, arg2)
		DWORD __fastcall callGenericLuaFunction_fastcall_3arg(DWORD ecx, DWORD edx, DWORD arg0, DWORD arg1, DWORD arg2) {
			// Get the calling address.
			byte ** asmEBP;
			__asm { mov asmEBP, ebp };
			DWORD callingAddress = DWORD(asmEBP[1] - 0x5);

			auto definition = fillLuaCallArguments(callingAddress, existingFunctionCalls[callingAddress], ecx, edx, arg0, arg1, arg2);
			return callGenericLuaFunctionFinal(callingAddress, definition);
		}

		// ret __stdcall/__thiscall/__fastcall(this, edx, arg0, arg1, arg2, arg3)
		DWORD __fastcall callGenericLuaFunction_fastcall_4arg(DWORD ecx, DWORD edx, DWORD arg0, DWORD arg1, DWORD arg2, DWORD arg3) {
			// Get the calling address.
			byte ** asmEBP;
			__asm { mov asmEBP, ebp };
			DWORD callingAddress = DWORD(asmEBP[1] - 0x5);

			auto definition = fillLuaCallArguments(callingAddress, existingFunctionCalls[callingAddress], ecx, edx, arg0, arg1, arg2, arg3);
			return callGenericLuaFunctionFinal(callingAddress, definition);
		}

		// ret __stdcall/__thiscall/__fastcall(this, edx, arg0, arg1, arg2, arg3, arg4)
		DWORD __fastcall callGenericLuaFunction_fastcall_5arg(DWORD ecx, DWORD edx, DWORD arg0, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4) {
			// Get the calling address.
			byte ** asmEBP;
			__asm { mov asmEBP, ebp };
			DWORD callingAddress = DWORD(asmEBP[1] - 0x5);

			auto definition = fillLuaCallArguments(callingAddress, existingFunctionCalls[callingAddress], ecx, edx, arg0, arg1, arg2, arg3, arg4);
			return callGenericLuaFunctionFinal(callingAddress, definition);
		}

		// ret __stdcall/__thiscall/__fastcall(this, edx, arg0, arg1, arg2, arg3, arg4, arg5)
		DWORD __fastcall callGenericLuaFunction_fastcall_6arg(DWORD ecx, DWORD edx, DWORD arg0, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5) {
			// Get the calling address.
			byte ** asmEBP;
			__asm { mov asmEBP, ebp };
			DWORD callingAddress = DWORD(asmEBP[1] - 0x5);

			auto definition = fillLuaCallArguments(callingAddress, existingFunctionCalls[callingAddress], ecx, edx, arg0, arg1, arg2, arg3, arg4, arg5);
			return callGenericLuaFunctionFinal(callingAddress, definition);
		}

		sol::object reinterpret(sol::table params, sol::this_state ts) {
			sol::optional<DWORD> value = params["value"];
			if (!value) {
				throw std::invalid_argument("Invalid 'value' parameter provided.");
			}

			sol::optional<std::string> as = params["as"];
			if (!as) {
				throw std::invalid_argument("Invalid 'as' parameter provided.");
			}

			sol::state_view state = ts;
			sol::protected_function converter = state["mwse"]["memory"]["convertTo"][as.value()];
			sol::protected_function_result result = converter(value.value());
			if (!result.valid()) {
				sol::error error = result;
				throw std::exception(error.what());
			}

			return result;
		}

		DWORD getCallAddress(sol::table params) {
			sol::optional<DWORD> address = params["address"];
			if (!address) {
				throw std::invalid_argument("Invalid 'address' parameter provided.");
			}

			return mwse::getCallAddress(address.value());
		}

		sol::object readValue(sol::table params, sol::this_state ts) {
			sol::optional<DWORD> address = params["address"];
			if (!address) {
				throw std::invalid_argument("Invalid 'address' parameter provided.");
			}

			sol::optional<std::string> as = params["as"];
			if (!as) {
				throw std::invalid_argument("Invalid 'as' parameter provided.");
			}

			sol::state_view state = ts;
			sol::protected_function converter = state["mwse"]["memory"]["convertTo"][as.value()];
			sol::protected_function_result result = converter(*reinterpret_cast<DWORD*>(address.value()));
			if (!result.valid()) {
				sol::error error = result;
				throw std::exception(error.what());
			}
			return result;
		}

		bool writeByte(sol::table params) {
			sol::optional<DWORD> address = params["address"];
			if (!address) {
				throw std::invalid_argument("Invalid 'address' parameter provided.");
			}

			sol::optional<BYTE> byte = params["byte"];
			if (!byte) {
				throw std::invalid_argument("Invalid 'byte' parameter provided.");
			}

			writeByteUnprotected(address.value(), byte.value());
			return true;
		}

		bool writeBytes(sol::table params) {
			sol::optional<DWORD> address = params["address"];
			if (!address) {
				throw std::invalid_argument("Invalid 'address' parameter provided.");
			}

			sol::optional<sol::table> bytes = params["bytes"];
			if (!bytes) {
				throw std::invalid_argument("Invalid 'bytes' parameter provided.");
			}

			size_t byteCount = bytes.value().size();
			BYTE* data = new BYTE[byteCount];
			for (size_t i = 0; i < byteCount; i++) {
				sol::object byte = bytes.value()[i + 1];
				if (byte.is<BYTE>()) {
					data[i] = byte.as<BYTE>();
				}
				else {
					data[i] = 0x90;
				}
			}

			writeBytesUnprotected(address.value(), data, byteCount);

			delete[] data;

			return true;
		}

		bool writeFunctionCall(sol::table params) {
			sol::optional<DWORD> address = params["address"];
			if (!address) {
				throw std::invalid_argument("Invalid 'address' parameter provided.");
			}

			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			auto& state = stateHandle.state;

			sol::optional<DWORD> previousCall = params["previousCall"];
			DWORD length = params["length"].get_or(5U);

			sol::object newCall = params["call"];
			if (newCall.is<DWORD>()) {
				if (previousCall) {
					return genCallEnforced(address.value(), previousCall.value(), newCall.as<DWORD>(), length);
				}
				else {
					genCallUnprotected(address.value(), newCall.as<DWORD>(), length);
					return true;
				}
			}
			else if (newCall.is<sol::protected_function>()) {
				// Backup what we used to call to.
				auto definitionAddress = mwse::getCallAddress(address.value());
				if (definitionAddress != NULL) {
					existingFunctionCalls[address.value()] = definitionAddress;
				}
				else {
					definitionAddress = address.value();
				}

				// Inject the right function call.
				sol::optional<sol::table> signature = params["signature"];
				if (signature) {
					// Setup function definition for destination.
					FunctionDefinition definition;
					sol::table argumentConverters = state["mwse"]["memory"]["convertTo"];

					sol::optional<std::string> thisConverter = signature.value()["this"];
					if (thisConverter) {
						definition.thisCall = true;
						definition.thisCallConverter = argumentConverters[thisConverter.value()];
					}

					sol::optional<std::string> fastConverter = signature.value()["edx"];
					if (fastConverter) {
						definition.fastCall = true;
						definition.fastCallConverter = argumentConverters[fastConverter.value()];
					}

					size_t argumentCount = 0;
					sol::optional<sol::table> arguments = signature.value()["arguments"];
					if (arguments) {
						argumentCount = arguments.value().size();
						for (size_t i = 0; i < argumentCount; i++) {
							definition.argumentConverters.push_back(argumentConverters[arguments.value()[i + 1]]);
						}
					}

					sol::optional<std::string> returnConverter = signature.value()["returns"];
					if (returnConverter) {
						sol::table returnConverters = state["mwse"]["memory"]["convertFrom"];
						definition.returnConverter = returnConverters[returnConverter.value()];
					}

					functionDefinitions[definitionAddress] = std::move(definition);

					// Figure out what alias function to call.
					DWORD overwritingFunction = 0x0;
					switch (argumentCount) {
					case 0:
						overwritingFunction = reinterpret_cast<DWORD>(callGenericLuaFunction_fastcall_0arg);
						break;
					case 1:
						overwritingFunction = reinterpret_cast<DWORD>(callGenericLuaFunction_fastcall_1arg);
						break;
					case 2:
						overwritingFunction = reinterpret_cast<DWORD>(callGenericLuaFunction_fastcall_2arg);
						break;
					case 3:
						overwritingFunction = reinterpret_cast<DWORD>(callGenericLuaFunction_fastcall_3arg);
						break;
					case 4:
						overwritingFunction = reinterpret_cast<DWORD>(callGenericLuaFunction_fastcall_4arg);
						break;
					case 5:
						overwritingFunction = reinterpret_cast<DWORD>(callGenericLuaFunction_fastcall_5arg);
						break;
					case 6:
						overwritingFunction = reinterpret_cast<DWORD>(callGenericLuaFunction_fastcall_6arg);
						break;
					default:
						throw std::invalid_argument("No overload could be mapped for the given argument count.");
					}

					genCallUnprotected(address.value(), overwritingFunction, length);
				}
				else {
					functionDefinitions[address.value()] = {};
					genCallUnprotected(address.value(), reinterpret_cast<DWORD>(callGenericLuaFunction_fastcall_0arg), length);
				}

				// Make sure we can look back up our lua function from this address.
				luaFunctionOverrides[address.value()] = newCall.as<sol::protected_function>();

				return true;
			}

			return false;
		}

		bool writeNoOperation(sol::table params) {
			sol::optional<DWORD> address = params["address"];
			if (!address) {
				throw std::invalid_argument("Invalid 'address' parameter provided.");
			}

			sol::optional<DWORD> length = params["length"];

			return genNOPUnprotected(address.value(), length.value_or(1U));
		}

		template <typename T>
		T convertArgTo(DWORD arg) {
			return *reinterpret_cast<T*>(&arg);
		}

		template <>
		bool convertArgTo(DWORD arg) {
			return arg != 0;
		}

		template <>
		unsigned int convertArgTo(DWORD arg) {
			return arg;
		}

		template <typename T>
		DWORD convertArgFrom(T arg) {
			return *reinterpret_cast<DWORD*>(&arg);
		};

		template <>
		DWORD convertArgFrom(bool arg) {
			return arg;
		}

		template <>
		DWORD convertArgFrom(unsigned int arg) {
			return arg;
		}

		void bindMWSEMemoryUtil() {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			sol::table mwse = state["mwse"];
			auto memory = mwse.create_named("memory");

			//
			// Memory reinterpretation functions.
			//

			memory["reinterpret"] = reinterpret;

			//
			// Read operations.
			//

			memory["readCallAddress"] = getCallAddress;
			memory["readValue"] = readValue;

			//
			// Write operations.
			//

			memory["writeByte"] = writeByte;
			memory["writeBytes"] = writeBytes;
			memory["writeFunctionCall"] = writeFunctionCall;
			memory["writeNoOperation"] = writeNoOperation;

			//
			// Converters for various types of arguments.
			//

			memory["reinterpret"] = reinterpret;

			convertTo = memory.create_named("convertTo");
			convertTo["bool"] = convertArgTo<bool>;
			convertTo["byte"] = convertArgTo<BYTE>;
			convertTo["char"] = convertArgTo<char>;
			convertTo["float"] = convertArgTo<float>;
			convertTo["int"] = convertArgTo<int>;
			convertTo["niObject"] = convertArgTo<NI::Object*>;
			convertTo["string"] = convertArgTo<const char*>;
			convertTo["tes3equipmentStackIterator"] = convertArgTo<LegacyIteratedList<TES3::EquipmentStack*>*>;
			convertTo["tes3equipmentStackIteratorNode"] = convertArgTo<LegacyIteratedList<TES3::EquipmentStack*>::Node*>;
			convertTo["tes3gameFile"] = convertArgTo<TES3::GameFile*>;
			convertTo["tes3inventory"] = convertArgTo<TES3::Inventory*>;
			convertTo["tes3inventoryTile"] = convertArgTo<TES3::UI::InventoryTile*>;
			convertTo["tes3itemData"] = convertArgTo<TES3::ItemData*>;
			convertTo["tes3itemStack"] = convertArgTo<TES3::ItemStack*>;
			convertTo["tes3magicEffectInstance"] = convertArgTo<TES3::MagicEffectInstance*>;
			convertTo["tes3mobileObject"] = convertArgTo<TES3::MobileObject*>;
			convertTo["tes3object"] = convertArgTo<TES3::BaseObject*>;
			convertTo["tes3uiElement"] = convertArgTo<TES3::UI::Element*>;
			convertTo["uint"] = convertArgTo<DWORD>;

			convertFrom = memory.create_named("convertFrom");
			convertFrom["bool"] = convertArgFrom<bool>;
			convertFrom["byte"] = convertArgFrom<byte>;
			convertFrom["char"] = convertArgFrom<char>;
			convertFrom["float"] = convertArgFrom<float>;
			convertFrom["int"] = convertArgFrom<int>;
			convertFrom["niObject"] = convertArgFrom<NI::Object*>;
			convertFrom["string"] = convertArgFrom<const char*>;
			convertFrom["tes3equipmentStackIterator"] = convertArgFrom<LegacyIteratedList<TES3::EquipmentStack*>*>;
			convertFrom["tes3equipmentStackIteratorNode"] = convertArgFrom<LegacyIteratedList<TES3::EquipmentStack*>::Node*>;
			convertFrom["tes3gameFile"] = convertArgFrom<TES3::GameFile*>;
			convertFrom["tes3inventory"] = convertArgFrom<TES3::Inventory*>;
			convertFrom["tes3inventoryTile"] = convertArgFrom<TES3::UI::InventoryTile*>;
			convertFrom["tes3itemData"] = convertArgFrom<TES3::ItemData*>;
			convertFrom["tes3itemStack"] = convertArgFrom<TES3::ItemStack*>;
			convertFrom["tes3magicEffectInstance"] = convertArgFrom<TES3::MagicEffectInstance*>;
			convertFrom["tes3mobileObject"] = convertArgFrom<TES3::MobileObject*>;
			convertFrom["tes3object"] = convertArgFrom<TES3::BaseObject*>;
			convertFrom["tes3uiElement"] = convertArgFrom<TES3::UI::Element*>;
			convertFrom["uint"] = convertArgFrom<unsigned int>;

			//
			// Special lower level usertype binding.
			//

			// TES3::IteratedList<TES3::EquipmentStack*>
			{
				auto usertypeDefinition = state.new_usertype<LegacyIteratedList<TES3::EquipmentStack*>>("tes3equipmentStackIterator");
				usertypeDefinition["current"] = &LegacyIteratedList<TES3::EquipmentStack*>::current;
				usertypeDefinition["count"] = &LegacyIteratedList<TES3::EquipmentStack*>::count;
				usertypeDefinition["head"] = &LegacyIteratedList<TES3::EquipmentStack*>::head;
				usertypeDefinition["tail"] = &LegacyIteratedList<TES3::EquipmentStack*>::tail;
			}

			// TES3::IteratedList<TES3::EquipmentStack*>::Node
			{
				auto usertypeDefinition = state.new_usertype<LegacyIteratedList<TES3::EquipmentStack*>::Node>("tes3equipmentStackIteratorNode");
				usertypeDefinition["nodeData"] = &LegacyIteratedList<TES3::EquipmentStack*>::Node::data;
				usertypeDefinition["nextNode"] = &LegacyIteratedList<TES3::EquipmentStack*>::Node::next;
				usertypeDefinition["previousNode"] = &LegacyIteratedList<TES3::EquipmentStack*>::Node::previous;
			}

		}
	}
}
