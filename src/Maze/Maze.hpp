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


// ---- ビットフラグで表すセルのデータ

/**
    セルの上方向の壁を表すビットフラグ
 */
extern const int    kCell_TopBorder;

/**
    セルの右方向の壁を表すビットフラグ
 */
extern const int    kCell_RightBorder;

/**
    セルの下方向の壁を表すビットフラグ
 */
extern const int    kCell_BottomBorder;

/**
    セルの左方向の壁を表すビットフラグ
 */
extern const int    kCell_LeftBorder;

/**
    セルのすべての方向の壁を表すビットフラグ
 */
extern const int    kCell_AllBorders;


/**
    迷路上の各セルの座標を表す構造体。
 */
struct CellPoint
{
    /**
        セルのX座標
     */
    int x;

    /**
        セルのY座標
     */
    int y;

    /**
        コンストラクタ。X座標とY座標はゼロで初期化されます。
     */
    CellPoint();

    /**
        コンストラクタ。X座標とY座標は指定された値で初期化されます。
        @param  x   セルのX座標
        @param  y   セルのY座標
     */
    CellPoint(int x, int y);

    /**
        コピーコンストラクタ。座標は指定された座標値で初期化されます。
        @param  pos セルの座標
     */
    CellPoint(const CellPoint& pos);

    /**
        現在のセルの座標から、指定された方向に移動した場所にあるセルの座標を計算します。
     */
    CellPoint   Move(Direction direction) const;
};


/**
    迷路の線の各交点を表す構造体。
 */
struct CrossPoint
{
    int x;
    int y;

    CrossPoint();
    CrossPoint(int x, int y);
    CrossPoint(const CrossPoint& pos);

    CrossPoint  Move(Direction direction) const;
};

/**
    壁を表す構造体。
    セルの座標と方向で特定の壁を表します。
 */
struct Wall
{
    CellPoint   pos;
    Direction   dir;

    Wall(const CellPoint& pos, Direction dir);
};

/**
    迷路を表すクラス。
    各セルのデータは32ビットのビットフラグで管理しており、0〜3ビット目が壁の有無、4〜31ビット目はタグの値を表します。
 */
class Maze
{
    // ---- 内部データ

    /**
        迷路のX方向のサイズ
     */
    int         xSize;

    /**
        迷路のY方向のサイズ
     */
    int         ySize;

    /**
        迷路のセルのデータ
     */
    unsigned    *data;

public:
    // ---- コンストラクタ・デストラクタ

    /**
        コンストラクタ
        @param xSize    X方向のサイズ
        @param ySize    Y方向のサイズ
     */
    Maze(int xSize, int ySize);

    /**
        コンストラクタ
        @param xSize    X方向のサイズ
        @param ySize    Y方向のサイズ
        @param data     すべてのセルに設定するデータ
     */
    Maze(int xSize, int ySize, unsigned data);

    /**
        デストラクタ
     */
    ~Maze();

private:
    /**
        すべてのコンストラクタから呼び出される初期化関数。
        @param xSize    X方向のサイズ
        @param ySize    Y方向のサイズ
        @param data     すべてのセルに設定するデータ
     */
    void    Create(int xSize, int ySize, unsigned data);

public:
    // ---- Getter系の関数

    /**
        迷路のX方向のサイズをリターンします。
     */
    int     GetXSize() const;

    /**
        迷路のY方向のサイズをリターンします。
     */
    int     GetYSize() const;


    // ---- 迷路生成用の関数（タグ操作）

    /**
        指定された座標のセルのタグをリターンします。
        @param  x   セルのX座標
        @param  y   セルのY座標
     */
    int     GetCellTag(int x, int y) const;

    /**
        指定された座標のセルのタグをリターンします。
        @param  pos セルの座標
     */
    int     GetCellTag(const CellPoint& pos) const;

    /**
        指定された座標のセルのタグをセットします。
        @param  x   セルのX座標
        @param  y   セルのY座標
        @param  tag タグ（0以上65535以下のint値）
     */
    void    SetCellTag(int x, int y, int tag);

    /**
        指定された座標のセルのタグをセットします。
        @param  pos セルの座標
        @param  tag タグ（0以上65535以下のint値）
     */
    void    SetCellTag(const CellPoint& pos, int tag);

    /**
        すべてのセルに同じタグをセットします。
        @param  tag タグ（0以上65535以下のint値）
     */
    void    SetTagForAllCells(int tag);


    // ---- 迷路生成用の関数（セルベース）
    bool    CanMove(const CellPoint& pos, Direction dir) const;
    bool    CanRemoveWall(int x, int y, Direction dir) const;
    bool    CanRemoveWall(const CellPoint& pos, Direction dir) const;
    bool    CanRemoveWall(const Wall& wall) const;
    bool    CheckWall(int x, int y, Direction dir) const;
    bool    CheckWall(const CellPoint& pos, Direction dir) const;
    bool    CheckWall(const Wall& wall) const;
    bool    IsValidCell(const CellPoint& pos) const;
    vector<Direction>   MakeValidMoveDirectionList(const CellPoint& pos) const;
    vector<Direction>   MakeValidMoveDirectionList_shuffled(const CellPoint& pos) const;
    void    MakeWall(int x, int y, Direction dir);
    void    MakeWall(const CellPoint& pos, Direction dir);
    void    MakeWall(const Wall& wall);
    void    RemoveWall(int x, int y, Direction dir);
    void    RemoveWall(const CellPoint& pos, Direction dir);
    void    RemoveWall(const Wall& wall);


    // ---- 迷路生成用の関数（交点ベース）
    bool    CanMoveFromCrossPoint(const CrossPoint& cp, Direction dir) const;
    bool    CheckWallFromCrossPoint(const CrossPoint& cp) const;
    bool    CheckWallFromCrossPoint(const CrossPoint& cp, Direction dir) const;
    bool    IsValidCrossPoint(const CrossPoint& cp) const;
    void    MakeWallFromCrossPoint(const CrossPoint& cp, Direction dir);


    // ---- 迷路の描画

    /**
        迷路を描画します。
        @param  usesBatch   バッチ処理を使用する場合はtrue, 使用しない場合はfalse（デフォルトはtrue）
     */
    void    Draw(bool usesBatch = true);

    /**
        迷路上の交点を描画します。
        @param  pos         交点の座標
        @param  usesBatch   バッチ処理を使用する場合はtrue, 使用しない場合はfalse（デフォルトはtrue）
     */
    void    DrawCrossPoint(const CrossPoint& pos, bool usesBatch = true);


private:
    // ---- 迷路生成用の関数（ビットフラグの操作）

    /**
        指定された座標のセルにビットフラグを追加します。
        @param  x       セルのX座標
        @param  y       セルのY座標
        @param  flag    ビットフラグの値
     */
    void    AddCellFlag(int x, int y, unsigned flag);

    /**
        指定された座標のセルにビットフラグを追加します。
        @param  pos     セルの座標
        @param  flag    ビットフラグの値
     */
    void    AddCellFlag(const CellPoint& pos, unsigned flag);

    /**
        指定された座標のセルからビットフラグを削除します。
        @param  x       セルのX座標
        @param  y       セルのY座標
        @param  flag    ビットフラグの値
     */
    void    RemoveCellFlag(int x, int y, unsigned flag);

    /**
        指定された座標のセルあからビットフラグを削除します。
        @param  pos     セルの座標
        @param  flag    ビットフラグの値
     */
    void    RemoveCellFlag(const CellPoint& pos, unsigned flag);

};


// ---- 迷路生成用のヘルパー関数

/**
    上, 右, 下, 左のすべての方向をこの順番で格納したvectorをリターンします。
 */
vector<Direction>   MakeAllDirectionsList();

/**
    上下左右すべての方向をシャッフルして格納したvectorをリターンします。
 */
vector<Direction>   MakeAllDirectionsList_shuffled();


#endif /* Maze_hpp */

