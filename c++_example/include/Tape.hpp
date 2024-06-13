#include "Utilities.h"

class Tape
{

  public:
    Tape(char, std::set<char>);
    ~Tape();
    void InsertChain(std::string);
    std::string GetChain();
    int GetHeadPos();
    char GetCharAt(int);
    void MoveHead(char);
    void WriteChar(char);
    void Reset();

  private:
    std::vector<char> tape_chars;
    int head;
    char EPSILON;
    std::set<char> alphabet;
};
