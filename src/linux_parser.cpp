#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>
#include <numeric>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T LinuxParser::keyToValue(string const& filename, string const& key) {
  string key_;
  string line_;
  T value_;
  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line_)){
        std::istringstream linestream(line_);
        linestream >> key_ >> value_;
        if (key_ == key) { break; }
    }
  }
  stream.close();
  return value_;
}

template <typename T>
vector<T> LinuxParser::rowToVector(string const& filename) {
  string line; 
  T val;
  vector<T> values;
  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);    
    while(linestream >> val){
        values.push_back(val);
    }
  }
  stream.close();
  return values;
}

// string LinuxParser::ReadValue(const string file_path, const string key){
//   string key_;
//   string value_;
//   string line_;
//   std::ifstream stream(file_path);
//   if (stream.is_open()) {
//     while (std::getline(stream, line_)){
//         std::istringstream linestream(line_);
//         linestream >> key_ >> value_;
//         if (key_ == key) { return value_; }
//     }
//   }
//   stream.close();
//   return string();
// }

// // Returns first row in file_path as vector of strings.
// vector<string> LinuxParser::ReadSingleRow(string file_path) { 
//   string line; 
//   string val;
//   vector<string> values;
//   std::ifstream stream(file_path);
//   if (stream.is_open()) {
//     std::getline(stream, line);
//     std::istringstream linestream(line);    
//     while(linestream >> val){
//         values.push_back(val);
//     }
//   }
//   stream.close();
//   return values;
// }

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyOSPrettyName) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// Read Linux Kernel from /proc/version. 
// File starts out:
// Linux version <kernel-id>
string LinuxParser::Kernel() {
  vector<string> values 
    = rowToVector<string>(kVersionFilename);
  return values[2];
}

// BONUS: Update this to use std::filesystem (available in gcc-17, and provides a cleaner solution)
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
// See data in /proc/meminfo
// float LinuxParser::MemoryUtilization() { 
//   string key;
//   string value;
//   string kb;
//   string line;
//   long MemTotal, MemFree, MemUsed, Buffers, Cached;
//   std::ifstream stream(kProcDirectory + kMeminfoFilename);
//   if (stream.is_open()) {
//     while (std::getline(stream, line)){
//         std::istringstream linestream(line);
//         linestream >> key >> value >> kb;
//         if (key == keyMemTotal)   { MemTotal = stol(value); }
//         if (key == keyMemFree)    { MemFree  = stol(value); }
//         if (key == keyMemBuffers) { Buffers  = stol(value); }
//         if (key == keyMemCached)  { Cached   = stol(value); }
//     }
//   }
//   stream.close();
//   MemUsed = MemTotal - MemFree;
//   return static_cast<float>(MemUsed - (Buffers + Cached)) / MemTotal;
// }

float LinuxParser::MemoryUtilization() { 
  long MemTotal = keyToValue<long>(kMeminfoFilename, keyMemTotal);
  long MemFree  = keyToValue<long>(kMeminfoFilename, keyMemFree);
  long Buffers  = keyToValue<long>(kMeminfoFilename, keyMemBuffers);
  long Cached   = keyToValue<long>(kMeminfoFilename, keyMemCached);
  long MemUsed  = MemTotal - MemFree;
  return static_cast<float>(MemUsed - (Buffers + Cached)) / MemTotal;
}

// TODO: Read and return the system uptime in seconds
// See file /proc/uptime
long int LinuxParser::UpTime() { 
  vector<long> values 
    = rowToVector<long>(kUptimeFilename);
  return values[0];
}

// TODO: Read and return CPU utilization
// Reads first line of proc/stat and returns vector of strings representing cpuTimes from each column.
vector<string> LinuxParser::CpuUtilization() { 
  return rowToVector<string>(kStatFilename);
}

// TODO: Read and return the total number of processes
// See bottom of file /proc/stat
int LinuxParser::TotalProcesses() {
  return keyToValue<int>(kStatFilename, keyProcesses);
}

// TODO: Read and return the number of running processes
// // See bottom of file /proc/stat
int LinuxParser::RunningProcesses() {
  return keyToValue<int>(kStatFilename, keyRunningProcesses);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string addSpace(string const& a, string const& b){
  return a + " " + b;
}
string LinuxParser::Command(int pid) { 
  string out = string();
  vector<string> values_ 
    = rowToVector<string>(std::to_string(pid) + kCmdlineFilename);
  // combine all flags from cmdline, and elim space at front.
  if (values_.size() != 0){
    out = values_[0];
    out += std::accumulate(values_.begin() + 1, values_.end(), string(""), addSpace);
  }
  return out; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
// Parse /proc/[pid]/status using key VmRSS
string LinuxParser::Ram(int pid) {
  long int ram_kb = keyToValue<long int>(to_string(pid) + kStatusFilename, 
                                         keyProcMem);
  return to_string(
    static_cast<int>(std::round(ram_kb * 0.001))
    );                                         
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  return keyToValue<string>(to_string(pid) + kStatusFilename, keyUID);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  // Get user id for process.
  string uid = Uid(pid);

  // Find and return username associated with user id. 
  string line, key, value, username, x, uid_, rest;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> uid_ >> rest) {
        if (uid_ == uid) { return username; }
      }
    }
  }
  filestream.close();
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid) { 
  // Get process timing info (clock ticks) from /proc/pid/stat
  // Create strings since not all entries are numerical.
  vector<string> values = rowToVector<string>(to_string(pid) + kStatFilename);

  // if length is not numStatCols = 52, then there is a problem, return 0.
  if (values.size() != numStatCols){
    return 0;
  }

  // starttime is the 22nd entry in the file.
  long int proc_starttime = stol(values[21]) / sysconf(_SC_CLK_TCK);
  return UpTime() - proc_starttime;
}

// Returns active time in seconds.
// Extracts (utime + stime + cutime + cstime) in clock ticks for process and divides by system clock ticks per second. 
// Method returns 0 if the number of entries in the vector 'values' read from [pid]/stat file is not LinuxParser::numStatCol = 52. 
long int LinuxParser::ActiveTime(int pid) {
  long int total_clock_ticks;
  // Vector of strings since not all entries are numerical.
  vector<string> values = rowToVector<string>(to_string(pid) + kStatFilename);
  // if length is not numStatCols = 52, then there is a problem (e.g. process was terminated)
  if (values.size() != numStatCols){
    return 0;
  }
  // tottime = utime + stime + cutime + cstime
  try
  {
    total_clock_ticks = stol(values[13]) 
                      + stol(values[14])
                      + stol(values[15])
                      + stol(values[16]);
  }
  catch(...) // any other failures
  {
    total_clock_ticks = 0;
  }
  return total_clock_ticks / sysconf(_SC_CLK_TCK);
}