#include <vector>
#include <cassert>

#include "Input.hpp"
#include "Cloud.hpp"
#include "Simulator.hpp"
#include "DispatcherFactory.hpp"

int main(int argc, char ** argv)
{
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++)
    args.push_back(argv[i]);

  assert(argc >= 2 + 1);

  srand(time(0));

  auto input = std::make_shared<Input>();
  auto cloud = std::make_shared<Cloud>();
  auto dispatcherFactory = std::make_shared<DispatcherFactory>(std::vector<std::string>(args.begin()+2, args.end()));
  auto dispatcher = dispatcherFactory->getDispatcher();

  input->readFromStdin();
  cloud->resize(std::stoi(args[1]));

  Simulator simulator(input, cloud, dispatcher);
  simulator.run();

  return 0;
}
