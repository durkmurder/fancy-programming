import Data.List(group, sort, tails, find)
import Data.Array

myLast :: [a] -> a
myLast [x] = x
myLast (x:xs) = myLast xs

myButLast :: [a] -> a
myButLast = head . tail . reverse

elementAt :: [a] -> Int -> a
elementAt = (. subtract 1) . (!!) 

myReverse :: [a] -> [a]
myReverse [] = []
myReverse (x:xs) = myReverse xs ++ [x]

--isPalindrome :: (Eq a) -> Bool
isPalindrome word 
	| word == (reverse word) = True
	| otherwise = False
	
myCompress :: Eq a => [a] -> [a]
myCompress = map head . group

myPack (x:xs) = let (first, rest) = span (==x) xs
               in (x:first) : myPack rest
myPack [] = []

encode :: [Char] -> [[Char]]
encode xs = map (\x -> encode' x) (group xs)
	where encode' x
		| (length x == 1) = "Single " ++ show (head x)
		| otherwise = "Multple " ++ show (length x) ++ " " ++ show (head x)
	
dublicate :: [a] -> [a]
dublicate (x:xs) = [x] ++ [x] ++ dublicate xs
dublicate [] = []

myReplicate :: [a] -> Int -> [a]
myReplicate xs n = concatMap (take n . repeat) xs

dropEvery :: [a] -> Int -> [a]
dropEvery xs n
  | length xs < n = xs
  | otherwise     = take (n-1) xs ++ dropEvery (drop n xs) n
  
split :: [a] -> Int -> ([a],[a])
split xs n = (take n xs, drop n xs)

slice :: [a] -> Int -> Int -> [a]
slice xs i k = take (k - i + 1) (drop (i - 1) xs)

rotate :: [a] -> Int -> [a]
rotate xs n = take (length xs) $ drop (length xs + n) $ cycle xs

removeAt :: [a] -> Int -> (a, [a])
removeAt xs i = (xs !! i, take i xs ++ tail (drop i xs))

insertAt :: [a] -> a -> Int -> [a]
insertAt xs value i = take (i - 1) xs ++ [value] ++ drop (i - 1) xs

range :: Enum a => a -> a -> [a]
range l r = [l..r]

myGcd :: Int -> Int -> Int
myGcd x y = gcd' (abs x) (abs y) where
	gcd' a 0 = a
	gcd' a b  =  gcd' b (a `rem` b) 
	
totientPhi :: Int -> Int
totientPhi n = length [ x | x <- xs, myGcd n x == 1] where xs = [1..n]

primeFactors :: Int -> [Int]
primeFactors n = primeFactors' n 2
  where
    primeFactors' n f
      | f*f > n        = [n]
      | n `mod` f == 0 = f : primeFactors' (n `div` f) f
      | otherwise      = primeFactors' n (f + 1) 
      
isPrime n | n < 4 = n /= 1 
isPrime n = all ((/=0) . mod n) $ takeWhile (<= m) candidates 
        where candidates = (2:3:[x + i | x <- [6,12..], i <- [-1,1]])
              m = floor . sqrt $ fromIntegral n
      
primesRange :: Int -> Int -> [Int]
primesRange l r = filter isPrime [l..r]

goldbach :: Int -> (Int, Int)
goldbach n = head [(x,y) | x <- pr, y <- pr, x+y==n]
    where pr = primesRange 2 (n-2)
    
goldbachList :: Int -> Int -> [(Int, Int)]
goldbachList l r = map goldbach $ filter even [l..r]

goldbachList' :: Int -> Int -> Int -> [(Int, Int)]
goldbachList' l r n = filter (\(first, second) -> first > n) . map goldbach $ filter even [l..r]

primeFactorsMult :: Int -> [(Int, Int)]
primeFactorsMult = map encode . group . primeFactors
    where encode xs = (head xs, length xs)
    
sieve :: [Int] -> [Int]
sieve [] = []
sieve (x:xs) = x : sieve [ y | y <- xs, y `mod` x > 0]

isPrimeLarge :: Int -> Bool
isPrimeLarge p = if (modularPow x (p - 1) p) == 1 then True else False
	where x = (fromIntegral p) `div` 2
	
modularPow :: Int -> Int -> Int -> Int
modularPow base exponent modulus = helper (base `mod` modulus) exponent 1 modulus 
	where 
	helper b 0 result modulus = result
	helper b e result modulus
		| (e `mod` 2) == 1 = helper (b * b `mod` modulus) (e `div` 2) (result * b `mod` modulus) modulus
		| otherwise = helper (b * b `mod` modulus) (e `div` 2) result modulus
	
quickSelect :: [Int] -> Int -> Int
quickSelect xs k 
	| k <= length(left) = quickSelect left k
	| k > length(xs) - length(right) = quickSelect right $ k - (length(xs) - length(right))
	| otherwise = pivot where
	pivot = head $ drop (length xs `div` 2) xs
	left = [x | x <- xs, x < pivot]
	right = [y | y <- xs, y > pivot]
	
editDistance :: Eq a => [a] -> [a] -> Int
editDistance xs ys = table ! (m, n)
    where
    (m,n) = (length xs, length ys)
    x     = array (1, m) (zip [1..] xs)
    y     = array (1, n) (zip [1..] ys)
 
    table :: Array (Int,Int) Int
    table = array bnds [(ij, dist ij) | ij <- Data.Array.range bnds]
    bnds  = ((0, 0), (m, n))
 
    dist (0, j) = j
    dist (i, 0) = i
    dist (i, j) = minimum [table ! (i - 1, j) + 1, table ! (i, j - 1) + 1,
        if x ! i == y ! j then table ! (i - 1, j - 1) else 1 + table ! (i - 1, j - 1)]
        
convolve :: (Num a) => [a] -> [a] -> [a]
convolve hs xs =
  let pad = replicate ((length hs) - 1) 0
      ts  = pad ++ xs
  in map (sum . zipWith (*) (reverse hs)) (init $ tails ts)
  
binarySearch :: Integral a => (a -> Ordering) -> (a, a) -> Maybe a
binarySearch p (low,high) 
  | high < low = Nothing
  | otherwise = 
      let mid = (low + high) `div` 2 in 
      case p mid of
        LT -> binarySearch p (low, mid - 1)
        GT -> binarySearch p (mid + 1, high)
        EQ -> Just mid
	
binarySearchArray :: (Ix i, Integral i, Ord e) => Array i e -> e -> Maybe i
binarySearchArray a x = binarySearch p (bounds a) where
  p m = x `compare` (a ! m)
  
    
lcs :: (Eq a) => [a] -> [a] -> [a]
lcs xs ys = a!(0,0) where
  n = length xs
  m = length ys
  a = array ((0,0),(n,m)) $ l1 ++ l2 ++ l3
  l1 = [((i,m),[]) | i <- [0..n]]
  l2 = [((n,j),[]) | j <- [0..m]]
  l3 = [((i,j), f x y i j) | (x,i) <- zip xs [0..], (y,j) <- zip ys [0..]]
  f x y i j 
    | x == y    = x : a!(i+1,j+1)
    | otherwise = longest (a!(i,j+1)) (a!(i+1,j))
  longest xs ys = if length xs > length ys then xs else ys
  
lis xs = table
  where
    n = length xs
    bnds = (1, n)
    x = listArray bnds xs
    table = array bnds [(i, score i) | i <- Data.Array.range bnds]
    --
    score i = sc smallers
      where
        smallers = [j | j <- [1..i-1], x!j < x!i]
        sc [] = 1
        sc js = 1 + (maximum $ map (table!) js)

linearInversion xs ks = sum $ map (\x -> helper ks x) xs
	where
	helper ks value = (foldl (\x y -> if value `mod` y == 0 then succ x else x) 0 ks) `mod` 2
	
pocket :: [Int] -> Int -> [Int] -> Int
pocket xs n c = d ! (n - 1)
	where
	bnds = (1, n)
	d = array bnds [(i, score i) | i <- Data.Array.range bnds]
	score w = sc lighter 
		where
		lighter = [ d ! j + c !! j | j <- [1..w] ]
		sc [] = 0
		sc js = maximum lighter
	

--parseList :: [String] -> [[String], [String]]
parseList (x:xs)
    | x == "ANIMALS" = helper False xs ([], [])
    | x == "NUMBERS" = helper True xs ([], [])
    where
        helper _ [] acum = (sort (fst acum), sort (snd acum))
        helper flag (y:ys) acum 
            | y == "ANIMALS" = helper False ys acum
            | y == "NUMBERS" = helper True ys acum
            | flag == False = helper flag ys ((fst acum) ++ [y], snd acum)
            | flag == True = helper flag ys (fst acum, (snd acum) ++ [y])
groupAndSort :: (Ord a) => [a] -> [(a, Int)]          
groupAndSort = map (\l@(x:xs) -> (x, length l)) . group . sort 
    

fullWords :: Int -> String
fullWords x = helper x digits []
	where
	digits = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"] 
	helper 0 dig xs = xs
	helper x dig xs = helper (x `div` 10) digits $ digits !! (x `mod` 10) ++ "-" ++ xs 

identifier :: [Char] -> Bool
identifier xs = True
