//
//  Maze_Kruskal.cpp
//
//  Created by Satoshi Numata on 2018/12/30.
//  License: Public domain.
//

#include "Maze_Kruskal.hpp"


struct KruskalSet
{
    int tag;
    vector<CellPoint>   cells;

    KruskalSet(int _tag)
        : tag(_tag)
    {
        // Do nothing
    }

    void AddCell(const CellPoint& cell)
    {
        cells.push_back(cell);
    }

    bool Contains(const CellPoint& cell) const
    {
        for (auto aCell : cells) {
            if (aCell.x == cell.x && aCell.y == cell.y) {
                return true;
            }
        }
        return false;
    }

    CellPoint FindRandomNeighbor(Maze *maze, Direction &theDir)
    {
        for (auto cell : cells) {
            vector<Direction> dirs = MakeAllDirectionsList_shuffled();
            for (auto dir : dirs) {
                CellPoint cell2 = cell.Move(dir);
                if (maze->IsValidCell(cell2) && maze->GetCellTag(cell2) != tag) {
                    theDir = dir;
                    return cell2;
                }
            }
        }
        return CellPoint(-1, -1);
    }

    void Merge(Maze *maze, KruskalSet *otherSet)
    {
        for (auto cell : otherSet->cells) {
            maze->SetCellTag(cell, tag);
            maze->Draw();
            cells.push_back(cell);
        }
        otherSet->cells.clear();
    }
};


struct KruskalSetMap
{
    vector<KruskalSet *>    sets;

    ~KruskalSetMap()
    {
        for (auto set : sets) {
            delete set;
        }
        sets.clear();
    }

    void AddSet(KruskalSet *set)
    {
        sets.push_back(set);
    }

    KruskalSet *FindSet(const CellPoint& cell)
    {
        for (auto set : sets) {
            if (set->Contains(cell)) {
                return set;
            }
        }
        return nullptr;
    }
};

Maze *CreateMaze_Kruskal(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 各セルごとに異なるセットを用意する
    KruskalSetMap setMap;
    int tag = 1;
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            // 新しいセットの作成
            KruskalSet *set = new KruskalSet(tag);
            set->AddCell(CellPoint(x, y));
            setMap.AddSet(set);

            // タグを付ける
            maze->SetCellTag(x, y, tag);
            maze->Draw();
            tag++;
        }
    }

    KruskalSet *firstSet = setMap.FindSet(CellPoint(0, 0));
    while (firstSet->cells.size() < maze->GetXSize() * maze->GetYSize()) {
        // ランダムにセルを1つ選ぶ
        CellPoint cellA;
        cellA.x = random() % maze->GetXSize();
        cellA.y = random() % maze->GetYSize();

        // セルの属するセットに隣接する異なるセットのセルを見つける
        KruskalSet *setA = setMap.FindSet(cellA);
        Direction theDir;
        CellPoint cellB = setA->FindRandomNeighbor(maze, theDir);
        KruskalSet *setB = setMap.FindSet(cellB);
        maze->RemoveWall(cellB, GetOppositeDirection(theDir));
        if (setA->tag < setB->tag) {
            setA->Merge(maze, setB);
        } else {
            setB->Merge(maze, setA);
        }
    }

    // タグをクリア
    Sleep(0.7f);
    maze->SetTagForAllCells(0);
    maze->Draw();

    // 終了
    return maze;
}

