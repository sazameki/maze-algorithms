//
//  Drawing.hpp
//
//  Created by Satoshi Numata on 2018/09/16.
//  License: Public domain.
//

#ifndef Drawing_hpp
#define Drawing_hpp


// ---- 変数・定数の定義

// 実行全体に関する変数
extern bool     gIsBatchDrawing;    // バッチ処理がONになっているかどうか
extern float    gFrameRate;         // 現時点でのフレームレート
extern bool     gIsPausing;         // 実行が一時停止中かどうか

// グラフィックス関係の変数
extern unsigned char    *gpBuffer;  // 画面のバッファ(ABGR,ABGR,...)

// キーボード関係の変数
extern unsigned gKeyData;           // キーボードの入力データ

// マウス関係の変数
extern bool     gIsMouseDown;       // 左マウスボタンが押されているかどうか
extern int      gMouseX;            // マウスの現在のX座標
extern int      gMouseY;            // マウスの現在のY座標


// ---- 色の定数

/**
    RGB=(0, 0, 0)の黒を表す色定数です。
 */
extern const int    kColorBlack;

/**
    RGB=(192, 192, 192)の明るいグレーを表す色定数です。
 */
extern const int    kColorLightGray;

/**
    RGB=(64, 64, 64)の暗いグレーを表す色定数です。
 */
extern const int    kColorDarkGray;

/**
    RGB=(128, 128, 128)のグレーを表す色定数です。
 */
extern const int    kColorGray;

/**
    RGB=(255, 255, 255)の白を表す色定数です。
 */
extern const int    kColorWhite;

/**
    RGB=(0, 0, 255)の青を表す色定数です。
 */
extern const int    kColorBlue;

/**
    RGB=(0, 255, 255)の水色を表す色定数です。
 */
extern const int    kColorCyan;

/**
    RGB=(0, 255, 0)の緑を表す色定数です。
 */
extern const int    kColorGreen;

/**
    RGB=(255, 128, 0)のオレンジを表す色定数です。
 */
extern const int    kColorOrange;

/**
    RGB=(255, 0, 255)の紫色を表す色定数です。
 */
extern const int    kColorPurple;

/**
    RGB=(255, 0, 0)の赤を表す色定数です。
 */
extern const int    kColorRed;

/**
    RGB=(255, 255, 0)の黄色を表す色定数です。
 */
extern const int    kColorYellow;

/**
    RGB=(128, 128, 255)の明るい青を表す色定数です。
 */
extern const int    kColorLightBlue;

/**
    RGB=(128, 255, 255)の明るい水色を表す色定数です。
 */
extern const int    kColorLightCyan;

/**
    RGB=(128, 255, 128)の明るい緑を表す色定数です。
 */
extern const int    kColorLightGreen;

/**
    RGB=(255, 128, 64)の明るいオレンジを表す色定数です。
 */
extern const int    kColorLightOrange;

/**
    RGB=(255, 128, 255)の明るい紫色を表す色定数です。
 */
extern const int    kColorLightPurple;

/**
    RGB=(255, 128, 128)の明るい赤を表す色定数です。
 */
extern const int    kColorLightRed;

/**
    RGB=(255, 255, 128)の明るい黄色を表す色定数です。
 */
extern const int    kColorLightYellow;

/**
    RGB=(0, 0, 128)の暗い青を表す色定数です。
 */
extern const int    kColorDarkBlue;

/**
    RGB=(0, 128, 128)の暗い水色を表す色定数です。
 */
extern const int    kColorDarkCyan;

/**
    RGB=(0, 128, 0)の暗い緑を表す色定数です。
 */
extern const int    kColorDarkGreen;

/**
    RGB=(128, 64, 0)の暗いオレンジを表す色定数です。
 */
extern const int    kColorDarkOrange;

/**
    RGB=(128, 0, 128)の暗い紫色を表す色定数です。
 */
extern const int    kColorDarkPurple;

/**
    RGB=(128, 0, 0)の暗い赤を表す色定数です。
 */
extern const int    kColorDarkRed;

/**
    RGB=(128, 128, 0)の暗い黄色を表す色定数です。
 */
extern const int    kColorDarkYellow;


// ---- キーボードの定数

/**
    Aのキーを表すキーマスク定数です。
 */
extern const unsigned   kKeyA;

/**
    Sのキーを表すキーマスク定数です。
 */
extern const unsigned   kKeyS;

/**
    Dのキーを表すキーマスク定数です。
 */
extern const unsigned   kKeyD;

/**
    Wのキーを表すキーマスク定数です。
 */
extern const unsigned   kKeyW;

/**
    上矢印キーを表すキーマスク定数です。
 */
extern const unsigned   kKeyUpArrow;

/**
    下矢印キーを表すキーマスク定数です。
 */
extern const unsigned   kKeyDownArrow;

/**
    左矢印キーを表すキーマスク定数です。
 */
extern const unsigned   kKeyLeftArrow;

/**
    右矢印キーを表すキーマスク定数です。
 */
extern const unsigned   kKeyRightArrow;

/**
    リターンキーを表すキーマスク定数です。
 */
extern const unsigned   kKeyReturn;

/**
    スペースキーを表すキーマスク定数です。
 */
extern const unsigned   kKeySpace;



// ---- 全般的な操作

/**
    秒数で指定した時間だけ処理を中断します。
    @param  seconds 秒単位のスリープ時間
 */
void    Sleep(float seconds);

/**
    バッチ処理を開始します。バッチ処理を行っている間、画面上に描画結果は反映されません。
 */
void    StartBatch();

/**
    バッチ処理を終了します。バッチ処理を行っている間に実行されたすべての描画結果が、画面上に反映されます。
 */
void    EndBatch();



// ---- グラフィックス操作

/**
    指定した色で画面をクリアします。
    @param  color   色
 */
void    Clear(int color);

/**
    指定した座標の色を取得します。
    @param  x   X座標
    @param  y   Y座標
    @return 指定された座標の色
 */
int     GetColor(int x, int y);

/**
    指定した座標に点を描画します。
    @param  x   X座標
    @param  y   Y座標
    @param  color   色
 */
void    DrawPoint(int x, int y, int color);

/**
    指定した2点の座標を結ぶ直線を描画します。
    @param  x1  点1のX座標
    @param  y1  点1のY座標
    @param  x2  点2のX座標
    @param  y2  点2のY座標
    @param  color   色
 */
void    DrawLine(int x1, int y1, int x2, int y2, int color);

/**
    指定した3点の座標を結ぶ三角形を描画します。
    @param  x1  点1のX座標
    @param  y1  点1のY座標
    @param  x2  点2のX座標
    @param  y2  点2のY座標
    @param  x3  点3のX座標
    @param  y3  点3のY座標
    @param  color   色
 */
void    DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);

/**
    指定した3点の座標を結ぶ三角形を塗りつぶします。
    @param  x1  点1のX座標
    @param  y1  点1のY座標
    @param  x2  点2のX座標
    @param  y2  点2のY座標
    @param  x3  点3のX座標
    @param  y3  点3のY座標
    @param  color   色
 */
void    FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);

/**
    指定した座標から指定した大きさの四角形を描画します。
    @param  x       矩形の左下の点のX座標
    @param  y       矩形の左下の点のY座標
    @param  width   矩形の横のサイズ
    @param  height  矩形の縦のサイズ
    @param  color   色
 */
void    DrawRect(int x, int y, int width, int height, int color);

/**
    指定した座標から指定した大きさの四角形を塗りつぶします。
    @param  x       矩形の左下の点のX座標
    @param  y       矩形の左下の点のY座標
    @param  width   矩形の横のサイズ
    @param  height  矩形の縦のサイズ
    @param  color   色
 */
void    FillRect(int x, int y, int width, int height, int color);


// TODO: 正円の縦横比を変えて楕円を描画するための引数をDrawCircle()関数とFillCircle()関数に追加する。


/**
    指定した中心点と半径で円を描画します。
    @param  cx      中心点のX座標
    @param  cy      中心点のY座標
    @param  radius  半径
    @param  color   色
 */
void    DrawCircle(int cx, int cy, int radius, int color);

/**
    指定した中心点と半径、開始角度と終了角度をラジアン単位で指定して、円弧を描画します。
    角度は-πから+πの範囲に修正された上で比較されます。
    0〜5/2πが指定された場合、これは0〜1/2πが指定されたことになりますので、第1象限の90度の円弧が描画されます。
    @param  cx      中心点のX座標
    @param  cy      中心点のY座標
    @param  radius  半径
    @param  color   色
    @param  startAngleRad   ラジアン単位の開始角度
    @param  endAngleRad     ラジアン単位の終了角度
 */
void    DrawCircle(int cx, int cy, int radius, int color, float startAngleRad, float endAngleRad);

/**
    指定した中心点と半径で円を塗りつぶします。
    @param  cx      中心点のX座標
    @param  cy      中心点のY座標
    @param  radius  半径
    @param  color   色
 */
void    FillCircle(int cx, int cy, int radius, int color);

/**
    指定した中心点と半径、開始角度と終了角度をラジアン単位で指定して、扇形の図形を塗りつぶし描画します。
    角度は-πから+πの範囲に修正された上で比較されます。
    0〜5/2πが指定された場合、これは0〜1/2πが指定されたことになりますので、第1象限の90度の扇形が描画されます。
    @param  cx      中心点のX座標
    @param  cy      中心点のY座標
    @param  radius  半径
    @param  color   色
    @param  startAngleRad   ラジアン単位の開始角度
    @param  endAngleRad     ラジアン単位の終了角度
 */
void    FillCircle(int cx, int cy, int radius, int color, float startAngleRad, float endAngleRad);

/**
    指定した座標からスキャンを開始して、borderColorの色で囲まれた領域をpaintColorの色で塗りつぶします。
    画面(-320, -240)-(319, 239)より外の領域の座標が指定された場合、塗りつぶしは行われません。
    @param  x   スキャンを開始する点のX座標
    @param  y   スキャンを開始する点のY座標
    @param  paintColor  塗りつぶし色
    @param  borderColor 境界線の色
 */
void    Paint(int x, int y, int paintColor, int borderColor);

/**
    指定された座標にグラフィック・パターンを描画します。
    グラフィックパターンはunsigned int型の配列で、先頭2つの要素がX方向の大きさとY方向の大きさ、
    その後はARGB,ARGB,ARGB,...となります。ARGBの各要素は、0x00〜0xffの8ビットで表されます。
    A（アルファ値）のデータは、0だと不透明、それ以外だと透明なピクセルとして扱われます。
    @param  buffer  グラフィック・パターンを格納したバッファ
    @param  x       パターン描画の対象矩形の左下の点のX座標
    @param  y       パターン描画の対象矩形の左下の点のY座標
 */
void    DrawPattern(unsigned int *buffer, int x, int y);

/**
    指定された座標に文字を描画します。それぞれの文字は12x20ピクセルのサイズです。
    @param  c       描画する文字
    @param  x       描画対象の矩形領域の左下の点のX座標
    @param  y       描画対象の矩形領域の左下の点のY座標
    @param  color   色
 */
void    DrawCharacter(char c, int x, int y, int color);

/**
    指定された座標に文字列を描画します。それぞれの文字は12x20ピクセルのサイズです。
    @param  str     描画する文字列
    @param  x       描画対象の矩形領域の左下の点のX座標
    @param  y       描画対象の矩形領域の左下の点のY座標
    @param  color   色
 */
void    DrawText(const char *str, int x, int y, int color);

/**
    画面をスクロールします。
    @param  x   X方向の移動量
    @param  y   Y方向の移動量
 */
void    Scroll(int x, int y);



// ---- キーボード関係の操作

/**
    指定したキーが押されているかどうかを判定します。
    @param  key キーマスク（kKeyUpArrow, kKeyRightArrow, kKeyReturn, kKeySpaceなどの定数またはその組み合わせ）
    @return 指定されたキーが押されていればtrue、そうでなければfalse
 */
bool    CheckKey(unsigned int key);



// ---- マウス関係の操作

/**
    マウスカーソルの画面上のX座標を取得します。
    画面の中央が0になる、描画システムと同じ座標です。
 */
int     GetMouseX();

/**
    マウスカーソルの画面上のY座標を取得します。
    画面の中央が0になる、描画システムと同じ座標です。
 */
int     GetMouseY();

/**
    マウスの左ボタンが押されているかどうかを取得します。
    @return マウスの左ボタンが押されていればtrue、そうでなければfalse
 */
bool    CheckMouse();


#endif /* Drawing_hpp */

