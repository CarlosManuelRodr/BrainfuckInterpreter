#ifndef BFINTERPRETER_H
#define BFINTERPRETER_H

#include <string>
#include <vector>

/*
 Some possible error codes that the interpreter will report in case something
 goes wrong. Currently only reports if there are unmatched brackets or the memory pointer
 reaches the end of memory.
*/
enum BF_RETURN_CODES
{
    OK, UNMATCHED_BRACKETS, END_OF_MEMORY
};

/*
 This is the translation layer between Brainfuck and some custom interface. Use it if you
 want to make your own Brainfuck-ish language.
*/

class BrainfuckInterface
{
    std::string p_increase, p_decrease, m_increase, m_decrease;
    std::string putchar, getchar, br_open, br_close;
    char separator;
    char sentence_end;
    bool m_first_is_uppercase, m_dot_at_end;
    
    static std::string DeleteComments(std::string input);
    static bool IsComment(char c);
    
public:
    BrainfuckInterface();
    bool IsComplete();
    std::string ToBrainfuck(std::string input);
    std::string ToInterface(std::string input);
    
    void SetPIncrease(std::string i);
    void SetPDecrease(std::string i);
    void SetMIncrease(std::string i);
    void SetMDecrease(std::string i);
    void SetPutchar(std::string i);
    void SetGetchar(std::string i);
    void SetBrOpen(std::string i);
    void SetBrClose(std::string i);
    void SetSeparator(char i);
    void SetSentenceEnd(char i);
    void SetOptions(bool first_is_uppercase, bool dot_at_end);
    
};

/*
 Simple Brainfuck interpreter. You set the code, run it, and get the output.
 If you want to get a live output or input you have to set the Putchar and Getchar functions.
*/

class BrainfuckInterpreter
{
    std::string m_program;
    std::string m_output;
    unsigned m_memory_size;
    char *m_memory;
    char *m_memory_pointer;
    const char *m_program_pointer;
    unsigned m_program_pointer_idx;
    unsigned m_memory_pointer_idx;
    char (*m_getchar_function)();
    void (*m_putchar_function)(char);
    
    // Auxiliary
    bool BracketsAreMatched();
    void DeleteComments();
    static bool IsComment(char c);
    
    
    // Brainfuck instructions 
    void PIncrease();
    void PDecrease();
    void MIncrease();
    void MDecrease();
    void Putchar();
    void Getchar();
    void BrOpen();
    void BrClose();
    
public:
    BrainfuckInterpreter(unsigned memory_size = 30000);
    BrainfuckInterpreter(std::string program, unsigned memory_size = 30000);
    ~BrainfuckInterpreter();
    void SetCode(std::string program);
    void SetPutcharFunction(void (*putchar_function)(char));
    void SetGetcharFunction(char (*getchar_function)());
    BF_RETURN_CODES Run();
    std::string GetOutput();
    std::vector<char> GetMemoryDump();
    unsigned GetMemoryPointer();
};

#endif