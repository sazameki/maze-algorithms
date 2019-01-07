//
//  Maze_Dijkstra.cpp
//
//  Created by Satoshi Numata on 2018/12/30.
//  License: Public domain.
//

#include "Maze_Dijkstra.hpp"

#include <vector>

using namespace std;


/**
    隣接したfromとtoのセルについて、fromからtoに移動するための方向を計算します。
 */
static Direction CheckMoveDirection(const CellPoint& from, const CellPoint& to)
{
    if (to.x > from.x) {
        return Right;
    } else if (to.x < from.x) {
        return Left;
    } else if (to.y > from.y) {
        return Down;
    } else {
        return Up;
    }
}

/**
    重複のないようにvectorにセル座標を追加します。
    @param  cells   追加先のvector
    @param  newCell 追加するセル座標
 */
static void AddCellToVector(vector<CellPoint> &cells, const CellPoint& newCell)
{
    for (CellPoint &cell : cells) {
        if (cell.Equals(newCell)) {
            return;
        }
    }
    cells.push_back(newCell);
}

/**
    ヒープ上での再帰処理のためのスタックフレーム。
 */
struct DijkstraStackFrame
{
    vector<CellPoint> cells;
    int tag;
};

static vector<DijkstraStackFrame> stack;

/**
    Dijkstraの探索アルゴリズムのために番号を割り振ります。
    @param  maze    迷路
    @param  start   スタート位置
    @param  goal
 */
static void StepDijkstra(Maze *maze, const CellPoint& start, const CellPoint& goal)
{
    DijkstraStackFrame frame = stack[0];
    stack.erase(stack.begin());

    DijkstraStackFrame nextFrame;
    nextFrame.tag = frame.tag + 1;

    //vector<CellPoint> nextCells;
    for (CellPoint cell : frame.cells) {
        maze->SetCellTag(cell, frame.tag);

        vector<Direction> dirs = MakeAllDirectionsList();
        for (Direction dir : dirs) {
            if (maze->CheckWall(cell, dir)) {
                continue;
            }
            CellPoint cell2 = cell.Move(dir);
            if (maze->IsValidCell(cell2) && maze->GetCellTag(cell2) == 0) {
                AddCellToVector(nextFrame.cells, cell2);
            }
        }
    }
    StartBatch();
    maze->Draw(false);
    maze->DrawStart(start);
    maze->DrawGoal(goal);
    EndBatch();

    if (nextFrame.cells.size() > 0) {
        stack.push_back(nextFrame);
    }
}


// Dijkstraのアルゴリズムによる迷路探索
void SolveMaze_Dijkstra(Maze *maze, const CellPoint& start, const CellPoint& goal)
{
    // スタート位置から番号を付けていく
    vector<CellPoint> nextCells;
    nextCells.push_back(start);
    DijkstraStackFrame frame;
    frame.cells.push_back(start);
    frame.tag = 1;
    stack.push_back(frame);
    while (stack.size() > 0) {
        StepDijkstra(maze, start, goal);
    }

    // ゴール位置からタグを小さくしながらたどっていく
    vector<CellPoint> path;
    CellPoint cell = goal;
    int tag = maze->GetCellTag(goal);
    while (tag > 0) {
        path.push_back(cell);
        maze->SetCellTag(cell, 0);
        StartBatch();
        maze->Draw(false);
        maze->DrawStart(start);
        maze->DrawGoal(goal);
        EndBatch();
        tag--;

        // ランダムに最短経路を選択してたどっていきます。ランダムにしない場合、コメントアウトされた次の方向リストを使ってください。
        vector<Direction> dirs = MakeAllDirectionsList();
        //vector<Direction> dirs = MakeAllDirectionsList_shuffled();
        for (Direction dir : dirs) {
            if (maze->CheckWall(cell, dir)) {
                continue;
            }
            CellPoint cell2 = cell.Move(dir);
            if (maze->IsValidCell(cell2) && maze->GetCellTag(cell2) == tag) {
                cell = cell2;
                break;
            }
        }
    }
    Sleep(2.7f);

    // タグをクリアする
    maze->SetTagForAllCells(0);
    StartBatch();
    maze->Draw(false);
    maze->DrawStart(start);
    maze->DrawGoal(goal);
    EndBatch();

    // 開始位置の設定
    cell = *(path.end() - 1);
    path.erase(path.end() - 1);
    Direction dir = Down;
    while (path.size() > 0) {
        CellPoint nextCell = *(path.end() - 1);
        path.erase(path.end() - 1);
        dir = CheckMoveDirection(cell, nextCell);
        StartBatch();
        maze->SetCellTag(cell, 1);
        maze->Draw(false);
        maze->DrawStart(start);
        maze->DrawGoal(goal);
        DrawMan(maze, cell, dir);
        EndBatch();
        cell = nextCell;
    }

    // 最後の移動
    StartBatch();
    maze->SetCellTag(cell, 1);
    maze->Draw(false);
    maze->DrawStart(start);
    maze->DrawGoal(goal);
    DrawMan(maze, cell, dir);
    EndBatch();
}


