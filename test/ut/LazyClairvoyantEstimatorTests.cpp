#include <gtest/gtest.h>

#include "LazyClairvoyantEstimator.hpp"

TEST(LazyClairvoyantEstimatorTests, CheckDefault) {
  auto op = std::make_shared<Operation>(1,1,1,1,1,1);
  LazyClairvoyantEstimator estimator;
  EXPECT_EQ(estimator.estimate(op), 42);
}

TEST(LazyClairvoyantEstimatorTests, CheckClv) {
  auto op1 = std::make_shared<Operation>(1,1,1337,1,1,1);
  auto op2 = std::make_shared<Operation>(2,1,1337,1,1,1234);
  LazyClairvoyantEstimator estimator;
  estimator.handleNotification({0, op2});
  EXPECT_EQ(estimator.estimate(op2), 1234);
}

TEST(LazyClairvoyantEstimatorTests, CheckAvg) {
  auto op1 = std::make_shared<Operation>(1,1,1337,1,1,75);
  auto op2 = std::make_shared<Operation>(2,1,1338,1,1,25);
  auto op3 = std::make_shared<Operation>(3,1,1339,1,1,5);
  LazyClairvoyantEstimator estimator;
  estimator.handleNotification({0, op1});
  estimator.handleNotification({0, op2});
  EXPECT_EQ(estimator.estimate(op3), 50);
}
