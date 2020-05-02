#include "SoundModule.h"
#include <iostream>

using namespace Aftr;

// Init function
SoundModule* SoundModule::init() {
	SoundModule * sound = new SoundModule();
	irrklang::ISoundEngine* isound_engine = irrklang::createIrrKlangDevice();
	if (isound_engine == NULL) {
		std::cout << "Sound engine not initialized\n";
	}
	sound->set_sound_engine(isound_engine);
	return sound;
}

// Functions for 2D sounds
void SoundModule::play_sound_2D(const char* file, bool loop, bool pause, bool track) {
	if (!this->isound_engine) {
		std::cout << "No sound engine (play 2D)\n";
		return;
	}
	this->sound_2D.push_back(this->isound_engine->play2D(file, loop, pause, track));
}

void SoundModule::stop_sound_2D() {
	if (sound_2D.empty())
		return;
	sound_2D.at(0)->stop();
	sound_2D.erase(sound_2D.begin());
}

std::vector<irrklang::ISound*> SoundModule::get_sound_2D() {
	return this->sound_2D;
}


// Functions for 3D sounds
void SoundModule::play_sound_3D(const char* file, Vector position, bool loop, bool pause, bool track) {
	if (!this->isound_engine) {
		std::cout << "No sound engine (play 3D)\n";
		return;
	}
	this->sound_3D.push_back(this->isound_engine->play3D(file, to_vec3df(position), loop, pause, track));
}


void SoundModule::stop_sound_3D() {
	if (sound_3D.empty())
		return;
	sound_3D.at(0)->stop();
	sound_3D.erase(sound_3D.begin());
}

std::vector<irrklang::ISound*> SoundModule::get_sound_3D() {
	return this->sound_3D;
}

// Sets the engine
void SoundModule::set_sound_engine(irrklang::ISoundEngine* isound_engine) {
	this->isound_engine = isound_engine;
}

// Gets the engine
irrklang::ISoundEngine* SoundModule::get_sound_engine() {
	return this->isound_engine;
}

// Vector conversion
irrklang::vec3df SoundModule::to_vec3df(Vector position) {
	return irrklang::vec3df(position.x, position.y, position.z);
}
