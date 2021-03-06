#pragma once

#include <vector>

#include "IExecutionsListener.hpp"

class ExecutionsSubject
{
 public:
  virtual void subscribe(std::shared_ptr<IExecutionsListener> listener);
  virtual void unsubscribe(std::shared_ptr<IExecutionsListener> listener);
  virtual size_t numberOfSubscriptions();

  virtual void notify(const Assignation& notification);

 private:
  std::vector<std::shared_ptr<IExecutionsListener>> mSubscribers;
};
