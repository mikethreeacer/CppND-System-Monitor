#include <fstream>
#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"


// Done: Return the aggregate CPU utilization
float Processor::Utilization() { 
  total_last = LinuxParser::Jiffies();
  active_last = LinuxParser::ActiveJiffies();

  usleep(100000);

  total_now = LinuxParser::Jiffies();
  active_now = LinuxParser::ActiveJiffies();

  return (active_now - active_last) / (total_now - total_last);
}