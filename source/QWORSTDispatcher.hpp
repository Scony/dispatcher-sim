#pragma once

#include "QOPTDispatcher.hpp"

class QWORSTDispatcher : public QOPTDispatcher
{
public:
  QWORSTDispatcher(std::shared_ptr<Input> input,
		   std::shared_ptr<Cloud> cloud,
		   std::shared_ptr<IEstimator> estimator);
};
