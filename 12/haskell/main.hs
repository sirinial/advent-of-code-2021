import System.Environment
import Data.List
import Data.Char


data Node = Start |  Bigcave String | Smallcave String | End
        deriving (Eq, Ord)

instance Show Node where
    show Start = "Start"
    show End = "End"
    show (Bigcave c) =  c
    show (Smallcave c) = c


data Edge = Edge Node Node
        deriving (Eq, Ord)

startOfEdge :: Edge -> Node
startOfEdge (Edge a _) = a

endOfEdge :: Edge -> Node
endOfEdge (Edge _ a) = a

instance Show Edge where
    show (Edge a b) = (show a) ++ "-->" ++ (show b)

type Path = [Edge]
type Edges = [Edge]

readNode :: String -> Node
readNode "start" = Start
readNode "end" = End
readNode c = if (and . map isLower) c then Smallcave c else Bigcave c


splitOn :: Char -> String -> [String]
splitOn c str = case break (==c) str of
                (a, c:b) -> a : splitOn c b
                (a, "")    -> [a]


readEdge :: String -> Edge
readEdge s = let [fi, se] = splitOn '-' s
              in Edge (readNode fi) (readNode se)

showPath :: [Edge] -> String
showPath [] = ""
showPath [Edge x y] = (show x) ++ "," ++ (show y)
showPath (x:xs) = (show (startOfEdge x)  )++ ","  ++ (showPath xs)

addReverses :: Edges -> Edges
addReverses [] = []
addReverses (Edge Start b: xs) = (Edge Start b) : (addReverses xs)
addReverses (Edge a End: xs) = (Edge a End) : (addReverses xs)
addReverses (Edge a b: xs) = (Edge a b): (Edge b a) : (addReverses xs)



isStartOfPath :: [Edge] -> Bool
isStartOfPath [] = True
isStartOfPath [_] = True
isStartOfPath (a: (b :c)) = (endOfEdge a) == (startOfEdge b)

possiblePathsOfLength :: Int -> Edges -> (Path -> Bool)  -> [Path]
possiblePathsOfLength 0 _ _ = [[]]
possiblePathsOfLength 1 edges _ = [[e] | e <- edges, endOfEdge e == End]
possiblePathsOfLength n edges cond = [e:p | p <- possiblePathsOfLength (n-1) edges cond, e <- edges, isStartOfPath (e:p), cond (e:p)] 

addPossiblePaths :: Int -> Edges -> (Path -> Bool) -> [Path]
addPossiblePaths n edges cond = let pathsOfLevelN =possiblePathsOfLength n edges cond
                            in if null pathsOfLevelN then [] else pathsOfLevelN ++ (addPossiblePaths (n+1) edges cond)

possiblePaths ::  (Path -> Bool) -> Edges -> [Path]
possiblePaths cond edges  = [p | p <- addPossiblePaths 0 edges cond, startsWith Start p, endsWith End p]



readInput :: String -> [Edge]
readInput = map readEdge . lines

startsWith :: Node -> Path -> Bool
startsWith _ [] = False
startsWith n (x:xs) = (startOfEdge x) == n

endsWith :: Node -> Path -> Bool
endsWith _ [] = False
endsWith n [x] = (endOfEdge x) == n
endsWith n (x:xs) =  endsWith n xs

visitCaveAtMost :: Int -> Node -> Path -> Bool 
visitCaveAtMost _ _ [] = True
visitCaveAtMost 0 node (x:xs) = and [not ((endOfEdge x) == node), visitCaveAtMost 0 node xs ]
visitCaveAtMost nb node (x:xs) = if  (endOfEdge x) == node then visitCaveAtMost (nb -1) node xs else visitCaveAtMost nb node xs 

onlyVisitSmallCavesOnce :: Path -> Bool 
onlyVisitSmallCavesOnce [] = True
onlyVisitSmallCavesOnce ((Edge (Bigcave c) e ):xs) = onlyVisitSmallCavesOnce xs
onlyVisitSmallCavesOnce ((Edge x _):xs) = and [visitCaveAtMost 0 x xs, onlyVisitSmallCavesOnce xs]

onlyVisitOneSmallCavesTwice :: Path -> Bool
onlyVisitOneSmallCavesTwice [] = True
onlyVisitOneSmallCavesTwice ((Edge (Bigcave c) e ):xs) = True
onlyVisitOneSmallCavesTwice ((Edge (Smallcave c) e ):xs) = if visitCaveAtMost 0 (Smallcave c) xs then  onlyVisitOneSmallCavesTwice xs else and [visitCaveAtMost 1 (Smallcave c) xs, onlyVisitSmallCavesOnce xs]
onlyVisitOneSmallCavesTwice ((Edge x _):xs) = and [visitCaveAtMost 0 x xs, onlyVisitOneSmallCavesTwice xs]


solvePartA :: String -> IO ()
solvePartA s = do
                putStrLn "Solution a"
                let edges = (addReverses . readInput) s
                (putStrLn . show . length . (possiblePaths onlyVisitSmallCavesOnce)) edges




solvePartB :: String -> IO ()
solvePartB s = do
                putStrLn "Solution b"
                let edges = (addReverses . readInput) s
                (putStrLn . show . length . (possiblePaths onlyVisitOneSmallCavesTwice)) edges

solve :: String -> IO()
solve a = do
        solvePartA a
        solvePartB a

solveFile :: FilePath -> IO ()
solveFile a = do
    content <- readFile a
    solve content

main :: IO ()
main = do
    putStrLn "---- Begin -----"
    args <- getArgs
    solutions <- mapM solveFile args
    putStrLn "---- End -----"


