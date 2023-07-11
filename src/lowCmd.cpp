#include "lowCmd.h"
#include "common.h"
#include <iostream>

namespace FREE_DOG_SDK {

LowCmd::LowCmd() {
    head = { 0xFE, 0xEF };
    levelFlag = 0xFF;
    frameReserve = 0;
    SN.fill(0);
    version.fill(0);
    bandWidth = 0x3A |  (uint16_t(0xC0) << 8);
    motorCmd = MotorCmdArray();
    wirelessRemote.fill(0);
    reserve = 0;
    crc = 0;
    encrypt = true;
}

void LowCmd::buildCmd(std::array<uint8_t, 614>* cmd, bool debug) {
    (*cmd).fill(0);

    (*cmd)[0] = head[0];
    (*cmd)[1] = head[1];
    (*cmd)[2] = levelFlag;
    (*cmd)[3] = frameReserve;
    
    std::memcpy(cmd->data() + 4, &SN, sizeof(SN));
    std::memcpy(cmd->data() + 12, version.data(), version.size());
    std::memcpy(cmd->data() + 20, &bandWidth, sizeof(bandWidth));

    std::array<uint8_t, 540> motor_cmd_bytes = motorCmd.getBytes();
    std::memcpy(cmd->data() + 22, motor_cmd_bytes.data(), motor_cmd_bytes.size());

    std::array<uint8_t, 4> bms_bytes = bms.getBytes();
    std::memcpy(cmd->data() + 562, bms_bytes.data(), bms_bytes.size());
    std::memcpy(cmd->data() + 566, wirelessRemote.data(), wirelessRemote.size());

    std::vector<uint8_t> crc = genCrc(std::vector<uint8_t>(cmd->begin(), cmd->end() - 6));

    if (encrypt) {
        std::vector<uint8_t> encrypted_crc = encryptCrc(crc);
        std::memcpy(cmd->data() + 610, encrypted_crc.data(), encrypted_crc.size());
    } else {
        std::memcpy(cmd->data() + 610, crc.data(), crc.size());
    }

    if (debug) {
        std::cout << "Length: " << cmd->size() << std::endl;
        std::cout << "Data: ";
        // byte_print(*cmd);
        std::cout << std::endl;
    }
}

}
