#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"


#include <glm/gtc/type_ptr.hpp>

#include <random>

// referenced the following codes/examples/documentation
// https://github.com/cmu15466-gsilvera/15-466-f22-base4/blob/main/PlayMode.cpp

Load< Sound::Sample > Beijing_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Beijing.opus"));
});

Load< Sound::Sample > Rich_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Rich.opus"));
});

Load< Sound::Sample > Happy_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Happy.opus"));
});

Load< Sound::Sample > Poker_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Poker.opus"));
});

Load< Sound::Sample > KFC_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("KFC.opus"));
});



PlayMode::PlayMode(){
	auto origin = Scene::Transform();
    camera = new Scene::Camera(&origin);

	current_game_state = 0;
	init_game_states();
	
	context_text.init();
	left_choice.init();
	right_choice.init();
	instruction.init();

	update_text();
}

PlayMode::~PlayMode() {
}

void PlayMode::init_game_states(){
	states.addState("Test of Chinese as a Foreign Language (*tocfl)", " ", " ", "Press Space Key to Continue", 0);
	states.addState("Volume Test: Qing Miao Shu Ni Ju Zhu De Cheng Shi", " ", " ", "Press Space Key to Continue", 0);
	states.addState("Listening Session", " ", " ", "Press Space Key to Continue", 0);
	states.addState("Where does the speaker come from?", "1: Beijing", "2: Shanghai", "Press 1 or 2 to Select and Space Key to Confirm", 1);
	states.addState("What does this greeting mean?", "1: Happy New Year!", "2: Hope You Get RICH!", "Press 1 or 2 to Select and Space Key to Confirm", 2);
	states.addState("What is the speaker saying?", "1. I'm sad.", "2. I'm happy!", "Press 1 or 2 to Select and Space Key to Confirm", 2);
	states.addState("What is the speaker playing?", "1. Poker", "2. Mahjong", "Press 1 or 2 to Select and Space Key to Confirm", 1);
	states.addState("Which restaurant will the speaker go to?", "1. KFC", "2. McDonald's", "Press 1 or 2 to Select and Space Key to Confirm", 1);
}

void PlayMode::update_text(){
	context_text.set_text(states.game_state[current_game_state].context);
	left_choice.set_text(states.game_state[current_game_state].left_text);
	right_choice.set_text(states.game_state[current_game_state].right_text);
	instruction.set_text(states.game_state[current_game_state].instruction);
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_1) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_2) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.downs += 1;
			space.pressed = true;
			to_next_state = true;
			return true;
		} 
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_1) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_2) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.pressed = false;
			return true;
		} 
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	} 

	return false;
}

void PlayMode::update(float elapsed) {

	if(to_next_state && space.pressed == false){
		if(current_game_state < states.game_state.size() - 1){
			if(choice == states.game_state[current_game_state].correct_choice){
				score += 1;
			}
			current_game_state += 1;
			sound_played = false;
			update_text();
			to_next_state = false;
		}
		else{
			context_text.showScore(score);
			left_choice.disappear();
			right_choice.disappear();
			instruction.disappear();
		}
	}

	if(left.pressed == true){
		left_choice.highlight();
		right_choice.reset();
		choice = 1;
	} else if(right.pressed == true){
		right_choice.highlight();
		left_choice.reset();
		context_text.showScore(score);
		choice = 2;
	}

	if(current_game_state == 3 && !sound_played){
		Beijing_sound = Sound::play(*Beijing_sample, 1.0f, 20.0f);
		sound_played = true;
	}

	if(current_game_state == 4 && !sound_played){
		Rich_sound = Sound::play(*Rich_sample, 1.0f, 20.0f);
		sound_played = true;
	}

	if(current_game_state == 5 && !sound_played){
		Happy_sound = Sound::play(*Happy_sample, 1.0f, 20.0f);
		sound_played = true;
	}

	if(current_game_state == 6 && !sound_played){
		Poker_sound = Sound::play(*Poker_sample, 1.0f, 20.0f);
		sound_played = true;
	}

	if(current_game_state == 7 && !sound_played){
		Poker_sound = Sound::play(*KFC_sample, 1.0f, 20.0f);
		sound_played = true;
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	space.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	{
        float x = drawable_size.x * 0.2f;
        float y = drawable_size.y * 0.3f;
        float width = drawable_size.x * 0.75f;
        left_choice.draw(drawable_size, width, glm::vec2(x, y), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

	{
        float x = drawable_size.x * 0.8f;
        float y = drawable_size.y * 0.3f;
        float width = drawable_size.x * 0.75f;
       	right_choice.draw(drawable_size, width, glm::vec2(x, y), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

	{
        float x = drawable_size.x * 0.5f;
        float y = drawable_size.y * 0.6f;
        float width = drawable_size.x * 0.75f;
        context_text.draw(drawable_size, width, glm::vec2(x, y), 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

	{
        float x = drawable_size.x * 0.5f;
        float y = drawable_size.y * 0.1f;
        float width = drawable_size.x * 0.75f;
        instruction.draw(drawable_size, width, glm::vec2(x, y), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
	GL_ERRORS();
}

