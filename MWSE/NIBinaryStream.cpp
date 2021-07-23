#include "NIBinaryStream.h"

namespace NI {
    unsigned int BinaryStream::read(void* data, unsigned int bytes) {
        return vTable->read(this, data, bytes);
    }

    unsigned int BinaryStream::write(const void* data, unsigned int bytes) {
        return vTable->write(this, data, bytes);
    }
}
