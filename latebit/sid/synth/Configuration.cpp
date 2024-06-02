#include "Configuration.h"

#include "Wavetable.h"

using namespace sid;
using namespace std;

int Configuration::bufferSize = 4096;
int Configuration::sampleRate = 44100;

auto Configuration::getBufferSize() -> int { return bufferSize; }
auto Configuration::getSampleRate() -> int { return sampleRate; }
void Configuration::setBufferSize(int bufferSize) {
  Configuration::bufferSize = bufferSize;
  WaveTable::createWaveTable(bufferSize);
}
void Configuration::setSampleRate(int sampleRate) {
  Configuration::sampleRate = sampleRate;
}