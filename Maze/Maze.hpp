//
//  Maze.hpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#ifndef Maze_hpp
#define Maze_hpp

#include "Direction.hpp"
#include <vector>


using namespace std;


///// ビットフラグで表すブロックのデータ

extern const int    kBlock_TopBorder;
extern const int    kBlock_RightBorder;
extern const int    kBlock_BottomBorder;
extern const int    kBlock_LeftBorder;
extern const int    kBlock_AllBorders;

extern const int    kBlock_CreateMarker1;
extern const int    kBlock_CreateMarker2;
extern const int    kBlock_CreateMarker3;
extern const int    kBlock_CreateMarker4;
extern const int    kBlock_AllCreateMarkers;

extern const int    kBlock_SolveMarker1;
extern const int    kBlock_SolveMarker2;
extern const int    kBlock_SolveMarker3;
extern const int    kBlock_AllSolveMarkers;


/// 迷路上の各マスの座標を表す構造体
struct Point
{
    int x;
    int y;

    Point();
    Point(int x, int y);
    Point(const Point& pos);

    Point   Move(int direction) const;
};

/// 壁を表す構造体
struct Wall
{
    Point       pos;
    Direction   dir;

    Wall(const Point& pos, Direction dir);
};

/// 迷路を表すクラス。
/// 各マスのデータはビットフラグで管理しており、0〜3ビット目が壁の有無、4〜6ビット目が生成用のマーカー、7〜9ビット目が解くためのマーカーである。
class Maze
{
    // 内部データ
    int     xSize;
    int     ySize;
    int     *data;

public:
    // コンストラクタ・デストラクタ
    Maze(int xSize, int ySize);
    Maze(int xSize, int ySize, int data);
    ~Maze();
    void    Create(int xSize, int ySize, int data);

public:
    // Getter系の関数
    int     GetXSize() const;
    int     GetYSize() const;
    int     GetData(int x, int y) const;
    int     GetData(const Point& pos) const;
    bool    CheckFlag(int x, int y, int flag) const;
    bool    CheckFlag(const Point& pos, int flag) const;
    bool    IsValid(const Point& pos) const;

    // 迷路生成用の関数
    void    AddFlag(int x, int y, int flag);
    void    AddFlag(const Point& pos, int flag);
    void    RemoveFlag(int x, int y, int flag);
    void    RemoveFlag(const Point& pos, int flag);
    bool    CheckWall(int x, int y, Direction dir) const;
    bool    CheckWall(const Point& pos, Direction dir) const;
    bool    CheckWall(const Wall& wall) const;
    bool    CanRemoveWall(int x, int y, Direction dir) const;
    bool    CanRemoveWall(const Point& pos, Direction dir) const;
    bool    CanRemoveWall(const Wall& wall) const;
    void    MakeWall(int x, int y, Direction dir);
    void    MakeWall(const Point& pos, Direction dir);
    void    MakeWall(const Wall& wall);
    void    RemoveWall(int x, int y, Direction dir);
    void    RemoveWall(const Point& pos, Direction dir);
    void    RemoveWall(const Wall& wall);

    // 迷路の描画
    void    Draw(bool usesBatch = true);

};


///// 迷路生成用のヘルパー関数

vector<Direction>   MakeAllDirectionsList();
vector<Direction>   MakeAllDirectionsList_shuffled();


#endif /* Maze_hpp */

