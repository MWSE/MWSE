#pragma once

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

		template <typename T>
		requires std::is_trivially_copyable_v<T>
		T readValue(std::string_view fileName) {
			T result{};
			if (read(fileName, &result, sizeof(T)) != sizeof(T)) {
				throw std::exception("Invalid size read.");
			}
			return result;
		}

		std::string readString(std::string_view fileName, bool stopAtEndOfLine);

		template <typename T>
		requires std::is_trivially_copyable_v<T>
		void writeValue(std::string_view fileName, T value) {
			write(fileName, &value, sizeof(T));
		}

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
