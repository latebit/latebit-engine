#include <string>

int assert(const std::string name, bool assertion, const std::string message);
int assert_string(std::string name, std::string want, std::string got);
int assert_regex(std::string name, std::string want, std::string pattern);