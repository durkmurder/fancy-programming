epsilon :: Double
epsilon = 0.00001


rectangles :: (Double -> Double) -> (Double, Double) -> Double -> Double 
rectangles func bounds n = h * (foldl (\acc i -> acc + func(a + (i + 0.5) * h)) 0 [1..n])
  where
    a = fst bounds
    b = snd bounds
    h = (b - a) / n
    
trap :: (Double -> Double) -> (Double, Double) -> Double -> Double 
trap func bounds n = h / 2 * (func(a) + func(b)) + h * (foldl (\acc i -> acc + func(a + i * h)) 0 [1..n - 1])
  where
    a = fst bounds
    b = snd bounds
    h = (b - a) / n
    
    
simpson :: (Double -> Double) -> (Double, Double) -> Double -> Double 
simpson func bounds n = h / 6 * (func(a) + func(b) + 2 * trap' + 4 * rect') 
  where
    a = fst bounds
    b = snd bounds
    h = (b - a) / n
    trap' = (foldl (\acc i -> acc + func(a + i * h)) 0 [1..n - 1])
    rect' = (foldl (\acc i -> acc + func(a + (i + 0.5) * h)) 0 [1..n]) 