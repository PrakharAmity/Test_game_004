#include "SoundEngine.hpp"
#include "WavGenerator.hpp"
#include "../utils/Logger.hpp"
#include <fstream>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif

namespace pixelverse {
namespace audio {

static bool fileExists(const std::string& path) {
    std::ifstream f(path.c_str());
    return f.good();
}

SoundEngine::SoundEngine(const std::string& assetsPath) : assetsDir(assetsPath) {
    soundFiles[SoundEvent::ButtonClick] = assetsDir + "/click.wav";
    soundFiles[SoundEvent::RewardUnlock] = assetsDir + "/reward.wav";
    soundFiles[SoundEvent::Victory] = assetsDir + "/victory.wav";
    soundFiles[SoundEvent::Error] = assetsDir + "/error.wav";
    soundFiles[SoundEvent::Notification] = assetsDir + "/notification.wav";
    soundFiles[SoundEvent::Purchase] = assetsDir + "/purchase.wav";
    soundFiles[SoundEvent::TournamentWin] = assetsDir + "/tournament_win.wav";
}

void SoundEngine::initializeOfflineAssets() {
    // Ensure directory exists
#ifdef _WIN32
    CreateDirectoryA("assets", NULL);
    CreateDirectoryA("assets/audio", NULL);
#endif

    // Generate click sound (crisp 800Hz transient)
    if (!fileExists(soundFiles[SoundEvent::ButtonClick])) {
        auto samples = WavGenerator::generateTone(800.0, 0.05, 0.4);
        WavGenerator::generateWavFile(soundFiles[SoundEvent::ButtonClick], samples);
    }

    // Generate reward unlock (bright ascending arpeggio C5 -> E5 -> G5 -> C6)
    if (!fileExists(soundFiles[SoundEvent::RewardUnlock])) {
        std::vector<double> notes = {523.25, 659.25, 783.99, 1046.50};
        auto samples = WavGenerator::generateArpeggio(notes, 0.12, 0.5);
        WavGenerator::generateWavFile(soundFiles[SoundEvent::RewardUnlock], samples);
    }

    // Generate victory fanfare (G4 -> C5 -> E5 -> G5)
    if (!fileExists(soundFiles[SoundEvent::Victory])) {
        std::vector<double> notes = {392.00, 523.25, 659.25, 783.99};
        auto samples = WavGenerator::generateArpeggio(notes, 0.18, 0.5);
        WavGenerator::generateWavFile(soundFiles[SoundEvent::Victory], samples);
    }

    // Generate error sound (descending buzz 220Hz -> 110Hz)
    if (!fileExists(soundFiles[SoundEvent::Error])) {
        std::vector<double> notes = {220.00, 164.81, 110.00};
        auto samples = WavGenerator::generateArpeggio(notes, 0.1, 0.6);
        WavGenerator::generateWavFile(soundFiles[SoundEvent::Error], samples);
    }

    // Generate notification ping (two-tone 880Hz -> 1320Hz)
    if (!fileExists(soundFiles[SoundEvent::Notification])) {
        std::vector<double> notes = {880.0, 1320.0};
        auto samples = WavGenerator::generateArpeggio(notes, 0.08, 0.4);
        WavGenerator::generateWavFile(soundFiles[SoundEvent::Notification], samples);
    }

    // Generate purchase sound (coin clink 987Hz -> 1318Hz)
    if (!fileExists(soundFiles[SoundEvent::Purchase])) {
        std::vector<double> notes = {987.77, 1318.51};
        auto samples = WavGenerator::generateArpeggio(notes, 0.15, 0.5);
        WavGenerator::generateWavFile(soundFiles[SoundEvent::Purchase], samples);
    }

    // Generate tournament win (triumphant brass celebration)
    if (!fileExists(soundFiles[SoundEvent::TournamentWin])) {
        std::vector<double> notes = {523.25, 523.25, 523.25, 659.25, 783.99, 1046.50};
        auto samples = WavGenerator::generateArpeggio(notes, 0.14, 0.6);
        WavGenerator::generateWavFile(soundFiles[SoundEvent::TournamentWin], samples);
    }
}

void SoundEngine::playSound(SoundEvent event) {
    if (muted) return;
    std::string path = getSoundFilePath(event);
    utils::Logger::debug("Playing Sound: " + getEventName(event) + " [" + path + "]");

#ifdef _WIN32
    PlaySoundA(path.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
#endif
}

std::string SoundEngine::getSoundFilePath(SoundEvent event) const {
    auto it = soundFiles.find(event);
    if (it != soundFiles.end()) {
        return it->second;
    }
    return "";
}

std::string SoundEngine::getEventName(SoundEvent event) {
    switch (event) {
        case SoundEvent::ButtonClick: return "ButtonClick";
        case SoundEvent::RewardUnlock: return "RewardUnlock";
        case SoundEvent::Victory: return "Victory";
        case SoundEvent::Error: return "Error";
        case SoundEvent::Notification: return "Notification";
        case SoundEvent::Purchase: return "Purchase";
        case SoundEvent::TournamentWin: return "TournamentWin";
    }
    return "Unknown";
}

} // namespace audio
} // namespace pixelverse
