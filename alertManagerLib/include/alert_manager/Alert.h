#pragma once
#include <array>
#include <cstdint>
#include <string>

namespace alert_manager {

enum class Priority { Low, Medium, High, Critical };
enum class AlertType { BloodTemperature, LowOxidation, LowBattery };

struct Alert {
  int64_t timestamp; // Unix timestamp (seconds)
  Priority priority;
  AlertType type;
  std::string message;       // Predefined message with params
  std::array<int, 2> params; // Up to 2 integer parameters

  bool operator==(const Alert &other) const;
};

} // namespace alert_manager