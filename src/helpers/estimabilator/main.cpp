#include <iostream>
#include <unordered_set>

int main()
{
  unsigned totalCases = 0;
  unsigned estimatedCases = 0;
  std::unordered_set<long long> knownCaseIds;

  long long firstNumber;
  std::cin >> firstNumber;

  auto isV1Instance = firstNumber != 0;
  if (isV1Instance)
  {
    long long suites = firstNumber;
    while (suites--)
    {
      long long skip;
      long long cases;
      std::cin >> skip >> skip >> skip >> skip >> cases;
      for (long long i = 0; i < cases; i++)
      {
        long long caseId;
        std::cin >> caseId >> skip >> skip >> skip;
        totalCases++;
        if (knownCaseIds.find(caseId) == knownCaseIds.end())
          knownCaseIds.insert(caseId);
        else
          estimatedCases++;
      }
    }
  }
  else
  {
    // TODO: implement
  }

  std::cout << std::fixed << ((double)estimatedCases / totalCases) << std::endl;

  return 0;
}
