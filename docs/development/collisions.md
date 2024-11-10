Physics
---

Latebit supports Kinematic physics, that means there is no concept of force (although we have Acceleration) and collisions are resolved by displacing objects that would be overlapping.

## What participates in the physics simulation?

The physics engine operates on [Objects](../../latebit/core/world/Object.h) and it's currently managed by the [WorldManager](../../latebit/core/world/WorldManager.h).
The parameters of the Objects utilized by the simulation are:

* **Box** the axis-aligned bounding box representing the shape of the object.
* **Position** the position of the object in World coordinates.
* **Velocity** the rate of change of position in a given direction in cells per second.
* **Acceleration** the rate of change of velocity in a given direction in cells per squared second.
* **Solidness** whether the object participates in overlaps and collisions.

Go ahead and check the headers for more details.

## What's the difference between overlap and collision?

The physics engine is only interested in _solid_ objects, namely `HARD` or `SOFT` objects. `SPECTRAL` objects will always move freely, regardless of other objects around them.

Whenever two solid objects occupy the same region in space an [Overlap](../../latebit/core/events/EventOverlap.h) occurs. That is, SOFT-SOFT, HARD-HARD, and HARD-SOFT will all yield an Overlap event.

If the two solid objects are `HARD` then a [Collision](../../latebit/core/events/EventCollision.h) occurs. The objects respond to the collision by pushing each other our of the collision along the minimum displacement axis.

## Why kinematics only?

We took a stab at [introducing impulse-based physics simulation](https://github.com/latebit/latebit-engine/commit/815e900adbf53d7778aaaa6fee60817ad585aeb4) but the result was much harder to understand and manage from the developer's perspective. Simple behaviours like platforming or jumping required too many knobs to tweak and yielded much less predictable results.

On the other hand, we now have a [Physics](../../latebit/core/world/Physics.h) interface and we can make the physics engine pluggable: developers can choose whether they want Kinematic, Dynamic, and so forth.