#pragma once

#include <chrono>

// Simple chrono based timer (asw no longer provides one)
class Timer {
 public:
  void start() {
    t1 = std::chrono::high_resolution_clock::now();
    t2 = t1;
    running = true;
  }

  void stop() {
    t2 = std::chrono::high_resolution_clock::now();
    running = false;
  }

  bool isRunning() const { return running; }

  void reset() {
    t1 = std::chrono::high_resolution_clock::now();
    t2 = t1;
  }

  template <typename Precision>
  double getElapsedTime() {
    if (running) {
      t2 = std::chrono::high_resolution_clock::now();
    }

    return std::chrono::duration_cast<Precision>(t2 - t1).count();
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> t1{
      std::chrono::high_resolution_clock::now()};
  std::chrono::time_point<std::chrono::high_resolution_clock> t2{
      std::chrono::high_resolution_clock::now()};
  bool running{false};
};
