#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
  std::string EnsureTwoDigits(std::string time_unit);
  std::string ElapsedTime(long times);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif