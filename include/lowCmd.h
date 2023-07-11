#ifndef LOW_CMD_H
#define LOW_CMD_H
#pragma once

#include <array>
#include <cstdint>
#include "enums.h"
#include "complex.h"

namespace FREE_DOG_SDK {

class LowCmd {
public:
    LowCmd();
    void buildCmd(std::array<uint8_t, 614>* cmd, bool debug = false);
    std::array<uint8_t, 2> head;
    uint8_t levelFlag;
    uint8_t frameReserve;
    std::array<uint32_t, 2> SN;
    std::array<uint32_t, 2> version;
    
    uint16_t bandWidth;
    
    MotorCmdArray motorCmd;
    struct bmsCmd bms;
    std::array<uint8_t, 40> wirelessRemote;
    uint32_t reserve;
    uint32_t crc;
    bool encrypt;
};

}

#endif // LOW_CMD_H
