import System.Environment
import Data.List

y0 :: [[Double]]
y0 = [[0.2, 0.2, 0.2]]

innerProduct :: [[Double]] -> [[Double]] -> Double
innerProduct = (sum.) . zipWith (\x y -> head x * head y)

aIn :: [[Double]]
aIn = [[1.8, 0.9, 1.0], 
	   [0.9, 1.8, 0.3], 
	   [1.0, 0.3, 1.8]]

multiply a b = [ [ sum $ zipWith (*) ar bc | bc <- (transpose b) ] | ar <- a ]

calculateY :: Int -> [[Double]]
calculateY = (map (helper aIn) [0..] !!) 
	where
	helper :: [[Double]] -> Int -> [[Double]]
	helper a i 
		| i == 0 = multiply a $ transpose y0 
		| otherwise = helper (multiply aIn a) (i - 1)

eigenValue2 :: Int -> Bool -> Double -> Double
eigenValue2 k check l1 
	| check == True = lambda1
	| otherwise = lambda2
	where
	x = innerProduct (calculateY (k + 1)) (calculateY k)
	y = innerProduct (calculateY k) (calculateY k)
	z = innerProduct (calculateY k) (calculateY (k - 1) ) 
	lambda1 = x / y 
	lambda2 = (x - l1 * y) / (y - l1 * z) 
	
eigenValue1 :: Int -> [[Double]]
eigenValue1 k = [[lambda1] ++ [lambda2]] ++ (calculateY k)
	where
	x = head $ head $ calculateY (k + 1) 
	y = head $ head $ calculateY k
	z = head $ head $ calculateY (k - 1)
	lambda1 = x / y
	lambda2 = (x - lambda1 * y) / (y - lambda1 * z)


--solver2 :: Int -> [[Double]]
--solver2 k = if abs (abs (lambda_i) - abs (lambda_j)) < 0.000001 then lambda_i  else solver2 (k + 1) 
--	where
--	lambda_i = eigenValue2 k 
--	lambda_j = eigenValue2 (k + 1)

main = print $ eigenValue2 10 False 0.0
