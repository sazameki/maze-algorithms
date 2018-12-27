#ifndef Drawing_hpp
#define Drawing_hpp


// ---- 変数・定数の定義

// 実行全体に関する変数
extern bool     gIsBatchDrawing;    //!< バッチ処理がONになっているかどうか
extern float    gFrameRate;         //!< 現時点でのフレームレート
extern bool     gIsPausing;         //!< 実行が一時停止中かどうか

// グラフィックス関係の変数
extern unsigned char    *gpBuffer;  //!< 画面のバッファ(ABGR,ABGR,...)

// キーボード関係の変数
extern unsigned gKeyData;           //!< キーボードの入力データ

// マウス関係の変数
extern bool     gIsMouseDown;       //!< 左マウスボタンが押されているかどうか
extern int      gMouseX;            //!< マウスの現在のX座標
extern int      gMouseY;            //!< マウスの現在のY座標

// 色の定数
extern const int    kColorBlack;
extern const int    kColorLightGray;
extern const int    kColorDarkGray;
extern const int    kColorGray;
extern const int    kColorWhite;
extern const int    kColorRed;
extern const int    kColorGreen;
extern const int    kColorBlue;
extern const int    kColorYellow;
extern const int    kColorOrange;
extern const int    kColorDarkBlue;

// キーボードの定数
extern const unsigned   kKeyA;
extern const unsigned   kKeyS;
extern const unsigned   kKeyD;
extern const unsigned   kKeyW;
extern const unsigned   kKeyUpArrow;
extern const unsigned   kKeyDownArrow;
extern const unsigned   kKeyLeftArrow;
extern const unsigned   kKeyRightArrow;
extern const unsigned   kKeyReturn;
extern const unsigned   kKeySpace;



// ---- 全般的な操作

/**
    秒数で指定した時間だけ処理を中断します。
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
 */
void    Clear(int color);

/**
    指定した座標の色を取得します。
 */
int     GetColor(int x, int y);

/**
    指定した座標に点を描画します。
 */
void    DrawPoint(int x, int y, int color);

/**
    指定した2点の座標を結ぶ直線を描画します。
 */
void    DrawLine(int x1, int y1, int x2, int y2, int color);

/**
    指定した3点の座標を結ぶ三角形を描画します。
 */
void    DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);

/**
    指定した3点の座標を結ぶ三角形を塗りつぶします。
 */
void    FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);

/**
    指定した座標から指定した大きさの四角形を描画します。
 */
void    DrawRect(int x, int y, int width, int height, int color);

/**
    指定した座標から指定した大きさの四角形を塗りつぶします。
 */
void    FillRect(int x, int y, int width, int height, int color);


// TODO: 正円の縦横比を変えて楕円を描画するための引数をDrawCircle()関数とFillCircle()関数に追加する。


/**
    指定した中心点と半径で円を描画します。
 */
void    DrawCircle(int cx, int cy, int radius, int color);

/**
    指定した中心点と半径、開始角度と終了角度をラジアン単位で指定して、円弧を描画します。
    角度は-πから+πの範囲に修正された上で比較されます。
    0〜5/2πが指定された場合、これは0〜1/2πが指定されたことになりますので、第1象限の90度の円弧が描画されます。
 */
void    DrawCircle(int cx, int cy, int radius, int color, float startAngleRad, float endAngleRad);

/**
    指定した中心点と半径で円を塗りつぶします。
 */
void    FillCircle(int cx, int cy, int radius, int color);

// 指定した中心点と半径、開始角度と終了角度をラジアン単位で指定して、扇形の図形を塗りつぶし描画します。
// 角度は-πから+πの範囲に修正された上で比較されます。
// 0〜5/2πが指定された場合、これは0〜1/2πが指定されたことになりますので、第1象限の90度の扇形が描画されます。
void    FillCircle(int cx, int cy, int radius, int color, float startAngleRad, float endAngleRad);

/**
    指定した座標からスキャンを開始して、borderColorの色で囲まれた領域をpaintColorの色で塗りつぶします。
    画面(-320, -240)-(319, 239)より外の領域の座標が指定された場合、塗りつぶしは行われません。
 */
void    Paint(int x, int y, int paintColor, int borderColor);

/**
    指定された座標にグラフィック・パターンを描画します。
    グラフィックパターンはunsigned int型の配列で、先頭2つの要素がX方向の大きさとY方向の大きさ、
    その後はARGB,ARGB,ARGB,...となります。ARGBの各要素は、0x00〜0xffの8ビットで表されます。
    A（アルファ値）のデータは、0だと不透明、それ以外だと透明なピクセルとして扱われます。
 */
void    DrawPattern(unsigned int *buffer, int x, int y);

/**
    指定された座標に文字を描画します。それぞれの文字は12x20ピクセルのサイズです。
 */
void    DrawCharacter(char c, int x, int y, int color);

/**
    指定された座標に文字列を描画します。それぞれの文字は12x20ピクセルのサイズです。
 */
void    DrawText(const char *str, int x, int y, int color);

/**
    画面をスクロールします。
 */
void    Scroll(int x, int y);



// ---- キーボード関係の操作

/**
    指定したキーが押されているかどうかを判定します。
 */
bool    CheckKey(unsigned int key);



// ---- マウス関係の操作

/**
    マウスカーソルの画面上のX座標を取得します。
 */
int     GetMouseX();

/**
    マウスカーソルの画面上のY座標を取得します。
 */
int     GetMouseY();

/**
    マウスの左ボタンが押されているかどうかを取得します。
 */
bool    CheckMouse();


#endif /* Drawing_hpp */

