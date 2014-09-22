import Data.List(sort, group, isPrefixOf)

countDNA :: [Char] -> [Int]
countDNA = map length . group . sort

transformRD :: [Char] -> [Char]
transformRD = map (\x -> if x == 'T' then 'U' else x) 

reverseComplement :: [Char] -> [Char]
reverseComplement = map complement . reverse where
	complement 'A' = 'T'
	complement 'T' = 'A'
	complement 'C' = 'G'
	complement 'G' = 'C'

fibonachi :: Int -> Int
fibonachi 0 = 0
fibonachi 1 = 1
fibonachi n = fibonachi (n - 1) + fibonachi (n - 2)

findMotif :: [Char] -> [Char] -> [Int]
findMotif xs substr = findMotif' xs substr [] (length xs) where
	findMotif' [] substr result l = result
	findMotif' xs substr result l = if substr `isPrefixOf` xs then 
									findMotif' (tail xs) substr (result ++ [l - length xs + 1]) l
									else
									findMotif' (tail xs) substr result l
								
