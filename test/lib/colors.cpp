#include "colors.h"

auto supportsEscapeSequences() -> bool {
  const char* term = std::getenv("TERM");
  if (term == nullptr) {
    return false;
  }

  std::string termStr(term);
  return termStr == "xterm" || termStr == "xterm-256color" ||
         termStr == "screen" || termStr == "screen-256color" ||
         termStr == "tmux" || termStr == "tmux-256color" || termStr == "rxvt" ||
         termStr == "rxvt-unicode" || termStr == "rxvt-unicode-256color";
}

auto color(std::string color, std::string s) -> std::string {
  if (supportsEscapeSequences()) {
    return "\033[" + color + "m" + s + "\033[0m";
  }

  return s;
}

auto red(std::string s) -> std::string { return color("31", s); }
auto green(std::string s) -> std::string { return color("32", s); }
auto yellow(std::string s) -> std::string { return color("33", s); }