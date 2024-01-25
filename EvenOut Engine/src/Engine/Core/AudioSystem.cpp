#include "epch.h"
#include "AudioSystem.h"

namespace Engine {

	bool AudioSystem::isInitialized = false;
	std::vector<AudioClip> AudioSystem::audioClips;
	SDL_AudioDeviceID AudioSystem::deviceID;

	bool AudioSystem::initialize() {
		if (isInitialized) {
			return true; // Already initialized
		}

		if (SDL_Init(SDL_INIT_AUDIO) < 0) {
			std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
			return false;
		}

		SDL_AudioSpec desiredSpec;
		desiredSpec.freq = 44100;
		desiredSpec.format = SDL_AUDIO_S16;
		desiredSpec.channels = 2;

		if (SDL_OpenAudioDevice(deviceID, &desiredSpec) < 0) {
			std::cerr << "SDL audio initialization error: " << SDL_GetError() << std::endl;
			SDL_Quit();
			return false;
		}

		isInitialized = true;
		return true;
	}

	void AudioSystem::cleanup() {
		if (isInitialized) {
			for (auto& clip : audioClips) {
				SDL_free(clip.audioBuffer);
			}
			SDL_CloseAudioDevice(deviceID);
			SDL_Quit();
			isInitialized = false;
		}
	}

	bool AudioSystem::loadSound(const std::string& filePath, AudioClip& audioClip) {
		if (!isInitialized) {
			std::cerr << "AudioSystem not initialized" << std::endl;
			return false;
		}

		if (SDL_LoadWAV(filePath.c_str(), &audioClip.audioSpec, &audioClip.audioBuffer, &audioClip.audioLength) == nullptr) {
			std::cerr << "Error loading WAV file: " << SDL_GetError() << std::endl;
			return false;
		}

		audioClips.push_back(audioClip);
		return true;
	}

	void AudioSystem::play(const std::string& filePath) {
		if (!isInitialized) {
			std::cerr << "AudioSystem not initialized" << std::endl;
			return;
		}

		AudioClip audioClip;
		if (loadSound(filePath, audioClip)) {
			SDL_QueueAudio(deviceID, audioClip.audioBuffer, audioClip.audioLength);
			SDL_PauseAudioDevice(deviceID); // Unpause audio playback
		}
	}

	void AudioSystem::waitAll() {
		while (SDL_GetQueuedAudioSize(0) > 0) {
			SDL_Delay(100);
		}
	}

	void AudioSystem::audioCallback(void* userdata, Uint8* stream, int len) {
		// This callback is required but can be left empty for simple playback
	}

}