#include <algorithm>

#include "ExecutionsSubject.hpp"

void ExecutionsSubject::subscribe(std::shared_ptr<IExecutionsListener> listener)
{
  unsubscribe(listener);
  mSubscribers.push_back(listener);
}

void ExecutionsSubject::unsubscribe(std::shared_ptr<IExecutionsListener> listener)
{
  mSubscribers.erase(std::remove_if(mSubscribers.begin(),
				    mSubscribers.end(),
				    [](std::shared_ptr<IExecutionsListener> subscriber) {
				      return subscriber;
				    }),
		     mSubscribers.end());
}

void ExecutionsSubject::notify(const Assignation& notification)
{
  for (const auto& subscriber : mSubscribers)
    subscriber->handleNotification(notification);
}
