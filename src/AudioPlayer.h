#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <vector>
#include <iostream>

class AudioPlayer {
public:
    void playAudio(const std::string& filePath) {
        // Load audio file using libsndfile
        SF_INFO sfInfo;
        SNDFILE* audioFile = sf_open(filePath.c_str(), SFM_READ, &sfInfo);
        if (!audioFile) {
            std::cerr << "Failed to open audio file!" << std::endl;
            return;
        }

        // Initialize OpenAL
        ALCdevice* device = alcOpenDevice(nullptr);
        if (!device) {
            std::cerr << "Failed to open OpenAL device!" << std::endl;
            return;
        }

        ALCcontext* context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);

        // Create a buffer and source for OpenAL playback
        ALuint buffer, source;
        alGenBuffers(1, &buffer);
        alGenSources(1, &source);

        // Read the audio samples into a buffer
        std::vector<short> samples(sfInfo.frames * sfInfo.channels);
        sf_read_short(audioFile, samples.data(), samples.size());
        sf_close(audioFile);

        // Upload the data to OpenAL buffer
        ALenum format = (sfInfo.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        alBufferData(buffer, format, samples.data(), samples.size() * sizeof(short), sfInfo.samplerate);

        // Attach the buffer to the source
        alSourcei(source, AL_BUFFER, buffer);

        // Play the audio
        alSourcePlay(source);

        // Wait until the audio finishes playing
        ALint state;
        do {
            alGetSourcei(source, AL_SOURCE_STATE, &state);
        } while (state == AL_PLAYING);

        // Cleanup OpenAL resources
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
};

