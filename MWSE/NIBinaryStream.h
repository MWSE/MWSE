#pragma once

namespace NI {
    struct BinaryStream {
        struct VirtualTable {
            void(__thiscall* destructor)(BinaryStream*, bool); // 0x0
            bool(__thiscall* asBool)(BinaryStream*); // 0x4
            unsigned int(__thiscall* read)(BinaryStream*, void*, unsigned int); // 0x8
            unsigned int(__thiscall* write)(BinaryStream*, const void*, unsigned int); // 0xC
        };

        VirtualTable* vTable;

        //
        // Virtual table wrapping functions.
        //

        unsigned int read(void* data, unsigned int bytes);
        unsigned int write(const void* data, unsigned int bytes);
    };
}
