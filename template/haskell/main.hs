import System.Environment


solvePartA :: String -> String
solvePartA _ = "Solution a"

solvePartB :: String -> String
solvePartB _ = "Solution b"

solve :: String -> String
solve a = unlines [solvePartA a, solvePartB a]


solveFile :: FilePath -> IO String
solveFile a = do
    result <- (fmap solve . readFile) a
    return (a ++ "\n---------------------\n" ++ result)

main :: IO ()
main = do
    args <- getArgs
    solutions <- mapM solveFile args
    (putStr . unlines) solutions
    

countLines :: FilePath -> IO Int
countLines = fmap (length . lines) .  readFile

