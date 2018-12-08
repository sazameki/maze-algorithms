//
//  Maze_Eller.cpp
//
//  Created by Satoshi Numata on 2018/12/08.
//  License: Public domain.
//

#include "Maze_Eller.hpp"
#include <map>
#include <vector>

using namespace std;


struct EllerSet
{
    Maze    *maze;
    int     tag;
    vector<CellPoint>   cells;

    EllerSet(Maze *_maze, int _tag)
        : maze(_maze), tag(_tag)
    {
        // Do nothing
    }

    void AddCell(const CellPoint& cell)
    {
        cells.push_back(cell);
    }

    void Merge(EllerSet *otherSet)
    {
        for (auto cell : otherSet->cells) {
            cells.push_back(cell);
            maze->SetCellTag(cell, tag);
            maze->Draw();
        }
    }
};


// Ellerのアルゴリズムによる迷路生成
// cf. http://www.neocomputer.org/projects/eller.html
Maze *CreateMaze_Eller(int xSize, int ySize)
{
    map<int, EllerSet *> ellerSetMap;

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

    // タグ番号
    int tagCount = 0;

    for (int y = 0; y < maze->GetYSize(); y++) {
        // タグ付けされていない横方向のマスをすべてタグ付けする
        for (int x = 0; x < maze->GetXSize(); x++) {
            if (maze->GetCellTag(x, y) == 0) {
                tagCount++;
                int tag = tagCount;
                EllerSet *set = new EllerSet(maze, tag);
                ellerSetMap[tag] = set;
                set->AddCell(CellPoint(x, y));
                maze->SetCellTag(x, y, tag);
                maze->Draw();
            }
        }

        // 横方向に縦の壁を作るか、マージするかしていく。
        for (int x = 0; x < maze->GetXSize() - 1; x++) {
            // 同じタグのセルが並んでいれば壁を作らなければならない。そうでなければ最後の行以外、1/2の確率で選ぶ。
            int tag1 = maze->GetCellTag(x, y);
            int tag2 = maze->GetCellTag(x + 1, y);
            if (tag1 == tag2 || (y < maze->GetYSize()-1 && random() % 2 == 0)) {
                maze->MakeWall(x, y, Right);
            }
            // 壁を作らないのであればマージする
            else {
                EllerSet *set1 = ellerSetMap[tag1];
                EllerSet *set2 = ellerSetMap[tag2];
                if (tag1 < tag2) {
                    set1->Merge(set2);
                } else {
                    set2->Merge(set1);
                }
            }
            maze->Draw();
        }

        // ラストの行であればここで終了
        if (y == maze->GetYSize() - 1) {
            break;
        }

        // 各セットの下に床を作る。ただし、必ず一箇所は空けておく。
        int x = 0;
        while (x < maze->GetXSize()) {
            // 連続したセルの長さを求める
            int startX = x;
            int tag = maze->GetCellTag(startX, y);
            int length = 1;
            while (x + 1 < maze->GetXSize() && maze->GetCellTag(x + 1, y) == tag) {
                length++;
                x++;
            }

            // 床を作るX座標
            vector<int> floors;
            for (int x2 = startX; x2 <= x; x2++) {
                floors.push_back(x2);
            }

            // 1つ以上X座標を取り除く
            int removeCount = (int)(random() % floors.size() + 1);
            for (int i = 0; i < removeCount; i++) {
                int r = (int)(random() % floors.size());
                floors.erase(floors.begin() + r);
            }

            // 床を作る
            for (int fx : floors) {
                maze->MakeWall(fx, y, Down);
                maze->Draw();
            }

            // 次のセル
            x++;
        }

        // 床のないセルを次の行にマージしていく
        for (int x = 0; x < maze->GetXSize(); x++) {
            if (!maze->CheckWall(x, y, Down)) {
                int tag = maze->GetCellTag(x, y);
                EllerSet *set = ellerSetMap[tag];
                set->AddCell(CellPoint(x, y + 1));
                maze->SetCellTag(x, y + 1, tag);
                maze->Draw();
            }
        }
    }

    // タグをクリア
    Sleep(0.7f);
    maze->SetTagForAllCells(0);
    maze->Draw();

    // 終了
    return maze;
}

