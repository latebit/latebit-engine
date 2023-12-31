#include <string>
namespace df {
const std::string UNDEFINED_EVENT = "df::undefined";

class Event {
 private:
  std::string m_event_type;

 public:
  Event();
  virtual ~Event();
  void setType(std::string t);
  std::string getType() const;
};
}  // namespace df
