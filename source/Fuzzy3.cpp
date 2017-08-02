#include <cassert>

#include "Fuzzy3.hpp"

long long Fuzzy3::nextUuid = 0;

Fuzzy3::Fuzzy3(long long a, long long b, long long c)
{
  mUuid = nextUuid++;
  mNth[0] = a;
  mNth[1] = b;
  mNth[2] = c;
}

const long long Fuzzy3::operator[](unsigned n)
{
  assert(n < 3);
  return mNth[n];
}

// Nakamura '86 - http://www.sciencedirect.com/science/article/pii/0165011486900746
double Fuzzy3::dominationOver(Fuzzy3 const& other) const
{
  auto const& a = mNth;
  auto const& b = other.mNth;

  assert(a[0] <= a[1] && a[1] <= a[2]);
  assert(b[0] <= b[1] && b[1] <= b[2]);

  double Slab = 0;
  double Slba = 0;

  if (a[0] == b[0] && a[1] == b[1])
    {
      Slab = 0;
      Slba = 0;
    }
  else if (a[0] >= b[0] && a[1] >= b[1])
    {
      Slab = ((double)((a[1] - b[1]) + (a[1] - b[0])) / 2.0) - ((double)(a[1] - a[0]) / 2.0);
      Slba = 0;
    }
  else if (a[0] <= b[0] && a[1] <= b[1])
    {
      Slab = 0;
      Slba = ((double)((b[1] - a[1]) + (b[1] - a[0])) / 2.0) - ((double)(b[1] - b[0]) / 2.0);
    }
  else if (a[0] >= b[0] && a[1] <= b[1])
    {
      double x;
      double y;

      if (a[0] == a[1])
	{
	  double aa = (double)(1 - 0) / (b[1] - b[0]);
	  double bb = (double)(0 - (aa * b[0]));

	  x = a[0];
	  y = (aa * x) + bb;
	}
      else if (b[0] == b[1])
	{
	  double aa = (double)(1 - 0) / (a[1] - a[0]);
	  double bb = (double)(0 - (aa * a[0]));

	  x = b[0];
	  y = (aa * x) + bb;
	}
      else
	{
	  double aa1 = (double)(1 - 0) / (b[1] - b[0]);
	  double bb1 = (double)(0 - (aa1 * b[0]));

	  double aa2 = (double)(1 - 0) / (a[1] - a[0]);
	  double bb2 = (double)(0 - (aa2 * a[0]));

	  x = (bb1 - bb2) / (aa2 - aa1);
	  y = (aa2 * x) + bb2;
	}

      Slab = ((double)(x - b[0]) * y / 2.0) - ((double)(x - a[0]) * y / 2.0);
      Slba = ((double)(b[1] - x) * (1.0 - y) / 2.0) - ((double)(a[1] - x) * (1.0 - y) / 2.0);
    }
  else if (a[0] <= b[0] && a[1] >= b[1])
    {
      double x;
      double y;

      if (a[0] == a[1])
	{
	  double aa = (double)(1 - 0) / (b[1] - b[0]);
	  double bb = (double)(0 - (aa * b[0]));

	  x = a[0];
	  y = (aa * x) + bb;
	}
      else if (b[0] == b[1])
	{
	  double aa = (double)(1 - 0) / (a[1] - a[0]);
	  double bb = (double)(0 - (aa * a[0]));

	  x = b[0];
	  y = (aa * x) + bb;
	}
      else
	{
	  double aa1 = (double)(1 - 0) / (b[1] - b[0]);
	  double bb1 = (double)(0 - (aa1 * b[0]));

	  double aa2 = (double)(1 - 0) / (a[1] - a[0]);
	  double bb2 = (double)(0 - (aa2 * a[0]));

	  x = (bb1 - bb2) / (aa2 - aa1);
	  y = (aa2 * x) + bb2;
	}

      Slab = ((double)(x - a[0]) * y / 2.0) - ((double)(x - b[0]) * y / 2.0);
      Slba = ((double)(a[1] - x) * (1.0 - y) / 2.0) - ((double)(b[1] - x) * (1.0 - y) / 2.0);
    }

  double Srab = 0;
  double Srba = 0;

  if (a[1] == b[1] && a[2] == b[2])
    {
      Srab = 0;
      Srba = 0;
    }
  else if (a[1] >= b[1] && a[2] >= b[2])
    {
      Srab = ((double)((a[2] - b[1]) + (a[1] - b[1])) / 2.0) - ((double)(b[2] - b[1]) / 2.0);
      Srba = 0;
    }
  else if (a[1] <= b[1] && a[2] <= b[2])
    {
      Srab = 0;
      Srba = ((double)((b[2] - a[1]) + (b[1] - a[1])) / 2.0) - ((double)(a[2] - a[1]) / 2.0);
    }
  else if (a[1] >= b[1] && a[2] <= b[2])
    {
      double x;
      double y;

      if (a[1] == a[2])
	{
	  double aa = (double)(0 - 1) / (b[2] - b[1]);
	  double bb = (double)(1 - (aa * b[1]));

	  x = a[1];
	  y = (aa * x) + bb;
	}
      else if (b[1] == b[2])
	{
	  double aa = (double)(0 - 1) / (a[2] - a[1]);
	  double bb = (double)(1 - (aa * a[1]));

	  x = b[1];
	  y = (aa * x) + bb;
	}
      else
	{
	  double aa1 = (double)(0 - 1) / (b[2] - b[1]);
	  double bb1 = (double)(1 - (aa1 * b[1]));

	  double aa2 = (double)(0 - 1) / (a[2] - a[1]);
	  double bb2 = (double)(1 - (aa2 * a[1]));

	  x = (bb1 - bb2) / (aa2 - aa1);
	  y = (aa2 * x) + bb2;
	}

      Srab = ((double)(x - b[1]) * (1.0 - y) / 2.0) - ((double)(x - a[1]) * (1.0 - y) / 2.0);
      Srba = ((double)(b[2] - x) * y / 2.0) - ((double)(a[2] - x) * y / 2.0);
    }
  else if (a[1] <= b[1] && a[2] >= b[2])
    {
      double x;
      double y;

      if (a[1] == a[2])
	{
	  double aa = (double)(0 - 1) / (b[2] - b[1]);
	  double bb = (double)(1 - (aa * b[1]));

	  x = a[1];
	  y = (aa * x) + bb;
	}
      else if (b[1] == b[2])
	{
	  double aa = (double)(0 - 1) / (a[2] - a[1]);
	  double bb = (double)(1 - (aa * a[1]));

	  x = b[1];
	  y = (aa * x) + bb;
	}
      else
	{
	  double aa1 = (double)(0 - 1) / (b[2] - b[1]);
	  double bb1 = (double)(1 - (aa1 * b[1]));

	  double aa2 = (double)(0 - 1) / (a[2] - a[1]);
	  double bb2 = (double)(1 - (aa2 * a[1]));

	  x = (bb1 - bb2) / (aa2 - aa1);
	  y = (aa2 * x) + bb2;
	}

      Srab = ((double)(x - a[1]) * (1.0 - y) / 2.0) - ((double)(x - b[1]) * (1.0 - y) / 2.0);
      Srba = ((double)(a[2] - x) * y / 2.0) - ((double)(b[2] - x) * y / 2.0);
    }

  return Slab + Srab - Slba - Srba;
}

bool Fuzzy3::operator<(Fuzzy3 const& other)
{
  auto otherDominatesThis = other.dominationOver(*this) > 0;
  auto thisDominatesOther = this->dominationOver(other) > 0;

  if (otherDominatesThis && thisDominatesOther)
    return other.mUuid > mUuid;

  return otherDominatesThis;
}

Fuzzy3& Fuzzy3::operator+=(Fuzzy3 const& other)
{
  mNth[0] += other.mNth[0];
  mNth[1] += other.mNth[1];
  mNth[2] += other.mNth[2];

  return *this;
}
