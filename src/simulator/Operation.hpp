#pragma once

#include <memory>

struct Operation
{
  Operation(long long aId,
	    long long aParentId,
	    long long aName,
	    long long aResult,
	    long long aArrival,
	    long long aDuration,
	    long long aCapacityReq = 1) :
    id(aId),
    parentId(aParentId),
    name(aName),
    result(aResult),
    arrival(aArrival),
    duration(aDuration),
    capacityReq(aCapacityReq)
  {
  }

  const long long id;
  const long long parentId;
  const long long name;
  const long long result;
  const long long arrival;
  const long long duration;
  const long long capacityReq;
};

using OperationSP = std::shared_ptr<Operation>;
