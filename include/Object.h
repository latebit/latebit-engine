#pragma once

#include <array>
#include <string>

#include "Animation.h"
#include "Box.h"
#include "Event.h"
#include "Vector.h"

using namespace std;

namespace lb {
enum Solidness {
  HARD,
  SOFT,
  SPECTRAL,
};

const int MAX_EVENTS_PER_OBEJCT = 20;

class Object {
 private:
  // Unique id of this object.
  int id;

  // A string representing the type of object.
  string type = "Object";

  // Position of the object in World coordinates.
  Vector position = Vector();

  // Rendering layer for this object.
  int altitude = 0;

  // Where this object is moving.
  Vector direction = Vector();

  // Speed in cells per frame.
  float speed = 0;

  // Solidness of the object:
  // - SPECTRAL won't collide;
  // - SOFT collides but does not impede movement;
  // - HARD collides and impedes movement.
  Solidness solidness = HARD;

  // The animation for this object.
  Animation animation = Animation();

  // Box defining the bounds of this object.
  Box boundingBox = Box(1, 1);

  // True if the engine interacts with this object.
  bool active = true;

  // True if this object will be drawn.
  bool visible = true;

  // If true it displays debugging information.
  bool debug = false;

  // List of events this object is subscribed to.
  array<string, MAX_EVENTS_PER_OBEJCT> events = {};
  // Current number of subscribed events
  int eventCount = 0;

 public:
  Object();

  virtual ~Object();

  // Returns the unique id of this object.
  [[nodiscard]] auto getId() const -> int;

  // Sets the type identifier of this object.
  void setType(string t);
  // Returns the type identifier of this object.
  [[nodiscard]] auto getType() const -> string;

  // Sets the position of this object in the game world.
  void setPosition(Vector p);
  // Returns the position of this object in the game world.
  [[nodiscard]] auto getPosition() const -> Vector;

  // Sets the the rendering layer for this object.
  void setAltitude(int a);
  // Returns the rendering layer of this object.
  [[nodiscard]] auto getAltitude() const -> int;

  // Sets the direction of the movement this object.
  void setDirection(Vector d);
  // Returns the direction of the movement of this object.
  [[nodiscard]] auto getDirection() const -> Vector;

  // Sets the speed of this object in cells per frame.
  void setSpeed(float s);
  // Returns the speed of this object in cells per frame.
  [[nodiscard]] auto getSpeed() const -> float;

  // Sets the velocity vector of this object.
  void setVelocity(Vector v);
  // Returns the velocity vector of this object.
  [[nodiscard]] auto getVelocity() const -> Vector;

  // Returns the predicted position of this object in the next frame.
  [[nodiscard]] auto predictPosition() -> Vector;

  // Returns true if this object is not SPECTRAL.
  [[nodiscard]] auto isSolid() const -> bool;
  // Sets the solidness of this object.
  void setSolidness(Solidness s);
  // Returns the solidness of this object.
  [[nodiscard]] auto getSolidness() const -> Solidness;

  // Sets the animation for this object.
  void setAnimation(Animation a);
  // Returns the animation for this object.
  [[nodiscard]] auto getAnimation() const -> Animation;

  // Sets the debug flag for this object.
  void setDebug(bool debug);
  // Returns the debug flag for this object.
  [[nodiscard]] auto getDebug() const -> bool;

  // Assign a loaded sprite to this object
  auto setSprite(string label) -> int;

  // Set bounding box for this object.
  void setBox(Box box);
  // Returns bounding box for this object.
  [[nodiscard]] auto getBox() const -> Box;
  // Draw bounding box around this object.
  [[nodiscard]] auto drawBoundingBox() const -> int;

  // Returns bounding box in world coordinates relative to this object's
  // position.
  [[nodiscard]] auto getWorldBox() const -> Box;
  // Returns bounding box in world coordinates relative to center.
  [[nodiscard]] auto getWorldBox(Vector center) const -> Box;

  // Subscribe to event, e.g., "COLLISION" or "KEYBOARD".
  auto subscribe(string eventType) -> int;
  // Unsubscribe to event, e.g., "COLLISION" or "KEYBOARD".
  auto unsubscribe(string eventType) -> int;

  // Handle event (default is to ignore everything).
  virtual auto eventHandler(const Event *e) -> int;
  // Draw single sprite frame and bounding box (if debug).
  virtual auto draw() -> int;

  // Set object to be active or not active.
  auto setActive(bool active = true) -> int;
  // Return true if object is active, else false.
  [[nodiscard]] auto isActive() const -> bool;

  // Set object to be visible or invisible.
  auto setVisible(bool visible = true) -> int;
  // Return true if object is visible, else false.
  [[nodiscard]] auto isVisible() const -> bool;

  // Represent the object as a string
  [[nodiscard]] auto toString() const -> string;
};
}  // namespace lb
