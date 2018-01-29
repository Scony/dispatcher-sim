#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

double vecAvarage(const std::vector<double>& values)
{
  double avg = 0.0;

  for (const auto& value : values)
    avg += value;
  avg /= values.size();

  return avg;
}

double vecMedian(const std::vector<double>& aValues)
{
  std::vector<double> values(aValues);
  double med;

  std::sort(values.begin(), values.end(), [](double a, double b) {
      return a < b;
    });
  if (values.size() % 2)
    med = values[values.size() / 2];
  else
    med = (values[values.size() / 2] + values[(values.size() / 2) - 1]) / 2;

  return med;
}

double vecMin(const std::vector<double>& aValues)
{
  return *std::min_element(aValues.begin(), aValues.end());
}

double vecMax(const std::vector<double>& aValues)
{
  return *std::max_element(aValues.begin(), aValues.end());
}

double vecVariance(const std::vector<double>& aValues)
{
  double avg = vecAvarage(aValues);
  double variance = 0.0;

  for (const auto& value : aValues)
    variance += pow(value - avg, 2.0);

  variance /= aValues.size();

  return variance;
}

double vecStandardDeviation(const std::vector<double>& aValues)
{
  return sqrt(vecVariance(aValues));
}

int main()
{
  std::vector<double> durations;

  double duration;
  while (std::cin >> duration)
    durations.push_back(duration);

  std::cout << std::fixed;
  std::cout << vecAvarage(durations);
  std::cout << " " << vecMedian(durations);
  std::cout << " " << vecMin(durations);
  std::cout << " " << vecMax(durations);
  std::cout << " " << vecStandardDeviation(durations);
  std::cout << std::endl;

  return 0;
}
