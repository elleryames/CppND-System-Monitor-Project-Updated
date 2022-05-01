#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::stol;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    // Get processor timing info (clock ticks) from /proc/pid/stat
    string file_path = LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename;
    vector<string> values = LinuxParser::ReadSingleRow( file_path);

    // tottime = utime + stime + cutime + cstime
    long int tottime = stol(values[13]) 
                     + stol(values[14])
                     + stol(values[15])
                     + stol(values[16]);
    tottime *= 1. / sysconf(_SC_CLK_TCK);

    // get time (seconds) that process has been running
    long int runtime = Process::UpTime();

    // cpu utilization is fraction of run time using cpu.
    return tottime / runtime; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    string ram = LinuxParser::Ram(pid_);   
    return ram;
}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
//     return CpuUtilization() < a.CpuUtilization(); 
// }