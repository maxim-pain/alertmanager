#pragma once
#include "Alert.h"
#include <mutex>
#include <unordered_map>
#include <vector>

namespace alert_manager {

class AlertManager {
public:
  static constexpr size_t MaxAlerts = 20;
  static constexpr size_t MaxAlertsPerType = 10;

  AlertManager();

  // Add or update an alert; returns true if successful
  bool addAlert(const Alert &alert);

  // Update priority of an alert by type and timestamp; returns true if found
  bool updatePriority(int64_t timestamp, AlertType type, Priority newPriority);

  // Get all alerts sorted by priority (high to low) and timestamp (new to old)
  std::vector<Alert> getAlerts() const;

  // Clear a specific alert by timestamp and type; returns true if found
  bool clearAlert(int64_t timestamp, AlertType type);

  // Clear all alerts
  void clearAllAlerts();

  // Get predefined message for an alert type with parameters
  std::string getPredefinedMessage(AlertType type,
                                   const std::array<int, 2> &params) const;

private:
  std::vector<Alert> alerts_;
  std::unordered_map<AlertType, size_t> alerts_per_type_;
  mutable std::mutex mutex_; // For thread safety

  // Check if adding an alert is allowed (within limits)
  bool canAddAlert(AlertType type) const;
};

} // namespace alert_manager