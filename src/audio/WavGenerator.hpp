#ifndef PIXELVERSE_WAVGENERATOR_HPP
#define PIXELVERSE_WAVGENERATOR_HPP

#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstdint>

namespace pixelverse {
namespace audio {

class WavGenerator {
public:
    static bool generateWavFile(const std::string& filepath, const std::vector<int16_t>& samples, int sampleRate = 44100) {
        std::ofstream out(filepath, std::ios::binary);
        if (!out.is_open()) return false;

        uint32_t dataSize = samples.size() * sizeof(int16_t);
        uint32_t chunkSize = 36 + dataSize;
        uint16_t audioFormat = 1; // PCM
        uint16_t numChannels = 1; // Mono
        uint32_t byteRate = sampleRate * numChannels * sizeof(int16_t);
        uint16_t blockAlign = numChannels * sizeof(int16_t);
        uint16_t bitsPerSample = 16;
        uint32_t subchunk1Size = 16;

        // RIFF header
        out.write("RIFF", 4);
        out.write(reinterpret_cast<const char*>(&chunkSize), 4);
        out.write("WAVE", 4);

        // fmt subchunk
        out.write("fmt ", 4);
        out.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
        out.write(reinterpret_cast<const char*>(&audioFormat), 2);
        out.write(reinterpret_cast<const char*>(&numChannels), 2);
        out.write(reinterpret_cast<const char*>(&sampleRate), 4);
        out.write(reinterpret_cast<const char*>(&byteRate), 4);
        out.write(reinterpret_cast<const char*>(&blockAlign), 2);
        out.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

        // data subchunk
        out.write("data", 4);
        out.write(reinterpret_cast<const char*>(&dataSize), 4);
        out.write(reinterpret_cast<const char*>(samples.data()), dataSize);

        return true;
    }

    static std::vector<int16_t> generateTone(double freq, double durationSec, double volume = 0.5, int sampleRate = 44100) {
        size_t totalSamples = static_cast<size_t>(durationSec * sampleRate);
        std::vector<int16_t> samples(totalSamples);
        double twoPiF = 2.0 * 3.141592653589793 * freq;

        for (size_t i = 0; i < totalSamples; ++i) {
            double t = static_cast<double>(i) / sampleRate;
            // Envelope (attack, decay)
            double env = 1.0;
            if (t < 0.01) env = t / 0.01;
            else env = 1.0 - (t / durationSec);
            if (env < 0) env = 0;

            double val = std::sin(twoPiF * t) * volume * env;
            samples[i] = static_cast<int16_t>(val * 32767.0);
        }
        return samples;
    }

    static std::vector<int16_t> generateArpeggio(const std::vector<double>& freqs, double noteDurationSec, double volume = 0.5, int sampleRate = 44100) {
        std::vector<int16_t> totalSamples;
        for (double f : freqs) {
            auto note = generateTone(f, noteDurationSec, volume, sampleRate);
            totalSamples.insert(totalSamples.end(), note.begin(), note.end());
        }
        return totalSamples;
    }
};

} // namespace audio
} // namespace pixelverse

#endif // PIXELVERSE_WAVGENERATOR_HPP
