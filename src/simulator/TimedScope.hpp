#pragma once

#include <string>
#include <chrono>

class TimedScope
{
 public:
  using Message = std::string;
  TimedScope(Message);
  ~TimedScope();

 private:
  Message mMessage;
  std::chrono::system_clock::time_point mStart;
};
