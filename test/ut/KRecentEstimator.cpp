#include <gtest/gtest.h>

#include "KRecentEstimator.hpp"

TEST(KRecentEstimatorTests, CheckDefault) {
  auto op = std::make_shared<Operation>(1,1,1,1,1,1);
  KRecentEstimator estimator(3);
  EXPECT_EQ(estimator.estimate(op), 42);
}

TEST(KRecentEstimatorTests, CheckAvg1) {
  auto op1 = std::make_shared<Operation>(1,1,1337,1,1,2);
  auto op2 = std::make_shared<Operation>(2,1,1337,1,1,1234);
  KRecentEstimator estimator(3);
  estimator.handleNotification({0, op1, 0});
  estimator.handleNotification({0, op2, 0});
  EXPECT_EQ(estimator.estimate(op2), 618);
}

TEST(KRecentEstimatorTests, CheckCeil) {
  auto op1 = std::make_shared<Operation>(1,1,1337,1,1,1);
  auto op2 = std::make_shared<Operation>(2,1,1337,1,1,1234);
  KRecentEstimator estimator(3);
  estimator.handleNotification({0, op1, 0});
  estimator.handleNotification({0, op2, 0});
  EXPECT_EQ(estimator.estimate(op2), 618);
}

TEST(KRecentEstimatorTests, CheckAvg2) {
  auto op1 = std::make_shared<Operation>(1,1,1337,1,1,1);
  auto op2 = std::make_shared<Operation>(2,1,1337,1,1,1234);
  auto op3 = std::make_shared<Operation>(3,1,1337,1,1,1);
  KRecentEstimator estimator(3);
  estimator.handleNotification({0, op1, 0});
  estimator.handleNotification({0, op2, 0});
  estimator.handleNotification({0, op3, 0});
  EXPECT_EQ(estimator.estimate(op2), 412);
}

TEST(KRecentEstimatorTests, CheckWindow) {
  auto op1 = std::make_shared<Operation>(1,1,1337,1,1,1234);
  auto op2 = std::make_shared<Operation>(2,1,1337,1,1,1);
  auto op3 = std::make_shared<Operation>(3,1,1337,1,1,1);
  auto op4 = std::make_shared<Operation>(4,1,1337,1,1,4);
  KRecentEstimator estimator(3);
  estimator.handleNotification({0, op1, 0});
  estimator.handleNotification({0, op2, 0});
  estimator.handleNotification({0, op3, 0});
  estimator.handleNotification({0, op4, 0});
  EXPECT_EQ(estimator.estimate(op2), 2);
}

TEST(KRecentEstimatorTests, CheckManyAvgs) {
  auto op1 = std::make_shared<Operation>(1,1,1337,1,1,1);
  auto op2 = std::make_shared<Operation>(2,1,1337,1,1,3);
  auto op3 = std::make_shared<Operation>(3,1,1338,1,1,5);
  auto op4 = std::make_shared<Operation>(4,1,1338,1,1,7);
  KRecentEstimator estimator(3);
  estimator.handleNotification({0, op1, 0});
  estimator.handleNotification({0, op2, 0});
  estimator.handleNotification({0, op3, 0});
  estimator.handleNotification({0, op4, 0});
  EXPECT_EQ(estimator.estimate(op2), 2);
  EXPECT_EQ(estimator.estimate(op4), 6);
}

// TEST(LazyClairvoyantEstimatorTests, CheckClv) {
//   auto op1 = std::make_shared<Operation>(1,1,1337,1,1,1);
//   auto op2 = std::make_shared<Operation>(2,1,1337,1,1,1234);
//   LazyClairvoyantEstimator estimator;
//   estimator.handleNotification({0, op2, 0});
//   EXPECT_EQ(estimator.estimate(op2), 1234);
// }

// TEST(LazyClairvoyantEstimatorTests, CheckAvg) {
//   auto op1 = std::make_shared<Operation>(1,1,1337,1,1,75);
//   auto op2 = std::make_shared<Operation>(2,1,1338,1,1,25);
//   auto op3 = std::make_shared<Operation>(3,1,1339,1,1,5);
//   LazyClairvoyantEstimator estimator;
//   estimator.handleNotification({0, op1, 0});
//   estimator.handleNotification({0, op2, 0});
//   EXPECT_EQ(estimator.estimate(op3), 50);
// }
