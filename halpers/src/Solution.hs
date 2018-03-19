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
  let machine = fst mo
      x [] op =
        let jobArrival = arrival $ parentOf js op
        in [Assignment (jobArrival + duration op) op machine]
      x acc op =
        let finishOfPrev = finish $ last acc
            jobArrival = arrival $ parentOf js op
            beginTime = max finishOfPrev jobArrival
        in acc ++ [Assignment (beginTime + duration op) op machine]
  in foldl x [] (snd mo) -- TODO
