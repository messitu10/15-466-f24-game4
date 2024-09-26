#pragma once

#include <string>
#include <iostream>
#include <vector>


// referenced the following codes/examples/documentation

// https://github.com/cmu15466-gsilvera/15-466-f22-base4/blob/main/Story.hpp

struct State {
    const std::string context;
    const std::string left_text, right_text, instruction;
    uint32_t correct_choice;
    State(const std::string& text, 
          const std::string& left_choice, 
          const std::string& right_choice, 
          const std::string& instruction,
          const uint32_t correct)
        : context(text)
        , left_text(left_choice)
        , right_text(right_choice)
        , instruction(instruction)
        , correct_choice(correct)
    {
    }
};

struct GameState{
    std::vector<State> game_state;
    
    void addState(const std::string& context, const std::string& left_choice,
                const std::string& right_choice, const std::string& instruction, 
                uint32_t correct_choice) {
        game_state.emplace_back(context, left_choice, right_choice, instruction, correct_choice);
    }
};



