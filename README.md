# InsertedChainedHashTable

## 01. 概要
 本プログラムは，key: UINT64, value: UINT64 or void* のみに対応した，
 クラスタリングの発生しない Open Addressing 方式の簡易ハッシュテーブルです．
 特に，key: UINT64, value: UINT64，に関しては，
 Microsoft Visual Studio C++ に実装されている std::unorderd_map
 よりも高速に動作します．

 詳細: https://admiswalker.blogspot.jp/2017/01/ichasht.html

## 02. 使用手順
 ./~/ICHashT/ をファイルごとコピーして  
 #include "./ICHashT/ICHashT.hpp"  
 と記述してください．
 
 具体的な使用方法は，  
 ./~/ICHashT_ExampleFunctions.cpp  
 ./~/ICHashT_ExampleFunctions.hpp  
 を参考にしてください．
 
## 03. ベンチマーク
 ベンチマークは，  
 ./~/ICHashT_benchmark.cpp  
 ./~/ICHashT_benchmark.hpp  
 に記述されています．
 
 下記のファイルを使用します．  
 ./~/mt64.cpp  
 ./~/mt64.hpp  
 ./~/rounds.hpp  

## 04. その他のファイル  
 - FNV-1 ハッシュ関数です．  
 ./~/ICHashT/FNV_Hash.cpp  
 ./~/ICHashT/FNV_Hash.hpp  
 
 - Windows のコンソールを一時停止するための関数です．UINX 環境では無効化されます．  
 ./~/ICHashT/SysPauseForWin32.h  

 - 開発中に使用したデバッグ用の関数です．  
 ./~/ICHashT_DebugFunctions.cpp  
 ./~/ICHashT_DebugFunctions.hpp  
 ./~/ICHashT/FileWriter.cpp  
 ./~/ICHashT/FileWriter.hpp  
 ./~/ICHashT/printf_dbger.h  
 ./~/ICHashT/str_sprintf.cpp  
 ./~/ICHashT/str_sprintf.hpp  

