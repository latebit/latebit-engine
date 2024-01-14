#include "../include/EventOut.h"

#include "test.h"

auto EventOut_test() -> int {
  int result = 0;

  // test constructor
  df::EventOut event_out;
  result += assert_string("df::EventOut::getType()", event_out.getType(),
                          df::OUT_EVENT);

  return result;
};