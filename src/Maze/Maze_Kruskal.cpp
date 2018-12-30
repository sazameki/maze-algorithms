//
//  Maze_Kruskal.cpp
//
//  Created by Satoshi Numata on 2018/12/30.
//  License: Public domain.
//

#include "Maze_Kruskal.hpp"

#include <algorithm>


/**
    Kruskalのアルゴリズムで使用するセット
 */
struct KruskalSet
{
    /**
        このセットが表すタグ
     */
    int tag;

    /**
        このセットに属するセル
     */
    vector<CellPoint>   cells;

    /**
        コンストラクタ
        @param  _tag    タグ
        @param  cell    最初のセル
     */
    KruskalSet(int _tag, const CellPoint& cell)
        : tag(_tag)
    {
        cells.push_back(cell);
    }

    /**
        与えられたセルがこのセットに属するかどうかをチェックします。
     */
    bool Contains(const CellPoint& cell) const
    {
        for (auto aCell : cells) {
            if (aCell.x == cell.x && aCell.y == cell.y) {
                return true;
            }
        }
        return false;
    }

    /**
        このセットに含まれるいずれかのセルに隣接する、異なるセットのセルを探します。
        @param  maze    迷路
        @param  outDir  見つかったセルにたどり着くために最後に移動した方向がこの引数にセットされます
     */
    CellPoint FindRandomNeighbor(Maze *maze, Direction &outDir)
    {
        vector<int> indices;
        for (int i = 0; i < cells.size(); i++) {
            indices.push_back(i);
        }
        random_shuffle(indices.begin(), indices.end());
        for (int index : indices) {
            CellPoint cell = cells[index];
            vector<Direction> dirs = MakeAllDirectionsList_shuffled();
            for (auto dir : dirs) {
                CellPoint cell2 = cell.Move(dir);
                if (maze->IsValidCell(cell2) && maze->GetCellTag(cell2) != tag) {
                    outDir = dir;
                    return cell2;
                }
            }
        }
        return CellPoint(-1, -1);
    }

    /**
        異なるセットを、このセットにマージします。
        @param  maze        迷路
        @param  otherSet    マージするセット
     */
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


/**
    Kruskalのアルゴリズムで使用するセットを管理するクラス
 */
struct KruskalSetMap
{
    /**
        セットの配列
     */
    vector<KruskalSet *>    sets;

    /**
        デストラクタ
     */
    ~KruskalSetMap()
    {
        for (auto set : sets) {
            delete set;
        }
        sets.clear();
    }

    /**
        新しいセットを追加します。
     */
    void AddSet(KruskalSet *set)
    {
        sets.push_back(set);
    }

    /**
        与えられたセルを含むセットをリターンします。
     */
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

// Kruskalの迷路生成アルゴリズム
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
    int tag = 0;
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            // 新しいセットの作成
            KruskalSet *set = new KruskalSet(tag, CellPoint(x, y));
            setMap.AddSet(set);

            // タグを付ける
            maze->SetCellTag(x, y, tag);
            maze->Draw();
            tag++;
        }
    }

    // 最初のセットにすべてのセルがマージされるまで繰り返す
    while (setMap.sets[0]->cells.size() < maze->GetXSize() * maze->GetYSize()) {
        // ランダムにセルを1つ選ぶ
        CellPoint cellA;
        cellA.x = random() % maze->GetXSize();
        cellA.y = random() % maze->GetYSize();

        // セルの属するセットに隣接する異なるセットのセルを見つける
        KruskalSet *setA = setMap.FindSet(cellA);
        Direction theDir;
        CellPoint cellB = setA->FindRandomNeighbor(maze, theDir);
        KruskalSet *setB = setMap.FindSet(cellB);

        // 見つかったセルとの間の壁を取り除く
        maze->RemoveWall(cellB, GetOppositeDirection(theDir));

        // 小さなタグのセットにマージする
        if (setA->tag < setB->tag) {
            setA->Merge(maze, setB);
        } else {
            setB->Merge(maze, setA);
        }
    }

    // 終了
    return maze;
}

