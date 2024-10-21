#pragma once

#include <string>

#include "latebit/core/events/Event.h"
#include "latebit/core/events/EventTarget.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Animation.h"

using namespace std;

namespace lb {
namespace Solidness {
enum Solidness {
  // Collides and impedes movement.
  HARD,
  // Collides but does not impede movement.
  SOFT,
  // Skip collision detection.
  SPECTRAL,
};
}

const int MAX_EVENTS_PER_OBEJCT = 20;

class SceneGraph;

class Object : public EventTarget {
 private:
  // Unique id of this object.
  int id = 0;

  // A string representing the type of object.
  string type = "Object";

  // Pointer to the scene graph this object belongs to.
  SceneGraph* sceneGraph;

  // Position of the object in World coordinates.
  Vector position = Vector();

  // Rendering layer for this object.
  int altitude = 0;

  // Velocity vector of this object.
  Vector velocity = Vector();

  // Acceleration vector of this object.
  Vector acceleration = Vector();

  // Solidness of the object
  Solidness::Solidness solidness = Solidness::HARD;

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

  // Scale of the object. 1 is normal size, 2 is double size, etc.
  uint8_t scale = 1;

 public:
  // Create an object with a default type.
  Object();

  // Create an object with a type.
  Object(const string& type);

  virtual ~Object() = default;

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

  void setScale(uint8_t scale);
  [[nodiscard]] auto getScale() const -> uint8_t;

  // Sets the the rendering layer for this object.
  void setAltitude(int a);
  // Returns the rendering layer of this object.
  [[nodiscard]] auto getAltitude() const -> int;

  // Sets the velocity vector of this object.
  // For any practical purposes, it is recommended to clamp the components of
  // the velocity vector to a range that makes sense for your game.
  void setVelocity(Vector v);
  // Returns the velocity vector of this object.
  [[nodiscard]] auto getVelocity() const -> Vector;

  // Sets the acceleration vector of this object.
  // For any practical purposes, it is recommended to clamp the components of
  // the acceleration vector to a range that makes sense for your game.
  void setAcceleration(Vector v);
  // Returns the acceleration vector of this object.
  [[nodiscard]] auto getAcceleration() const -> Vector;

  // Returns true if this object is not SPECTRAL.
  [[nodiscard]] auto isSolid() const -> bool;
  // Sets the solidness of this object.
  void setSolidness(Solidness::Solidness s);
  // Returns the solidness of this object.
  [[nodiscard]] auto getSolidness() const -> Solidness::Solidness;

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

  // Handle event (default is to ignore everything).
  auto eventHandler(const Event* e) -> int override;
  // Draw single sprite frame and bounding box (if debug).
  virtual auto draw() -> int;
  // Called just before object is destroyed. Can be overridden to cleanup
  // resources.
  virtual void teardown() {};

  // Set object to be active or not active.
  auto setActive(bool active = true) -> int;
  // Return true if object is active, else false.
  [[nodiscard]] auto isActive() const -> bool override;

  // Set object to be visible or invisible.
  auto setVisible(bool visible = true) -> int;
  // Return true if object is visible, else false.
  [[nodiscard]] auto isVisible() const -> bool;

  // Represent the object as a string
  [[nodiscard]] auto toString() const -> string;
};
}  // namespace lb
