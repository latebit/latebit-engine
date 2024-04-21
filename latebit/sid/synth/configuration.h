#pragma once

namespace sid {
class Configuration {
 private:
  static int bufferSize;
  static int sampleRate;

 public:
  static auto getBufferSize() -> int;
  static void setBufferSize(int bufferSize);

  static auto getSampleRate() -> int;
  static void setSampleRate(int sampleRate);
};
}  // namespace sid