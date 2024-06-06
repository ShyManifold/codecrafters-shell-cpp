#include <iostream>
#include <sstream>
#include <string>

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // You can use print statements as follows for debugging, they'll be visible when running tests.
  // std::cout << "Logs from your program will appear here!\n";
  bool finished = false;

  while (!finished)
  {

    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    std::cout << input << ": command not found" << std::endl;

    if (input == "exit 0")
      finished = true;
  }
  return 0;
}
