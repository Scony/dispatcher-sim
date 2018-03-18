module Operation
  ( Operation(uuid)
  ) where

data Operation = Operation { parent :: Int
                           , uuid :: Int
                           , kind :: Int
                           , outcome :: Int
                           , duration :: Int
                           , capacityReq :: Int
                           } deriving (Show)

instance Read Operation where
  readsPrec _ s = do
    let fs = map read . words $ s
    [(Operation (fs !! 0) (fs !! 1) (fs !! 2) (fs !! 3) (fs !! 4) (fs !! 5), "")]
