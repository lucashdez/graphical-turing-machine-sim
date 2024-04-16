#include "Tape.h"

Tape::Tape(char epsilon, std::set<char> tape_alphabet)
    : head(1), EPSILON(epsilon), alphabet(tape_alphabet)
{
}

void
Tape::Reset()
{
    head = 1;
    tape_chars.clear();
}

Tape::~Tape() {}

void
Tape::InsertChain(std::string chain)
{
    Reset();
    tape_chars.push_back(EPSILON);
    for (char c : chain)
    {
        tape_chars.push_back(c);
    }
    tape_chars.push_back(EPSILON);
}

std::string
Tape::GetChain()
{
    std::string result = "";

    for (char c : tape_chars)
    {
        result += c;
    }
    return result;
}

int
Tape::GetHeadPos()
{
    return head;
}

char
Tape::GetCharAt(int pos)
{
    return tape_chars[pos];
}

void
Tape::WriteChar(char to_write)
{
    tape_chars[head] = to_write;
}

void
Tape::MoveHead(char movement)
{
    switch (movement)
    {

        case 'R':
        {
            head++;
        }
        break;
        case 'L':
        {
            head--;
        }
        break;
        default:
            return;
    }
    if (head == -1)
    {
        head++;
        tape_chars.insert(tape_chars.begin(), '.');
    }
    if (head == tape_chars.size())
    {
        tape_chars.push_back('.');
    }
}
