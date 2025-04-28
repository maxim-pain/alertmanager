#include "alert_manager/AlertManager.h"
#include <algorithm>
#include <chrono>
#include <sstream>

namespace alert_manager {

bool Alert::operator==(const Alert &other) const {
  return timestamp == other.timestamp && type == other.type;
}

AlertManager::AlertManager() = default;

bool AlertManager::addAlert(const Alert &alert) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!canAddAlert(alert.type)) {
    return false;
  }

  // Add or replace alert
  auto it =
      std::find_if(alerts_.begin(), alerts_.end(), [&alert](const Alert &a) {
        return a.timestamp == alert.timestamp && a.type == alert.type;
      });
  if (it != alerts_.end()) {
    *it = alert; // Update existing alert
  } else {
    alerts_.push_back(alert);
    alerts_per_type_[alert.type]++;
  }
  return true;
}

bool AlertManager::updatePriority(int64_t timestamp, AlertType type,
                                  Priority newPriority) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = std::find_if(alerts_.begin(), alerts_.end(),
                         [timestamp, type](const Alert &a) {
                           return a.timestamp == timestamp && a.type == type;
                         });
  if (it == alerts_.end()) {
    return false;
  }
  it->priority = newPriority;
  return true;
}

std::vector<Alert> AlertManager::getAlerts() const {
  std::lock_guard<std::mutex> lock(mutex_);
  auto result = alerts_;
  std::sort(result.begin(), result.end(), [](const Alert &a, const Alert &b) {
    if (static_cast<int>(a.priority) != static_cast<int>(b.priority)) {
      return static_cast<int>(a.priority) >
             static_cast<int>(b.priority); // High to low
    }
    return a.timestamp > b.timestamp; // New to old
  });
  return result;
}

bool AlertManager::clearAlert(int64_t timestamp, AlertType type) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = std::find_if(alerts_.begin(), alerts_.end(),
                         [timestamp, type](const Alert &a) {
                           return a.timestamp == timestamp && a.type == type;
                         });
  if (it == alerts_.end()) {
    return false;
  }
  alerts_per_type_[type]--;
  if (alerts_per_type_[type] == 0) {
    alerts_per_type_.erase(type);
  }
  alerts_.erase(it);
  return true;
}

void AlertManager::clearAllAlerts() {
  std::lock_guard<std::mutex> lock(mutex_);
  alerts_.clear();
  alerts_per_type_.clear();
}

std::string
AlertManager::getPredefinedMessage(AlertType type,
                                   const std::array<int, 2> &params) const {
  std::stringstream ss;
  switch (type) {
  case AlertType::BloodTemperature:
    ss << "High blood temperature: " << params[0] << "." << params[1];
    break;
  case AlertType::LowOxidation:
    ss << "Low oxidation level: " << params[0] << "%";
    break;
  case AlertType::LowBattery:
    ss << "Low battery: " << params[0] << "%";
    break;
  }
  return ss.str();
}

bool AlertManager::canAddAlert(AlertType type) const {
  // Use find to safely check the count without modifying the map
  size_t count = alerts_per_type_.find(type) != alerts_per_type_.end()
                     ? alerts_per_type_.at(type)
                     : 0;
  return alerts_.size() < MaxAlerts && count < MaxAlertsPerType;
}

} // namespace alert_manager