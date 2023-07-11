#include "common.h"
#include <array>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <map>
#include <unordered_map>

namespace FREE_DOG_SDK {

std::string lib_version() {
    return "0.2";
}

std::pair<std::string, std::string> decode_sn(const std::array<uint32_t, 2>& data) {
    std::string type_name = "UNKNOWN";
    std::string model_name = "UNKNOWN";

    uint8_t type = data[0] & 0xFF;
    switch (type) {
        case 1:
            type_name = "Laikago";
            break;
        case 2:
            type_name = "Aliengo";
            break;
        case 3:
            type_name = "A1";
            break;
        case 4:
            type_name = "Go1";
            break;
        case 5:
            type_name = "B1";
            break;
    }

    uint8_t model = (data[0] >> 8) & 0xFF;
    switch (model) {
        case 1:
            model_name = "AIR";
            break;
        case 2:
            model_name = "PRO";
            break;
        case 3:
            model_name = "EDU";
            break;
        case 4:
            model_name = "PC";
            break;
        case 5:
            model_name = "XX";
            break;
    }

    std::string product_name = type_name + "_" + model_name;
    std::string id = std::to_string((data[0] >> 16) & 0xFF) + "-" + std::to_string((data[0] >> 24) & 0xFF) + "-" +
                     std::to_string(data[1] & 0xFF) + "[" + std::to_string((data[1] >> 8) & 0xFF) + "]";

    return std::make_pair(product_name, id);
}

std::pair<std::string, std::string> decode_version(const std::array<uint32_t, 2>& data) {
    std::string hardware_version =
        std::to_string(data[0] & 0xFF) + "." +
        std::to_string((data[0] >> 8) & 0xFF) + "." +
        std::to_string((data[0] >> 16) & 0xFF);

    std::string software_version =
        std::to_string((data[1] >> 8) & 0xFF) + "." +
        std::to_string((data[1] >> 16) & 0xFF) + "." +
        std::to_string((data[1] >> 24) & 0xFF);

    return std::make_pair(hardware_version, software_version);
}

uint16_t getVoltage(const std::array<uint16_t, 10>& cellVoltages) {
    uint16_t sum = 0;
    for (uint16_t voltage : cellVoltages) {
        sum += voltage;
    }
    return sum;
}

std::array<uint8_t, 4> float_to_hex(float f) {
    std::array<uint8_t, 4> bytes;
    std::memcpy(bytes.data(), &f, sizeof(float));
    return bytes;
}

float hex_to_float(const std::vector<uint8_t>& hex) {
    if (hex.size() != 4) {
        throw std::invalid_argument("Invalid hex array size. Expected 4 bytes.");
    }
    uint32_t value = (hex[3] << 24) | (hex[2] << 16) | (hex[1] << 8) | hex[0];
    float f;
    std::memcpy(&f, &value, sizeof(f));
    return f;
}

float signed_hex_to_float(const std::vector<uint8_t>& hex) {
    if (hex.size() == 2) {
        int16_t val;
        std::memcpy(&val, hex.data(), sizeof(int16_t));
        return static_cast<float>(val);
    } else {
        throw std::invalid_argument("Invalid hex array size. Expected 2 bytes.");
    }
}

int32_t signed_hex_to_int32(const std::vector<uint8_t>& hex) {
    if (hex.size() == 4) {
        int32_t val = (hex[0]) + (hex[1] << 8) + (hex[2] << 16) + (hex[3] << 24);
        return val;
    } else {
        throw std::invalid_argument("Invalid hex array size. Expected 2 or 4 bytes.");
    }
}

uint8_t fraction_to_hex(float fraction, bool neg) {
    if (fraction == 0.0f) {
        neg = false;
    }
    uint8_t hex_value = static_cast<uint8_t>(fraction * 256);
    if (neg) {
        hex_value = 255 + hex_value + 1;
    }
    return hex_value;
}

float hex_to_fraction(uint8_t hex_byte, bool neg) {
    if (neg) {
        return -1.0f + (static_cast<float>(hex_byte) / 256.0f);
    } else {
        return static_cast<float>(hex_byte) / 256.0f;
    }
}

std::array<uint8_t, 2> tau_to_hex(float tau) {
    tau = roundf(tau * 100.0f) / 100.0f;
    int integer_part = static_cast<int>(tau);
    float fractional_part = tau - integer_part;
    bool neg = false;
    if (tau < 0) {
        neg = true;
        integer_part = 255 + integer_part;
    }
    uint8_t fractional_hex = fraction_to_hex(fractional_part, neg);
    std::array<uint8_t, 2> arr;
    arr[0]  = fractional_hex;
    arr[1] = integer_part;
    return arr;
}

float hex_to_tau(const std::vector<uint8_t>& hex_bytes) {
    if (hex_bytes.size() != 1) {
        throw std::invalid_argument("Invalid hex array size. Expected 1 byte.");
    }
    uint8_t value = hex_bytes[0];
    bool neg = (value & 0x80) != 0;
    int integer_part = value & 0x7F;
    if (neg) {
        integer_part = -128 + integer_part;
    }
    return static_cast<float>(integer_part) + hex_to_fraction(value & 0x0F, neg);
}

std::array<uint8_t, 4> uint32_to_hex(uint32_t val) {
    std::array<uint8_t, 4> hexArr;

    hexArr[0] = (val >> 24) & 0xFF;
    hexArr[1] = (val >> 16) & 0xFF;
    hexArr[2] = (val >> 8) & 0xFF;
    hexArr[3] = val & 0xFF;

    return hexArr;
}

std::array<uint8_t, 2> kp_to_hex(float Kp) {
    int base = static_cast<int>(Kp);
    int frac = static_cast<int>(std::round((Kp - base) * 10));

    int val = 0;
    if (frac < 5) {
        val = (base * 32) + (frac * 3);
    } else {
        val = (base * 32) + ((frac - 1) * 3) + 4;
    }

    val &= 0xFFFF;

    std::array<uint8_t, 2> kp;
    kp[0] = static_cast<uint8_t>(val & 0xFF);
    kp[1] = static_cast<uint8_t>((val >> 8) & 0xFF);

    return kp;
}

std::array<uint8_t, 2> kd_to_hex(float decimal) {
    std::unordered_map<float, std::string> mapping = {
            {0.0, "0"}, {0.1, "1"}, {0.2, "3"}, {0.3, "4"}, {0.4, "6"},
            {0.5, "8"}, {0.6, "9"}, {0.7, "b"}, {0.8, "c"}, {0.9, "e"}
    };

    int integer_part = static_cast<int>(decimal);
    float fractional_part = roundf((decimal - integer_part) * 10) / 10;

    std::string hex_fractional_part = mapping.count(fractional_part) ? mapping[fractional_part] : "0";
    char hex_integer_part[4];
    sprintf(hex_integer_part, "%03x", integer_part);

    std::string kd_str = hex_integer_part + hex_fractional_part;
    std::array<uint8_t, 2> kd;
    kd[0] = std::stoi(kd_str.substr(2, 2), nullptr, 16);
    kd[1] = std::stoi(kd_str.substr(0, 2), nullptr, 16);

    return kd;
}

std::vector<uint8_t> genCrc(const std::vector<uint8_t>& data) {
    uint32_t crc = 0xFFFFFFFF;
    for (std::size_t i = 0; i < data.size(); i += 4) {
        uint32_t j = *(reinterpret_cast<const uint32_t*>(&data[i]));
        for (int b = 0; b < 32; ++b) {
            uint32_t x = (crc >> 31) & 1;
            crc <<= 1;
            crc &= 0xFFFFFFFF;
            if (x ^ (1 & (j >> (31 - b)))) {
                crc ^= 0x04c11db7;
            }
        }
    }
    std::vector<uint8_t> crc_bytes(sizeof(crc));
    std::memcpy(crc_bytes.data(), &crc, sizeof(crc));
    return crc_bytes;
}

std::vector<uint8_t> encryptCrc(const std::vector<uint8_t>& crc_val) {
    if (crc_val.size() != 4) {
        throw std::invalid_argument("Invalid CRC value size. Expected 4 bytes.");
    }
    uint32_t crc = *(reinterpret_cast<const uint32_t*>(crc_val.data()));
    uint32_t xor_val = 0xedcab9de;
    uint32_t val = crc ^ xor_val;
    val = ((val & 0xFF000000) >> 24) | ((val & 0x00FF0000) >> 8) |
          ((val & 0x0000FF00) << 8) | ((val & 0x000000FF) << 24);
    std::vector<uint8_t> tmp(sizeof(val));
    std::memcpy(tmp.data(), &val, sizeof(val));
    std::vector<uint8_t> encrypted_crc(sizeof(val));
    encrypted_crc[0] = tmp[2];
    encrypted_crc[1] = tmp[1];
    encrypted_crc[2] = tmp[0];
    encrypted_crc[3] = tmp[3];
    return encrypted_crc;
}

std::string byte_print(const std::vector<uint8_t>& bytes) {
    std::stringstream ss;
    for (uint8_t byte : bytes) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

}
