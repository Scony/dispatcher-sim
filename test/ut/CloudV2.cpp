#include <gtest/gtest.h>

#include "CloudV2.hpp"
#include "Utility.hpp"

TEST(CloudV2, BusyMachineCmp1) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 6, 0);
  EXPECT_LT(bm1, bm2);
  EXPECT_FALSE(bm2 < bm1);
}

TEST(CloudV2, BusyMachineCmp2) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 1);
  EXPECT_LT(bm1, bm2);
  EXPECT_FALSE(bm2 < bm1);
}

TEST(CloudV2, BusyMachineCmp3) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), std::make_shared<Operation>(1,0,0,0,0,0,0), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), std::make_shared<Operation>(2,0,0,0,0,0,0), 5, 0);
  EXPECT_LT(bm1, bm2);
  EXPECT_FALSE(bm2 < bm1);
}

TEST(CloudV2, BusyMachines1) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 6, 0);
  CloudV2::BusyMachines busyMachines;
  busyMachines.push(bm1);
  busyMachines.push(bm2);
  EXPECT_EQ(busyMachines.top(), bm1);
  busyMachines.pop();
  EXPECT_EQ(busyMachines.top(), bm2);
}

TEST(CloudV2, BusyMachines2) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 6, 0);
  CloudV2::BusyMachines busyMachines;
  busyMachines.push(bm2);
  busyMachines.push(bm1);
  EXPECT_EQ(busyMachines.top(), bm1);
  busyMachines.pop();
  EXPECT_EQ(busyMachines.top(), bm2);
}
