#ifndef LOW_STATE_H
#define LOW_STATE_H
#pragma once

#include <array>
#include <cstdint>
#include "enums.h"
#include "complex.h"

namespace FREE_DOG_SDK {

class LowState {
public:
    LowState();
    bool parseData(const std::array<uint8_t, 820>& data);

    std::array<uint8_t, 2> head;
    uint8_t levelFlag;
    uint8_t frameReserve;
    std::array<uint32_t, 2> SN;
    std::array<uint32_t, 2> version;
    uint16_t bandWidth; 
    IMU imu;
    std::array<MotorState, 20> motorState;
    struct bmsState bms;
    std::array<uint16_t, 4> footForce;
    std::array<uint16_t, 4> footForceEst;
    
    uint32_t tick;
    
    std::array<uint8_t, 40> wirelessRemote;
   
    uint32_t reserve;
    
    uint32_t crc;
};

}

#endif // LOW_STATE_H
