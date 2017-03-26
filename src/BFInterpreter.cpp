#include "BFInterpreter.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
using namespace std;

// Auxiliary: Split string to vector.
template<typename Out>
void split(const string &s, char delim, Out result) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}
vector<string> split(const string &s, char delim) {
    std::vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

// Interface
BrainfuckInterface::BrainfuckInterface()
{
    p_increase = p_decrease = m_increase = m_decrease = "";
    putchar = getchar = br_open = br_close  = "";
    separator = 0;
    m_first_is_uppercase = false;
    m_dot_at_end = false;
}
bool BrainfuckInterface::IsComplete()
{
    if (p_increase != "" && p_decrease != "" && m_increase != "" && m_decrease != ""
        && putchar != "" && getchar != "" && br_open != "" && br_close != "" && separator != 0)
        return true;
    else
        return false;
}
bool BrainfuckInterface::IsComment(char c)
{
    switch(c)
    {
        case '>': case '<': case '.': case ',': case '+': case '-': case '[': case ']':
            return false;
        default:
            break;
    }
    return true;
}
string BrainfuckInterface::DeleteComments(string input)
{
    input.erase(remove_if(begin(input), end(input), [](char c){return BrainfuckInterface::IsComment(c);}), end(input));
    return input;
}
string BrainfuckInterface::ToBrainfuck(string input)
{
    string output = "";
    
    // Delete extra spaces.
    for (unsigned i=0; i<input.length()-1; ++i)
    {
        if (input[i] == separator && input[i+1] == separator)
        {
            input.erase(i+1, 1);
            --i;
        }
    }
    if (input.back() == separator)
        input.pop_back();
    
    // To lowercase.
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    
    // Delete sentence end symbol.
    replace(input.begin(), input.end(), sentence_end, separator);
    
    // Get instructions.
    vector<string> instructions = split(input, separator);
    
    // Translate to Brainfuck.
    for (unsigned i=0; i<instructions.size(); ++i)
    {
        if (instructions[i] == p_increase)
            output += '>';
        else if (instructions[i] == p_decrease)
            output += '<';
        else if (instructions[i] == m_increase)
            output += '+';
        else if (instructions[i] == m_decrease)
            output += '-';
        else if (instructions[i] == putchar)
            output += '.';
        else if (instructions[i] == getchar)
            output += ',';
        else if (instructions[i] == br_open)
            output += '[';
        else if (instructions[i] == br_close)
            output += ']';
    }
    
    return output;    
}
string BrainfuckInterface::ToInterface(string input)
{
    input = this->DeleteComments(input);
    string output = "";
    for (unsigned i=0; i<input.length(); ++i)
    {
        switch (input[i])
        {
        case '>':
            output += p_increase;
            break;
        case '<':
            output += p_decrease;
            break;
        case '+':
            output += m_increase;
            break;
        case '-':
            output += m_decrease;
            break;
        case '.':
            output += putchar;
            break;
        case ',':
            output += getchar;
            break;
        case '[':
            output += br_open;
            break;
        case ']':
            output += br_close;
            break;
        }
        
        if (i+1 != input.length())
            output += separator;
    }
    
    if (m_dot_at_end)
        output += ".";
    
    if (m_first_is_uppercase && output.length() > 1)
        output[0] = toupper(output[0]);
    
    return output;
}

void BrainfuckInterface::SetPIncrease(string i)
{
    p_increase = i;
}
void BrainfuckInterface::SetPDecrease(string i)
{
    p_decrease = i;
}
void BrainfuckInterface::SetMIncrease(string i)
{
    m_increase = i;
}
void BrainfuckInterface::SetMDecrease(string i)
{
    m_decrease = i;
}
void BrainfuckInterface::SetPutchar(string i)
{
    putchar = i;
}
void BrainfuckInterface::SetGetchar(string i)
{
    getchar = i;
}
void BrainfuckInterface::SetBrOpen(string i)
{
    br_open = i;
}
void BrainfuckInterface::SetBrClose(string i)
{
    br_close = i;
}
void BrainfuckInterface::SetSeparator(char i)
{
    separator = i;
}
void BrainfuckInterface::SetSentenceEnd(char i)
{
    sentence_end = i;
}
void BrainfuckInterface::SetOptions(bool first_is_uppercase, bool dot_at_end)
{
    m_first_is_uppercase = first_is_uppercase;
    m_dot_at_end = dot_at_end;
}

// Interpreter
BrainfuckInterpreter::BrainfuckInterpreter(string program, unsigned memory_size)
{
    m_getchar_function = nullptr;
    m_putchar_function = nullptr;
    m_memory_size = memory_size;
    m_memory = new char[memory_size];
    m_program = program;
    m_program_pointer = m_program.c_str();
    m_memory_pointer = m_memory;
    m_output = "";
    m_program_pointer_idx = 0;
    m_memory_pointer_idx = 0;
    this->DeleteComments();
    
    for (unsigned i=0; i<memory_size; ++i)
        m_memory[i] = 0;
}
BrainfuckInterpreter::BrainfuckInterpreter(unsigned memory_size)
{
    m_getchar_function = nullptr;
    m_putchar_function = nullptr;
    m_memory_size = memory_size;
    m_memory = new char[memory_size];
    m_program = "";
    m_program_pointer = m_program.c_str();
    m_memory_pointer = m_memory;
    m_output = "";
    m_program_pointer_idx = 0;
    m_memory_pointer_idx = 0;
    
    for (unsigned i=0; i<memory_size; ++i)
        m_memory[i] = 0;
}
BrainfuckInterpreter::~BrainfuckInterpreter()
{
    delete[] m_memory;
}
void BrainfuckInterpreter::SetCode(std::string program)
{
    m_program = program;
    this->DeleteComments();
}
void BrainfuckInterpreter::SetGetcharFunction(char (*getchar_function)())
{
    m_getchar_function = getchar_function;
}
void BrainfuckInterpreter::SetPutcharFunction(void (*putchar_function)(char))
{
    m_putchar_function = putchar_function;
}

// Auxiliary
bool BrainfuckInterpreter::BracketsAreMatched()
{
    int depth = 0;
    for (unsigned i=0; i<m_program.length(); ++i)
    {
        if (m_program[i] == '[') depth++;
        if (m_program[i] == ']') depth--;
    }
    
    if (depth == 0)
        return true;
    else
        return false;
}
bool BrainfuckInterpreter::IsComment(char c)
{
    switch(c)
    {
        case '>': case '<': case '.': case ',': case '+': case '-': case '[': case ']':
            return false;
        default:
            break;
    }
    return true;
}
void BrainfuckInterpreter::DeleteComments()
{
    m_program.erase(std::remove_if(std::begin(m_program), std::end(m_program), [](char c){return BrainfuckInterpreter::IsComment(c);}), std::end(m_program));
}
vector<char> BrainfuckInterpreter::GetMemoryDump()
{
    return vector<char>(m_memory, m_memory + m_memory_size);
}

// Brainfuck instructions 
void BrainfuckInterpreter::PIncrease()
{
    ++m_memory_pointer;
}
void BrainfuckInterpreter::PDecrease()
{
    --m_memory_pointer;
}
void BrainfuckInterpreter::MIncrease()
{
    ++(*m_memory_pointer);
}
void BrainfuckInterpreter::MDecrease()
{
    --(*m_memory_pointer);
}
void BrainfuckInterpreter::Putchar()
{
    m_output += *m_memory_pointer;
    if (m_putchar_function != nullptr)
        m_putchar_function(*m_memory_pointer);
}
void BrainfuckInterpreter::Getchar()
{
    if (m_getchar_function != nullptr)
    {
        *m_memory_pointer = m_getchar_function(); 
   }
}
void BrainfuckInterpreter::BrOpen()
{
    int depth = 1;
    if (*m_memory_pointer == 0)
    {
        do
        {
            ++m_program_pointer;
            if (*m_program_pointer == '[')
            {
                ++depth;
            }
            else if (*m_program_pointer == ']')
            {
                --depth;
            }
        }
        while (depth != 0);
    }
}
void BrainfuckInterpreter::BrClose()
{
    int depth = 0;
    if (*m_memory_pointer != 0)
    {
        do
        {
            if (*m_program_pointer == '[')
            {
                ++depth;
            }
            else if (*m_program_pointer == ']')
            {
                --depth;
            }
            --m_program_pointer;
        }
        while (depth != 0);
    }
}
BF_RETURN_CODES BrainfuckInterpreter::Run()
{
    if (!this->BracketsAreMatched())
        return UNMATCHED_BRACKETS;
    
    while (*m_program_pointer != '\0')
    {        
        if (m_memory_pointer_idx == m_memory_size)
        {
            m_program_pointer_idx = m_program_pointer - m_program.c_str();
            m_memory_pointer_idx = m_memory_pointer - m_memory;
            return END_OF_MEMORY;
        }
        
        switch (*m_program_pointer)
        {
        case '>':
            this->PIncrease();
            break;
        case '<':
            this->PDecrease();
            break;
        case '+':
            this->MIncrease();
            break;
        case '-':
            this->MDecrease();
            break;
        case '.':
            this->Putchar();
            break;
        case ',':
            this->Getchar();
            break;
        case '[':
            this->BrOpen();
            break;
        case ']':
            this->BrClose();
            break;
        }
        
        ++m_program_pointer;
    }
    
    m_program_pointer_idx = m_program_pointer - m_program.c_str();
    m_memory_pointer_idx = m_memory_pointer - m_memory;
    
    return OK;
}
std::string BrainfuckInterpreter::GetOutput()
{
    return m_output;
}
unsigned BrainfuckInterpreter::GetMemoryPointer()
{
    return m_memory_pointer_idx;
}