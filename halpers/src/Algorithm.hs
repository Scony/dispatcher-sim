module Algorithm
  ( allInOne
  ) where

import Job
import Operation
import Machine

allInOne :: [Job] -> [Operation] -> [Machine] -> [(Machine, Operation)]
allInOne js ops ms = map (\x -> (ms !! 0, x)) ops
