#include "InputUnit.h"
#include "State.h"

class TuringMachine
{
  public:
    /**
     * The constructor of the class.
     * It instantiates all the states, transitions of the Turing machine and
     * sets it to the possibility of running
     *
     * @param {InputInfo} the information that the Input Unit collected
     */
    TuringMachine(InputInfo, std::string, bool);
    /** Destructor moment "yujiro hanma skull" */
    ~TuringMachine();

    /** This function analyzes the chain given and tells if it is accepted or
     * not
     *
     * @param {string} The chain to analyze
     * @param {bool} The debug option
     */
    bool Run(std::string, bool);

    /**
     * This function finds the state with the name that we are
     * searching
     *
     * @param {string} The name of the state we are searching
     */
    State *FindState(std::string);

    /**
     * This function shows all the information and the process of the Turing
     * machine while it's running at a 500ms sleep
     */
    void DebugVisualizer();

    /**
     * This function shows the execution of the machine
     */
    void ExecutionVisualizer();

    /**
     * Some deubug functions
     */
    std::string GetStatesStr();
    std::string GetHeadPosStrState();
    std::string GetHeadPosStr();

  private:
    std::string init_state_name;
    std::set<char> alphabet;
    Tape *tape;
    State *actual_state;
    std::string name;
    std::vector<State> state_directions;
    VariadicTable<std::string, char, std::string, char> *vc_debug;
};
