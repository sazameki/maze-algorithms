//
//  Maze_RecursiveBacktracking.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "Maze_RecursiveBacktracking.hpp"

#include <vector>

using namespace std;


/**
    ヒープ上での再帰処理のためのスタックフレーム
 */
static vector<CellPoint> stack;


/**
    Recurvsive Backtrackingの再帰関数
    @param  maze    処理対象の迷路
    @param  pos     処理を開始するセル座標
 */
static void CarvePassage(Maze *maze)
{
    CellPoint pos = *(stack.end() - 1);

    // 方向をランダムに格納した配列を用意する
    vector<Direction> dirList = MakeAllDirectionsList_shuffled();

    // それぞれの方向に掘り進める
    bool isFinished = true;
    for (auto dir : dirList) {
        CellPoint nextPos = pos.Move(dir);
        if (maze->IsValidCell(nextPos) && maze->GetCellTag(nextPos) == 1) {
            maze->RemoveWall(pos, dir);
            maze->SetCellTag(nextPos, 0);
            maze->Draw();

            stack.push_back(nextPos);
            isFinished = false;
            break;
        }
    }

    if (isFinished) {
        stack.erase(stack.end() - 1);
    }
}

Maze *CreateMaze_RecurvsiveBacktracking(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->SetTagForAllCells(1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // ランダムな座標から掘っていく
    CellPoint p;
    p.x = random() % maze->GetXSize();
    p.y = random() % maze->GetYSize();
    stack.push_back(p);
    while (stack.size() > 0) {
        CarvePassage(maze);
    }

    return maze;
}

