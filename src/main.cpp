#include <iostream>
#include <cstdlib>
#include <fstream>
#include "optionparser.h"
#include "BFInterpreter.h"
#include "BFOperations.h"
using namespace std;

/**********************
*                     *
* Interface functions *
*                     *
**********************/

char get_char()
{
    return (char)getchar();
}
void put_char(char c)
{
    putchar((int)c);
}


/**********************
*                     *
*   Argument parser   *
*                     *
**********************/

struct Arg : public option::Arg
{
    static void PrintError(const char* msg1, const option::Option& opt, const char* msg2)
    {
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
    }

    static option::ArgStatus Required(const option::Option& option, bool msg)
    {
        if (option.arg != 0)
            return option::ARG_OK;

        if (msg) PrintError("Option '", option, "' requires an argument.\n");
        return option::ARG_ILLEGAL;
    }
};

enum  OptionIndex {
    UNKNOWN, RUN, TEXT_INPUT, INPUT_FILE, OUTPUT_FILE, ENCODE, USE_INTERFACE, TO_INTERFACE, TO_BRAINFUCK, HELP
};

const option::Descriptor usage[] =
{
    { UNKNOWN, 0, "", "",Arg::None, "INSTRUCTIONS: BFInterpreter [options]\n" },
    { RUN, 0, "r", "run", Arg::None, "  -r, \t--run  \tRun code." },
    { ENCODE, 0, "e", "encode", Arg::None, "  -e <arg>, \t--encode  \tEncode text." },
    { TO_INTERFACE, 0, "g", "to_interface", Arg::None, "  -g <arg>, \t--to_interface  \tTranslate Brainfuck to interface language." },
    { TO_BRAINFUCK, 0, "b", "to_brainfuck", Arg::None, "  -b <arg>, \t--to_brainfuck  \tTranslate interface language to Brainfuck." },
    
    { TEXT_INPUT, 0, "t", "text_input", Arg::Required, "  -t <arg>, \t--text_input=<arg>  \tGive input as text parameter. Can be code or phrase to translate or encode." },
    { INPUT_FILE, 0, "i", "input", Arg::Required, "  -i <arg>, \t--input=<arg>  \tInput file." },
    { OUTPUT_FILE, 0, "o", "output", Arg::Required, "  -o <arg>, \t--output=<arg>  \tOutput file." },
    { USE_INTERFACE, 0, "c", "custom_interface", Arg::None, "  -c <arg>, \t--custom_interface  \tUse custom coding interface." },
    { HELP,    0,"", "help", Arg::None,    "  \t--help  \tShow instructions." },
    { 0,0,0,0,0,0 }
};

int write_to_file(string input, string file)
{
    ofstream out_file(file);
    if (out_file.is_open())
    {
        out_file << input;
        return 0;
    }
    else
    {
        cout << "Error: Can't create output file." << endl;
        return 1;
    }
}

int main(int argc, char** argv)
{
    // Execute argument parser.
    argc -= (argc > 0); argv += (argc > 0);
    option::Stats  stats(usage, argc, argv);
    option::Option *options = new option::Option[stats.options_max];
    option::Option *buffer = new option::Option[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);
    
    // Variables.
    string input_file = "", output_file = "", text_input = "";
    bool run = false;
    bool encode = false;
    bool use_interface = false;
    bool to_interface = false;
    bool to_brainfuck = false;
    unsigned selections = 0;

    
    if (parse.error())
        return 1;

    if (options[HELP] || argc == 0)
    {
        option::printUsage(cout, usage);
        return 0;
    }
    
    for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
        cout << "Opcion desconocida: " << opt->name << "\n";
    
    for (int i = 0; i < parse.optionsCount(); ++i)
    {
        option::Option& opt = buffer[i];
        switch (opt.index())
        {
        case RUN:
            run = true;
            ++selections;
            break;
        case ENCODE:
            encode = true;
            ++selections;
            break;
        case TO_INTERFACE:
            to_interface = true;
            ++selections;
            break;
        case TO_BRAINFUCK:
            to_brainfuck = true;
            ++selections;
            break;
        case TEXT_INPUT:
            text_input = opt.arg;
            break;
        case INPUT_FILE:
            input_file = opt.arg;
            break;
        case OUTPUT_FILE:
            output_file = opt.arg;
            break;
        case USE_INTERFACE:
            use_interface = true;
            break;
        }
    }

    delete[] options;
    delete[] buffer;
    
    // Check for conflicts.
    if (selections == 0)
    {
        cout << "Error: No task specified." << endl;
        return 1;
    }
    if (selections > 1)
    {
        cout << "Error: Conflicting options." << endl;
        return 1;
    }
    if (text_input != "" && input_file != "")
    {
        cout << "Error: Conflicting inputs given." << endl;
        return 1;
    }
    
    // Load file
    if (input_file != "")
    {
        ifstream file_stream(input_file);
        if (file_stream.is_open())
        {
            string file_content((istreambuf_iterator<char>(file_stream)), (istreambuf_iterator<char>()));
            text_input = file_content;
        }
        else
        {
            cout << "Error: Can't open file." << endl;
            return 1;
        }
    }
    if (text_input.length() == 0)
    {
        cout << "Error: Input is empty." << endl;
        return 1;
    }
    
    // Set up interface.
    BrainfuckInterface bi;
    bi.SetPIncrease("this");
    bi.SetPDecrease("is");
    bi.SetMIncrease("a");
    bi.SetMDecrease("simple");
    bi.SetPutchar("custom");
    bi.SetGetchar("brainfuck");
    bi.SetBrOpen("-ish");
    bi.SetBrClose("language");
    bi.SetSeparator(' ');
    bi.SetSentenceEnd('.');
    bi.SetOptions(true, true);
    
    // Execute options.
    int result = 0;
    if (run)
    {
        if (use_interface)
            text_input = bi.ToBrainfuck(text_input);
            
        BrainfuckInterpreter bf(text_input);
        bf.SetPutcharFunction(&put_char);
        bf.SetGetcharFunction(&get_char);
        result = bf.Run();
        cout << endl;
    }
    if (encode)
    { 
        string code = text_to_bf(text_input);
        
        if (use_interface)
            code = bi.ToInterface(code);
        
        if (output_file.length() > 0)
            result = write_to_file(code, output_file);
        else
            cout << code << endl;
    }
    if (to_interface)
    {
        string translated = bi.ToInterface(text_input);
        
        if (output_file.length() > 0)
            result = write_to_file(translated, output_file);
        else
            cout << translated << endl;
    }
    if (to_brainfuck)
    {
        string translated = bi.ToBrainfuck(text_input);
        
        if (output_file.length() > 0)
            result = write_to_file(translated, output_file);
        else
            cout << translated << endl;
    }
    
    return 0;
}