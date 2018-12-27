//
//  Maze_Wilson.cpp
//
//  Created by Satoshi Numata on 2018/12/21.
//  License: Public domain.
//

#include "Maze_Wilson.hpp"


struct WilsonLog
{
    CellPoint   cell;
    Direction   dir;

    WilsonLog(CellPoint _cell, Direction _dir)
        : cell(_cell), dir(_dir)
    {
        // 何もしない
    }
};

struct WilsonHistory
{
    vector<WilsonLog>   logs;

    void PutLog(WilsonLog log)
    {
        // 同じ座標のログがあれば上書きするために削除しておく
        auto it = logs.begin();
        auto itEnd = logs.end();
        while (it != itEnd) {
            if ((*it).cell.x == log.cell.x && (*it).cell.y == log.cell.y) {
                logs.erase(it);
                break;
            }
            it++;
        }

        // ログを追加
        logs.push_back(log);
    }
};

bool CheckVisited(const vector<WilsonLog>& history, const CellPoint& cell)
{
    for (auto log : history) {
        if (log.cell.x == cell.x && log.cell.y == cell.y) {
            return true;
        }
    }
    return false;
}

// Wilsonのアルゴリズムによる迷路生成
Maze *CreateMaze_Wilson(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->SetTagForAllCells(1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // まだ通過していないセルの数
    int cellCount = maze->GetXSize() * maze->GetYSize();

    // セルAをランダムに選択する
    CellPoint cellA;
    cellA.x = random() % maze->GetXSize();
    cellA.y = random() % maze->GetYSize();
    cellCount--;
    maze->SetCellTag(cellA, 2);
    maze->Draw();

    // まだ訪れていないセルをランダムに選択して、セルBとする
    WilsonHistory history;
    CellPoint cellB;
    while (true) {
        cellB.x = random() % maze->GetXSize();
        cellB.y = random() % maze->GetYSize();
        if (maze->GetCellTag(cellB) == 1) {
            maze->SetCellTag(cellB, 3);
            maze->Draw();
            history.PutLog(WilsonLog(cellB, NoDirection));
            break;
        }
    }

    // セルBからセルAにたどり着くまでランダムウォークして、ログを取る
    CellPoint cell = cellB;
    while (cell.x != cellA.x || cell.y != cellA.y) {
        vector<Direction> dirs = MakeAllDirectionsList_shuffled();
        for (int i = 0; i < 4; i++) {
            CellPoint nextCell = cell.Move(dirs[i]);
            if (maze->IsValidCell(nextCell)) {
                history.PutLog(WilsonLog(cellB, NoDirection));
                if (cell.x != cellB.x || cell.y != cellB.y) {
                    maze->SetCellTag(cell, 4);
                }
                maze->SetCellTag(nextCell, 3);
                maze->Draw();
                Sleep(0.01f);
                cell = nextCell;
                break;
            }
        }
    }


    // 終了
    return maze;
}

