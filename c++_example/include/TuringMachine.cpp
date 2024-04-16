#include "TuringMachine.h"
#include <__msvc_chrono.hpp>
#include <thread>

TuringMachine::TuringMachine(InputInfo information, std::string turing_name,
                             bool debug)
{
    // Atributes without loops
    {
        name = turing_name;
        alphabet = information.alphabet;
        init_state_name = information.init_state;
    }

    // States
    {
        for (std::string s : information.state_names)
        {
            State new_state(s, false);
            state_directions.push_back(new_state);
        }

        for (std::string final_state_name : information.end_states)
        {
            for (State &state : state_directions)
            {
                if (state.GetName() == final_state_name)
                {
                    state.SetFinal(true);
                }
            }
        }
    }

    // Inserting transitions block
    {
        for (InputTransition tr : information.transitions)
        {
            State *actual_state = FindState(tr.actual_state);
            Transition new_transition{.found_symbol = tr.detected_char,
                                      .next_state = FindState(tr.goto_state),
                                      .to_tape = tr.write_char,
                                      .MOVEMENT = tr.MOVEMENT};
            actual_state->Insert(new_transition);
        }
    }

    for (State &state : state_directions)
    {
        state.Visualize();
    }

    // Tape
    {
        tape = new Tape(information.EPSILON, information.tape_alphabet);
    }
}

TuringMachine::~TuringMachine() {}

bool
TuringMachine::Run(std::string chain, bool debug)
{
    actual_state = FindState(init_state_name);
    tape->InsertChain(chain);
    if (debug)
    {
        vc_debug = new VariadicTable<std::string, char, std::string, char>(
            {"CHAIN", "CHAR", "STATE", "MOVEMENT"}, 10);
    }
    while (actual_state->TransitionsLeft())
    {
        actual_state = actual_state->ProcessSymbol(tape);
        if (actual_state == nullptr)
        {
            return false;
        }
        if (debug)
        {
            DebugVisualizer();
        }
        else
        {
            ExecutionVisualizer();
        }
    }
    if (actual_state->IsFinal())
    {
        return true;
    }
    return false;
}

// NOTE : USAR STATIC PARA NO REDEFINIR
void
TuringMachine::DebugVisualizer()
{

    char the_CHAR = tape->GetCharAt(tape->GetHeadPos());
    Transition *debug_transition = actual_state->FindTransition(the_CHAR);
    /**CHAIN, DETECTED, CHAR, MOVEMENT*/
    char MOVEMENT;
    if (debug_transition != nullptr)
    {
        MOVEMENT = debug_transition->MOVEMENT;
    }
    else
    {
        MOVEMENT = '.';
    }
    vc_debug->addRow(GetHeadPosStr(), ' ', GetHeadPosStrState(), ' ');
    vc_debug->addRow(tape->GetChain(), the_CHAR, GetStatesStr(), MOVEMENT);
    vc_debug->addRow("", ' ', "", ' ');

    std::this_thread::sleep_for(166.66ms);
    system(CLEAR_SCREEN);
    vc_debug->print(std::cout);
}

void
TuringMachine::ExecutionVisualizer()
{
    // TODO : The following example
    /************************************************************
     *                 NAME-OF-THE-TURING-MACHINE               *
     *                             qx                           *
     *                          v                               *
     *                         .STRING.                         *
     ************************************************************/
    VariadicTable<std::string, std::string> vc({name, "ACTUAL_STATE"}, 10);
    vc.addRow(GetHeadPosStr(), GetHeadPosStrState());
    vc.addRow(tape->GetChain(), GetStatesStr());

    std::this_thread::sleep_for(16.66ms);
    system(CLEAR_SCREEN);
    vc.print(std::cout);
}

std::string
TuringMachine::GetStatesStr()
{
    std::string states_str = "";
    for (State state : state_directions)
    {
        states_str += state.GetName() + " ";
    }
    states_str.pop_back();
    return states_str;
}

std::string
TuringMachine::GetHeadPosStr()
{
    std::string head_pos = "";
    int idx = tape->GetHeadPos();
    for (int i = 0; i < idx; ++i)
    {
        head_pos += " ";
    }
    head_pos += "v";
    return head_pos;
}

std::string
TuringMachine::GetHeadPosStrState()
{
    std::string head_pos = "";
    int idx = 0;
    for (; idx < state_directions.size(); ++idx)
    {
        if (state_directions[idx].GetName() == actual_state->GetName())
        {
            break;
        }
    }

    std::string head = "vv";
    for (int i = 0; i < idx; ++i)
    {
        head_pos += "   ";
    }

    head_pos += head;

    return head_pos;
}

State *
TuringMachine::FindState(std::string find_name)
{
    for (State &state : state_directions)
    {
        if (state.GetName() == find_name)
        {
            return &state;
        }
    }
    throw "NO STATE NAMED: " + find_name;
}
