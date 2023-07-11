#ifndef COMMON_H
#define COMMON_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace FREE_DOG_SDK {

constexpr int HIGHLEVEL    = 0xee;
constexpr int LOWLEVEL     = 0xff;
constexpr int TRIGERLEVEL  = 0xf0;                                                         
constexpr double PosStopF  = (2.146E+9f);
constexpr double VelStopF  = (16000.0f);

std::string lib_version();
std::pair<std::string, std::string> decode_sn(const std::array<uint32_t, 2>& data);
std::pair<std::string, std::string> decode_version(const std::array<uint32_t, 2>& data);
uint16_t getVoltage(const std::array<uint16_t, 10>& cellVoltages);
std::array<uint8_t, 4> float_to_hex(float f);
float hex_to_float(const std::vector<uint8_t>& hex);
uint8_t fraction_to_hex(float fraction, bool neg = false);
float hex_to_fraction(uint8_t hex_byte, bool neg = false);
std::array<uint8_t, 4> uint32_to_hex(uint32_t val);
std::array<uint8_t, 2> tau_to_hex(float tau);
float signed_hex_to_float(const std::vector<uint8_t>& hex);
int32_t signed_hex_to_int32(const std::vector<uint8_t>& hex);
std::array<uint8_t, 2> kp_to_hex(float Kp);
std::array<uint8_t, 2> kd_to_hex(float decimal);
std::vector<uint8_t> genCrc(const std::vector<uint8_t>& data);
std::vector<uint8_t> encryptCrc(const std::vector<uint8_t>& crc_val);
std::string byte_print(const std::vector<uint8_t>& bytes);

}
#endif  // COMMON_H

