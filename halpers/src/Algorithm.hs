module Algorithm
  ( allInOne
  ) where

import Job
import Operation
import Machine

allInOne :: [Job] -> [Operation] -> [Machine] -> [(Machine, [Operation])]
allInOne js ops ms = [(ms !! 0, ops)]
