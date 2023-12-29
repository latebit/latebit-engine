#include "utils.h"

#include <cstdio>
#include <ctime>
#include <thread>

char *getTimeString() {
  static char time_str[30];

  time_t now;
  time(&now);
  struct tm *p_localtime = localtime(&now);

  sprintf(time_str, "%02d:%02d:%02d", p_localtime->tm_hour, p_localtime->tm_min,
          p_localtime->tm_sec);

  return time_str;
}

void sleep(long int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}