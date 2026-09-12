#include "../src/settings.h"
#include <afterhours/src/plugins/sound_system.h>
#include <cassert>

int main() {
  auto &settings = Settings::get();
  settings.update_music_volume(.8f);
  settings.update_sfx_volume(.3f);
  settings.update_master_volume(.5f);
  assert(afterhours::sound_system::get_master_volume() == .5f);
  assert(afterhours::sound_system::get_music_volume() == .8f);
  assert(afterhours::sound_system::get_sound_volume() == .3f);
  settings.update_master_volume(.2f);
  assert(settings.get_music_volume() == .8f);
  assert(settings.get_sfx_volume() == .3f);
  assert(afterhours::sound_system::get_music_volume() == .8f);
  assert(afterhours::sound_system::get_sound_volume() == .3f);
  settings.update_master_volume(2.f);
  settings.update_music_volume(-1.f);
  assert(afterhours::sound_system::get_master_volume() == 1.f);
  assert(afterhours::sound_system::get_music_volume() == 0.f);
}
