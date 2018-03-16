module Input
  ( readInstanceV2
  ) where

import Job
import Operation

readInstanceV2 = do
  sequence $ replicate 2 getLine -- omit header

  rawJobsNum <- getLine
  let jobsNum = read rawJobsNum :: Int
  rawJobs <- sequence $ replicate jobsNum getLine
  let jobs = map read rawJobs :: [Job]

  rawOperationsNum <- getLine
  let operationsNum = read rawOperationsNum :: Int
  rawOperations <- sequence $ replicate operationsNum getLine
  let operations = map read rawOperations :: [Operation]

  putStrLn $ show (jobs, operations)
