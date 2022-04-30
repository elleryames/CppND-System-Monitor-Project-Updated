#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
  // Previous times are stored in order to compute recent CPU utilization
 private:
   long prevNonIdle{0};
   long prevTotal{0};
};

#endif