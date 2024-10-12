#include "Scene.h"

namespace lb {
  const int MAX_SCENES = 10;

  class SceneManager {
    private:
      vector<unique_ptr<Scene>> scenes = {};
    public:
      // Register a scene with the SceneManager
      void registerScene(unique_ptr<Scene> scene, const string label);
      
      // Unregister a scene with the SceneManager and frees all the resources associated with it
      void unregisterScene(const string label);

      // Activate a scene
      void activateScene(const string label);

      // Register a transition between two scenes when an event of type eventType occurs
      void registerTransition(const string from, const string to, const string eventType);
  };
}