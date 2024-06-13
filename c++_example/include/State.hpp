#include "Tape.h"
class State;

struct Transition
{
    char found_symbol;
    State *next_state;
    char to_tape;
    char MOVEMENT;
};

class State
{
  public:
    /**
     *
     *
     */
    State(std::string, bool);

    /**
     *
     *
     *
     */
    ~State();

    /**
     *
     *
     */
    Transition *FindTransition(char);

    /**
     *
     *
     *
     */
    std::string GetName();

    /**
     *
     *
     *
     */
    State *ProcessSymbol(Tape *);

    /**
     *
     *
     *
     */
    void Visualize();

    void Insert(Transition);
    void SetFinal(bool);

    bool TransitionsLeft();

    bool IsFinal();

  private:
    /***/
    std::string name;
    /***/
    bool final_state;
    /***/
    std::vector<Transition> transitions;
};
