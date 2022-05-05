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

// Constructor: initializes pid and computes cpu utilization.
Process::Process(int pid) : pid_(pid) {
    Process::CpuUtilization(pid);
};

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_util_; }

// Compute process CPU utilization 
void Process::CpuUtilization(int pid) { 
    // Get processor timing info (clock ticks) from /proc/pid/stat
    string file_path = LinuxParser::kProcDirectory 
                       + to_string(pid) 
                       + LinuxParser::kStatFilename;
    vector<string> values = LinuxParser::ReadSingleRow(file_path);

    // tottime = utime + stime + cutime + cstime
    long int tottime = stol(values[13]) 
                     + stol(values[14])
                     + stol(values[15])
                     + stol(values[16]);
    tottime = tottime / sysconf(_SC_CLK_TCK);

    // get time (seconds) that process has been running
    long int runtime = Process::UpTime();

    // cpu utilization is fraction of run time using cpu.
    float cpu = 0.0;
    try
    {
        cpu = static_cast<float>(tottime) / runtime;
    }
    catch(...)
    {
        cpu = 0.0;
    }
    cpu_util_ = cpu;
}

// TODO: Return the command that generated this process
string Process::Command() const { 
    string cmd = LinuxParser::Command(pid_); 
    if (cmd.length() > commandStringLength){
        cmd = cmd.substr(0, commandStringLength);
        cmd += "...";
    }
    return cmd;
}

// TODO: Return this process's memory utilization
string Process::Ram() const { 
    string ram = LinuxParser::Ram(pid_);   
    return ram;
}

// TODO: Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    // first sort on CPU and then on PID.
    if (cpu_util_ != a.cpu_util_){
        return cpu_util_ < a.cpu_util_; 
    }
    return pid_ < a.pid_;
    // return true;
}