#include <iostream>

#include "args.hpp"

#include "Arguments.hpp"

Arguments::Arguments(int argc, char ** argv)
{
  args::ArgumentParser parser("Test workload dispatcher simulator");

  args::HelpFlag help(parser, "help", "Display this help and exit",
		      {'h', "help"});

  args::ValueFlag<unsigned> machinesArg(parser, "number", "Number of machines",
					{'m', "machines"});
  args::ValueFlag<unsigned> saIterationsArg(parser, "number", "Number of SA iterations",
                                            {'i', "iterations"});
  args::ValueFlag<std::string> estimationArg(parser, "method", "Estimation method",
					     {'e', "estimation"});
  args::ValueFlag<std::string> opAlgortihmArg(parser, "op_algorithm", "Operation-level algorithm",
					      {'l', "operation-level-algorithm"});
  args::ValueFlag<std::string> outputArg(parser, "type", "Output type",
					 {'o', "output-type"});
  args::ValueFlag<unsigned> setupTimeArg(parser, "number", "Operations setup time",
					 {'s', "setup-time"});
  args::ValueFlag<unsigned> instanceVersionArg(parser, "number", "Instance file version",
                                               {'v', "version"});
  args::ValueFlag<unsigned> kArg(parser, "number", "K-Recent's window size",
				 {'k', "window-size"});
  args::ValueFlag<std::string> representationArg(parser, "representation", "Internal solution representation",
                                                 {'r', "representation"});

  args::Positional<std::string> algorithmArg(parser, "algorithm", "Primary algorithm");

  try
  {
    parser.ParseCLI(argc, argv);
    if (!algorithmArg)
      throw args::Help("");
  }
  catch (args::Help)
  {
    std::cout << parser;
    exit(0);
  }
  catch (args::ParseError e)
  {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  primaryAlgorithm = args::get(algorithmArg);
  machinesNum = machinesArg ? args::get(machinesArg) : 1;
  estimationMethod = estimationArg ? args::get(estimationArg) : "no";
  operationLevelAlgorithm = opAlgortihmArg ? args::get(opAlgortihmArg) : "random";
  saIterations = saIterationsArg ? args::get(saIterationsArg) : 1;
  k = kArg ? args::get(kArg) : 3;
  setupTime = setupTimeArg ? args::get(setupTimeArg) : 0;
  outputType = outputArg ? args::get(outputArg) : "jflows";
  instanceVersion = instanceVersionArg ? args::get(instanceVersionArg) : 1;
  representation = representationArg ? args::get(representationArg) : "queue";
}
