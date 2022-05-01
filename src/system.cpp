#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){

    // initialize each process with its PID.
    vector<int> pids = LinuxParser::Pids();
    for (auto& pid : pids){
        processes_.push_back(Process(pid));
    }

}

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    
    //FIXME: sort is failing with error:
    // error: use of deleted function ‘Process& Process::operator=(Process&&)’

    // sort processes using class operator <.
    // std::sort(processes_.begin(), processes_.end(), std::less<Process>());    
    
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    
    return LinuxParser::Kernel();     
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    
    return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    
    return LinuxParser::OperatingSystem(); 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    
    return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long System::UpTime() { 
    
    return LinuxParser::UpTime(); 
}