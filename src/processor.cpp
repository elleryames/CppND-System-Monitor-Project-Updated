#include <vector>
#include <algorithm>
#include <numeric>
#include "processor.h"
#include "linux_parser.h"

using std::string;

// TODO: Return the aggregate CPU utilization
// Compute from info in /proc/stat (top line)
// Computation informed by:
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

float Processor::Utilization() { 
  long total, totIdle, nonIdle;
  float cpuUtil;

  // CPU Columns: cpuUser, cpuNice, cpuSystem, cpuIdle, cpuIOwait, 
  //              cpuIRQ, cpuSoftIRQ, cpuSteal, cpuGuest, cpuGuestNice
  std::vector<string> cpuTimes_string = LinuxParser::CpuUtilization();
  std::vector<long> cpuTimes(cpuTimes_string.size());

  // Populate cpuTimes
  std::transform(cpuTimes_string.begin(), cpuTimes_string.end(), 
                 cpuTimes.begin(), [](string s){return stol(s);});

  // Guest times are included in corresponding cpu times.
  cpuTimes[0] = cpuTimes[0] - cpuTimes[7];
  cpuTimes[1] = cpuTimes[1] - cpuTimes[8];

  // cpu is idle in states Idle(3) and IOwait(4)
  total = std::accumulate(cpuTimes.begin(), cpuTimes.end(), 0);
  totIdle = cpuTimes[3] + cpuTimes[4];
  nonIdle = total - totIdle;

  // Compute change in activity since last sampled.
  float dNonIdle = static_cast<float>(std::abs(nonIdle - prevNonIdle));
  float dTotal   = static_cast<float>(std::abs(total - prevTotal)); 
  cpuUtil = static_cast<float>(dNonIdle) / dTotal;

  // Update persisted variables
  prevNonIdle = nonIdle;
  prevTotal   = total;

  return cpuUtil; 
}

// TODO:
//  Processor::Processor(){
//         Compute nonIdle, total;
//         Save/initialize persisted values
// } 
//  void UpdatePrevValues(long nonIdle, long total)
//  int  ComputeNonIdle()
//  int  ComputeTotal()