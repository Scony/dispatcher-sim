#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Cloud.hpp"

using ::testing::_;
using ::testing::InSequence;

class IExecutionsListenerProxyEnd
{
public:
  virtual void handleNotification(long long, OperationSP) = 0;
};

class ExecutionsListenerProxyEndMock : public IExecutionsListenerProxyEnd
{
public:
  MOCK_METHOD2(handleNotification, void(long long, OperationSP));
};

class ExecutionsListenerProxy : public IExecutionsListener
{
public:
  ExecutionsListenerProxy(std::shared_ptr<IExecutionsListenerProxyEnd> proxyEnd) :
    mProxyEnd(proxyEnd) {}

  void handleNotification(const std::pair<long long, OperationSP>& notification) override
  {
    mProxyEnd->handleNotification(notification.first, notification.second);
  }

private:
  std::shared_ptr<IExecutionsListenerProxyEnd> mProxyEnd;
};

class QueueWrapper : public IQueue
{
public:
  QueueWrapper(std::vector<OperationSP>& operations) : mOperations(operations) {}

  OperationSP pop() override
  {
    auto operation = mOperations.back();
    mOperations.pop_back();
    return operation;
  }
  size_t size() override
  {
    return mOperations.size();
  }

private:
  std::vector<OperationSP>& mOperations;
};

TEST(Cloud, OperationsInSequence) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,6,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,7,5);
  std::vector<OperationSP> operations;
  std::shared_ptr<IQueue> queue = std::make_shared<QueueWrapper>(operations);

  auto mock = std::make_shared<ExecutionsListenerProxyEndMock>();
  auto proxy = std::make_shared<ExecutionsListenerProxy>(mock);
  Cloud cloud(1);
  cloud.assignQueue(queue.get());
  cloud.subscribe(proxy);

  InSequence dummy;

  EXPECT_CALL(*mock, handleNotification(10,op1)).Times(1);
  EXPECT_CALL(*mock, handleNotification(15,op2)).Times(1);
  EXPECT_CALL(*mock, handleNotification(20,op3)).Times(1);

  cloud.advance(5);
  operations = {op3, op2, op1};
  cloud.advance(100);
}

TEST(Cloud, QueueFetchingOrder) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,5,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,5,5);
  std::vector<OperationSP> operations;
  std::shared_ptr<IQueue> queue = std::make_shared<QueueWrapper>(operations);

  auto mock = std::make_shared<ExecutionsListenerProxyEndMock>();
  auto proxy = std::make_shared<ExecutionsListenerProxy>(mock);
  Cloud cloud(1);
  cloud.assignQueue(queue.get());
  cloud.subscribe(proxy);

  InSequence dummy;

  EXPECT_CALL(*mock, handleNotification(10,op1)).Times(1);
  EXPECT_CALL(*mock, handleNotification(15,op2)).Times(1);
  EXPECT_CALL(*mock, handleNotification(20,op3)).Times(1);

  cloud.advance(5);
  operations = {op3, op2, op1};
  cloud.advance(100);
}

TEST(Cloud, QueueWithGaps) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,11,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,17,5);
  std::vector<OperationSP> operations;
  std::shared_ptr<IQueue> queue = std::make_shared<QueueWrapper>(operations);

  auto mock = std::make_shared<ExecutionsListenerProxyEndMock>();
  auto proxy = std::make_shared<ExecutionsListenerProxy>(mock);
  Cloud cloud(1);
  cloud.assignQueue(queue.get());
  cloud.subscribe(proxy);

  InSequence dummy;

  EXPECT_CALL(*mock, handleNotification(10,op1)).Times(1);
  EXPECT_CALL(*mock, handleNotification(16,op2)).Times(1);
  EXPECT_CALL(*mock, handleNotification(22,op3)).Times(1);

  cloud.advance(5);
  operations = {op1};
  cloud.advance(11);
  operations = {op2};
  cloud.advance(17);
  operations = {op3};
  cloud.advance(100);
}

TEST(Cloud, RuntimeQueueChangeOnEdge) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,5,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,10,5);
  std::vector<OperationSP> operations;
  std::shared_ptr<IQueue> queue = std::make_shared<QueueWrapper>(operations);

  auto mock = std::make_shared<ExecutionsListenerProxyEndMock>();
  auto proxy = std::make_shared<ExecutionsListenerProxy>(mock);
  Cloud cloud(1);
  cloud.assignQueue(queue.get());
  cloud.subscribe(proxy);

  InSequence dummy;

  EXPECT_CALL(*mock, handleNotification(10,op1)).Times(1);
  EXPECT_CALL(*mock, handleNotification(15,op3)).Times(1);
  EXPECT_CALL(*mock, handleNotification(20,op2)).Times(1);

  cloud.advance(5);
  operations = {op2,op1};
  cloud.advance(10);
  operations = {op2,op3};
  cloud.advance(100);
}

TEST(Cloud, Simulate1) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,6,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,7,5);
  std::vector<OperationSP> operations = {op3, op2, op1};

  auto schedule = Cloud::simulate(1,operations);
  ASSERT_EQ(schedule.size(), 3);
  EXPECT_EQ(schedule[0].first, 10);
  EXPECT_EQ(schedule[1].first, 15);
  EXPECT_EQ(schedule[2].first, 20);
}

TEST(Cloud, Simulate2) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,10,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,15,5);
  std::vector<OperationSP> operations = {op3, op2, op1};

  auto schedule = Cloud::simulate(1,operations);
  ASSERT_EQ(schedule.size(), 3);
  EXPECT_EQ(schedule[0].first, 10);
  EXPECT_EQ(schedule[1].first, 15);
  EXPECT_EQ(schedule[2].first, 20);
}

TEST(Cloud, Simulate3) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,11,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,4,5);
  std::vector<OperationSP> operations = {op3, op2, op1};

  auto schedule = Cloud::simulate(1,operations);
  ASSERT_EQ(schedule.size(), 3);
  EXPECT_EQ(schedule[0].first, 10);
  EXPECT_EQ(schedule[1].first, 16);
  EXPECT_EQ(schedule[2].first, 21);
}
