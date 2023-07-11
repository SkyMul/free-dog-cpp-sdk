#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include "lowState.h"
#include "common.h"
#include "lowCmd.h"

using namespace FREE_DOG_SDK;

int main() {
    LowCmd lCmd;
    lCmd.motorCmd.FR_0.q = .212;
    lCmd.motorCmd.FR_0.tau = 31.2;
    lCmd.motorCmd.FR_0.Kp = 34.212;
    lCmd.motorCmd.FR_0.Kd = 5.832;
    lCmd.motorCmd.FR_0.dq = 2.3;

    lCmd.levelFlag = 3;

//    std::array<uint8_t , 27> a = lCmd.motorCmd.FR_0.getBytes();
    std::array<uint8_t, 614> a;
    lCmd.buildCmd(&a);
    for(uint8_t x : a) {
        std::cout << std::to_string(unsigned(x)) << " ";
    }
    std::cout << std::endl;

    return 0;
}
