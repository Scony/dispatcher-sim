#pragma once

#include "Input.hpp"

class InputV2 : public Input
{
 public:
  virtual void readFromStdin() override;
};
