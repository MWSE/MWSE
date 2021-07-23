/************************************************************************
               Log.h - Copyright (c) 2008 The MWSE Project
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

namespace mwse
{
	namespace log
	{
		void OpenLog(const char *path);
		void CloseLog();

		std::ostream& getLog();
		std::ostream& getDebug(); //outputs to OutputDebugString
		
        void prettyDump(const void* data, const size_t length);

        template<class T> void prettyDump(const T* data) {
            prettyDump(data, sizeof(T));
        };
	}
};
