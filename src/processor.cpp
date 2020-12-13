#include <fstream>
#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  float total_last = LinuxParser::Jiffies();
  float active_last = LinuxParser::ActiveJiffies();

  usleep(100000);

  float total_now = LinuxParser::Jiffies();
  float active_now = LinuxParser::ActiveJiffies();

  return (active_now - active_last)/(total_now - total_last);
}