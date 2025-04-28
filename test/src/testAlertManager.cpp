#include "alert_manager/AlertManager.h"
#include <gtest/gtest.h>
#include <thread>

using namespace alert_manager;

class AlertManagerTest : public ::testing::Test {
protected:
  AlertManager manager;

  Alert createAlert(int64_t timestamp, Priority priority, AlertType type,
                    const std::array<int, 2> &params) {
    Alert alert{.timestamp = timestamp,
                .priority = priority,
                .type = type,
                .message = manager.getPredefinedMessage(type, params),
                .params = params};
    return alert;
  }
};

TEST_F(AlertManagerTest, AddAlert) {
  Alert alert =
      createAlert(1000, Priority::High, AlertType::BloodTemperature, {38, 9});
  EXPECT_TRUE(manager.addAlert(alert));
  auto alerts = manager.getAlerts();
  EXPECT_EQ(alerts.size(), 1);
  EXPECT_EQ(alerts[0], alert);
}

TEST_F(AlertManagerTest, MaxAlertsLimit) {
  // Add 20 alerts across different types to avoid hitting MaxAlertsPerType (10)
  for (size_t i = 0; i < 7; ++i) {
    Alert alert =
        createAlert(1000 + i, Priority::Low, AlertType::LowBattery, {20, 0});
    EXPECT_TRUE(manager.addAlert(alert));
  }
  for (size_t i = 0; i < 7; ++i) {
    Alert alert = createAlert(2000 + i, Priority::Low,
                              AlertType::BloodTemperature, {38, 9});
    EXPECT_TRUE(manager.addAlert(alert));
  }
  for (size_t i = 0; i < 6; ++i) {
    Alert alert =
        createAlert(3000 + i, Priority::Low, AlertType::LowOxidation, {90, 0});
    EXPECT_TRUE(manager.addAlert(alert));
  }
  // Total: 7 + 7 + 6 = 20 alerts
  Alert extra =
      createAlert(4000, Priority::Low, AlertType::LowBattery, {20, 0});
  EXPECT_FALSE(manager.addAlert(extra));
}

TEST_F(AlertManagerTest, MaxAlertsPerType) {
  for (size_t i = 0; i < AlertManager::MaxAlertsPerType; ++i) {
    Alert alert = createAlert(1000 + i, Priority::Low,
                              AlertType::BloodTemperature, {38, 9});
    EXPECT_TRUE(manager.addAlert(alert));
  }
  Alert extra =
      createAlert(2000, Priority::Low, AlertType::BloodTemperature, {38, 9});
  EXPECT_FALSE(manager.addAlert(extra));
}

TEST_F(AlertManagerTest, UpdatePriority) {
  Alert alert =
      createAlert(1000, Priority::Low, AlertType::BloodTemperature, {38, 9});
  manager.addAlert(alert);
  EXPECT_TRUE(manager.updatePriority(1000, AlertType::BloodTemperature,
                                     Priority::Critical));
  auto alerts = manager.getAlerts();
  EXPECT_EQ(alerts[0].priority, Priority::Critical);
}

TEST_F(AlertManagerTest, SortAlerts) {
  manager.addAlert(
      createAlert(1000, Priority::Low, AlertType::LowBattery, {20, 0}));
  manager.addAlert(
      createAlert(2000, Priority::High, AlertType::BloodTemperature, {38, 9}));
  manager.addAlert(
      createAlert(1500, Priority::High, AlertType::LowOxidation, {90, 0}));
  auto alerts = manager.getAlerts();
  EXPECT_EQ(alerts[0].priority, Priority::High);
  EXPECT_EQ(alerts[0].timestamp, 2000);
  EXPECT_EQ(alerts[1].priority, Priority::High);
  EXPECT_EQ(alerts[1].timestamp, 1500);
  EXPECT_EQ(alerts[2].priority, Priority::Low);
}

TEST_F(AlertManagerTest, ClearAlert) {
  Alert alert =
      createAlert(1000, Priority::High, AlertType::BloodTemperature, {38, 9});
  manager.addAlert(alert);
  EXPECT_TRUE(manager.clearAlert(1000, AlertType::BloodTemperature));
  EXPECT_TRUE(manager.getAlerts().empty());
}

TEST_F(AlertManagerTest, ClearAllAlerts) {
  manager.addAlert(
      createAlert(1000, Priority::Low, AlertType::LowBattery, {20, 0}));
  manager.addAlert(
      createAlert(2000, Priority::High, AlertType::BloodTemperature, {38, 9}));
  manager.clearAllAlerts();
  EXPECT_TRUE(manager.getAlerts().empty());
}

TEST_F(AlertManagerTest, ThreadSafety) {
  std::vector<std::thread> threads;
  for (int i = 0; i < 5; ++i) {
    threads.emplace_back([this, i]() {
      Alert alert =
          createAlert(1000 + i, Priority::Low, AlertType::LowBattery, {20, 0});
      manager.addAlert(alert);
    });
  }
  for (auto &t : threads) {
    t.join();
  }
  EXPECT_EQ(manager.getAlerts().size(), 5);
}

TEST_F(AlertManagerTest, CanAddAlertNewType) {
  // Test adding an alert for a new type (not in alerts_per_type_)
  Alert alert =
      createAlert(1000, Priority::High, AlertType::BloodTemperature, {38, 9});
  EXPECT_TRUE(manager.addAlert(alert));
  EXPECT_EQ(manager.getAlerts().size(), 1);
}