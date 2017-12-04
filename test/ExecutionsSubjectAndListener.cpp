#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ExecutionsSubject.hpp"

using ::testing::_;

class MockExecutionsListener : public IExecutionsListener
{
public:
  MOCK_METHOD1(handleNotification, void(const std::pair<long long, OperationSP>& notification));
};

TEST(ExecutionsSubjectAndListenerTests, SubscribeNotif1) {
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  EXPECT_CALL(*listener, handleNotification(_)).Times(1);
  subject.notify({0,OperationSP()});
}

TEST(ExecutionsSubjectAndListenerTests, SubscribeNotif2) {
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  subject.subscribe(listener);
  subject.subscribe(listener);
  EXPECT_CALL(*listener, handleNotification(_)).Times(1);
  subject.notify({0,OperationSP()});
}

TEST(ExecutionsSubjectAndListenerTests, SubscribeNotif3) {
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  EXPECT_CALL(*listener, handleNotification(_)).Times(3);
  subject.notify({0,OperationSP()});
  subject.notify({0,OperationSP()});
  subject.notify({0,OperationSP()});
}

TEST(ExecutionsSubjectAndListenerTests, SubscribeNotif4) {
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  EXPECT_CALL(*listener,handleNotification(std::pair<long long, OperationSP>{123,OperationSP()}))
    .Times(1);
  subject.notify({123,OperationSP()});
}

TEST(ExecutionsSubjectAndListenerTests, UnsubscribeNotif1) {
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  subject.unsubscribe(listener);
  EXPECT_CALL(*listener, handleNotification(_)).Times(0);
  subject.notify({0,OperationSP()});
}

TEST(ExecutionsSubjectAndListenerTests, UnsubscribeNotif2) {
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  subject.unsubscribe(listener);
  subject.subscribe(listener);
  EXPECT_CALL(*listener, handleNotification(_)).Times(1);
  subject.notify({0,OperationSP()});
}
