#ifndef COMPLEX_H
#define COMPLEX_H

#include "enums.h"
#include "common.h"
#include <cstring>
#include <iostream>

namespace FREE_DOG_SDK {

struct bmsState {
    uint8_t version_h;
    uint8_t version_l;
    uint8_t bms_status;
    uint8_t SOC;
    int32_t current;
    uint16_t cycle;
    std::array<uint8_t, 2> BQ_NTC;
    std::array<uint8_t, 2> MCU_NTC;
    std::array<uint16_t, 10> cell_vol;
};

struct bmsCmd {
    uint8_t off;
    uint8_t reserve[3];

    bmsCmd() {
        off = 0;
        reserve[0] = 0;
        reserve[1] = 0;
        reserve[2] = 0;
    }

    std::array<uint8_t, 4> getBytes() {
        std::array<uint8_t, 4> data;
        data[0] = off;
        std::memcpy(&data[1], reserve, sizeof(reserve));
        return data;
    }

    bmsCmd* fromBytes(uint8_t* data) {
        off = data[0];
        std::memcpy(reserve, &data[1], sizeof(reserve));
        return this;
    }
};

struct MotorState {
    MotorModeLow mode;
    float q;
    float dq;
    float ddq;
    float tauEst;
    float q_raw;
    float dq_raw;
    float ddq_raw;
    uint8_t temperature;
    uint8_t reserve[2];
};


struct IMU {
    std::array<float, 4> quaternion;
    std::array<float, 3> gyroscope;
    std::array<float, 3> accelerometer;
    std::array<float, 3> rpy;
    int8_t temperature;
};


class MotorCmd {
public:
    MotorModeLow mode;
    float q;
    float dq;
    float tau;
    float Kp;
    float Kd;
    std::array<uint32_t, 3> reserve;

    MotorCmd(MotorModeLow mode = MotorModeLow::Servo, float q = 0, float dq = 0, float tau = 0, float Kp = 0, float Kd = 0)
        : mode(mode), q(q), dq(dq), tau(tau), Kp(Kp), Kd(Kd) {
        reserve.fill(0);
        
    }

    void update(float q, float dq, float tau, float Kp, float Kd) {
        this->q = q;
        this->dq = dq;
        this->tau = tau;
        this->Kp = Kp;
        this->Kd = Kd;
    }

    std::array<uint8_t, 27> getBytes() {
        std::array<uint8_t, 27> data;
        data[0] = static_cast<uint8_t>(mode);

        std::array<uint8_t, 4> q_hex = float_to_hex(q);
        std::copy(q_hex.begin(), q_hex.end(), data.begin()+1);

        std::array<uint8_t, 4> dq_hex = float_to_hex(dq);
        std::copy(dq_hex.begin(), dq_hex.end(), data.begin()+5);

        std::array<uint8_t, 2> tau_hex = tau_to_hex(tau);
        std::copy(tau_hex.begin(), tau_hex.end(), data.begin()+9);

        std::array<uint8_t, 2> kp_hex = kp_to_hex(Kp);
        std::copy(kp_hex.begin(), kp_hex.end(), data.begin()+11);

        std::array<uint8_t, 2> kd_hex = kd_to_hex(Kd);
        std::copy(kd_hex.begin(), kd_hex.end(), data.begin()+13);

        std::array<uint8_t, 4> reserve_0 = uint32_to_hex(reserve[0]);
        std::copy(reserve_0.begin(), reserve_0.end(), data.begin()+15);

        std::array<uint8_t, 4> reserve_1 = uint32_to_hex(reserve[1]);
        std::copy(reserve_1.begin(), reserve_1.end(), data.begin()+19);

        std::array<uint8_t, 4> reserve_2 = uint32_to_hex(reserve[2]);
        std::copy(reserve_2.begin(), reserve_2.end(), data.begin()+23);

        return data;
    }
};

enum MotorStateIndex {
    FR_0 = 0,
    FR_1 = 1,
    FR_2 = 2,
    FL_0 = 3,
    FL_1 = 4,
    FL_2 = 5,
    RR_0 = 6,
    RR_1 = 7,
    RR_2 = 8,
    RL_0 = 9,
    RL_1 = 10,
    RL_2 = 11,
    UNKNOWN_1 = 12,
    UNKNOWN_2 = 13,
    UNKNOWN_3 = 14,
    UNKNOWN_4 = 15,
    UNKNOWN_5 = 16,
    UNKNOWN_6 = 17,
    UNKNOWN_7 = 18,
    UNKNOWN_8 = 19,
};

class MotorCmdArray {
public:
    MotorCmd FR_0;
    MotorCmd FR_1;
    MotorCmd FR_2;
    MotorCmd FL_0;
    MotorCmd FL_1;
    MotorCmd FL_2;
    MotorCmd RR_0;
    MotorCmd RR_1;
    MotorCmd RR_2;
    MotorCmd RL_0;
    MotorCmd RL_1;
    MotorCmd RL_2;
    MotorCmd Unknown1;
    MotorCmd Unknown2;
    MotorCmd Unknown3;
    MotorCmd Unknown4;
    MotorCmd Unknown5;
    MotorCmd Unknown6;
    MotorCmd Unknown7;
    MotorCmd Unknown8;


    MotorCmdArray() {
        FR_0 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        FR_1 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        FR_2 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

        FL_0 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        FL_1 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        FL_2 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

        RR_0 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        RR_1 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        RR_2 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

        RL_0 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        RL_1 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        RL_2 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

        Unknown1 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        Unknown2 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        Unknown3 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        Unknown4 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        Unknown5 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        Unknown6 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        Unknown7 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        Unknown8 = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    }

    std::array<uint8_t, 540> getBytes() {
        std::array<uint8_t, 540> data;
        std::memcpy(data.data() + 0, FR_0.getBytes().data(), FR_0.getBytes().size());
        std::memcpy(data.data() + 27, FR_1.getBytes().data(), FR_1.getBytes().size());
        std::memcpy(data.data() + 54, FR_2.getBytes().data(), FR_2.getBytes().size());
        std::memcpy(data.data() + 81, FL_0.getBytes().data(), FL_0.getBytes().size());
        std::memcpy(data.data() + 108, FL_1.getBytes().data(), FL_1.getBytes().size());
        std::memcpy(data.data() + 135, FL_2.getBytes().data(), FL_2.getBytes().size());
        std::memcpy(data.data() + 162, RR_0.getBytes().data(), RR_0.getBytes().size());
        std::memcpy(data.data() + 189, RR_1.getBytes().data(), RR_1.getBytes().size());
        std::memcpy(data.data() + 216, RR_2.getBytes().data(), RR_2.getBytes().size());
        std::memcpy(data.data() + 243, RL_0.getBytes().data(), RL_0.getBytes().size());
        std::memcpy(data.data() + 270, RL_1.getBytes().data(), RL_1.getBytes().size());
        std::memcpy(data.data() + 297, RL_2.getBytes().data(), RL_2.getBytes().size());
        std::memcpy(data.data() + 324, Unknown1.getBytes().data(), Unknown1.getBytes().size());
        std::memcpy(data.data() + 351, Unknown2.getBytes().data(), Unknown2.getBytes().size());
        std::memcpy(data.data() + 378, Unknown3.getBytes().data(), Unknown3.getBytes().size());
        std::memcpy(data.data() + 405, Unknown4.getBytes().data(), Unknown4.getBytes().size());
        std::memcpy(data.data() + 432, Unknown5.getBytes().data(), Unknown5.getBytes().size());
        std::memcpy(data.data() + 459, Unknown6.getBytes().data(), Unknown6.getBytes().size());
        std::memcpy(data.data() + 486, Unknown7.getBytes().data(), Unknown7.getBytes().size());
        std::memcpy(data.data() + 513, Unknown8.getBytes().data(), Unknown8.getBytes().size());
        return data;
    }

    MotorCmd& operator[](std::size_t index) {
        switch (index) {
            case 0: return FR_0;
            case 1: return FR_1;
            case 2: return FR_2;
            case 3: return FL_0;
            case 4: return FL_1;
            case 5: return FL_2;
            case 6: return RR_0;
            case 7: return RR_1;
            case 8: return RR_2;
            case 9: return RL_0;
            case 10: return RL_1;
            case 11: return RL_2;
            case 12: return Unknown1;
            case 13: return Unknown2;
            case 14: return Unknown3;
            case 15: return Unknown4;
            case 16: return Unknown5;
            case 17: return Unknown6;
            case 18: return Unknown7;
            case 19: return Unknown8;
            default: throw std::out_of_range("Invalid index");
        }
    }

};

}
#endif // COMPLEX_H
