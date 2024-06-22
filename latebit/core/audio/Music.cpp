#include "Music.h"

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>

#include "latebit/core/audio/AudioManager.h"
#include "latebit/sid/parser/TuneParser.h"
#include "latebit/utils/Logger.h"

using namespace std;
using namespace sid;

namespace lb {
auto Music::play(bool loop) const -> void {
  AM.playMusic(this->music.get(), loop);
}
auto Music::pause() const -> void { AM.pauseMusic(); }
auto Music::stop() const -> void { AM.stopMusic(); }
auto Music::getLabel() const -> string { return this->label; }

}  // namespace lb
