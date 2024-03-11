//
// Created by Alienson on 4.3.2024..
//

#ifndef WINTER_UTIL_H
#define WINTER_UTIL_H

#endif //WINTER_UTIL_H

#include <string>
#include <chrono>

namespace wt{
    [[nodiscard]] inline std::string current_datetime(){
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::string timeStr = std::ctime(&end_time); //TODO optimize
        timeStr.pop_back();
        return timeStr;
    }
}

