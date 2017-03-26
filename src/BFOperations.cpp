#include "BFOperations.h"
using namespace std;

// Implementation of simple Brainfuck algorithms.
string bf_sum(char n)
{
    string output = "";
    for (char i=0; i<n; ++i)
        output += '+';
    return output;
}
string bf_multiply(char a, char b)
{
    string output = "";
    output += bf_sum(a);
    output += "[>";
    output += bf_sum(b);
    output += "<-]";
    return output;
}
string bf_write_char(char n)
{
    string output = "";
    if (n == 32)
    {
        output += bf_multiply(4, 8);
        output += ">.[-]<";
    }
    if (n == 46)
    {
        output += bf_multiply(5, 9);
        output += ">+.[-]<";
    }
    else if (n > 64 && n <= 96)
    {
        output += bf_multiply(5, 13);
        output += ">";
        output += bf_sum(n-65);
        output += ".[-]<";
    }
    else if (n > 96)
    {
        output += bf_multiply(8, 12);
        output += ">";
        output += bf_sum(n-96);
        output += ".[-]<";
    }
    return output;
}
string text_to_bf(string text)
{
    string output = "";
    for (unsigned i=0; i<text.length(); ++i)
        output += bf_write_char(text[i]);
    
    return output;
}