#include "LuaExecutor.h"

#include "LuaManager.h"

namespace mwse::lua {
	LuaExecutor::LuaExecutor(const char* _command) :
		handleStdOutRead(NULL),
		handleStdOutWrite(NULL),
		handleCreatedProcess(NULL),
		handleCreatedThread(NULL),
		command(_command),
		errorCode(0)
	{

	}

	LuaExecutor::~LuaExecutor() {
		if (handleCreatedProcess) {
			CloseHandle(handleCreatedProcess);
		}
		if (handleCreatedThread) {
			CloseHandle(handleCreatedThread);
		}

		// TODO: Pipe handles aren't closed. When trying to close them, something hangs. No idea why...

		if (backgroundAliveFuture.valid()) {
			backgroundAliveFuture.wait();
		}
	}

	HRESULT LuaExecutor::start() {
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		SECURITY_ATTRIBUTES saAttr;
		ZeroMemory(&saAttr, sizeof(saAttr));

		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		// Create our pipes for reading output.
		if (!CreatePipe(&handleStdOutRead, &handleStdOutWrite, &saAttr, 0)) {
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// Ensure the read handle to the pipe for STDOUT is not inherited.
		if (!SetHandleInformation(handleStdOutRead, HANDLE_FLAG_INHERIT, 0)) {
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// Start the child process. 
		si.cb = sizeof(si);
		si.hStdError = handleStdOutWrite;
		si.hStdOutput = handleStdOutWrite;
		si.dwFlags |= STARTF_USESTDHANDLES;
		if (!CreateProcessA(NULL, (TCHAR*)command.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
			errorCode = GetLastError();
			return HRESULT_FROM_WIN32(errorCode);
		}
		else {
			// Create the thread and store some info for later reference/closing.
			CreateThread(0, 0, readDataFromExtProgram, this, 0, NULL);
			handleCreatedProcess = pi.hProcess;
			handleCreatedThread = pi.hThread;

			// Ensure that this isn't destroyed until the thread exits.
			backgroundAliveFuture = std::async(std::launch::async, &keepSharedPointerAliveUntilExit, shared_from_this());

			return S_OK;
		}
	}

	bool LuaExecutor::isValid() const {
		return handleCreatedProcess;
	}

	sol::optional<DWORD> LuaExecutor::getExitCode() const {
		DWORD exitCode;
		if (!GetExitCodeProcess(handleCreatedProcess, &exitCode)) {
			std::stringstream ss;
			ss << "Failure in GetExitCodeProcess: " << GetLastError();
			throw std::runtime_error(std::move(ss.str()));
		}

		if (exitCode == STILL_ACTIVE) {
			return {};
		}

		return exitCode;
	}

	DWORD LuaExecutor::getErrorCode() const {
		return errorCode;
	}

	void LuaExecutor::wait() const {
		WaitForSingleObject(handleCreatedProcess, INFINITE);
	}

	bool LuaExecutor::ready() const {
		DWORD exitCode;
		if (!GetExitCodeProcess(handleCreatedProcess, &exitCode)) {
			std::stringstream ss;
			ss << "Failure in GetExitCodeProcess: " << GetLastError();
			throw std::runtime_error(std::move(ss.str()));
		}
		return exitCode != STILL_ACTIVE;
	}

	std::string LuaExecutor::getCommand() const {
		return command;
	}

	std::string LuaExecutor::getOutput() {
		outputBuilderMutex.lock();
		auto result = std::move(outputBuilder.str());
		outputBuilder.clear();
		outputBuilderMutex.unlock();
		return std::move(result);
	}

	void LuaExecutor::defineLuaBindings() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<LuaExecutor>("mwseLuaExecutor");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["command"] = sol::readonly_property(&LuaExecutor::getCommand);
		usertypeDefinition["exitCode"] = sol::readonly_property(&LuaExecutor::getExitCode);
		usertypeDefinition["errorCode"] = sol::readonly_property(&LuaExecutor::getErrorCode);
		usertypeDefinition["ready"] = sol::readonly_property(&LuaExecutor::ready);

		// Basic function binding
		usertypeDefinition["getOutput"] = &LuaExecutor::getOutput;
		usertypeDefinition["wait"] = &LuaExecutor::wait;
	}

	void LuaExecutor::keepSharedPointerAliveUntilExit(std::shared_ptr<LuaExecutor> executor) {
		executor->wait();
	}

	DWORD __stdcall LuaExecutor::readDataFromExtProgram(void* argh) {
		auto executor = reinterpret_cast<LuaExecutor*>(argh);

		constexpr size_t readBufferSize = 4096;
		DWORD dwRead;
		CHAR chBuf[readBufferSize] = {};
		BOOL bSuccess = FALSE;

		for (;;)
		{
			bSuccess = ReadFile(executor->handleStdOutRead, chBuf, readBufferSize, &dwRead, NULL);
			if (!bSuccess || dwRead == 0) continue;

			executor->outputBuilderMutex.lock();
			executor->outputBuilder << chBuf;
			executor->outputBuilderMutex.unlock();

			if (!bSuccess) break;
		}
		return 0;
	}
}
