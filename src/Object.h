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
  int m_altitude;

 public:
  Object();

  virtual ~Object();

  void setId(int id);
  int getId() const;

  void setType(std::string t);
  std::string getType() const;

  void setPosition(Vector p);
  Vector getPosition() const;

  void setAltitude(int a);
  int getAltitude() const;

  virtual int eventHandler(const Event *p_e);
  virtual int draw();
};
}  // namespace df

#endif