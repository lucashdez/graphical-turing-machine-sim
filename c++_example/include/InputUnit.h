/**
 * INPUT UNIT
 */
#include "Utilities.h"

/**
 * This struct has the transitions as defined in the input
 */
struct InputTransition
{
    /** The name of the state we are in */
    std::string actual_state;
    /** The char that our head reads */
    char detected_char;
    /** The name of the state we are going */
    std::string goto_state;
    /** The char we leave in the tape */
    char write_char;
    /** The movement that the head must do */
    char MOVEMENT;
};

/**
 * This struct has all the information needed for the turing machine to exist
 */
struct InputInfo
{
    /** A vector with the names of the states */
    std::vector<std::string> state_names;
    /** A set with the characters that we are allowed to read */
    std::set<char> alphabet;
    /** A set with the characters we can write in the tape */
    std::set<char> tape_alphabet;
    /** The name of the initial state */
    std::string init_state;
    /** The ANSII character we're using for the epsilon symbol */
    char EPSILON;
    /** A vector with the names of the acceptance states */
    std::vector<std::string> end_states;
    /** A vector with the information for the transitions */
    std::vector<InputTransition> transitions;
};

/**
 * The class that is used to read all the input file and to get the information
 * to the main program for the turing machine to do it.
 */
class InputUnit
{
  public:
    /**
     * The constructor of the class opens the file and assigns all the
     * information to the {information} atribute
     *
     * @param {string} The name of the input file
     * @param the boolean for the debugging info
     */
    InputUnit(std::string, bool);

    /**
     * This function allows us to visualize the information provided by the
     * reader
     */
    void Visualizer();
    /** This function provide an access to the copy of the information inside
     * the Input Unit
     *
     * @return  {InputInfo} A copy private atribute
     */
    InputInfo GetInformation();

  private:
    InputInfo information;
};
