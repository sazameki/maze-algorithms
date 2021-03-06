# Maze Algorithms

<p align="center"><img src="https://raw.githubusercontent.com/sazameki/maze-algorithms/readme/screenshot_kabenobashi.png" width=45%><img src="https://raw.githubusercontent.com/sazameki/maze-algorithms/readme/screenshot_growing-tree.png" width=45%><img src="https://raw.githubusercontent.com/sazameki/maze-algorithms/readme/screenshot_blobby-recursive-division.png" width=38%>　　<img src="https://raw.githubusercontent.com/sazameki/maze-algorithms/readme/screenshot_eller.png" width=38%></p>

## このプロジェクトについて

+ 迷路を生成するための各種アルゴリズムを実装したものです。迷路生成・迷路探索のアルゴリズムおよびその描画のコードはC++11で書いています。
+ Windows 10のVisual Studio 2017 CommunityおよびmacOS 10.12以降のXcode 9.0以上で動作します。
+ 日本の迷路生成アルゴリズムは、2次元配列の偶数列・偶数行を壁として扱うものが多いですが、英語サイトで解説される迷路生成アルゴリズムは、壁の有無をビットフラグで表すものが主流です。このプロジェクトでは、日本でよく見つかるアルゴリズムも、英語サイトで見つかるアルゴリズムも、ビットフラグベースで実装しています。

## ライセンス

+ このプロジェクトはパブリック・ドメインで提供しています。自由にお使いください。
+ 迷路生成について勉強しようとする方の一助になれば幸いです。

## 大きなサイズの迷路を扱う場合

+ アプリケーション実行に割り当てられたデフォルトのスタックサイズは、100x100マス以上のサイズの迷路を生成して解くには小さく、また速度も遅く感じられます。そのため、描画効率の最適化を行い、ヒープ上に確保したスタックを使用して再帰処理を行うバージョンの実装を https://github.com/sazameki/maze-algorithms/tree/stack-recursive のブランチ上に用意しています。大きなサイズの迷路を作成する際には、こちらを使用してください。

## 実装済みの迷路生成アルゴリズム

### ランダムウォーク系

+ 穴掘り法
+ Hunt and Kill
+ Recursive Backtracking
+ Growing Tree / Growing Binary Tree
+ Primの迷路生成アルゴリズム
+ Aldous-Broderの迷路生成アルゴリズム
+ Wilsonの迷路生成アルゴリズム
+ Houstonの迷路生成アルゴリズム
+ 壁のばし法

### 分割統治法

+ Recursive Division
+ Blobby Recursive Division
+ Ellerの迷路生成アルゴリズム
+ Kruskalの迷路生成アルゴリズム

### 各セルごとの処理によるアドホックな生成アルゴリズム

+ 棒倒し法
+ Sidewinder
+ Binary Tree

### セル・オートマトン系

+ セルオートマトン・ベースの迷路生成 (http://www.conwaylife.com/w/index.php?title=Maze などのMaze, Mazectric) は、考え方が大きく異なりますので（迷路生成が主ではなくライフゲームが主であり、壁はセルの一状態に過ぎない）、このプロジェクトの対象外としています。


## 実装済みの迷路探索アルゴリズム

+ 右手法
+ Dijkstraの迷路探索アルゴリズム

## プログラム実行について

+ Windowsで実行する場合、win/Maze.slnをVisual Studio 2017 Communityで開いて実行してください。
+ macOSで実行する場合、osx/Maze.xcodeprojをXcodeで開いて実行してください。
+ プログラム実行は、DrawMain.cppのDrawMain()関数から始まります。

## クラス構成について

+ Mazeクラスが迷路を表し、内部データを1次元配列（内部的には2次元配列のように扱いますが）で持っています。Mazeクラスには、迷路を生成するための機能が盛り込まれています。
+ 迷路の描画処理はDrawMaze()関数に書かれています。MazeDraw.hpp/MazeDraw.cppに書かれた定数の値を変更することで、マスの色や大きさ、壁の太さなどを変更することができます。
+ 迷路生成の各種アルゴリズムは、「Maze Creators」グループ以下のソースコードに書かれています。

