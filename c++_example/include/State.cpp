#include "State.h"

State::State(std::string n, bool f)
{
    name = n;
    final_state = f;
}

State::~State() {}

Transition *
State::FindTransition(char c)
{
    for (int i = 0; i < transitions.size(); ++i)
    {
        if (transitions[i].found_symbol == c)
        {
            return &transitions[i];
        }
    }
    return nullptr;
}

State *
State::ProcessSymbol(Tape *tape)
{
    char c = tape->GetCharAt(tape->GetHeadPos());
    Transition *t = FindTransition(c);
    if (t == nullptr)
    {
        return nullptr;
    }
    tape->WriteChar(t->to_tape);
    tape->MoveHead(t->MOVEMENT);
    return t->next_state;
}

void
State::Visualize()
{
    std::string printer = "";

    printer += name + " ";
    if (final_state)
    {
        printer += "(final_state) ";
    }
    printer += "Transitions: \n";
    for (Transition transition : transitions)
    {
        printer += "\t{|";
        printer += transition.found_symbol;
        printer += '|' + transition.next_state->GetName() + '|' +
                   transition.to_tape + '|' + transition.MOVEMENT + "|}\n";
    }
    printf("%s", printer.c_str());
}

void
State::Insert(Transition transition)
{
    transitions.push_back(transition);
}

void
State::SetFinal(bool f)
{
    final_state = f;
}

std::string
State::GetName()
{
    return name;
}

bool
State::TransitionsLeft()
{
    return !transitions.empty();
}

bool
State::IsFinal()
{
    return final_state;
}
