
#ifndef RECEIVER_H
#define RECEIVER_H

#include <vector>
#include <stdint.h>

namespace Communication {
    class Receiver {
    public:
        Receiver() = default;
        virtual ~Receiver() = default;

        virtual void receive(int*, std::vector<uint8_t>*) = 0;
    };
}

#endif RECEIVER_H
