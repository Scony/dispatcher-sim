#pragma once

#include <map>
#include <vector>

#include "SJDispatcher.hpp"

class LJSODispatcher : public SJDispatcher
{
 public:
  using SJDispatcher::SJDispatcher;

  virtual void dispatch(JobSP job) override;
};
