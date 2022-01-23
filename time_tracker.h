#ifndef FASTCPP_TIME_TRACKER_H
#define FASTCPP_TIME_TRACKER_H

#include <chrono>

namespace mytt{
    using clock = std::chrono::steady_clock;
    using TP = std::chrono::time_point<std::chrono::steady_clock>;

    [[nodiscard]]
    TP start(){
        return clock::now();
    }

    double stop(TP start_tp){
        auto end = clock::now();
        std::chrono::duration<double> diff = end - start_tp;
        return diff.count();
    }


}

#endif //FASTCPP_TIME_TRACKER_H
