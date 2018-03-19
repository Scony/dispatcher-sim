module Algorithm
  ( allInOne
  , opt
  ) where

import Data.List (permutations, elemIndex)

import Job
import Operation
import Machine
import Utils
import Solution

allInOne :: [Job] -> [Operation] -> [Machine] -> [(Machine, [Operation])]
allInOne js ops ms = [(ms !! 0, ops)]


opt :: [Job] -> [Operation] -> [Machine] -> [(Machine, [Operation])]
opt js ops ms = snd $ (filter ((==bestWeight) . fst) weightedSolutions) !! 0
  where solutions = map (zip ms) $ possibilities ops $ length ms
        weightedSolutions = map (\x -> (eval x, x)) solutions
          where
            eval = calculateJobsTotalFlow
              . calculateJobFlows js
              . calculateAssignments js
        bestWeight :: Int
        bestWeight = minimum $ map fst weightedSolutions

possibilities :: [a] -> Int -> [[[a]]]
possibilities xs subsetsNum = process xs
  where process = map (\x -> x ++ replicate (subsetsNum - length x) [])
          . concat
          . map superPermutations
          . filter (\x -> length x <= subsetsNum)
          . partitions

superPermutations :: [[a]] -> [[[a]]]
superPermutations p = cartProdN $ map permutations p
  where cartProdN = sequence

partitions :: [a] -> [[[a]]]
partitions [] = [[]]
partitions (x:xs) = expand x $ partitions xs
  where expand :: a -> [[[a]]] -> [[[a]]]
        expand x ys = concatMap (extend x) ys

        extend :: a -> [[a]] -> [[[a]]]
        extend x [] = [[[x]]]
        extend x (y:ys) = ((x:y):ys) : map (y:) (extend x ys)
