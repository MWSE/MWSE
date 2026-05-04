#pragma once

#ifndef NELEM
#define NELEM(a) (sizeof(a)/sizeof(a[0]))
#endif

namespace mwse {

	struct mwseFileState_t {
		HANDLE file;
		size_t position;
	};

	typedef std::map<std::string, mwseFileState_t> mwseFileMap_t;

	class FileSystem {
	public:
		static FileSystem& getInstance() { return singleton; };

		HANDLE getFile(const char* fileName);

		short readShort(const char* fileName);
		long readLong(const char* fileName);
		float readFloat(const char* fileName);
		std::string readString(const char* fileName, bool stopAtEndOfLine);

		void writeShort(const char* fileName, const short value);
		void writeLong(const char* fileName, const long value);
		void writeFloat(const char* fileName, const float value);
		void writeString(const char* fileName, std::string_view value, bool suppressNull = false);

		bool seek(const char* fileName, long absolute);

	private:
		FileSystem();

		HANDLE openFileAt(const char* fileName, size_t position);

		bool validFileName(const char* fileName);

		int read(const char* fileName, void* data, size_t size);

		int write(const char* fileName, const void* data, size_t size);

		static FileSystem singleton;

		mwseFileMap_t fileMap;
	};
};
