
f :: Double -> Double
f x = (1 / (tan x) - x / 2)

f' :: Double -> Double
f' x = (-4 * sin(x) * sin(x) / ((cos(2 * x) - 1) * (cos(2 * x) - 1))) - 0.5

epsilon :: Double
epsilon = 0.000001

hords :: Double -> Double -> (Double, Double)
hords a b = (b, a - (a - b) * f(a)/(f(a) - f(b)))

newton :: Double -> Double -> (Double, Double)
newton a b = (b, b - (f(b) / f'(b)))

combined a b = helper a b 1
    where
        helper x_i x_j it = let value = (hords x_i x_j, newton x_i x_j)
                                x_n = snd $ fst value
                                z_n = snd $ snd value
                            in if abs (x_n - z_n) < 2 * epsilon then ((x_n + z_n) / 2, it) else helper x_n z_n (succ it)

--solver :: a -> Double
solver func a b = helper a b 1
    where
    helper :: Double -> Double -> Int -> (Double, Int)
    helper x_i x_j it = let value = func x_i x_j
                        in if abs((snd value) - (fst value)) < epsilon then (snd value, it) else helper (fst value) (snd value) (succ it)
                     
algorithm flag a b =
    case flag of
         0 -> solver (hords) a b
         1 -> solver (newton) a b
         2 -> combined a b       
                     
                 
iteration :: Double -> Double -> (Double, Double)
iteration x y = ((1 - ((sin y) / 2)), (0.7 - cos (x - 1)))

systemSolver :: (Double -> Double -> (Double, Double)) -> Double -> Double -> (Double, Double, Int)
systemSolver func x0 y0 = helper x0 y0 1
    where
        helper x y it = let value = func x y
                        in if abs((fst value) - x) < epsilon && abs((snd value) - y) < epsilon then (fst value, snd value, it) else helper (fst value) (snd value) (succ it) 

newtonSystem :: Double -> Double -> (Double, Double)
newtonSystem x y = let delta = (g x y 2) * (g x y 5) - (g x y 3) * (g x y 4)
                       delta_x = -((g x y 0) * (g x y 5) - (g x y 1) * (g x y 3))
                       delta_y = (g x y 0) * (g x y 4) - (g x y 1) * (g x y 2)
                   in (x + delta_x / delta, y + delta_y / delta)
    
    
    
    
g :: Double -> Double -> Int -> Double
g x y flag = 
    case flag of 
         0 -> (sin y) + 2 * x - 2 -- F(x,y)
         1 -> (cos (x - 1)) + y - 0.7 -- G(x,y)
         2 -> x -- F'x
         3 -> cos y -- F'y
         4 -> -sin(x - 1) -- G'x
         5 -> 1 -- G'y

test x y 0 = (x, y)
test x y k = let value = newtonSystem x y in test (fst value) (snd value) (k - 1)