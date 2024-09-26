#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"
#include "Text.hpp"
#include "State.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, space;

	//music coming from the tip of the leg (as a demonstration):
	std::shared_ptr< Sound::PlayingSample > Beijing_sound;
	std::shared_ptr< Sound::PlayingSample > Rich_sound;
	std::shared_ptr< Sound::PlayingSample > Happy_sound;
	std::shared_ptr< Sound::PlayingSample > Poker_sound;
	std::shared_ptr< Sound::PlayingSample > KFC_sound;
	bool sound_played;

	uint32_t score = 0;
	uint32_t choice = 3;

	GameState states;
	uint32_t current_game_state;
	bool to_next_state;

	void init_game_states();
	void update_text();

	Text context_text, left_choice, right_choice, instruction;
	
	//camera:
	Scene::Camera *camera = nullptr;

};
