#include <iostream>
#include "Bot.hpp"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cerr << "Arguments error" << std::endl;
        return (1);
    }
    try
    {
        Bot kb_bot(argv[1], argv[2], argv[3], argv[4]);
        kb_bot.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}
