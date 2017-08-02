#pragma once

class Fuzzy3
{
public:
  Fuzzy3(long long a, long long b, long long c);
  // TODO: disable copying

  const long long operator[](unsigned n);
  bool operator<(Fuzzy3 const& other);
  Fuzzy3& operator+=(Fuzzy3 const& other);

  double dominationOver(Fuzzy3 const& other) const;

private:
  static long long nextUuid;

  long long uuid;
  long long mNth[3];
};
