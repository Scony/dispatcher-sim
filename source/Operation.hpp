#pragma once

struct Operation
{
  Operation(long long aId, long long aParentId, long long aName, long long aResult, long long aDuration) :
    id(aId),
    parentId(aParentId),
    name(aName),
    result(aResult),
    duration(aDuration)
  {
  }

  const long long id;
  const long long parentId;
  const long long name;
  const long long result;
  const long long duration;
};
