#include <vector>
#include <cassert>

#include "Input.hpp"
#include "Cloud.hpp"
#include "Simulator.hpp"

int main(int argc, char ** argv)
{
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++)
    args.push_back(argv[i]);

  assert(argc >= 2 + 1);

  srand(time(0));

  auto input = std::make_shared<Input>();
  auto cloud = std::make_shared<Cloud>(); // pass std::stoi(args[1])
  // dispatcher factory // pass args[2:]

  input->readFromStdin();

  Simulator simulator(input, cloud);
  simulator.run();

  return 0;
}
