/************************************************************************
               MemAccess.h - Copyright (c) 2008 The MWSE Project
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

template <class T>
class MemAccess
{
public:
	static inline T Get(uintptr_t address)
	{
		return *reinterpret_cast<T *>(address);
	}
	static inline void Set(uintptr_t address, T value)
	{
		*reinterpret_cast<T *>(address) = value;
	}
};
