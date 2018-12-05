#include "Drawing.hpp"
#include "MazeDraw.hpp"
#include "Maze_BouTaoshi.hpp"
#include "Maze_AnaHori.hpp"
#include "Maze_KabeNobashi.hpp"
#include "Maze_RecursiveBacktracking.hpp"
#include "Maze_Prim.hpp"
#include "Maze_RecursiceDivision.hpp"
#include "Maze_BlobbyRecursiveDivision.hpp"
#include "Maze_RightHand.hpp"
#include "DungeonDraw.hpp"

#include <cstdlib>
#include <ctime>

using namespace std;


void DrawMain()
{
    int xSize = 30;
    int ySize = 20;

    srandom((unsigned int)time(NULL));

    //Maze *maze = CreateMaze_BouTaoshi(xSize, ySize);
    //Maze *maze = CreateMaze_AnaHori(xSize, ySize);
    Maze *maze = CreateMaze_KabeNobashi(xSize, ySize);
    //Maze *maze = CreateMaze_RecurvsiveBacktracking(xSize, ySize);
    //Maze *maze = CreateMaze_Prim(xSize, ySize);
    //Maze *maze = CreateMaze_RecursiveDivision(xSize, ySize);
    //Maze *maze = CreateMaze_BlobbyRecursiveDivision(xSize, ySize);
    //SolveMaze_RightHand(maze);

    DungeonDraw(maze);

    delete maze;
}

