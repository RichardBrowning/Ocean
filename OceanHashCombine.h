//source: https://stackoverflow.com/questions/2590677
#pragma once
#include <functional>

namespace ocean{
    template <typename T, typename... Rest>

    void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
        /**no matching function for call to ‘std::hash<glm::vec<3, float, glm::packed_highp> >::hash(const glm::vec<3, float, glm::packed_highp>&)’*/
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        (hashCombine(seed, rest), ...);
    };
}