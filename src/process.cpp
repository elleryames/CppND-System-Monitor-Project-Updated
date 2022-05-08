#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

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
    // Get processor timing info (seconds)
    long int tottime = LinuxParser::ActiveTime(pid);
    
    // get time (seconds) that process has been running
    long int runtime = Process::UpTime();

    // cpu utilization is fraction of run time using cpu.
    float cpu;
    if (runtime != 0){
        cpu = (float)tottime / runtime;
    }
    else {
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