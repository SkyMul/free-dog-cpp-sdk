#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include "lowState.h"
#include "common.h"
#include "lowCmd.h"
#include "unitreeConnection.h"
#include <thread>
#include <algorithm>
#include <math.h>

using namespace FREE_DOG_SDK;

float jointLinearInterpolation(float initPos, float targetPos, float rate) {
    rate = std::min(std::max(rate, 0.0f), 1.0f);
    float p = initPos * (1 - rate) + targetPos * rate;
    return p;
}


int main() {

    UnitreeConnection conn(std::make_tuple(8010, "192.168.123.10", 8007, "192.168.123.14"));
    conn.startRecv();

    LowCmd lCmd;
    LowState lState;

    float qInit[3];
    float qDes[3];
    float kP[3];
    float kD[3];

    float sin_mid_q[3];
    sin_mid_q[0] = 0.0;
    sin_mid_q[1] = 1.2;
    sin_mid_q[2] = -2.0;

    float dt = .002;
    float sin_freq_hz = 1;
    float sin_freq_rad = sin_freq_hz * 2 * 3.1415926535; 

    int rate_count = 0;
    int sin_count = 0;

    int cycle = 0;

    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        conn.send(&lCmd);

        std::vector<std::vector<uint8_t>> data = conn.getData();

        for(std::vector<uint8_t> packet : data) {
            if(packet.size() != 820) continue;
            std::array<uint8_t , 820> packet_arr;
            std::copy(packet.begin(), packet.begin() + 820, packet_arr.begin());

            lState.parseData(packet_arr);
            // Print every 100 cycles
            if(cycle % 100 == 0) {
                for (int i = 0; i < 1; i++) {
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

        if(cycle >= 0 && cycle < 10) { // get initial pos
            qInit[0] = lState.motorState[MotorStateIndex::FR_0].q;
            qInit[1] = lState.motorState[MotorStateIndex::FR_1].q;
            qInit[2] = lState.motorState[MotorStateIndex::FR_2].q;
        }
        else if(cycle >= 10 && cycle < 400) {  // move to origin point of a sin movement
            rate_count++;
            float rate = rate_count / 200.f;

            kP[0] = 5;
            kP[1] = 5;
            kP[2] = 5;

            kD[0] = 1;
            kD[1] = 1;
            kD[2] = 1;

            qDes[0] = jointLinearInterpolation(qInit[0], sin_mid_q[0], rate);
            qDes[1] = jointLinearInterpolation(qInit[1], sin_mid_q[1], rate);
            qDes[2] = jointLinearInterpolation(qInit[2], sin_mid_q[2], rate);
        } else { // do sin wave
            float t = dt * sin_count;
            
            float sin_joint1 = 0.6 * std::sin(t*sin_freq_rad);
            float sin_joint2 = -0.9 * std::sin(t*sin_freq_rad);

            qDes[0] = sin_mid_q[0];
            qDes[1] = sin_mid_q[1] + sin_joint1;
            qDes[2] = sin_mid_q[2] + sin_joint2;
            sin_count++;
        }

        lCmd.motorCmd.FR_0.update(qDes[0], 0, -0.65, kP[0], kD[0]);
        lCmd.motorCmd.FR_1.update(qDes[1], 0, 0, kP[1], kD[1]);
        lCmd.motorCmd.FR_2.update(qDes[2], 0, 0, kP[2], kD[2]);


        cycle++;
    }
   

    return 0;
}
