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
    InitProcesses(); 
}

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

// Initialize each process with its PID.
void System::InitProcesses(){
    const vector<int> pids = LinuxParser::Pids();
    for (const auto& pid : pids){
        processes_.emplace_back(pid);
    }
}
// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

    // refresh processes list
    processes_.clear();
    InitProcesses();

    //FIXME: sort is failing with error:
    // error: use of deleted function ‘Process& Process::operator=(Process&&)’

    // sort processes using class operator <.
    std::sort(processes_.rbegin(), processes_.rend());
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