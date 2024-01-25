namespace Engine{

	struct AudioClip {
		SDL_AudioSpec audioSpec;
		Uint8* audioBuffer;
		Uint32 audioLength;
		Uint32 audioPosition;

		AudioClip() : audioBuffer(nullptr), audioLength(0), audioPosition(0) {}
	};

	class AudioSystem {
	public:
		static bool initialize();
		static void cleanup();
		static bool loadSound(const std::string& filePath, AudioClip& audioClip);
		static void play(const std::string& filePath);
		static void waitAll();

	private:
		static bool isInitialized;
		static std::vector<AudioClip> audioClips;

		static SDL_AudioDeviceID deviceID;

		static void audioCallback(void* userdata, Uint8* stream, int len);
	};

}