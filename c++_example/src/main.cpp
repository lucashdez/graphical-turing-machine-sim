#include "../include/TuringMachine.h"
#include <thread>

void
Usage()
{
    std::string printer = "";
    printer += "USAGE:\n";
    printer += "\t build/a.out [TuringMachineName] [Debug Mode] "
               "[chain_to_analyze] [chain_to_analyze] ...\n";
}

int
main(int argc, char **argv)
{
    if (argc < 4)
    {
        Usage();
        return -1;
    }
    std::string file = argv[1];
    bool debug;
    if (std::string(argv[2]) == "1")
    {
        debug = true;
    }
    else
    {
        debug = false;
    }
    std::string path_file = "machines/" + file;
    InputUnit new_input(path_file, debug);
    if (debug)
    {
        printf("Status -> Press Enter to start processing...\n");
        std::cin.get();
        system(CLEAR_SCREEN);
    }
    TuringMachine machine(new_input.GetInformation(), file, debug);
    printf("Status -> Press Enter to start processing...\n");
    std::cin.get();

    std::string printer = "";
    std::vector<std::string> chains;
    for (int i = 3; i < argc; ++i)
    {
        chains.push_back(std::string(argv[i]));
    }
    system(CLEAR_SCREEN);
    for (std::string chain : chains)
    {
        if (debug)
        {
            printf("Pass to the next chain?: %s (PRESS ENTER)\n",
                   chain.c_str());
            std::cin.get();
        }
        printf("Going to analyze: %s\n", chain.c_str());
        std::this_thread::sleep_for(1s);
        bool result = machine.Run(chain, debug);
        printer += chain + ": ";
        if (result)
        {
            printer += "ACCEPTED\n";
        }
        else
        {
            printer += "DENIED\n";
        }
    }

    printf("%s", printer.c_str());
}
