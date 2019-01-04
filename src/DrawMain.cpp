//
//  DrawMain.cpp
//
//  Created by Satoshi Numata on 2018/09/16.
//  License: Public domain.
//

#include "Drawing.hpp"
#include "MazeDraw.hpp"

#include "Maze_AnaHori.hpp"
#include "Maze_HuntAndKill.hpp"
#include "Maze_RecursiveBacktracking.hpp"
#include "Maze_GrowingTree.hpp"
#include "Maze_Prim.hpp"
#include "Maze_AldousBroder.hpp"
#include "Maze_Wilson.hpp"
#include "Maze_Houston.hpp"
#include "Maze_KabeNobashi.hpp"

#include "Maze_RecursiceDivision.hpp"
#include "Maze_BlobbyRecursiveDivision.hpp"
#include "Maze_Eller.hpp"
#include "Maze_Kruskal.hpp"

#include "Maze_BouTaoshi.hpp"
#include "Maze_Sidewinder.hpp"
#include "Maze_BinaryTree.hpp"

#include "Maze_RightHand.hpp"
#include "Maze_Dijkstra.hpp"

#include <cstdlib>
#include <ctime>

using namespace std;


// 右下の点をゴールとする
static CellPoint MakeRightBottomGoal(int xSize, int ySize)
{
    return CellPoint(xSize - 1, ySize - 1);
}

// 中央の点をゴールとする
static CellPoint MakeCenterGoal(int xSize, int ySize)
{
    return CellPoint(xSize / 2, ySize / 2);
}

// ランダムな座標をゴールとする
static CellPoint MakeRandomGoal(const CellPoint& start, int xSize, int ySize)
{
    CellPoint goal;
    do {
        goal.x = random() % xSize;
        goal.y = random() % ySize;
    } while (goal.Equals(start));
    return goal;
}

// 描画エンジンのエントリポイント
void DrawMain()
{
    // 乱数の初期化
    srandom((unsigned int)time(NULL));

    // 迷路のサイズ
    int xSize = 30;
    int ySize = 20;

    // スタート位置とゴール位置
    CellPoint start(0, 0);
    CellPoint goal = MakeRightBottomGoal(xSize, ySize);
    //CellPoint goal = MakeCenterGoal(xSize, ySize);
    //CellPoint goal = MakeRandomGoal(start, xSize, ySize);

    // 迷路の生成
    //Maze *maze = CreateMaze_AnaHori(xSize, ySize);
    //Maze *maze = CreateMaze_HuntAndKill(xSize, ySize);
    //Maze *maze = CreateMaze_RecurvsiveBacktracking(xSize, ySize);
    //Maze *maze = CreateMaze_GrowingTree(xSize, ySize);
    //Maze *maze = CreateMaze_Prim(xSize, ySize);
    //Maze *maze = CreateMaze_AldousBroder(xSize, ySize);
    //Maze *maze = CreateMaze_Wilson(xSize, ySize);
    //Maze *maze = CreateMaze_Houston(xSize, ySize);
    //Maze *maze = CreateMaze_KabeNobashi(xSize, ySize);

    //Maze *maze = CreateMaze_RecursiveDivision(xSize, ySize);
    //Maze *maze = CreateMaze_BlobbyRecursiveDivision(xSize, ySize);
    //Maze *maze = CreateMaze_Eller(xSize, ySize);
    Maze *maze = CreateMaze_Kruskal(xSize, ySize);

    //Maze *maze = CreateMaze_BouTaoshi(xSize, ySize);
    //Maze *maze = CreateMaze_Sidewinder(xSize, ySize);
    //Maze *maze = CreateMaze_BinaryTree(xSize, ySize);

    // 出来上がった迷路データをテキストファイルとして読み込み/保存
    //string filepath = AddPathComponent(GetDesktopFolderPath(), "mymaze.txt");
    //Maze *maze = new Maze(filepath);
    //maze->Save(filepath);

    // 念のために出来上がった迷路を再度描画
    StartBatch();
    maze->Draw(false);
    maze->DrawStart(start);
    maze->DrawGoal(goal);
    EndBatch();

    // 探索開始のためのキー入力待ち
    Sleep(0.7f);
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO SOLVE!!", -12*10, -240, kColorOrange);
    }

    // 迷路探索
    maze->SetTagForAllCells(0);
    SolveMaze_RightHand(maze, start, Down, goal);
    //SolveMaze_Dijkstra(maze, start, goal);
    DrawText("SOLVED!!", -4 * 13, -240, kColorBlue);

    delete maze;
}

