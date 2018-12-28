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
    迷路上の各セルの座標を表す構造体です。
    有効なセルのX座標の値は[0, xSize-1]、Y座標の値は[0, ySize-1]です。
 */
struct CellPoint
{
    /**
        セルのX座標。有効なセルのX座標の値は[0, xSize-1]です。
     */
    int x;

    /**
        セルのY座標。有効なセルのY座標の値は[0, ySize-1]です。
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
        移動後の座標は有効な座標ではない可能性があります。必ずMazeクラスのIsValidCell()関数で有効性をチェックしてください。
     */
    CellPoint   Move(Direction direction) const;
};


/**
    迷路の線の各交点を表す構造体です。
    有効な交点のX座標の値は[0, xSize], Y座標の値は[0, ySize]です。
 */
struct CrossPoint
{
    /**
        交点のX座標。有効な交点のX座標の値は[0, xSize]です。
     */
    int x;

    /**
        交点のY座標。有効な交点のY座標の値は[0, ySize]です。
     */
    int y;

    /**
        コンストラクタ。X座標とY座標はゼロで初期化されます。
     */
    CrossPoint();

    /**
        コンストラクタ。X座標とY座標は指定された値で初期化されます。
        @param  x   交点のX座標
        @param  y   交点のY座標
     */
    CrossPoint(int x, int y);

    /**
        コピーコンストラクタ。座標は指定された座標値で初期化されます。
        @param  pos 交点の座標
     */
    CrossPoint(const CrossPoint& pos);

    /**
        現在の交点の座標から、指定された方向に移動した場所にある交点の座標を計算します。
        移動後の座標は有効な座標ではない可能性があります。必ずMazeクラスのIsValidCrossPoint()関数で有効性をチェックしてください。
     */
    CrossPoint  Move(Direction direction) const;
};

/**
    壁を表す構造体。
    セルの座標と方向で特定の壁を表します。
 */
struct Wall
{
    /**
        壁のあるセルの座標
     */
    CellPoint   pos;

    /**
        壁の方向
     */
    Direction   dir;

    /**
        コンストラクタ。セルの座標と壁のある方向を指定して初期化します。
        @param  pos セルの座標
        @param  dir 壁のある方向
     */
    Wall(const CellPoint& pos, Direction dir);

    /**
        コピーコンストラクタ。壁のあるセルの座標と壁の方向をコピーします。
     */
    Wall(const Wall& wall);
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
        コンストラクタ。指定されたサイズで初期化します。すべてのセルのデータ値は0になります。
        @param xSize    X方向のサイズ
        @param ySize    Y方向のサイズ
     */
    Maze(int xSize, int ySize);

    /**
        コンストラクタ。指定されたサイズで初期化します。すべてのセルのデータ値は指定された値になります。
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

    /**
        指定された座標のセルから、指定された方向に移動できるか（移動先が有効なセルであるか）をチェックします。
        @param  pos 移動元のセル座標
        @param  dir 移動方向
     */
    bool    CanMove(const CellPoint& pos, Direction dir) const;

    /**
        指定された座標のセルから、指定された方向にある壁を削除できるかをチェックします。
        有効なセル座標でない場合や、壁がない場合には、falseがリターンされます。
        @param  x   セルのX座標
        @param  y   セルのY座標
        @param  dir 壁の方向
     */
    bool    CanRemoveWall(int x, int y, Direction dir) const;

    /**
        指定された座標のセルから、指定された方向にある壁を削除できるかをチェックします。
        有効なセル座標でない場合や、壁がない場合には、falseがリターンされます。
        @param  pos セルの座標
        @param  dir 壁の方向
     */
    bool    CanRemoveWall(const CellPoint& pos, Direction dir) const;

    /**
        指定された壁が削除できるかをチェックします。
        有効なセル座標でない場合や、壁がない場合には、falseがリターンされます。
        @param  wall    削除対象の壁（セル座標＋方向）
     */
    bool    CanRemoveWall(const Wall& wall) const;

    /**
        指定された座標のセルの指定された方向に壁があるかをチェックします。
        @param  x   セルのX座標
        @param  y   セルのY座標
        @param  dir 壁の方向
     */
    bool    CheckWall(int x, int y, Direction dir) const;

    /**
        指定された座標のセルの指定された方向に壁があるかをチェックします。
        @param  pos セルの座標
        @param  dir 壁の方向
     */
    bool    CheckWall(const CellPoint& pos, Direction dir) const;

    /**
        指定された情報の壁があるかをチェックします。
        @param  wall    壁の情報（セル座標＋方向）
     */
    bool    CheckWall(const Wall& wall) const;

    /**
        指定されたセル座標が有効な座標かどうかをチェックします。
        @param  pos チェック対象のセル座標
     */
    bool    IsValidCell(const CellPoint& pos) const;

    /**
        指定されたセルの座標から移動可能なすべての方向のリストをvectorとして取得します。
        @param  pos セルの座標
     */
    vector<Direction>   MakeValidMoveDirectionList(const CellPoint& pos) const;

    /**
        指定されたセルの座標から移動可能なすべての方向のリストを、シャッフルされたvectorとして取得します。
        @param  pos セルの座標
     */
    vector<Direction>   MakeValidMoveDirectionList_shuffled(const CellPoint& pos) const;

    /**
        指定されたセル座標の指定された方向に壁を作成します。
        @param  x   セルのX座標
        @param  y   セルのY座標
        @param  dir 壁の方向
     */
    void    MakeWall(int x, int y, Direction dir);

    /**
        指定されたセル座標の指定された方向に壁を作成します。
        @param  pos セルの座標
        @param  dir 壁の方向
     */
    void    MakeWall(const CellPoint& pos, Direction dir);

    /**
        指定された情報の壁を作成します。
        @param  wall    壁の情報（セル座標＋壁の方向）
     */
    void    MakeWall(const Wall& wall);

    /**
        指定されたセル座標の指定された方向の壁を削除します。
        @param  x   セルのX座標
        @param  y   セルのY座標
        @param  dir 壁の方向
     */
    void    RemoveWall(int x, int y, Direction dir);

    /**
        指定されたセル座標の指定された方向の壁を削除します。
        @param  pos セルの座標
        @param  dir 壁の方向
     */
    void    RemoveWall(const CellPoint& pos, Direction dir);

    /**
        指定された情報の壁を削除します。
        @param  wall    壁の情報（セル座標＋壁の方向）
     */
    void    RemoveWall(const Wall& wall);


    // ---- 迷路生成用の関数（交点ベース）

    /**
        指定された交点の座標から、指定された方向に移動できるかをチェックします。
        @param  cp  移動元の交点の座標
        @param  dir 移動方向
     */
    bool    CanMoveFromCrossPoint(const CrossPoint& cp, Direction dir) const;

    /**
        指定された交点から見て上下左右いずれかの方向に壁があるかどうかをチェックします。
        @param  cp  チェック対象の交点座標
     */
    bool    CheckWallFromCrossPoint(const CrossPoint& cp) const;

    /**
        指定された交点から見て、指定された方向に壁があるかどうかをチェックします。
        @param  cp  チェック対象の交点座標
        @param  dir 壁の方向
     */
    bool    CheckWallFromCrossPoint(const CrossPoint& cp, Direction dir) const;

    /**
        与えられた交点の座標が有効なものであるかどうかをチェックします。
        @param  cp  チェック対象の交点座標
     */
    bool    IsValidCrossPoint(const CrossPoint& cp) const;

    /**
        指定された交点座標から、指定された方向に壁を作成します。
        @param  cp  交点座標
        @param  dir 壁を作成する方向
     */
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

