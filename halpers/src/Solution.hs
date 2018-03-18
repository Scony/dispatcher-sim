module Solution
  ( calculateAssignments
  ) where

import Job
import Machine
import Operation
import Assignment

calculateAssignments :: [Job] -> [(Machine, [Operation])] -> [Assignment]
calculateAssignments js mos =
  foldl (\a x -> a ++ x) [] $ map (calculateMachineAssignments js) mos

calculateMachineAssignments :: [Job] -> (Machine, [Operation]) -> [Assignment]
calculateMachineAssignments js mo =
  foldl (\acc op -> if acc == [] then [] else []) [] (snd mo) -- TODO
