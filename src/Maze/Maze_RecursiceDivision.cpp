//
//  Maze_RecursiceDivision.cpp
//
//  Created by Satoshi Numata on 2018/12/05.
//  License: Public domain.
//

#include "Maze_RecursiceDivision.hpp"


struct RDRegion
{
    int x;
    int y;
    int width;
    int height;
    
    RDRegion()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }
    
    RDRegion(int _x, int _y, int _width, int _height)
    {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
    }
    
    RDRegion(const RDRegion& region)
    {
        x = region.x;
        y = region.y;
        width = region.width;
        height = region.height;
    }
};

/// 次に分割する方向を決定する
Direction ChooseDirection(const RDRegion& region)
{
    if (region.width == region.height) {
        if (random() % 2 == 0) {
            return Right;
        } else {
            return Down;
        }
    } else if (region.width < region.height) {
        return Right;
    } else {
        return Down;
    }
}

/// 領域を分割する（与えられた領域に対して壁を作る）
void DivideRegion(Maze *maze, const RDRegion& region)
{
    // 領域のマーキング
    for (int y = region.y; y < region.y+region.height; y++) {
        for (int x = region.x; x < region.x+region.width; x++) {
            maze->SetCellTag(x, y, 1);
        }
    }
    maze->Draw();

    // 壁を作る方向を確認する
    Direction makingDir = ChooseDirection(region);

    // 壁の座標を計算する
    CellPoint wallPos(region.x, region.y);
    if (makingDir == Right) {
        if (region.height > 2) {
            wallPos.y += random() % (region.height - 2);
        }
    } else {
        if (region.width > 2) {
            wallPos.x += random() % (region.width - 2);
        }
    }
    
    // 分割された領域のマーキング
    if (makingDir == Right) {
        for (int y = region.y; y <= wallPos.y; y++) {
            for (int x = region.x; x < region.x+region.width; x++) {
                maze->SetCellTag(x, y, 2);
            }
        }
        maze->Draw();
        for (int y = wallPos.y+1; y < region.y+region.height; y++) {
            for (int x = region.x; x < region.x+region.width; x++) {
                maze->SetCellTag(x, y, 3);
            }
        }
        maze->Draw();
    } else {
        for (int x = region.x; x <= wallPos.x; x++) {
            for (int y = region.y; y < region.y+region.height; y++) {
                maze->SetCellTag(x, y, 2);
            }
        }
        maze->Draw();
        for (int x = wallPos.x+1; x < region.x+region.width; x++) {
            for (int y = region.y; y < region.y+region.height; y++) {
                maze->SetCellTag(x, y, 3);
            }
        }
        maze->Draw();
    }

    // 壁の方向と距離
    Direction wallDir = (makingDir == Right)? Down: Right;
    int length = (makingDir == Right)? region.width: region.height;
    
    // 壁を延ばす処理
    vector<Wall> walls;
    CellPoint pos(wallPos);
    for (; length > 0; length--) {
        walls.push_back(Wall(pos, wallDir));
        pos = pos.Move(makingDir);
    }

    // 1つだけ壁を取り除いておく
    int removeIndex = (int)(random() % walls.size());
    walls.erase(walls.begin() + removeIndex);
    
    // 壁の作成の実行
    for (Wall wall : walls) {
        maze->MakeWall(wall);
    }
    maze->Draw();

    // 領域のマーキング
    for (int y = region.y; y < region.y+region.height; y++) {
        for (int x = region.x; x < region.x+region.width; x++) {
            maze->SetCellTag(x, y, 4);
        }
    }
    maze->Draw();

    // サブ領域の分割
    RDRegion subregions[2];
    if (makingDir == Right) {
        subregions[0] = RDRegion(region.x, region.y, region.width, wallPos.y-region.y+1);
        subregions[1] = RDRegion(region.x, wallPos.y+1, region.width, region.y+region.height-wallPos.y-1);
    } else {
        subregions[0] = RDRegion(region.x, region.y, wallPos.x-region.x+1, region.height);
        subregions[1] = RDRegion(wallPos.x+1, region.y, region.x+region.width-wallPos.x-1, region.height);
    }
    for (int i = 0; i < 2; i++) {
        if (subregions[i].width >= 2 && subregions[i].height >= 2) {
            DivideRegion(maze, subregions[i]);
        } else {
            for (int y = subregions[i].y; y < subregions[i].y+subregions[i].height; y++) {
                for (int x = subregions[i].x; x < subregions[i].x+subregions[i].width; x++) {
                    maze->SetCellTag(x, y, 0);
                }
            }
            maze->Draw();
        }
    }
}

// Recursive Divisionのアルゴリズムによる迷路生成
Maze *CreateMaze_RecursiveDivision(int xSize, int ySize)
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

    // スタート
    RDRegion region(0, 0, maze->GetXSize(), maze->GetYSize());
    DivideRegion(maze, region);
    
    // 終了
    return maze;
}

