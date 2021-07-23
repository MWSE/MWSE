/************************************************************************

	FileUtil.h - Copyright (c) 2008 The MWSE Project
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
		void writeString(const char* fileName, const std::string& value, bool suppressNull = false);

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
