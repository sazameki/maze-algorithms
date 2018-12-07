#include "Drawing.hpp"
#include "MazeDraw.hpp"
#include "Maze_BouTaoshi.hpp"
#include "Maze_AnaHori.hpp"
#include "Maze_KabeNobashi.hpp"
#include "Maze_RecursiveBacktracking.hpp"
#include "Maze_Prim.hpp"
#include "Maze_Eller.hpp"
#include "Maze_RecursiceDivision.hpp"
#include "Maze_BlobbyRecursiveDivision.hpp"
#include "Maze_RightHand.hpp"

#include <cstdlib>
#include <ctime>

using namespace std;


// 描画エンジンのエントリポイント
void DrawMain()
{
    // 迷路のサイズ
    int xSize = 20;
    int ySize = 12;

    // 乱数の初期化
    srandom((unsigned int)time(NULL));

    // 迷路の生成
    //Maze *maze = CreateMaze_BouTaoshi(xSize, ySize);
    //Maze *maze = CreateMaze_AnaHori(xSize, ySize);
    //Maze *maze = CreateMaze_KabeNobashi(xSize, ySize);
    //Maze *maze = CreateMaze_RecurvsiveBacktracking(xSize, ySize);
    //Maze *maze = CreateMaze_Prim(xSize, ySize);
    Maze *maze = CreateMaze_Eller(xSize, ySize);
    //Maze *maze = CreateMaze_RecursiveDivision(xSize, ySize);
    //Maze *maze = CreateMaze_BlobbyRecursiveDivision(xSize, ySize);

    // 念のために出来上がった迷路を再度描画
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO SOLVE!!", -12*10, -240, kColorOrange);
    }

    // 右手法で迷路を解く
    SolveMaze_RightHand(maze);

    delete maze;
}

