#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include <algorithm>
#include <array>
#include <fstream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Done: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids_id = LinuxParser::Pids();
    processes_.clear();

    for (int pid: pids_id){
        processes_.push_back(Process(pid));
    }

    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

std::string System::Kernel() { 
    if (kernel_.empty()){
        kernel_ = LinuxParser::Kernel();
    }
    return kernel_; 
}

// Done: Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization();
}

// Done: Return the operating system name
std::string System::OperatingSystem() {
    if (os_.empty()){
        os_ = LinuxParser::OperatingSystem();
    }
    return os_;
}

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// Done: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

// Done: Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime();
}