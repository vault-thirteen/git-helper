#include <iostream>

#include "std.hpp"

void print_repository_folder(const char* infp)
{
    std::cout << "Repository folder:" << std::endl;
    std::cout << "\033[38;5;220m" << infp << "\033[0m" << std::endl << std::endl;
}
void print_count(const char* object_name, size_t count)
{
    std::cout << std::endl << "Total " << object_name << " count: ";
    std::cout << "\033[38;5;46m" << count << "\033[0m" << std::endl;
}
bool ask_to_proceed()
{
    std::cout << "Would you like to proceed ? (yes / no): ";

    std::string userAnswer;
    std::cin >> userAnswer;
    userAnswer = toLower(userAnswer);
    if (userAnswer == "yes")
    {
        return true;
    }

    return false;
}
void show_usage_A()
{
    std::cout << "Usage: <executable> <folder> <action>" << std::endl;
}
void show_usage_B()
{
    std::cout << "Usage: <executable> <folder> <action> <repository URL>" << std::endl;
}
