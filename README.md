# Maze Algorithms

<p align="center"><img src="https://github.com/sazameki/maze-algorithms/raw/readme/screenshot_prim.png" width=45%><img src="https://github.com/sazameki/maze-algorithms/raw/readme/screenshot_recursive-backtracking.png" width=45%></p>

## このプロジェクトについて

+ 迷路を生成するための各種アルゴリズムを実装したものです。描画エンジン以外の部分は純粋なC++11で書いていますので、Macユーザでなくても参考にしていただける部分はあるかと思います。
+ macOS 10.13以上、Xcode 9以上で動作します。
+ 日本の迷路生成アルゴリズムは、2次元配列の偶数列・偶数行を壁として扱うものが多いですが、英語サイトで解説される迷路生成アルゴリズムは、壁の有無をビットフラグで表すものが主流です。このプロジェクトでは、日本でよく見つかるアルゴリズムも、英語サイトで見つかるアルゴリズムも、ビットフラグベースで実装しています。
+ ライセンスはパブリックドメインです。ご自由にお使いください。

## 実装済みの迷路生成アルゴリズム

### ランダムウォーク系

+ 穴掘り法
+ Hunt and Kill
+ Recursive Backtracking
+ Growing Tree / Growing Binary Tree
+ Primの迷路生成アルゴリズム
+ Aldous-Broderの迷路生成アルゴリズム
+ 壁のばし法

### 分割統治法

+ Recursive Division
+ Blobby Recursive Division
+ Ellerの迷路生成アルゴリズム

### 各セルごとの処理によるアドホックな生成アルゴリズム

+ 棒倒し法
+ Sidewinder
+ Binary Tree

## プログラム実行について

+ プログラム実行は、DrawMain.cppのDrawMain()関数から始まります（基本的にはCocoaアプリなので、より内部的に言えば、main()関数から始まってAppDelegate.mmに書かれた処理が行われ、その後DrawMain()関数が描画用スレッドの中から呼び出されます）。

## クラス構成について

+ Mazeクラスが迷路を表し、内部データを1次元配列（内部的には2次元配列のように扱いますが）で持っています。Mazeクラスには、迷路を生成するための機能が盛り込まれています。
+ 迷路の描画処理はDrawMaze()関数に書かれています。MazeDraw.hpp/MazeDraw.cppに書かれた定数の値を変更することで、マスの色や大きさ、壁の太さなどを変更することができます。
+ 迷路生成の各種アルゴリズムは、「Maze Creators」グループ以下のソースコードに書かれています。

