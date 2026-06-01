
#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <vector>
#include <stdint.h>

namespace Communication {
    class Transmitter {
    public:
        Transmitter() = default;
        virtual ~Transmitter() = default;

        virtual void sendData(int, std::vector<uint8_t>) = 0;
        virtual void sendRequest(int) = 0;
    };
}

#endif TRANSMITTER_H