#include "alert_manager/AlertManager.h"
#include <chrono>
#include <iostream>
#include <thread>

using namespace alert_manager;

void printAlerts(const std::vector<Alert> &alerts) {
  for (const auto &alert : alerts) {
    std::cout << "Alert: [Priority=" << static_cast<int>(alert.priority)
              << ", Type=" << static_cast<int>(alert.type)
              << ", Time=" << alert.timestamp << ", Message=" << alert.message
              << "]\n";
  }
}

void simulateAlerts(AlertManager &manager) {
  Alert alert1{
      .timestamp =
          std::chrono::system_clock::now().time_since_epoch().count() / 1000000,
      .priority = Priority::High,
      .type = AlertType::BloodTemperature,
      .message =
          manager.getPredefinedMessage(AlertType::BloodTemperature, {38, 9}),
      .params = {38, 9}};
  manager.addAlert(alert1);

  Alert alert2{
      .timestamp =
          std::chrono::system_clock::now().time_since_epoch().count() / 1000000,
      .priority = Priority::Low,
      .type = AlertType::LowBattery,
      .message = manager.getPredefinedMessage(AlertType::LowBattery, {20, 0}),
      .params = {20, 0}};
  manager.addAlert(alert2);
}

int main() {
  AlertManager manager;

  // Simulate alerts from another thread
  std::thread t1(simulateAlerts, std::ref(manager));
  t1.join();

  // Display alerts
  auto alerts = manager.getAlerts();
  std::cout << "Current alerts:\n";
  printAlerts(alerts);

  // Clear an alert
  if (manager.clearAlert(alerts[0].timestamp, alerts[0].type)) {
    std::cout << "Cleared first alert\n";
  }

  // Display remaining alerts
  alerts = manager.getAlerts();
  std::cout << "After clearing:\n";
  printAlerts(alerts);

  // Clear all alerts
  manager.clearAllAlerts();
  std::cout << "After clearing all:\n";
  alerts = manager.getAlerts();
  printAlerts(alerts);

  return 0;
}