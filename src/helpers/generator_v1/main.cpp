#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <unistd.h>

using namespace std;

int numberOfJobs = 0;

class Operation
{
 public:

  int id;
  int type;
  int result;
  int processingTime;
};

class Job
{
 public:

  int id;
  int priority;
  int arrivalTime;
  vector<Operation> operations;
};

vector<Job> jobs;

int main( int argc , char ** argv )
{
  if ( argc < 3 )
  {
    cout << "Usage: " << argv[0] << " numberOfJobs numberOfJobsPerDay" << endl;
    return 1;
  }

  char buff[255] = { 0 };
  readlink("/proc/self/exe", buff, 255);
  std::string exePath(buff);
  std::string exeDir = exePath.substr(0, exePath.find_last_of("/"));

  int numberOfJobs = stoi( argv[1] );
  int numberOfJobsPerDay = stoi( argv[2] );

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  mt19937 generator(seed);

  double priority1 = 142574.0;
  double priority0 = 40018.0;
  double result0 = 20789501.0;
  double result1 = 1197709.0;

  discrete_distribution<int> priorityDist { priority0 , priority1 };
  discrete_distribution<int> resultDist { result0 , result1 };

  int limit;

  vector<int> operationsPerJobIndexes;
  vector<int> operationsPerJobValues;
  vector<int> operationsPerJobTest;
  ifstream ifs1( exeDir + "/../resources/generator_v1_operationsPerJob.txt" );

  ifs1 >> limit;

  for ( int i = 0; i < limit; ++i )
  {
    int left;
    int right;

    ifs1 >> left >> right;

    operationsPerJobIndexes.push_back( left );
    operationsPerJobValues.push_back( right );
    operationsPerJobTest.push_back( 0 );
  }
  ifs1.close();

  discrete_distribution<int> operationsPerJobDist( operationsPerJobValues.begin() , operationsPerJobValues.end() );

  vector<int> operationTypesIndexes;
  vector<int> operationTypesValues;
  vector<int> operationTypesTest;
  ifstream ifs2( exeDir + "/../resources/generator_v1_operationTypes.txt" );

  ifs2 >> limit;

  for ( int i = 0; i < limit; ++i )
  {
    int left;
    int right;

    ifs2 >> left >> right;

    operationTypesIndexes.push_back( left );
    operationTypesValues.push_back( right );
    operationTypesTest.push_back( 0 );
  }
  ifs2.close();

  discrete_distribution<int> operationTypesDist( operationTypesValues.begin() , operationTypesValues.end() );

  vector<int> processingTimesIndexes;
  vector<int> processingTimesValues;
  vector<int> processingTimesTest;
  ifstream ifs3( exeDir + "/../resources/generator_v1_processingTimes.txt" );

  ifs3 >> limit;

  for ( int i = 0; i < limit; ++i )
  {
    int left;
    int right;

    ifs3 >> left >> right;

    processingTimesIndexes.push_back( left );
    processingTimesValues.push_back( right );
    processingTimesTest.push_back( 0 );
  }
  ifs3.close();

  discrete_distribution<int> processingTimesDist( processingTimesValues.begin() , processingTimesValues.end() );

  weibull_distribution<double> weibullBig( 2.29025 , 37672.86375 );
  double offsetBig = 12387.757213304;
  weibull_distribution<double> weibullSmall( 2.29025 , 8698.388597224 );
  double offsetSmall = -536.5082662500;
  double weightBig = 0.92078125;
  double weightSmall = 0.07921875;

  discrete_distribution<int> massDist { weightBig , weightSmall };

  int numberOfDays = numberOfJobs / numberOfJobsPerDay;

  int jobId = 0;
  int priority;
  int arrivalTime;
  int numberOfOperations;

  int operationId = 0;
  int type;
  int result;
  int processingTime;
  int draw;

  for ( int d = 0; d < numberOfDays; ++d )
  {
    for ( int j = 0; j < numberOfJobsPerDay; ++j )
    {
      Job job;

      priority = priorityDist(generator);

      do
      {
        draw = massDist( generator );

        if ( draw == 0 )
        {
          draw = weibullBig( generator );
          draw += offsetBig;
        }
        else
        {
          draw = weibullSmall( generator );
          draw += offsetSmall;
        }

      } while ( draw < 1.0 || draw > 86400.0 );

      draw = (int) (draw + 0.5);

      arrivalTime = draw + 86400 * d;
      numberOfOperations = operationsPerJobIndexes[ operationsPerJobDist(generator) ];

      job.id = 0;
      job.priority = priority;
      job.arrivalTime = arrivalTime;

      for ( int i = 0; i < numberOfOperations; ++i )
      {
        Operation operation;

        type = operationTypesIndexes[ operationTypesDist(generator) ];
        result = resultDist(generator);
        processingTime = processingTimesIndexes[ processingTimesDist(generator) ];

        operation.id = 0;
        operation.type = type;
        operation.result = result;
        operation.processingTime = processingTime;

        job.operations.push_back( operation );
      }

      jobs.push_back( job );
    }
  }

  int leftOverJobs = numberOfJobs - numberOfJobsPerDay * (numberOfJobs / numberOfJobsPerDay );

  for ( int j = 0; j < leftOverJobs; ++j )
  {
    Job job;

    priority = priorityDist(generator);

    do
    {
      draw = massDist( generator );

      if ( draw == 0 )
      {
        draw = weibullBig( generator );
        draw += offsetBig;
      }
      else
      {
        draw = weibullSmall( generator );
        draw += offsetSmall;
      }

    } while ( draw < 1.0 || draw > ( 86400.0 * ( (double) leftOverJobs / numberOfJobsPerDay ) ) );

    draw = (int) (draw + 0.5);

    arrivalTime = draw + 86400 * numberOfDays;
    numberOfOperations = operationsPerJobIndexes[ operationsPerJobDist(generator) ];

    job.id = 0;
    job.priority = priority;
    job.arrivalTime = arrivalTime;

    for ( int i = 0; i < numberOfOperations; ++i )
    {
      Operation operation;

      type = operationTypesIndexes[ operationTypesDist(generator) ];
      result = resultDist(generator);
      processingTime = processingTimesIndexes[ processingTimesDist(generator) ];

      operation.id = 0;
      operation.type = type;
      operation.result = result;
      operation.processingTime = processingTime;

      job.operations.push_back( operation );
    }

    jobs.push_back( job );
  }

  sort( jobs.begin(), jobs.end(),
        [](const Job & a, const Job & b) -> bool
	{
          return a.arrivalTime < b.arrivalTime;
	});

  cout << jobs.size() << endl;

  for ( size_t j = 0; j < jobs.size(); ++j )
  {
    cout << endl;

    cout << jobId++ << " " << jobs[j].priority << " " << jobs[j].arrivalTime << endl;
    cout << jobs[j].operations.size() << endl;

    for ( size_t i = 0; i < jobs[j].operations.size(); ++i )
    {
      cout << operationId++ << " " << jobs[j].operations[i].type << " " << jobs[j].operations[i].result << " " << jobs[j].operations[i].processingTime << endl;
    }
  }

  return 0;
}
