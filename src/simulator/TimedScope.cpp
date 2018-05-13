#include <iostream>

#include "TimedScope.hpp"

TimedScope::TimedScope(Message message)
    : mMessage(message)
    , mStart(std::chrono::system_clock::now())
{
  std::cerr << message << "..." << std::endl;
}

TimedScope::~TimedScope()
{
  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - mStart);
  std::cerr << mMessage << " done (took "
            << std::fixed << ((double)elapsed.count() / 1000)
            << " s)" << std::endl;
}
