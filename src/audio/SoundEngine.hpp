#ifndef PIXELVERSE_SOUNDENGINE_HPP
#define PIXELVERSE_SOUNDENGINE_HPP

#include <string>
#include <map>
#include <vector>

namespace pixelverse {
namespace audio {

enum class SoundEvent {
    ButtonClick,
    RewardUnlock,
    Victory,
    Error,
    Notification,
    Purchase,
    TournamentWin
};

class SoundEngine {
private:
    std::string assetsDir;
    std::map<SoundEvent, std::string> soundFiles;
    bool muted = false;

public:
    explicit SoundEngine(const std::string& assetsPath = "assets/audio");

    void initializeOfflineAssets();
    void playSound(SoundEvent event);
    void setMuted(bool mute) { muted = mute; }
    bool isMuted() const { return muted; }

    std::string getSoundFilePath(SoundEvent event) const;
    static std::string getEventName(SoundEvent event);
};

} // namespace audio
} // namespace pixelverse

#endif // PIXELVERSE_SOUNDENGINE_HPP
