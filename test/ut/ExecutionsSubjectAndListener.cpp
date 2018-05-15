#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ExecutionsSubject.hpp"

using ::testing::_;

class MockExecutionsListener : public IExecutionsListener
{
 public:
  MOCK_METHOD1(handleNotification, void(const Assignation& notification));
};

TEST(ExecutionsSubjectAndListenerTests, SubscribeNotif1)
{
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  EXPECT_EQ(subject.numberOfSubscriptions(), 1);
  EXPECT_CALL(*listener, handleNotification(_)).Times(1);
  subject.notify({0,OperationSP(),0});
}

TEST(ExecutionsSubjectAndListenerTests, SubscribeNotif2)
{
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  subject.subscribe(listener);
  subject.subscribe(listener);
  EXPECT_EQ(subject.numberOfSubscriptions(), 1);
  EXPECT_CALL(*listener, handleNotification(_)).Times(1);
  subject.notify({0,OperationSP(),0});
}

TEST(ExecutionsSubjectAndListenerTests, SubscribeNotif3)
{
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  EXPECT_EQ(subject.numberOfSubscriptions(), 1);
  EXPECT_CALL(*listener, handleNotification(_)).Times(3);
  subject.notify({0,OperationSP(),0});
  subject.notify({0,OperationSP(),0});
  subject.notify({0,OperationSP(),0});
}

TEST(ExecutionsSubjectAndListenerTests, SubscribeNotif4)
{
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  EXPECT_EQ(subject.numberOfSubscriptions(), 1);
  EXPECT_CALL(*listener,handleNotification(std::make_tuple(123ll,OperationSP(),234u)))
      .Times(1);
  subject.notify({123,OperationSP(),234});
}

TEST(ExecutionsSubjectAndListenerTests, SubscribeNotif5)
{
  auto listener1 = std::make_shared<MockExecutionsListener>();
  auto listener2 = std::make_shared<MockExecutionsListener>();
  auto listener3 = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener1);
  subject.subscribe(listener2);
  subject.subscribe(listener3);
  EXPECT_EQ(subject.numberOfSubscriptions(), 3);
  EXPECT_CALL(*listener1, handleNotification(_)).Times(1);
  EXPECT_CALL(*listener2, handleNotification(_)).Times(1);
  EXPECT_CALL(*listener3, handleNotification(_)).Times(1);
  subject.notify({0,OperationSP(),0});
}

TEST(ExecutionsSubjectAndListenerTests, UnsubscribeNotif1)
{
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  subject.unsubscribe(listener);
  EXPECT_CALL(*listener, handleNotification(_)).Times(0);
  subject.notify({0,OperationSP(),0});
}

TEST(ExecutionsSubjectAndListenerTests, UnsubscribeNotif2)
{
  auto listener = std::make_shared<MockExecutionsListener>();
  auto subject = ExecutionsSubject();

  subject.subscribe(listener);
  subject.unsubscribe(listener);
  subject.subscribe(listener);
  EXPECT_CALL(*listener, handleNotification(_)).Times(1);
  subject.notify({0,OperationSP(),0});
}
