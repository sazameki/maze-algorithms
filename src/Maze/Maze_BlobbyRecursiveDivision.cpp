//
//  Maze_BlobbyRecursiveDivision.cpp
//
//  Created by Satoshi Numata on 2018/12/04.
//  License: Public domain.
//

#include "Maze_BlobbyRecursiveDivision.hpp"
#include <vector>

using namespace std;


/// 分割を切り上げるまでの回数
static const int    kGrowSpeed  = 5;


/// 分割対象の領域を表す構造体
struct BlobbyRegion
{
    vector<CellPoint>   cells;

    void AddCell(const CellPoint& cell)
    {
        cells.push_back(cell);
    }

    void Clear()
    {
        cells.clear();
    }

    bool Contains(const CellPoint& p) const
    {
        for (CellPoint cell : cells) {
            if (cell.x == p.x && cell.y == p.y) {
                return true;
            }
        }
        return false;
    }

    int Count() const
    {
        return (int)cells.size();
    }
};


/// 与えられた領域を2分割する
void DivideRegion(Maze *maze, const BlobbyRegion& region)
{
    // 対象の領域をマーキング
    for (CellPoint cell : region.cells) {
        maze->SetCellTag(cell, 1);
    }
    maze->Draw();

    // マスを2つランダムに選ぶ
    int aIndex, bIndex;
    do {
        aIndex = (int)(random() % region.cells.size());
        bIndex = (int)(random() % region.cells.size());
    } while (aIndex == bIndex);
    CellPoint cellA = region.cells[aIndex];
    CellPoint cellB = region.cells[bIndex];

    // 選んだ2つのマスをマーキング
    maze->SetCellTag(cellA, 2);
    maze->SetCellTag(cellB, 3);
    maze->Draw();

    // 選んだ2つのマスをサブ領域に追加して、領域分割の開始点とする。
    BlobbyRegion    subregions[2];
    vector<CellPoint>   frontiers;
    frontiers.push_back(cellA);
    frontiers.push_back(cellB);
    subregions[0].AddCell(cellA);
    subregions[1].AddCell(cellB);

    // 領域分割の実行
    while (frontiers.size() > 0) {
        int growCount = 0;
        while (frontiers.size() > 0 && growCount < kGrowSpeed) {
            // ランダムにマスを選択
            int index = (int)(random() % frontiers.size());
            CellPoint cell = frontiers[index];

            // 未チェックの近隣のマスを取得する
            vector<CellPoint> neighbors;
            vector<Direction> dirs = MakeAllDirectionsList_shuffled();
            for (Direction dir : dirs) {
                CellPoint neighbor = cell.Move(dir);
                if (region.Contains(neighbor)) {
                    int tag = maze->GetCellTag(neighbor);
                    if (tag != 2 && tag != 3) {
                        neighbors.push_back(neighbor);
                    }
                }
            }

            // 近隣のマスに対して、さらに領域分割する。
            if (neighbors.size() > 0) {
                // 起点のマスがA,Bどちらの領域なのかを取得する
                int tag = maze->GetCellTag(cell);

                // 近隣のマスにA,Bの領域フラグをコピーする
                CellPoint neighbor = neighbors[0];
                maze->SetCellTag(neighbor, tag);
                maze->Draw();

                // サブ領域に近隣のマスを追加する
                subregions[tag - 2].AddCell(neighbor);

                // 近隣のマスもチェック対象の領域として追加する
                frontiers.push_back(neighbor);
                growCount++;
            } else {
                // 近隣をすべてチェック済みのマスは取り除く
                frontiers.erase(frontiers.begin() + index);
            }
        }
    }

    // 分割した領域の間に壁を作る
    vector<Wall> walls;
    for (CellPoint cell : subregions[0].cells) {
        // Aの領域の周囲を調べて、Bの領域のマスが見つかったら、その方向に壁を作る
        vector<Direction> dirs = MakeAllDirectionsList();
        for (Direction dir : dirs) {
            CellPoint neighbor = cell.Move(dir);
            if (maze->IsValidCell(neighbor)) {
                if (maze->GetCellTag(neighbor) == 3) {
                    walls.push_back(Wall(cell, dir));
                }
            }
        }
    }

    // 1つだけ壁を取り除いておく
    int removeIndex = (int)(random() % walls.size());
    walls.erase(walls.begin() + removeIndex);

    // 壁の作成の実行
    for (Wall wall : walls) {
        maze->MakeWall(wall);
        maze->Draw();
    }

    // 領域内のマスをチェック済み状態にする
    for (CellPoint cell : region.cells) {
        maze->SetCellTag(cell, 4);
    }
    maze->Draw();

    // サブ領域をさらに分割する
    for (int i = 0; i < 2; i++) {
        if (subregions[i].Count() >= 2) {
            DivideRegion(maze, subregions[i]);
        } else {
            for (CellPoint cell : subregions[i].cells) {
                maze->SetCellTag(cell, 0);
                maze->Draw();
            }
        }
    }
}

// "Blobby" RecursiveDivisionアルゴリズムの実装
Maze *CreateMaze_BlobbyRecursiveDivision(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize);
    for (int y = 0; y < maze->GetYSize(); y++) {
        maze->MakeWall(0, y, Left);
        maze->MakeWall(maze->GetXSize()-1, y, Right);
    }
    for (int x = 0; x < maze->GetXSize(); x++) {
        maze->MakeWall(x, 0, Up);
        maze->MakeWall(x, maze->GetYSize()-1, Down);
    }
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 最初の領域の用意（すべてのマスを追加する）
    BlobbyRegion region;
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            region.AddCell(CellPoint(x, y));
        }
    }

    // スタート
    DivideRegion(maze, region);

    // 終了
    return maze;
}

