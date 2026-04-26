#ifndef GWAIHIR_ENGINEINFO_HPP
#define GWAIHIR_ENGINEINFO_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <format>

namespace EngineInfo {
    constexpr std::string_view AUTHOR = "Nickname";
    constexpr std::string_view NAME = "Gwaihir";
    constexpr uint8_t MAJOR = 0;
    constexpr uint8_t MINOR = 4;
    constexpr uint8_t PATCH = 2;

    std::string engine_id() {
        return std::format("{}-{}.{}.{}", NAME, MAJOR, MINOR, PATCH);
    }
}

#endif // GWAIHIR_ENGINEINFO_HPP