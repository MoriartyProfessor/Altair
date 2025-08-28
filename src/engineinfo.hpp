#ifndef ALTAIR_ENGINEINFO_HPP
#define ALTAIR_ENGINEINFO_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <format>

namespace EngineInfo {
    constexpr std::string_view AUTHOR = "Nickname";
    constexpr std::string_view NAME = "Altair";
    constexpr uint8_t MAJOR = 0;
    constexpr uint8_t MINOR = 1;
    constexpr uint8_t PATCH = 0;

    std::string engine_id() {
        return std::format("{} {}.{}.{}", NAME, MAJOR, MINOR, PATCH);
    }
}

#endif // ALTAIR_ENGINEINFO_HPP