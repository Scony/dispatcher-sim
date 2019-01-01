#include <algorithm>

#include "ExecutionsSubject.hpp"

void ExecutionsSubject::subscribe(std::shared_ptr<IExecutionsListener> listener)
{
  unsubscribe(listener);
  mSubscribers.push_back(listener);
}

void ExecutionsSubject::unsubscribe(std::shared_ptr<IExecutionsListener> listener)
{
  mSubscribers.erase(
      std::remove_if(
          mSubscribers.begin(),
          mSubscribers.end(),
          [&listener](std::shared_ptr<IExecutionsListener> subscriber) {
            return subscriber == listener;
          }),
      mSubscribers.end());
}

size_t ExecutionsSubject::numberOfSubscriptions()
{
  return mSubscribers.size();
}

void ExecutionsSubject::notify(const Assignation& notification)
{
  for (const auto& subscriber : mSubscribers)
    subscriber->handleNotification(notification);
}
