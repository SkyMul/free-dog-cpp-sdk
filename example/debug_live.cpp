#include <array>
#include <iostream>
#include <string>
#include <vector>
#include "lowState.h"
#include "common.h"
#include "lowCmd.h"
#include "unitreeConnection.h"
#include <thread>

using namespace FREE_DOG_SDK;

int main() {

    UnitreeConnection conn(std::make_tuple(8010, "192.168.123.10", 8007, "192.168.123.14"));
    conn.startRecv();

    LowCmd lCmd;
    LowState lState;

    conn.send(&lCmd);

    int cycle = 0;

    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        std::vector<std::vector<uint8_t>> data = conn.getData();

        for(std::vector<uint8_t> packet : data) {
            if(packet.size() != 820) continue;
            std::array<uint8_t , 820> packet_arr;
            std::copy(packet.begin(), packet.begin() + 820, packet_arr.begin());

            lState.parseData(packet_arr);
            // Print every 100 cycles
            if(cycle % 100 == 0) {
                std::cout << "SN: " << decode_sn(lState.SN).first << " | " << decode_sn(lState.SN).second << std::endl;
                std::cout << "Hardware version: " << decode_version(lState.version).first << " | Software version: " << decode_version(lState.version).second << std::endl;
                std::cout << "Level Flag: " << unsigned(lState.levelFlag) << std::endl;
                std::cout << "SOC: " << unsigned(lState.bms.SOC) << " %" << std::endl;
                std::cout << "Voltage: " << unsigned(getVoltage(lState.bms.cell_vol)) << " mv" << std::endl;
                std::cout << "Current: " << std::to_string(lState.bms.current) << " mA" << std::endl;
                std::cout << "Cycles: " << unsigned(lState.bms.cycle) << std::endl;
                std::cout << "Temps BQ: " << unsigned(lState.bms.BQ_NTC[0]) << " °C, " << unsigned(lState.bms.BQ_NTC[1]) << " °C" << std::endl;
                std::cout << "Temps MCU: " << unsigned(lState.bms.MCU_NTC[0]) << " °C, " << unsigned(lState.bms.MCU_NTC[1]) << " °C" << std::endl;
                std::cout << "FootForce: [" << unsigned(lState.footForce[0]) << ", " << unsigned(lState.footForce[1]) << ", " << unsigned(lState.footForce[2]) << ", " << unsigned(lState.footForce[3]) << "]" << std::endl;
                std::cout << "FootForceEst: [" << unsigned(lState.footForceEst[0]) << ", " << unsigned(lState.footForceEst[1]) << ", " << unsigned(lState.footForceEst[2]) << ", " << unsigned(lState.footForceEst[3]) << "]" << std::endl;
                std::cout << "IMU Temp: " << unsigned(lState.imu.temperature) << std::endl;
                std::cout << "IMU Quaternion: [" << lState.imu.quaternion[0] << ", " << lState.imu.quaternion[1] << ", " << lState.imu.quaternion[2] << ", " << lState.imu.quaternion[3] << "]" << std::endl;
                std::cout << "IMU Gyroscope: [" << lState.imu.gyroscope[0] << ", " << lState.imu.gyroscope[1] << ", " << lState.imu.gyroscope[2] << "]" << std::endl;
                std::cout << "IMU Accelerometer: [" << lState.imu.accelerometer[0] << ", " << lState.imu.accelerometer[1] << ", " << lState.imu.accelerometer[2] << "]" << std::endl;
                std::cout << "IMU Roll/Pitch/Yaw: [" << lState.imu.rpy[0] << ", " << lState.imu.rpy[1] << ", " << lState.imu.rpy[2] << "]" << std::endl;

                for (int i = 0; i < 20; i++) {
                    std::cout << "Motor " << i + 1 << " Information:" << std::endl;
                    std::cout << "Mode: " << static_cast<int>(lState.motorState[i].mode) << std::endl;
                    std::cout << "q: " << lState.motorState[i].q << std::endl;
                    std::cout << "dq: " << lState.motorState[i].dq << std::endl;
                    std::cout << "ddq: " << lState.motorState[i].ddq << std::endl;
                    std::cout << "tauEst: " << lState.motorState[i].tauEst << std::endl;
                    std::cout << "q_raw: " << lState.motorState[i].q_raw << std::endl;
                    std::cout << "dq_raw: " << lState.motorState[i].dq_raw << std::endl;
                    std::cout << "ddq_raw: " << lState.motorState[i].ddq_raw << std::endl;
                    std::cout << "Temperature: " << unsigned(lState.motorState[i].temperature) << " °C" << std::endl;
                    std::cout << std::endl;
                }
            }
        }

        cycle++;
    }
   

    return 0;
}
