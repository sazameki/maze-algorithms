#include "Drawing.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <map>
#include <vector>


// 描画全体に関する変数
bool    gIsBatchDrawing = false;
float   gFrameRate = 0.0f;
bool    gIsPausing = false;

// グラフィックス関係の変数
unsigned char   *gpBuffer = NULL;

// キーボード関係の変数
unsigned gKeyData = 0;

// マウス関係の変数
bool    gIsMouseDown = false;
int     gMouseX = 320;
int     gMouseY = 240;

// 色の定数
const int   kColorBlack     = 0x000000;
const int   kColorLightGray = 0xc0c0c0;
const int   kColorDarkGray  = 0x404040;
const int   kColorGray      = 0x808080;
const int   kColorWhite     = 0xffffff;
const int   kColorRed       = 0xff0000;
const int   kColorGreen     = 0x00ff00;
const int   kColorBlue      = 0x0000ff;
const int   kColorYellow    = 0xffff00;
const int   kColorOrange    = 0xff8000;
const int   kColorDarkBlue  = 0x00007f;

// キーボードの定数
const unsigned  kKeyA           = 0x0001;
const unsigned  kKeyS           = 0x0002;
const unsigned  kKeyD           = 0x0004;
const unsigned  kKeyW           = 0x0008;
const unsigned  kKeyUpArrow     = 0x0010;
const unsigned  kKeyDownArrow   = 0x0020;
const unsigned  kKeyLeftArrow   = 0x0040;
const unsigned  kKeyRightArrow  = 0x0080;
const unsigned  kKeyReturn      = 0x0100;
const unsigned  kKeySpace       = 0x0200;


// 文字のビットマップフォント（Osaka等倍フォント12ptを参照）
const unsigned char kCharPattern[][10] =
{
    { 0b00000, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000, 0b00100, 0b00000, 0b00000 },   // !
    { 0b00000, 0b01010, 0b01010, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },   // "
    { 0b00000, 0b01010, 0b01010, 0b11111, 0b01010, 0b11111, 0b01010, 0b01010, 0b00000, 0b00000 },   // #
    { 0b00100, 0b01110, 0b10101, 0b10100, 0b01110, 0b00101, 0b10101, 0b01110, 0b00100, 0b00000 },   // $
    { 0b00000, 0b01111, 0b10101, 0b10110, 0b01010, 0b01101, 0b10101, 0b10010, 0b00000, 0b00000 },   // %
    { 0b00000, 0b01100, 0b10010, 0b10100, 0b01000, 0b10101, 0b10010, 0b01101, 0b00000, 0b00000 },   // &
    { 0b00000, 0b00100, 0b00100, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },   // '
    { 0b00000, 0b00010, 0b00100, 0b01000, 0b01000, 0b01000, 0b00100, 0b00010, 0b00000, 0b00000 },   // (
    { 0b00000, 0b01000, 0b00100, 0b00010, 0b00010, 0b00010, 0b00100, 0b01000, 0b00000, 0b00000 },   // )
    { 0b00000, 0b00100, 0b10101, 0b01110, 0b10101, 0b00100, 0b00000, 0b00000, 0b00000, 0b00000 },   // *
    { 0b00000, 0b00000, 0b00100, 0b00100, 0b11111, 0b00100, 0b00100, 0b00000, 0b00000, 0b00000 },   // +
    { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00100, 0b00100, 0b01000 },   // ,
    { 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },   // -
    { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00100, 0b00000, 0b00000 },   // .
    { 0b00000, 0b00001, 0b00001, 0b00010, 0b00010, 0b00100, 0b00100, 0b01000, 0b01000, 0b00000 },   // /
    { 0b00000, 0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110, 0b00000, 0b00000 },   // 0
    { 0b00000, 0b00010, 0b01110, 0b00010, 0b00010, 0b00010, 0b00010, 0b00010, 0b00000, 0b00000 },   // 1
    { 0b00000, 0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111, 0b00000, 0b00000 },   // 2
    { 0b00000, 0b01110, 0b10001, 0b00001, 0b00110, 0b00001, 0b10001, 0b01110, 0b00000, 0b00000 },   // 3
    { 0b00000, 0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010, 0b00000, 0b00000 },   // 4
    { 0b00000, 0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110, 0b00000, 0b00000 },   // 5
    { 0b00000, 0b01110, 0b10000, 0b11110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000 },   // 6
    { 0b00000, 0b11111, 0b00001, 0b00010, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000 },   // 7
    { 0b00000, 0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000 },   // 8
    { 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110, 0b00000, 0b00000 },   // 9
    { 0b00000, 0b00000, 0b00000, 0b00100, 0b00000, 0b00000, 0b00000, 0b00100, 0b00000, 0b00000 },   // :
    { 0b00000, 0b00000, 0b00000, 0b00100, 0b00000, 0b00000, 0b00000, 0b00100, 0b00100, 0b01000 },   // ;
    { 0b00000, 0b00000, 0b00010, 0b00100, 0b01000, 0b00100, 0b00010, 0b00000, 0b00000, 0b00000 },   // <
    { 0b00000, 0b00000, 0b00000, 0b11111, 0b00000, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000 },   // =
    { 0b00000, 0b00000, 0b01000, 0b00100, 0b00010, 0b00100, 0b01000, 0b00000, 0b00000, 0b00000 },   // >
    { 0b00000, 0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b00000, 0b00100, 0b00000, 0b00000 },   // ?
    { 0b00000, 0b01110, 0b10001, 0b11101, 0b10101, 0b11110, 0b10000, 0b01110, 0b00000, 0b00000 },   // @
    { 0b00000, 0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001, 0b00000, 0b00000 },   // A
    { 0b00000, 0b11110, 0b10001, 0b10001, 0b11110, 0b10001, 0b10001, 0b11110, 0b00000, 0b00000 },   // B
    { 0b00000, 0b01110, 0b10001, 0b10000, 0b10000, 0b10000, 0b10001, 0b01110, 0b00000, 0b00000 },   // C
    { 0b00000, 0b11110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11110, 0b00000, 0b00000 },   // D
    { 0b00000, 0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b11111, 0b00000, 0b00000 },   // E
    { 0b00000, 0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000, 0b00000, 0b00000 },   // F
    { 0b00000, 0b01110, 0b10001, 0b10000, 0b10011, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000 },   // G
    { 0b00000, 0b10001, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001, 0b00000, 0b00000 },   // H
    { 0b00000, 0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000, 0b00000 },   // I
    { 0b00000, 0b00001, 0b00001, 0b00001, 0b00001, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000 },   // J
    { 0b00000, 0b10001, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b10001, 0b00000, 0b00000 },   // K
    { 0b00000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111, 0b00000, 0b00000 },   // L
    { 0b00000, 0b10001, 0b11011, 0b10101, 0b10001, 0b10001, 0b10001, 0b10001, 0b00000, 0b00000 },   // M
    { 0b00000, 0b10001, 0b11001, 0b10101, 0b10011, 0b10001, 0b10001, 0b10001, 0b00000, 0b00000 },   // N
    { 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000 },   // O
    { 0b00000, 0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000, 0b10000, 0b00000, 0b00000 },   // P
    { 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0b00001, 0b00000 },   // Q
    { 0b00000, 0b11110, 0b10001, 0b10001, 0b11110, 0b10001, 0b10001, 0b10001, 0b00000, 0b00000 },   // R
    { 0b00000, 0b01110, 0b10001, 0b10000, 0b01110, 0b00001, 0b10001, 0b01110, 0b00000, 0b00000 },   // S
    { 0b00000, 0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000 },   // T
    { 0b00000, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000 },   // U
    { 0b00000, 0b10001, 0b10001, 0b10001, 0b10001, 0b01010, 0b01010, 0b00100, 0b00000, 0b00000 },   // V
    { 0b00000, 0b10001, 0b10001, 0b10001, 0b10001, 0b10101, 0b11011, 0b10001, 0b00000, 0b00000 },   // W
    { 0b00000, 0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b01010, 0b10001, 0b00000, 0b00000 },   // X
    { 0b00000, 0b10001, 0b10001, 0b01010, 0b01010, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000 },   // Y
    { 0b00000, 0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000, 0b11111, 0b00000, 0b00000 },   // Z
    { 0b00000, 0b00110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00110, 0b00000, 0b00000 },   // [
    { 0b00000, 0b01000, 0b01000, 0b00100, 0b00100, 0b00010, 0b00010, 0b00001, 0b00001, 0b00000 },   // (backslash)
    { 0b00000, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01100, 0b00000, 0b00000 },   // ]
    { 0b00000, 0b00100, 0b01010, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },   // ^
    { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b01110, 0b00000, 0b00000 },   // _
    { 0b00000, 0b00100, 0b00010, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },   // `
    { 0b00000, 0b00000, 0b00000, 0b01111, 0b10001, 0b10001, 0b10011, 0b01101, 0b00000, 0b00000 },   // a
    { 0b00000, 0b10000, 0b10000, 0b11110, 0b10001, 0b10001, 0b10001, 0b11110, 0b00000, 0b00000 },   // b
    { 0b00000, 0b00000, 0b00000, 0b01110, 0b10001, 0b10000, 0b10000, 0b01111, 0b00000, 0b00000 },   // c
    { 0b00000, 0b00001, 0b00001, 0b01111, 0b10001, 0b10001, 0b10001, 0b01111, 0b00000, 0b00000 },   // d
    { 0b00000, 0b00000, 0b00000, 0b01110, 0b10001, 0b11111, 0b10000, 0b01111, 0b00000, 0b00000 },   // e
    { 0b00000, 0b00011, 0b00100, 0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000 },   // f
    { 0b00000, 0b00000, 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110 },   // g
    { 0b00000, 0b10000, 0b10000, 0b11110, 0b10001, 0b10001, 0b10001, 0b10001, 0b00000, 0b00000 },   // h
    { 0b00000, 0b00100, 0b00000, 0b01100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000, 0b00000 },   // i
    { 0b00000, 0b00100, 0b00000, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b11000 },   // j
    { 0b00000, 0b10000, 0b10000, 0b10010, 0b10100, 0b11100, 0b10010, 0b10001, 0b00000, 0b00000 },   // k
    { 0b00000, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000, 0b00000 },   // l
    { 0b00000, 0b00000, 0b00000, 0b11110, 0b10101, 0b10101, 0b10101, 0b10101, 0b00000, 0b00000 },   // m
    { 0b00000, 0b00000, 0b00000, 0b10110, 0b11001, 0b10001, 0b10001, 0b10001, 0b00000, 0b00000 },   // n
    { 0b00000, 0b00000, 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000 },   // o
    { 0b00000, 0b00000, 0b00000, 0b11110, 0b10001, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000 },   // p
    { 0b00000, 0b00000, 0b00000, 0b01111, 0b10001, 0b10001, 0b10001, 0b01111, 0b00001, 0b00001 },   // q
    { 0b00000, 0b00000, 0b00000, 0b10110, 0b11001, 0b10000, 0b10000, 0b10000, 0b00000, 0b00000 },   // r
    { 0b00000, 0b00000, 0b00000, 0b01111, 0b10000, 0b01110, 0b00001, 0b11110, 0b00000, 0b00000 },   // s
    { 0b00000, 0b00100, 0b00100, 0b01110, 0b00100, 0b00100, 0b00100, 0b00011, 0b00000, 0b00000 },   // t
    { 0b00000, 0b00000, 0b00000, 0b10001, 0b10001, 0b10001, 0b10011, 0b01101, 0b00000, 0b00000 },   // u
    { 0b00000, 0b00000, 0b00000, 0b10001, 0b10001, 0b10001, 0b01010, 0b00100, 0b00000, 0b00000 },   // v
    { 0b00000, 0b00000, 0b00000, 0b10101, 0b10101, 0b10101, 0b10101, 0b01010, 0b00000, 0b00000 },   // w
    { 0b00000, 0b00000, 0b00000, 0b10001, 0b01010, 0b00100, 0b01010, 0b10001, 0b00000, 0b00000 },   // x
    { 0b00000, 0b00000, 0b00000, 0b10001, 0b10001, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110 },   // y
    { 0b00000, 0b00000, 0b00000, 0b11111, 0b00010, 0b00100, 0b01000, 0b11111, 0b00000, 0b00000 },   // z
    { 0b00001, 0b00010, 0b00010, 0b00010, 0b00100, 0b00010, 0b00010, 0b00010, 0b00001, 0b00000 },   // {
    { 0b00000, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000 },   // |
    { 0b10000, 0b01000, 0b01000, 0b01000, 0b00100, 0b01000, 0b01000, 0b01000, 0b10000, 0b00000 },   // }
    { 0b00000, 0b01101, 0b10110, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },   // }
};



// ビュー・コントローラとやり取りするための関数
void    FinishDrawing();


// ---- 全般的な操作

// バッチ処理の開始
void StartBatch()
{
    gIsBatchDrawing = true;
}

// バッチ処理の終了
void EndBatch()
{
    gIsBatchDrawing = false;
    FinishDrawing();
}


// ---- グラフィックス操作

// RGB形式の色情報を機種依存形式の色情報に変換する
static inline unsigned RGBToNativeColor(int value)
{
#ifdef _WINDOWS
    return value;
#else
    int r = (value >> 16) & 0xff;
    int g = (value >> 8) & 0xff;
    int b = value & 0xff;
    return ((b << 16) | (g << 8) | r);
#endif  //#ifdef _WINDOWS
}

// 機種依存形式の色情報をRGB形式の色情報に変換する
static inline int NativeColorToRGB(unsigned value)
{
#ifdef _WINDOWS
    return value;
#else
    int r = value & 0xff;
    int g = (value >> 8) & 0xff;
    int b = (value >> 16) & 0xff;
    return ((r << 16) | (g << 8) | b);
#endif  //#ifdef _WINDOWS
}

// 画面のクリア
void Clear(int color)
{
    // バッファの先頭から最後まで、同じ値を繰り返しセットする。
    unsigned nativeColor = RGBToNativeColor(color);
    unsigned *p = (unsigned *)gpBuffer;
    for (int i = 0; i < 640*480; i++) {
        *p = nativeColor;
        p++;
    }

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// バッファ上の色情報を取得する
int GetColor(int x, int y)
{
    // 範囲外なら0をリターンする
    if (x < -320 || x > 319 || y < -240 || y > 239) {
        return 0;
    }

    // 座標変換
    x = x + 320;
    y = 480 - (y + 240) - 1;

    // バッファの色情報を参照し、機種依存形式からRGB形式に変換してリターンする。
    unsigned *p = (unsigned *)gpBuffer;
    return NativeColorToRGB(*(p + y * 640 + x));
}

// 点の描画
void DrawPoint(int x, int y, int color)
{
    // 範囲のチェック
    if (x < -320 || x > 319 || y < -240 || y > 239) {
        return;
    }

    // 座標変換
    x = x + 320;
    y = 480 - (y + 240) - 1;

    // バッファにデータを書き込み
    unsigned nativeColor = RGBToNativeColor(color);
    unsigned *p = (unsigned *)gpBuffer;
    p[y * 640 + x] = nativeColor;

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 線分の描画（実装）
// ブレゼンハムのアルゴリズムで誤差を計算しながら線分を描画する。
static void DrawLine_impl(int x1, int y1, int x2, int y2, int nativeColor)
{
    // 座標変換
    x1 = x1 + 320;
    y1 = 480 - (y1 + 240) - 1;
    x2 = x2 + 320;
    y2 = 480 - (y2 + 240) - 1;

    // 線分の描画
    unsigned *p = (unsigned *)gpBuffer;
    int x = x1;
    int y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    bool isXBigger = (dx >= dy)? true: false;
    int step = (isXBigger? ((x1 <= x2)? 1: -1): ((y1 <= y2)? 1: -1));
    int error = 0;
    while (x != x2 || y != y2) {
        if (x >= 0 && x <= 639 && y >= 0 && y <= 479) {
            p[y * 640 + x] = nativeColor;
        }
        if (isXBigger) {
            x += step;
            error += (y2 - y1) * 2;
            if (error > dx) {
                y += 1;
                error -= dx * 2;
            } else if (error < -dx) {
                y -= 1;
                error += dx * 2;
            }
        } else {
            y += step;
            error += (x2 - x1) * 2;
            if (error > dy) {
                x += 1;
                error -= dy * 2;
            } else if (error < -dy) {
                x -= 1;
                error += dy * 2;
            }
        }
    }
    if (x >= 0 && x <= 639 && y >= 0 && y <= 479) {
        p[y * 640 + x] = nativeColor;
    }
}

// 線分の描画
void DrawLine(int x1, int y1, int x2, int y2, int color)
{
    unsigned nativeColor = RGBToNativeColor(color);
    DrawLine_impl(x1, y1, x2, y2, nativeColor);

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 三角形の描画
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color)
{
    // バッチ処理を強制的にONにして3つの頂点を結ぶ3つの線分を描画
    bool oldIsBatchDrawing = gIsBatchDrawing;
    gIsBatchDrawing = true;
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x2, y2, x3, y3, color);
    DrawLine(x3, y3, x1, y1, color);
    gIsBatchDrawing = oldIsBatchDrawing;
    
    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 三角形塗りつぶしのための補助関数。
// 直線を引くための座標をブレゼンハムのアルゴリズムで計算しながら、Y座標に対してX座標の値を登録していく。
static void DrawLineMap(std::map<int, std::vector<int>>& yMap, int x1, int y1, int x2, int y2)
{
    int x = x1;
    int y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    bool isXBigger = (dx >= dy)? true: false;
    int step = (isXBigger? ((x1 <= x2)? 1: -1): ((y1 <= y2)? 1: -1));
    int error = 0;
    while (x != x2 || y != y2) {
        if (y >= 0 && y <= 479) {
            std::vector<int>& xList = yMap[y];
            xList.push_back(x);
        }
        if (isXBigger) {
            x += step;
            error += (y2 - y1) * 2;
            if (error > dx) {
                y += 1;
                error -= dx * 2;
            } else if (error < -dx) {
                y -= 1;
                error += dx * 2;
            }
        } else {
            y += step;
            error += (x2 - x1) * 2;
            if (error > dy) {
                x += 1;
                error -= dy * 2;
            } else if (error < -dy) {
                x -= 1;
                error += dy * 2;
            }
        }
    }
    if (y >= 0 && y <= 479) {
        std::vector<int>& xList = yMap[y];
        xList.push_back(x);
    }
}

// 三角形の塗りつぶし
void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color)
{
    // Y座標が全部同じ場合は横方向の直線を引いて終わる
    if (y1 == y2 && y2 == y3) {
        int minX = std::min(std::min(x1, x2), x3);
        int maxX = std::max(std::max(x1, x2), x3);
        DrawLine(minX, y1, maxX, y1, color);
        return;
    }
    
    // X座標がすべて同じ場合は縦方向の直線を引いて終わる
    if (x1 == x2 && x2 == x3) {
        int minY = std::min(std::min(y1, y2), y3);
        int maxY = std::max(std::max(y1, y2), y3);
        DrawLine(x1, minY, x1, maxY, color);
        return;
    }

    // バッファ上の座標値に変換しておく
    x1 = x1 + 320;
    y1 = 480 - (y1 + 240) - 1;
    x2 = x2 + 320;
    y2 = 480 - (y2 + 240) - 1;
    x3 = x3 + 320;
    y3 = 480 - (y3 + 240) - 1;

    // 三角形の各辺を描画して、X座標のリストを作る
    std::map<int, std::vector<int>> yMap;
    DrawLineMap(yMap, x1, y1, x2, y2);
    DrawLineMap(yMap, x2, y2, x3, y3);
    DrawLineMap(yMap, x3, y3, x1, y1);

    // 最小のY座標から最大のY座標まで、各Y座標におけるX座標の最小値と最大値を直線で結ぶ。
    int yMin = std::min(std::min(y1, y2), y3);
    int yMax = std::max(std::max(y1, y2), y3);
    if (yMin < 0) {
        yMin = 0;
    }
    if (yMax > 479) {
        yMax = 479;
    }
    unsigned *p = (unsigned *)gpBuffer;
    unsigned nativeColor = RGBToNativeColor(color);
    for (int y = yMin; y <= yMax; y++) {
        std::vector<int>& xList = yMap[y];
        if (xList.size() == 0) {
            continue;   // 通常1個のY座標に1個以上のX座標が登録されているはずだが、念のため。
        }
        int xMin = *std::min_element(xList.begin(), xList.end());
        int xMax = *std::max_element(xList.begin(), xList.end());
        if (xMin < 0) {
            xMin = 0;
        }
        if (xMax > 639) {
            xMax = 639;
        }
        for (int x = xMin; x <= xMax; x++) {
            p[y * 640 + x] = nativeColor;
        }
    }

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 矩形の描画
void DrawRect(int x, int y, int width, int height, int color)
{
    int x1 = x;
    int x2 = x + width;
    int y1 = y + height;
    int y2 = y;

    // ラインを描画していく
    bool oldIsBatchDrawing = gIsBatchDrawing;
    gIsBatchDrawing = true;
    DrawLine(x1, y1, x2, y1, color);
    DrawLine(x2, y1, x2, y2, color);
    DrawLine(x2, y2, x1, y2, color);
    DrawLine(x1, y2, x1, y1, color);
    gIsBatchDrawing = oldIsBatchDrawing;

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 矩形の塗りつぶし
void FillRect(int x, int y, int width, int height, int color)
{
    unsigned nativeColor = RGBToNativeColor(color);
    unsigned *p = (unsigned *)gpBuffer;

    x = x + 320;
    y = 480 - (y + 240) - 1;

    // (x, y, width, height)から、(x1, y1), (x2, y2)の組に変換する。
    int x1 = x;
    int x2 = x + width;
    int y1 = y - height;
    int y2 = y;
    if (x1 > x2) {
        std::swap(x1, x2);
    }
    if (y1 > y2) {
        std::swap(y1, y2);
    }

    // クリッピング処理
    if (x1 < 0) {
        x1 = 0;
    }
    if (x2 >= 640) {
        x2 = 639;
    }
    if (y1 < 0) {
        y1 = 0;
    }
    if (y2 >= 480) {
        y2 = 479;
    }

    // 横方向に塗りつぶしていく
    p += y1 * 640;
    for (int y = y1; y <= y2; y++) {
        unsigned *p2 = p + x1;
        for (int x = x1; x <= x2; x++) {
            *(p2++) = nativeColor;
        }
        p += 640;
    }

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 円の描画
void DrawCircle(int cx, int cy, int radius, int color)
{
    DrawCircle(cx, cy, radius, color, 0.0f, 2 * M_PI);
}

// 円を描画する範囲を表すクラス
class AngleRange
{
    float   startRad;
    float   endRad;
    bool    coversAll;

public:
    AngleRange(float start, float end)
    {
        while (start < -M_PI) {
            start += 2 * M_PI;
        }
        while (start > M_PI) {
            start -= 2 * M_PI;
        }
        while (end < -M_PI) {
            end += 2 * M_PI;
        }
        while (end > M_PI) {
            end -= 2 * M_PI;
        }
        startRad = start;
        endRad = end;
        coversAll = (fabsf(endRad - startRad) <= 0.0001f);
    }

public:
    bool Contains(float rad) const
    {
        if (coversAll) {
            return true;
        }
        while (rad < -M_PI) {
            rad += 2 * M_PI;
        }
        while (rad > M_PI) {
            rad -= 2 * M_PI;
        }
        if (startRad <= endRad) {
            return (startRad <= rad && rad <= endRad);
        } else {
            return (rad >= startRad || rad <= endRad);
        }
        return false;
    }
};

// fillXとfillYは、円または円弧の描画中に、画面内に確実に描画される円/円弧の中の点を保存しておくための変数です。
// 扇形の塗りつぶし描画をする時に活用します（画面からはみ出た扇形の場合、塗りつぶしの開始点を計算するのが難しいため）。
static int fillX = 0;
static int fillY = 0;

// 円または円弧の描画の実装
static void DrawCircle_impl(int cx, int cy, int r, int nativeColor, float startRad, float endRad)
{
    // 座標の調整
    unsigned *p = (unsigned *)gpBuffer;
    cx = cx + 320;
    cy = 480 - (cy + 240) - 1;
    fillX = INT_MAX;
    fillY = INT_MAX;

    // 半径=0の場合は中心に点を描画する
    if (r == 0) {
        if (cx >= 0 && cx <= 639 && cy >= 0 && cy <= 479) {
            p[cy*640+cx] = nativeColor;
        }
        return;
    }

    // 円の描画
    AngleRange range(startRad, endRad);
    int x = cx + r;
    int y = cy;
    float angle = atan2f(cy - y, x - cx);
    if (range.Contains(angle) && x >= 0 && x <= 639 && y >= 0 && y <= 479) {
        p[y*640+x] = nativeColor;
        if (x > 1) {
            fillX = x - 1;
            fillY = y;
        }
    }
    while (x != cx) {
        int x1 = x;
        int y1 = y + 1;
        int x2 = x - 1;
        int y2 = y;
        int d1 = abs(r * r - (x1 - cx) * (x1 - cx) - (y1 - cy) * (y1 - cy));
        int d2 = abs(r * r - (x2 - cx) * (x2 - cx) - (y2 - cy) * (y2 - cy));
        if (d1 <= d2) {
            x = x1;
            y = y1;
        } else {
            x = x2;
            y = y2;
        }
        angle = atan2f(cy - y, x - cx);
        if (range.Contains(angle) && x >= 0 && x <= 639 && y >= 0 && y <= 479) {
            p[y*640+x] = nativeColor;
            if (x > 1) {
                fillX = x - 1;
                fillY = y;
            }
        }
    }
    x = cx;
    y = cy + r;
    while (y != cy) {
        int x1 = x;
        int y1 = y - 1;
        int x2 = x - 1;
        int y2 = y;
        int d1 = abs(r * r - (x1 - cx) * (x1 - cx) - (y1 - cy) * (y1 - cy));
        int d2 = abs(r * r - (x2 - cx) * (x2 - cx) - (y2 - cy) * (y2 - cy));
        if (d1 < d2) {
            x = x1;
            y = y1;
        } else {
            x = x2;
            y = y2;
        }
        angle = atan2f(cy - y, x - cx);
        if (range.Contains(angle) && x >= 0 && x <= 639 && y >= 0 && y <= 479) {
            p[y*640+x] = nativeColor;
            if (x < 639) {
                fillX = x + 1;
                fillY = y;
            }
        }
    }
    x = cx - r;
    y = cy;
    while (x != cx) {
        int x1 = x;
        int y1 = y - 1;
        int x2 = x + 1;
        int y2 = y;
        int d1 = abs(r * r - (x1 - cx) * (x1 - cx) - (y1 - cy) * (y1 - cy));
        int d2 = abs(r * r - (x2 - cx) * (x2 - cx) - (y2 - cy) * (y2 - cy));
        if (d1 <= d2) {
            x = x1;
            y = y1;
        } else {
            x = x2;
            y = y2;
        }
        angle = atan2f(cy - y, x - cx);
        if (range.Contains(angle) && x >= 0 && x <= 639 && y >= 0 && y <= 479) {
            p[y*640+x] = nativeColor;
            if (x < 639) {
                fillX = x + 1;
                fillY = y;
            }
        }
    }
    x = cx;
    y = cy - r;
    while (y != cy) {
        int x1 = x;
        int y1 = y + 1;
        int x2 = x + 1;
        int y2 = y;
        int d1 = abs(r * r - (x1 - cx) * (x1 - cx) - (y1 - cy) * (y1 - cy));
        int d2 = abs(r * r - (x2 - cx) * (x2 - cx) - (y2 - cy) * (y2 - cy));
        if (d1 < d2) {
            x = x1;
            y = y1;
        } else {
            x = x2;
            y = y2;
        }
        angle = atan2f(cy - y, x - cx);
        if (range.Contains(angle) && x >= 0 && x <= 639 && y >= 0 && y <= 479) {
            p[y*640+x] = nativeColor;
            if (x > 1) {
                fillX = x - 1;
                fillY = y;
            }
        }
    }
}

// 円または円弧の描画
void DrawCircle(int cx, int cy, int r, int color, float startRad, float endRad)
{
    unsigned nativeColor = RGBToNativeColor(color);
    DrawCircle_impl(cx, cy, r, nativeColor, startRad, endRad);

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 円の塗りつぶし
void FillCircle(int cx, int cy, int r, int color)
{
    // 座標の調整
    unsigned nativeColor = RGBToNativeColor(color);
    unsigned *p = (unsigned *)gpBuffer;
    cx = cx + 320;
    cy = 480 - (cy + 240) - 1;

    // 半径=0の場合は中心に点を描画する
    if (r == 0) {
        if (cx >= 0 && cx <= 639 && cy >= 0 && cy <= 479) {
            p[cy*640+cx] = nativeColor;
        }
        return;
    }

    // 円の描画
    int x = cx + r;
    int y = cy;
    for (int x2 = cx; x2 <= x; x2++) {
        if (x2 >= 0 && x2 <= 639 && y >= 0 && y <= 479) {
            p[y*640+x2] = nativeColor;
        }
    }
    while (x != cx) {
        int x1 = x;
        int y1 = y + 1;
        int x2 = x - 1;
        int y2 = y;
        int d1 = abs(r * r - (x1 - cx) * (x1 - cx) - (y1 - cy) * (y1 - cy));
        int d2 = abs(r * r - (x2 - cx) * (x2 - cx) - (y2 - cy) * (y2 - cy));
        if (d1 <= d2) {
            x = x1;
            y = y1;
        } else {
            x = x2;
            y = y2;
        }
        for (int x2 = cx; x2 <= x; x2++) {
            if (x2 >= 0 && x2 <= 639 && y >= 0 && y <= 479) {
                p[y*640+x2] = nativeColor;
            }
        }
    }
    x = cx;
    y = cy + r;
    while (y != cy) {
        int x1 = x;
        int y1 = y - 1;
        int x2 = x - 1;
        int y2 = y;
        int d1 = abs(r * r - (x1 - cx) * (x1 - cx) - (y1 - cy) * (y1 - cy));
        int d2 = abs(r * r - (x2 - cx) * (x2 - cx) - (y2 - cy) * (y2 - cy));
        if (d1 < d2) {
            x = x1;
            y = y1;
        } else {
            x = x2;
            y = y2;
        }
        for (int x2 = x; x2 < cx; x2++) {
            if (x2 >= 0 && x2 <= 639 && y >= 0 && y <= 479) {
                p[y*640+x2] = nativeColor;
            }
        }
    }
    x = cx - r;
    y = cy;
    while (x != cx) {
        int x1 = x;
        int y1 = y - 1;
        int x2 = x + 1;
        int y2 = y;
        int d1 = abs(r * r - (x1 - cx) * (x1 - cx) - (y1 - cy) * (y1 - cy));
        int d2 = abs(r * r - (x2 - cx) * (x2 - cx) - (y2 - cy) * (y2 - cy));
        if (d1 <= d2) {
            x = x1;
            y = y1;
        } else {
            x = x2;
            y = y2;
        }
        for (int x2 = x; x2 < cx; x2++) {
            if (x2 >= 0 && x2 <= 639 && y >= 0 && y <= 479) {
                p[y*640+x2] = nativeColor;
            }
        }
    }
    x = cx;
    y = cy - r;
    while (y != cy) {
        int x1 = x;
        int y1 = y + 1;
        int x2 = x + 1;
        int y2 = y;
        int d1 = abs(r * r - (x1 - cx) * (x1 - cx) - (y1 - cy) * (y1 - cy));
        int d2 = abs(r * r - (x2 - cx) * (x2 - cx) - (y2 - cy) * (y2 - cy));
        if (d1 < d2) {
            x = x1;
            y = y1;
        } else {
            x = x2;
            y = y2;
        }
        for (int x2 = cx; x2 <= x; x2++) {
            if (x2 >= 0 && x2 <= 639 && y >= 0 && y <= 479) {
                p[y*640+x2] = nativeColor;
            }
        }
    }

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

static void Paint_impl(int x, int y, int paintNativeColor, int borderNativeColor);

// 円または円弧の塗りつぶし
void FillCircle(int cx, int cy, int r, int color, float startRad, float endRad)
{
    // 開始角度と終了角度を-π〜+πの範囲にしておく
    while (startRad < -M_PI) {
        startRad += 2 * M_PI;
    }
    while (startRad > M_PI) {
        startRad -= 2 * M_PI;
    }
    while (endRad < -M_PI) {
        endRad += 2 * M_PI;
    }
    while (endRad > M_PI) {
        endRad -= 2 * M_PI;
    }

    // 同一角であれば円とみなし、単純な方のFillCircle()関数を呼び出す
    if (fabsf(endRad - startRad) <= 0.0001f) {
        FillCircle(cx, cy, r, color);
        return;
    }

    // アルファプレーンに円弧を描画する
    DrawCircle_impl(cx, cy, r, 0xff000000, startRad, endRad);

    // アルファプレーンに中心からの直線を描画する
    int x1 = (int)(roundf(cosf(startRad) * r)) + cx;
    int y1 = (int)(roundf(sinf(startRad) * r)) + cy;
    DrawLine_impl(cx, cy, x1, y1, 0xff000000);

    int x2 = (int)(roundf(cosf(endRad) * r)) + cx;
    int y2 = (int)(roundf(sinf(endRad) * r)) + cy;
    DrawLine_impl(cx, cy, x2, y2, 0xff000000);

    // 指定色で塗りつぶす
    unsigned nativeColor = RGBToNativeColor(color);
    Paint_impl(fillX - 320, 239 - fillY, nativeColor, 0xff000000);

    // 円弧と直線を指定色で上書きする
    DrawCircle_impl(cx, cy, r, nativeColor, startRad, endRad);
    DrawLine_impl(cx, cy, x1, y1, nativeColor);
    DrawLine_impl(cx, cy, x2, y2, nativeColor);

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 指定色で囲まれた領域の塗りつぶし（実装）
static void Paint_impl(int x, int y, int paintNativeColor, int borderNativeColor)
{
    x = x + 320;
    y = 480 - (y + 240) - 1;
    if (x < 0 || x > 639 || y < 0 || y > 479) {
        return;
    }

    unsigned *p = (unsigned *)gpBuffer;

    struct IntPoint { int x, y; };
    std::vector<IntPoint> scanPoints;
    std::vector<IntPoint> scannedPoints;

    // 最初の左端を見つける
    while (x > 0 && p[y*640+(x-1)] != borderNativeColor) {
        x--;
    }
    scanPoints.push_back({x, y});

    // 左端の要素を取り出しながら処理していく
    while (scanPoints.size() > 0) {
        // 次の要素を取り出す
        IntPoint pos = *(scanPoints.end() - 1);
        scanPoints.erase(scanPoints.end() - 1);
        scannedPoints.push_back(pos);
        if (p[pos.y*640+pos.x] == borderNativeColor) {
            continue;
        }
        // 右端まで塗りつぶしていく
        bool hasUpYChecked = false;
        int upY = pos.y - 1;
        bool hasDownYChecked = false;
        int downY = pos.y + 1;
        for (int xx = pos.x; xx <= 639; xx++) {
            if (p[pos.y*640+xx] == borderNativeColor) {
                break;
            }
            p[pos.y*640+xx] = paintNativeColor;
            // 上下のピクセルをチェック
            if (upY >= 0) {
                // 上のピクセルからスキャンした左端がチェック済みの場合、直上に境界色が出てきたらチェック済みフラグを取り下げる
                if (hasUpYChecked) {
                    if (p[upY*640+xx] == borderNativeColor) {
                        hasUpYChecked = false;
                    }
                }
                // チェック済みでない場合、左端の境界色が出てくる直前のピクセルを見つけて登録する
                else {
                    // 直上が境界色でなければ左端の境界色が出てくる直前のピクセルを探す
                    if (p[upY*640+xx] != borderNativeColor) {
                        int theX = xx;
                        while (theX > 0 && p[upY*640+(theX-1)] != borderNativeColor) {
                            theX--;
                        }
                        bool hasScanned = false;
                        auto it = scannedPoints.begin();
                        while (it != scannedPoints.end()) {
                            if ((*it).x == theX && (*it).y == upY) {
                                hasScanned = true;
                            }
                            it++;
                        }
                        if (!hasScanned) {
                            scanPoints.push_back({theX, upY});
                        }
                        hasUpYChecked = true;
                    }
                }
            }
            if (downY <= 479) {
                // 下のピクセルからスキャンした左端がチェック済みの場合、直下に境界色が出てきたらチェック済みフラグを取り下げる
                if (hasDownYChecked) {
                    if (p[downY*640+xx] == borderNativeColor) {
                        hasDownYChecked = false;
                    }
                }
                // チェック済みでない場合、左端の境界色が出てくる直前のピクセルを見つけて登録する
                else {
                    // 直下が境界色でなければ左端の境界色が出てくる直前のピクセルを探す
                    if (p[downY*640+xx] != borderNativeColor) {
                        int theX = xx;
                        while (theX > 0 && p[downY*640+(theX-1)] != borderNativeColor) {
                            theX--;
                        }
                        bool hasScanned = false;
                        auto it = scannedPoints.begin();
                        while (it != scannedPoints.end()) {
                            if ((*it).x == theX && (*it).y == downY) {
                                hasScanned = true;
                            }
                            it++;
                        }
                        if (!hasScanned) {
                            scanPoints.push_back({theX, downY});
                        }
                        hasDownYChecked = true;
                    }
                }
            }
        }
    }
}

// 指定色で囲まれた領域の塗りつぶし
void Paint(int x, int y, int paintColor, int borderColor)
{
    unsigned borderNativeColor = RGBToNativeColor(borderColor);
    unsigned paintNativeColor = RGBToNativeColor(paintColor);
    Paint_impl(x, y, paintNativeColor, borderNativeColor);

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// パターンの描画
void DrawPattern(unsigned int *buffer, int x, int y)
{
    unsigned width = buffer[0];
    unsigned height = buffer[1];
    unsigned *p = (unsigned *)gpBuffer;
    x = x + 320;
    y = 480 - (y + 240) - 1;

    for (int y2 = 0; y2 < height; y2++) {
        for (int x2 = 0; x2 < width; x2++) {
            int px = x + x2;
            int py = y + y2 - height + 1;
            if (px < 0 || px >= 640 || py < 0 || py >= 480) {
                continue;
            }
            unsigned argb = buffer[y2*width+x2+2];
            if (!(argb & 0xff000000)) {
                *(p+py*640+px) = RGBToNativeColor((int)argb);
            }
        }
    }

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 文字の描画
void DrawCharacter(char c, int x, int y, int color)
{
    // 文字コードから文字のパターン情報のインデックスに変換する
    int patternIndex = c - '!';
    if (patternIndex < 0 || patternIndex > 93) {
        return;
    }

    // パターン情報を参照して描画する
    const unsigned char *pattern = kCharPattern[patternIndex];
    unsigned width = 5;
    unsigned height = 10;
    unsigned *p = (unsigned *)gpBuffer;
    unsigned nativeColor = RGBToNativeColor(color);
    x = x + 320;
    y = 480 - (y + 240) - 1;
    for (int y2 = 0; y2 < height*2; y2++) {
        unsigned char patternY = pattern[y2/2];
        for (int x2 = 0; x2 < width*2; x2++) {
            int px = x + x2;
            int py = y + y2 - height*2 + 1;
            if (px < 0 || px >= 640 || py < 0 || py >= 480) {
                continue;
            }
            if ((patternY >> (4 - x2 / 2)) & 1) {
                *(p+py*640+px) = nativeColor;
            }
        }
    }

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 文字列の描画
void DrawText(const char *str, int x, int y, int color)
{
    // 1文字ずつ描画していく
    bool oldIsBatchDrawing = gIsBatchDrawing;
    gIsBatchDrawing = true;
    int length = (int)strlen(str);
    int px = x;
    int py = y;
    for (int i = 0; i < length; i++) {
        char c = str[i];
        if (c == '\n') {
            px = x;
            py -= 20;
        } else {
            DrawCharacter(str[i], px, py, color);
            px += 12;
        }
    }
    gIsBatchDrawing = oldIsBatchDrawing;

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 横方向のスクロール処理
static void ScrollH(int dx)
{
    // スクロールなし
    if (dx == 0) {
        return;
    }

    // 左方向へのスクロール
    if (dx < 0) {
        // 1行ずつ左にコピーする
        unsigned *p = (unsigned *)gpBuffer;
        for (int y = 0; y < 479; y++) {
            memmove(p+640*y, p+640*y-dx, sizeof(unsigned)*(640+dx));
        }
        // 空いた部分をクリア
        for (int y = 0; y < 479; y++) {
            memset(p+640*y+(640+dx), 0x00, sizeof(unsigned)*-dx);
        }
    }
    // 右方向へのスクロール
    else {
        // 1行ずつ右にコピーする
        unsigned *p = (unsigned *)gpBuffer;
        for (int y = 0; y < 479; y++) {
            memmove(p+640*y+dx, p+640*y, sizeof(unsigned)*(640-dx));
        }
        // 空いた部分をクリア
        for (int y = 0; y < 479; y++) {
            memset(p+640*y, 0x00, sizeof(unsigned)*dx);
        }
    }

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 縦方向のスクロール処理
static void ScrollV(int dy)
{
    // スクロールなし
    if (dy == 0) {
        return;
    }

    // 下方向へのスクロール
    if (dy < 0) {
        // 1行ずつ下にコピーする
        unsigned *p = (unsigned *)gpBuffer;
        for (int y = 479+dy; y >= 0; y--) {
            int destY = y - dy;
            memcpy(p+destY*640, p+y*640, sizeof(unsigned) * 640);
        }
        // 空いた部分をクリア
        memset(p, 0x00, sizeof(unsigned) * 640 * -dy);
    }
    // 上方向へのスクロール
    else {
        // 1行ずつ上にコピーする
        unsigned *p = (unsigned *)gpBuffer;
        for (int y = dy; y <= 479; y++) {
            int destY = y - dy;
            memcpy(p+destY*640, p+y*640, sizeof(unsigned) * 640);
        }
        // 空いた部分をクリア
        memset(p + 640 * (480 - dy), 0x00, sizeof(unsigned) * 640 * dy);
    }

    // バッチ処理中でなければ画面表示
    if (!gIsBatchDrawing) {
        FinishDrawing();
    }
}

// 縦横のスクロール処理
void Scroll(int x, int y)
{
    ScrollH(x);
    ScrollV(y);
}

// 塗りつぶしのデバッグ用のヘルパー関数。
// 各画素のアルファ値が0以外になっている部分を黄色で、それ以外の部分を青で表示します。
void ShowAlphaPixels()
{
    // TODO: Windowsの場合のピクセル値の並びを考慮する。
    unsigned *p = (unsigned *)gpBuffer; // 生のバッファの色の並びはABGR

    for (int i = 0; i < 640*480; i++) {
        if (p[i] & 0xff000000) {
            p[i] = 0x0000ffff;
        } else {
            p[i] = 0x00ff0000;
        }
    }
    FinishDrawing();
}


// ---- キーボード関係の操作

// キーが押されているかどうか
bool CheckKey(unsigned int key)
{
    return (gKeyData & key)? true: false;
}



// ---- マウス関係の操作

// マウスのX座標を取得する
int GetMouseX()
{
    return gMouseX - 320;
}

// マウスのY座標を取得する
int GetMouseY()
{
    return gMouseY - 240;
}

// マウスが押されているかどうか
bool CheckMouse()
{
    return gIsMouseDown;
}

