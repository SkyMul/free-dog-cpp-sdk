#include "lowState.h"

namespace FREE_DOG_SDK {

LowState::LowState() {
    head.fill(0);
    levelFlag = 0;
    frameReserve = 0;
    SN.fill(0);
    version.fill(0);
    bandWidth = 0;
    footForce.fill(0);
    footForceEst.fill(0);
    imu.quaternion.fill(0);
    imu.gyroscope.fill(0);
    imu.accelerometer.fill(0);
    imu.rpy.fill(0);
    imu.temperature = 0;
    tick = 0;
    wirelessRemote.fill(0);
    crc = 0;
    reserve = 0;
}

bool LowState::parseData(const std::array<uint8_t, 820>& data) {
    head[0] = data[0];
    head[1] = data[1];
    levelFlag = data[2];
    frameReserve = data[3];

    SN[0] = data[4] | ((uint32_t)data[5] << 8) | ((uint32_t)data[6] << 16) | ((uint32_t)data[7] << 24);
    SN[1] = data[8] | ((uint32_t)data[9] << 8) | ((uint32_t)data[10] << 16) | ((uint32_t)data[11] << 24);

    version[0] = data[12] | ((uint32_t)data[13] << 8) | ((uint32_t)data[14] << 16) | ((uint32_t)data[15] << 24);
    version[1] = data[16] | ((uint32_t)data[17] << 8) | ((uint32_t)data[18] << 16) | ((uint32_t)data[19] << 24);
    
    bandWidth = uint16_t(data[20]) | (uint16_t(data[21]) << 8);
    
    imu.quaternion[0] = hex_to_float(std::vector<uint8_t>(data.begin() + 22, data.begin() + 26));
    imu.quaternion[1] = hex_to_float(std::vector<uint8_t>(data.begin() + 26, data.begin() + 30));
    imu.quaternion[2] = hex_to_float(std::vector<uint8_t>(data.begin() + 30, data.begin() + 34));
    imu.quaternion[3] = hex_to_float(std::vector<uint8_t>(data.begin() + 34, data.begin() + 38));
    imu.gyroscope[0] = hex_to_float(std::vector<uint8_t>(data.begin() + 38, data.begin() + 42));
    imu.gyroscope[1] = hex_to_float(std::vector<uint8_t>(data.begin() + 42, data.begin() + 46));
    imu.gyroscope[2] = hex_to_float(std::vector<uint8_t>(data.begin() + 46, data.begin() + 50));
    imu.accelerometer[0] = hex_to_float(std::vector<uint8_t>(data.begin() + 50, data.begin() + 54));
    imu.accelerometer[1] = hex_to_float(std::vector<uint8_t>(data.begin() + 54, data.begin() + 58));
    imu.accelerometer[2] = hex_to_float(std::vector<uint8_t>(data.begin() + 58, data.begin() + 62));
    imu.rpy[0] = hex_to_float(std::vector<uint8_t>(data.begin() + 62, data.begin() + 66));
    imu.rpy[1] = hex_to_float(std::vector<uint8_t>(data.begin() + 66, data.begin() + 70));
    imu.rpy[2] = hex_to_float(std::vector<uint8_t>(data.begin() + 70, data.begin() + 74));
    imu.temperature = data[74];

    for (int i = 0; i < 20; ++i){
        motorState[i].mode = (MotorModeLow) data[75 + i * 32];
        motorState[i].q = hex_to_float(std::vector<uint8_t>(data.begin() + 76 + i * 32, data.begin() + 80 + i * 32));
        motorState[i].dq = hex_to_float(std::vector<uint8_t>(data.begin() + 80 + i * 32, data.begin() + 84 + i * 32));
        motorState[i].ddq = signed_hex_to_float(std::vector<uint8_t>(data.begin() + 84 + i * 32, data.begin() + 86 + i * 32));
        motorState[i].tauEst = signed_hex_to_float(std::vector<uint8_t>(data.begin() + 86 + i * 32, data.begin() + 88 + i * 32)) * 0.00390625f;
        motorState[i].q_raw = hex_to_float(std::vector<uint8_t>(data.begin() + 88 + i * 32, data.begin() + 92 + i * 32));
        motorState[i].dq_raw = hex_to_float(std::vector<uint8_t>(data.begin() + 92 + i * 32, data.begin() + 96 + i * 32));
        motorState[i].ddq_raw = signed_hex_to_float(std::vector<uint8_t>(data.begin() + 96 + i * 32, data.begin() + 98 + i * 32));
        motorState[i].temperature = data[99 + i * 32];
        motorState[i].reserve[0] = int32_t(data[99 + i * 32]) | (int32_t(data[100 + i * 32]) << 8) | (int32_t(data[101 + i * 32]) << 16) | (int32_t(data[102 + i * 32]) << 24);
        motorState[i].reserve[1] = int32_t(data[103 + i * 32]) | (int32_t(data[104 + i * 32]) << 8) | (int32_t(data[105 + i * 32]) << 16) | (int32_t(data[106 + i * 32]) << 24);
    }

    bms.version_h = data[715];
    bms.version_l = data[716];
    bms.bms_status = data[717];
    bms.SOC = data[718];
    bms.current = signed_hex_to_int32(std::vector<uint8_t>(data.begin() + 719, data.begin() + 723));    
    bms.cycle = int16_t(data[723]) | (int16_t(data[724]) << 8);
    bms.BQ_NTC[0] = data[725];
    bms.BQ_NTC[1] = data[726];
    bms.MCU_NTC[0] = data[727];
    bms.MCU_NTC[1] = data[728];
    bms.cell_vol[0] = data[729] * 32;
    bms.cell_vol[1] = data[730] * 32;
    bms.cell_vol[2] = data[731] * 32;
    bms.cell_vol[3] = data[732] * 32;
    bms.cell_vol[4] = data[733] * 32;
    bms.cell_vol[5] = data[734] * 32;
    bms.cell_vol[6] = data[735] * 32;
    bms.cell_vol[7] = data[736] * 32;
    bms.cell_vol[8] = data[737] * 32;
    bms.cell_vol[9] = data[738] * 32;

    footForce[0] = int16_t(data[739]) | (int16_t(data[740]) << 8);
    footForce[1] = int16_t(data[751]) | (int16_t(data[752]) << 8);
    footForce[2] = int16_t(data[753]) | (int16_t(data[754]) << 8);
    footForce[3] = int16_t(data[755]) | (int16_t(data[756]) << 8);

    footForceEst[0] = int16_t(data[747]) | (int16_t(data[748]) << 8);
    footForceEst[1] = int16_t(data[759]) | (int16_t(data[760]) << 8);
    footForceEst[2] = int16_t(data[761]) | (int16_t(data[762]) << 8);
    footForceEst[3] = int16_t(data[763]) | (int16_t(data[764]) << 8);
    
    tick = data[755] | ((uint32_t)data[756] << 8) | ((uint32_t)data[757] << 16) | ((uint32_t)data[758] << 24);

    std::copy(data.begin() + 759, data.begin() + 799, wirelessRemote.begin());
    
    reserve = data[799] | ((uint32_t)data[800] << 8) | ((uint32_t)data[801] << 16) | ((uint32_t)data[802] << 24);

    return true;
}

}
