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

		HANDLE getFile(std::string_view fileName);

		short readShort(std::string_view fileName);
		long readLong(std::string_view fileName);
		float readFloat(std::string_view fileName);
		std::string readString(std::string_view fileName, bool stopAtEndOfLine);

		void writeShort(std::string_view fileName, const short value);
		void writeLong(std::string_view fileName, const long value);
		void writeFloat(std::string_view fileName, const float value);
		void writeString(std::string_view fileName, std::string_view value, bool suppressNull = false);

		bool seek(std::string_view fileName, long absolute);

	private:
		FileSystem();

		HANDLE openFileAt(std::string_view fileName, size_t position);

		bool validFileName(std::string_view fileName);

		int read(std::string_view fileName, void* data, size_t size);

		int write(std::string_view fileName, const void* data, size_t size);

		static FileSystem singleton;

		mwseFileMap_t fileMap;
	};
};
