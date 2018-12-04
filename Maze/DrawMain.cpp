#include "Drawing.hpp"
#include "MazeDraw.hpp"
#include "Maze_BouTaoshi.hpp"
#include "Maze_AnaHori.hpp"
#include "Maze_RecursiveBacktracking.hpp"
#include "Maze_Prim.hpp"
#include "Maze_Blobby.hpp"
#include "Maze_RightHand.hpp"

#include <cstdlib>
#include <ctime>

using namespace std;


void DrawMain()
{
    int xSize = 10;
    int ySize = 10;

    srandom((unsigned int)time(NULL));

    //Maze *maze = CreateMaze_BouTaoshi(xSize, ySize);
    //Maze *maze = CreateMaze_AnaHori(xSize, ySize);
    //Maze *maze = CreateMaze_RecurvsiveBacktracking(xSize, ySize);
    //Maze *maze = CreateMaze_Prim(xSize, ySize);
    Maze *maze = CreateMaze_Blobby(xSize, ySize);
    //SolveMaze_RightHand(maze);
    delete maze;
}

