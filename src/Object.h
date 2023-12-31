#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <string>

#include "Event.h"
#include "Vector.h"

namespace df {
class Object {
 private:
  int m_id;
  std::string m_type;
  Vector m_position;

 public:
  Object();

  virtual ~Object();

  void setId(int id);
  int getId() const;

  void setType(std::string t);
  std::string getType() const;

  void setPosition(Vector p);
  Vector getPosition() const;

  virtual int eventHandler(const Event *p_e);
};
}  // namespace df

#endif