#include "FileUtil.h"

#include "Log.h"

namespace mwse {
	FileSystem FileSystem::singleton;

	FileSystem::FileSystem() {}

	HANDLE FileSystem::getFile(const char* fileName) {
		HANDLE result = NULL;

		std::string str(fileName);
		mwseFileMap_t::iterator it = fileMap.find(str);
		if (it != fileMap.end()) {
			mwseFileState_t& state = it->second;
			result = state.file;
			if (result == INVALID_HANDLE_VALUE) {
				result = state.file = openFileAt(fileName, state.position);
			}
		}
		else {
			mwseFileState_t state = { openFileAt(fileName, 0), 0 };
			fileMap[fileName] = state;
			result = state.file;
		}

		return result;
	}

	short FileSystem::readShort(const char* fileName) {
		short result = 0;
		if (read(fileName, &result, sizeof(short)) != sizeof(short)) {
			throw std::exception("Invalid size read.");
		}
		return result;
	}

	long FileSystem::readLong(const char* fileName) {
		long result = 0;
		if (read(fileName, &result, sizeof(long)) != sizeof(long)) {
			throw std::exception("Invalid size read.");
		}
		return result;
	}

	float FileSystem::readFloat(const char* fileName) {
		float result = 0.0f;
		if (read(fileName, &result, sizeof(float)) != sizeof(float)) {
			throw std::exception("Invalid size read.");
		}
		return result;
	}

	std::string FileSystem::readString(const char* fileName, bool stopAtEndOfLine) {
		HANDLE file = getFile(fileName);

		// String buffer.
		std::string buffer;
		char readCharacter = 0;

		// Abort if we don't have a valid handle.
		if (file == INVALID_HANDLE_VALUE) {
			return buffer;
		}

		// Read until we hit EOF or read a \0 character.
		DWORD bytesRead = 0;
		while (true) {
			// Read a single byte into a buffer.
			ReadFile(file, &readCharacter, 1, &bytesRead, 0);

			// EOF, we're done.
			if (bytesRead == 0) {
				break;
			}

			// Line feed in EOL mode, we're done.
			if (readCharacter == '\r' && stopAtEndOfLine) {
				// We need to read one more byte to get past the following \n.
				ReadFile(file, &readCharacter, 1, &bytesRead, 0);
				if (readCharacter != '\n') {
					mwse::log::getLog() << "FileSystem::readString: Warning: EOL file read for '" << fileName << "' does not have a valid CRLF ending." << std::endl;
					SetFilePointer(file, -1, NULL, FILE_CURRENT);
				}
				break;
			}

			// Add support for non-CRLF line endings because of Wrye Mash forks.
			else if (readCharacter == '\n' && stopAtEndOfLine) {
				mwse::log::getLog() << "FileSystem::readString: Warning: EOL file read for '" << fileName << "' does not have a valid CRLF ending." << std::endl;
				break;
			}

			// End of string found, we're done.
			if (!readCharacter) {
				break;
			}

			// Valid character. Add it to the buffer.
			buffer.push_back(readCharacter);
		}

		return buffer;
	}

	void FileSystem::writeShort(const char* fileName, const short value) {
		write(fileName, &value, sizeof(short));
	}

	void FileSystem::writeLong(const char* fileName, const long value) {
		write(fileName, &value, sizeof(long));
	}

	void FileSystem::writeFloat(const char* fileName, const float value) {
		write(fileName, &value, sizeof(float));
	}

	void FileSystem::writeString(const char* fileName, const std::string& value, bool suppressNull) {
		size_t length = value.length();
		if (!suppressNull) {
			length++;
		}
		write(fileName, value.c_str(), length);
	}

	bool FileSystem::seek(const char* fileName, long position) {
		bool result = false;
		HANDLE file = getFile(fileName);
		if (file != INVALID_HANDLE_VALUE) {
			DWORD newPosition = SetFilePointer(file, position, 0, FILE_BEGIN);
			result = (newPosition == position);
		}
		return result;
	}
	HANDLE FileSystem::openFileAt(const char* fileName, size_t position) {
		if (!validFileName(fileName)) {
			return INVALID_HANDLE_VALUE;
		}

		char realName[BUFSIZ] = "Data Files\\MWSE\\";

		// Create the file storage area if it doesn't already exist
		CreateDirectoryA("Data Files\\MWSE", NULL);

		// Allow connection to named pipes one the local machine.
		if (*fileName == '|') {
			strcpy(realName, "\\\\.\\pipe\\MWSE");
			fileName++;
		}

		strncpy(&realName[strlen(realName)], fileName, NELEM(realName) - strlen(realName));
		HANDLE result = CreateFileA(
			realName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
			NULL
		);
		if (result != INVALID_HANDLE_VALUE && SetFilePointer(result, position, 0, FILE_BEGIN) < 0) {
			CloseHandle(result);
			result = INVALID_HANDLE_VALUE;
		}

		return result;
	}

	bool FileSystem::validFileName(const char* fileName) {
		// Allow for a named pipe (I'm not sure it's wise, but
		// it's requested enough.)
		if (*fileName == '|') {
			fileName++;
		}

		int len = 0;
		while (*fileName) {
			if (len++ >= 61) {
				return false;
			}
			// Allow _ and . in filenames but limit their length.
			else if (!isalnum(*fileName) && *fileName != '_' && *fileName != '.') {
				return false;
			}
			fileName++;
		}

		// By forcing at least 5 characters, we don't have to
		// worry about someone opening the .. or . files.
		// Files 4 characters and under might be special like
		// the CON, PRN, COM1, etc. DOS device files. 
		if (len < 5) {
			return false;
		}

		return true;
	}

	int FileSystem::read(const char* fileName, void* data, size_t size) {
		int result = 0;
		HANDLE file = getFile(fileName);
		if (file != INVALID_HANDLE_VALUE)
		{
			//Tp21 2006-06-21: Stop MWSE getting stuck if there's no data available to be read (original code from timeslip)
			if (*fileName == '|') { //check if it's a pipe
				DWORD toread;
				PeekNamedPipe(file, NULL, 0, NULL, &toread, NULL); //look if there is something to read
				if (!toread) return 0; //if not, return
			}

			DWORD bytesRead = 0;
			ReadFile(file, data, size, &bytesRead, 0);
			result = (int)bytesRead;
		}

		return result;
	}

	int FileSystem::write(const char* fileName, const void* data, size_t size) {
		int result = 0;
		HANDLE file = getFile(fileName);
		if (file != INVALID_HANDLE_VALUE) {
			DWORD red = 0;
			WriteFile(file, data, size, &red, 0);
			SetEndOfFile(file);
			result = (int)red;
		}
		return result;
	}
}