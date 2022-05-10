#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// File parsing keys
const std::string keyProcesses("processes");
const std::string keyRunningProcesses("procs_running");
const std::string keyMemTotal("MemTotal:");
const std::string keyMemFree("MemFree:");
const std::string keyMemBuffers("Buffers:");
const std::string keyMemCached("Cached:");
const std::string keyCpu("cpu");
const std::string keyOSPrettyName("PRETTY_NAME");
const std::string keyUID("Uid:");
const std::string keyProcMem("VmRSS:"); // Use VmRSS instead of VmSize. 

// System
float MemoryUtilization();
long int UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();
std::vector<std::string> CpuUtilization();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
long int ActiveTime(int pid);
const size_t numStatCols{52};

// Helper functions
template <typename T>
T keyToValue(std::string const& file_path, std::string const& key);
template <typename T>
std::vector<T> rowToVector(std::string const& file_path);
};  // namespace LinuxParser

#endif