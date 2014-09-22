import System.Environment
import Data.List(sort, isInfixOf)

process :: [Char] -> [Char]
process = show . map (snd) . sort . makeTurple . lines
    where
        makeTurple :: [String] -> [((Int,Int), Float)]
        makeTurple xs = map (\x -> ( let sep = (takeWhile (/=';') x) in (read (takeWhile (/='_') sep), read (tail $ dropWhile (/='_') sep)),  read (tail $ dropWhile(/=';') x)) ) xs

--predicate :: ((Int, Int), Float) -> ((Int, Int), Float) -> Bool
predicate left right = let a = fst left
                           b = fst right
                       in if (fst a) == (fst b) then compare (snd a) (snd b)
                                                else compare (fst a) (fst b)
                                                
 
main = do s <- readFile "GFLOPS.csv"
          writeFile "output.txt" $ process s
    