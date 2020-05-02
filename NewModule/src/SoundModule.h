#pragma once
#include "../thirdParty/irrKlang/irrKlang.h"
#include "../thirdParty/irrKlang/ik_ISoundEngine.h"
#include "../../../include/aftr/Vector.h"

namespace Aftr {
	class SoundModule {
		public:
			static SoundModule* init();
			virtual void play_sound_2D(const char* file, bool loop, bool pause, bool track);
			virtual void stop_sound_2D();
			virtual void play_sound_3D(const char* file, Aftr::Vector position, bool loop, bool pause, bool track);
			virtual void stop_sound_3D();
			std::vector<irrklang::ISound*> get_sound_2D();
			std::vector<irrklang::ISound*> get_sound_3D();
			void SoundModule::set_sound_engine(irrklang::ISoundEngine* isound_engine);
			irrklang::ISoundEngine* get_sound_engine();
			irrklang::vec3df to_vec3df(Vector position);
		protected:
			irrklang::ISoundEngine * isound_engine = nullptr;
			std::vector<irrklang::ISound*> sound_2D;
			std::vector<irrklang::ISound*> sound_3D;
	};
}