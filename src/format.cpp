#include <string>

#include "format.h"

using std::string;

// Helper function that ensures each time unit (s, m, h) is printed with two digits. For example 1h, 2m, 3s should be printed 01:02:03, not 1:2:3.
string Format::EnsureTwoDigits(string time_unit){
  return time_unit.insert(0, 2 - time_unit.size(), '0');
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hours_, minutes_, seconds_;
  hours_ = seconds / 3600;
  minutes_ = (seconds % 3600) / 60;
  seconds_ = seconds - hours_ * 3600 - minutes_ * 60;
  return EnsureTwoDigits(std::to_string(hours_))   + ":" + 
         EnsureTwoDigits(std::to_string(minutes_)) + ":" + 
         EnsureTwoDigits(std::to_string(seconds_));
}