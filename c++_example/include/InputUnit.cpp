#include "InputUnit.h"

InputUnit::InputUnit(std::string filename, bool debug)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        printf("Status -> Reading: OK\n");
    }
    else
    {
        std::cerr << "Status -> Reading: ERROR\n";
    }
    // BYE COMMENTS
    std::string aux;
    do
    {
        getline(file, aux);
    } while (aux[0] == '#');
    // STATES
    {
        std::stringstream ss(aux);
        std::string name;
        while (ss >> name)
        {
            information.state_names.push_back(name);
        }
    }
    aux.clear();
    getline(file, aux);
    // ALPHABET
    {
        std::stringstream ss(aux);
        char symbol;
        while (ss >> symbol)
        {
            information.alphabet.insert(symbol);
        }
    }
    aux.clear();
    getline(file, aux);
    // TAPE ALPHABET
    {
        std::stringstream ss(aux);
        char symbol;
        while (ss >> symbol)
        {
            information.tape_alphabet.insert(symbol);
        }
    }
    aux.clear();
    getline(file, aux);
    // INIT STATE
    {
        information.init_state = aux;
    }
    aux.clear();
    getline(file, aux);
    // EPSILON
    {
        information.EPSILON = aux[0];
    }
    aux.clear();
    getline(file, aux);
    // END STATES
    {
        std::stringstream ss(aux);
        std::string name;
        while (ss >> name)
        {
            information.end_states.push_back(name);
        }
    }
    aux.clear();
    getline(file, aux);
    // TRANSITIONS
    // TODO : TRANSITIONS
    // NOTE : FILL TRANSITION STRUCT WITH THE INFORMATION
    {
        do
        {
            std::stringstream ss(aux);
            InputTransition new_transition;
            ss >> new_transition.actual_state;
            ss >> new_transition.detected_char;
            ss >> new_transition.goto_state;
            ss >> new_transition.write_char;
            ss >> new_transition.MOVEMENT;
            information.transitions.push_back(new_transition);

        } while (getline(file, aux));
    }
    if (file.get() == EOF)
    {
        printf("Status -> Reading: finished\n");
    }

    if (debug)
    {
        Visualizer();
    }
}

void
InputUnit::Visualizer()
{
    std::string printer = "Status -> INFORMATION READED:\n";
    printer += "STATES:\n\t{|";
    for (auto name : information.state_names)
    {
        printer += name + "|";
    }
    printer += "}\n";

    printer += "ALPHABET:\n\t{|";
    for (char symbol : information.alphabet)
    {
        printer += symbol;
        printer += "|";
    }
    printer += "}\n";

    printer += "TAPE_ALPHABET:\n\t{|";
    for (char symbol : information.tape_alphabet)
    {
        printer += symbol;
        printer += "|";
    }
    printer += "}\n";

    printer += "INIT_STATE:\n\t{" + information.init_state + "}\n";

    printer += "EPSILON:\n\t{";
    printer += information.EPSILON;
    printer += "}\n";

    printer += "FINAL_STATES:\n\t{|";
    for (auto name : information.end_states)
    {
        printer += name + "|";
    }
    printer += "}\n";

    printer += "TRANSITIONS: \n";
    for (InputTransition transition : information.transitions)
    {
        printer += "\t{" + transition.actual_state + ", " +
                   transition.detected_char + ", " + transition.goto_state +
                   ", " + transition.write_char + ", " + transition.MOVEMENT +
                   "}\n";
    }

    printf("%s", printer.c_str());
}

InputInfo
InputUnit::GetInformation()
{
    return information;
}
