#ifndef ENUMS_H
#define ENUMS_H

namespace FREE_DOG_SDK {

enum class MotorModeHigh {
    IDLE,
    FORCE_STAND,
    VEL_WALK,
    POS_WALK,
    PATH,
    STAND_DOWN,
    STAND_UP,
    DAMPING,
    RECOVERY,
    BACKFLIP,
    JUMPYAW,
    STRAIGHTHAND,
    DANCE1,
    DANCE2
};

enum class GaitType {
    IDLE,
    TROT,
    CLIMB_STAIR,
    TROT_OBSTACLE
};

enum class SpeedLevel {
    LOW_SPEED,
    MEDIUM_SPEED,
    HIGH_SPEED
};

enum class Motor {
    FR_0,
    FR_1,
    FR_2,
    FL_0,
    FL_1,
    FL_2,
    RR_0,
    RR_1,
    RR_2,
    RL_0,
    RL_1,
    RL_2
};

enum MotorModeLow {
    Damping = 0x00,
    Servo = 0x0A,
    Overheat = 0x08
};

}
#endif  // ENUMS_H

