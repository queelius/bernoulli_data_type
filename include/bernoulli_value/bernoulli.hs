

data Bernoulli a =
    ZerothOrder
    FirstOrder ErrorRate |
    Positive FalsePositiveRate |
    Negative FalseNegaiveRate |
    PositiveNegative False PositiveRate FalseNegativeRate
    HigherOrder [ErrorRate]

error_rate :: Bernoulli -> Interval Double 
error_rate ZerothOrder = 0
error_rate (FirstOrder e) = Interval e e
error_rate (SecondOrder e1 e2) = Interval e1 e2

error_rate_k :: Bernoulli Int -> Interval Double
error_rate_k (FirstOrder e) 0 = e
error_rate_k (SecondOrder e1 _) 0 = e1
error_rate_k (SecondOrder _ e2) 1 = e2
error_rate_k (HigherOrder es) k = es !! k


order :: Bernoulli -> Int
order ZerothOrder = 0
order (FirstOrder _) = 1
order (SecondOrder _ _) = 2
order (HigherOrder es) = length es


-- personAge :: Person -> Int
-- personAge p = case p of
  -- Adult _ _ _ a _ -> a
  -- Child _ a _ -> a