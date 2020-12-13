#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

// BONUS: Update this to use std::filesystem
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


// Split string function
vector<string> LinuxParser::Split(string &target_str, const char split_icon){
    vector<string> target_af_split;
    
    uint now_index = 0;
    uint last_index = 0;
    while (now_index < target_str.size() + 1){
        if (target_str[now_index] == split_icon || now_index == target_str.size()){
            if (now_index != last_index){
                target_af_split.push_back(target_str.substr(last_index, now_index - last_index));
            }
            last_index = now_index + 1;
        }
        now_index += 1;
    }
    
    return target_af_split;
}


// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
    std::ifstream meminfo(kProcDirectory + kMeminfoFilename);
    string memory_line[2];

    uint index = 0;
    while (index < 2){
        getline(meminfo, memory_line[index]);
        index += 1;
    }
  
    float total = stof(Split(memory_line[0], ' ')[1]);
    float free = stof(Split(memory_line[1], ' ')[1]);

    return (total - free) / total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
    std::ifstream uptime_file(kProcDirectory + kUptimeFilename);
    string line, up_time;
    
    getline(uptime_file, line);
    std::istringstream uptime_line(line);
    uptime_line >> up_time;

  return std::stol(up_time);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> cpu_utilization = CpuUtilization();
  long total_jiffies;

  for (int index = 0; index < kGuest_; index++){
    total_jiffies += std::stol(cpu_utilization[index]);
  }

  return total_jiffies; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream pid_stat_file(kProcDirectory + to_string(pid) + kStatFilename);
  string line;

  getline(pid_stat_file, line);
  vector<string> pid_stat = Split(line, ' ');

  long total_time = stol(pid_stat[13]) + stol(pid_stat[14]) + stol(pid_stat[15]) + stol(pid_stat[16]);

  return total_time;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); }

// TODO: Read and return the number of idle jiffies for the system  //system error!!!
long LinuxParser::IdleJiffies() { 
  vector<string> cpu_utilization = CpuUtilization();
  long idle_jiffies = std::stol(cpu_utilization[kIdle_]) + std::stol(cpu_utilization[kIOwait_]);
  return idle_jiffies; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  std::ifstream process_file(kProcDirectory + kStatFilename);
  string line, temp_string;
  vector<string> total_jiffies;

  getline(process_file, line);
  std::istringstream jiffies(line);

  jiffies >> temp_string; //exclude cpu string
  while(jiffies >> temp_string){
    total_jiffies.push_back(temp_string);
  }

  return total_jiffies;  
}

// process pick
std::array<int, 2> ProcessPick(){
    std::ifstream process_file(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    string line;

    int total_processes;
    int running_processes;

    while (getline(process_file, line)){
        if (line.substr(0, 9) == "processes"){
            total_processes = std::stoi(LinuxParser::Split(line, ' ')[1]);
        }
        else if(line.substr(0, 13) == "procs_running"){
            running_processes = std::stoi(LinuxParser::Split(line, ' ')[1]);
        }
    }

    return {total_processes, running_processes};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return ProcessPick()[0]; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return ProcessPick()[1];
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream pid_command_file(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;

  getline(pid_command_file, line);

  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::ifstream pid_status_file(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, memory_size;

  while(getline(pid_status_file, line)){
    std::istringstream pid_line(line);
    pid_line >> key;
    if (key == "VmSize:"){
      pid_line >> memory_size;
      int memory = stof(memory_size);
      memory = round(memory/1000.);
      return to_string(memory);
    }
  }

  return "0"; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream pid_status_file(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, uid;

  while(getline(pid_status_file, line)){
    std::istringstream pid_line(line);
    pid_line >> key;
    if (key == "Uid:"){
      pid_line >> uid;
      return uid;
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::ifstream passwd_file(kPasswordPath);
  string line;
  string uid = Uid(pid);

  while(getline(passwd_file, line)){
    vector<string> uid_search = Split(line, ':');
    if (uid_search[2] == uid){
      return uid_search[0];
    }
  }

  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function

long LinuxParser::UpTime(int pid) { 
  std::ifstream pid_stat_file(kProcDirectory + to_string(pid) + kStatFilename);
  string line;

  getline(pid_stat_file, line);
  vector<string> pid_stat = Split(line, ' ');

  //float total_time = stof(pid_stat[13]) + stof(pid_stat[14]) + stof(pid_stat[15]) + stof(pid_stat[16]);
  long start_time = stol(pid_stat[21]);
  return start_time / sysconf(_SC_CLK_TCK);
}