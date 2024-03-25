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

    [[nodiscard]] inline std::string urlDecode(std::string_view text){
        std::string escaped;
        for (auto i = text.begin(), nd = text.end(); i < nd; ++i){
            auto c = *i;
            switch(c){
                case '%':
                    if (i[1] && i[2]) {
                        char hs[]{ i[1], i[2] };
                        escaped += static_cast<char>(strtol(hs, nullptr, 16));
                        i += 2;
                    }
                    break;
                case '+':
                    escaped += ' ';
                    break;
                default:
                    escaped += c;
            }
        }
        return escaped;
    }

    template<typename T, typename U>
    vector<U*> vector_cast(vector<T*>* source){
        vector<U*> dest;
        dest.reserve(source->size());
        for (T* t : *source){
            dest.push_back((U*)t);
        }
        return dest;
    }

    template<typename T, typename U>
    vector<U*>* vector_ptr_cast(vector<T*>* source){
        auto* dest = new vector<U*>();
        dest->reserve(source->size());
        for (T* t : *source){
            dest->push_back((U*)t);
        }
        return dest;
    }

}

