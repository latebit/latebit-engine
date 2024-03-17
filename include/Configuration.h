#include <array>
#include <string>

using namespace std;

namespace lb {
const int DEFAULT_FRAME_RATE = 60;
const string DEFAULT_TITLE = "LateBit";

const array<string, 2> CONFIG_KEYS = {"max_frame_rate", "initial_window_title"};

// This is a static class that can be used to set and retrieve
// configuration values from a file.
//
// Using a configuration file is optional, and calling the getters of this class
// will return some sensible defaults.
class Configuration {
 private:
  static int frameRate;
  static string title;

 public:
  // Returns the maximum frame rate for the game
  static auto getMaxFrameRate() -> int;
  // Returns the initial window title
  static auto getInitialWindowTitle() -> string;

  // Reads a configuration file and sets the values accordingly
  static auto fromFile(string filename) -> int;

  // Resets the configuration to its default values
  static auto reset() -> int;
};
}  // namespace lb