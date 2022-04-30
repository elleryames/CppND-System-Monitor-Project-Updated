#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::ReadValue(const string file_path, const string key){
  string key_, value_, line_;
  std::ifstream stream(file_path);
  if (stream.is_open()) {
    while (std::getline(stream, line_)){
        std::istringstream linestream(line_);
        linestream >> key_ >> value_;
        if (key_ == key) { return value_; }
    }
  }
  return string();
}

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
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
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
float LinuxParser::MemoryUtilization() { 
  string key, value, kb;
  string line;
  long MemTotal, MemFree, MemUsed, Buffers, Cached;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
        std::istringstream linestream(line);
        linestream >> key >> value >> kb;
        if (key == "MemTotal:") { MemTotal = stol(value); }
        if (key == "MemFree:")  { MemFree  = stol(value); }
        if (key == "Buffers:")  { Buffers  = stol(value); }
        if (key == "Cached:")   { Cached   = stol(value); }
    }
  }

  MemUsed = MemTotal - MemFree;
  return static_cast<float>(MemUsed - (Buffers + Cached)) / MemTotal;
}

// TODO: Read and return the system uptime
// See file /proc/uptime
long LinuxParser::UpTime() { 
  string uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
    return stol(uptime);
  }
  return 0; 
}

// TODO: Read and return the number of jiffies for the system
// A jiffie is a unit of time used by the OS. 
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
// Reads first line of proc/stat and returns vector of strings representing cpuTimes from each column.
vector<string> LinuxParser::CpuUtilization() { 
  std::vector<string> cpuTimes;
  string line, cpuTime, cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while(linestream >> cpuTime){
        cpuTimes.push_back(cpuTime);
    }
  }    
  return cpuTimes; 
}

// TODO: Read and return the total number of processes
// See bottom of file /proc/stat
int LinuxParser::TotalProcesses() {
  string value_ = ReadValue(kProcDirectory + kStatFilename, "processes");
  if (!value_.empty()){
    return stoi(value_);
  }
  return 0; 
}

// TODO: Read and return the number of running processes
// See bottom of file /proc/stat
int LinuxParser::RunningProcesses() {
  string value_ = ReadValue(kProcDirectory + kStatFilename, "procs_running");
  if (!value_.empty()){
    return stoi(value_);
  }
  return 0; 
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    //std::istringstream linestream(line);
    //linestream >> uptime >> idle;
    return line;
  }  
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string key, value, kb;
  long ram_in_kb = 0;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
        std::istringstream linestream(line);
        linestream >> key >> value >> kb;
        if (key == "VmSize:") {  ram_in_kb = stol(value); }
    }
  }
  return to_string(static_cast<int>(std::round(ram_in_kb * 0.001))); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string file_path = kProcDirectory + std::to_string(pid) + kStatusFilename;
  return ReadValue(file_path, "Uid:"); 
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
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, val;
  vector<string> values;
  // get time in clock-ticks.
  // file /proc/[pid]/stat consists of one-line
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);    
    while(linestream >> val){
        values.push_back(val);
    }
  }
  // starttime is the 22nd entry in the file.
  long proc_starttime = stol(values[21]) / sysconf(_SC_CLK_TCK);
  return UpTime() - proc_starttime;
}
