#include <vector>
#include <memory>

#include "Operation.hpp"
#include "Job.hpp"

namespace Solution
{
  using Solution = std::vector<std::pair<long long, std::shared_ptr<Operation> > >;

  void validate(std::vector<std::shared_ptr<Job> > jobs, std::shared_ptr<Solution> solution);
  std::vector<std::pair<long long, std::shared_ptr<Job> > > calculateFlow(std::vector<std::shared_ptr<Job> > jobs,
									  std::shared_ptr<Solution> solution);
}
