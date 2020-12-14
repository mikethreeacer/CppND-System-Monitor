#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // Done: See src/processor.cpp

  // Done: Declare any necessary private members
 private:
  float total_now = 0;
  float active_now = 0;
  float total_last = 0;
  float active_last = 0;
};

#endif