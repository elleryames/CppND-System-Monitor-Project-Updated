#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours_, minutes_, seconds_;
    hours_ = seconds / 3600;
    minutes_ = (seconds % 3600) / 60;
    seconds_ = seconds - hours_ * 3600 - minutes_ * 60;
    return std::to_string(hours_) + ":" + std::to_string(minutes_) + ":" + std::to_string(seconds_); 
}