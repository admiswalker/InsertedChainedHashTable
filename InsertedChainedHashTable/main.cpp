/* -*- coding: utf-8 -*- */
// InsertedChainedHashTable_Project2 ( 「埋め込み式」の「数珠つなぎ型」の「ハッシュテーブル」開発計画2 )
// 注意::64bit環境に依存(keyのサイズとハッシュ値の生成の長さを調整することで32bit環境にも対応はできるはず．)
// 開発環境::Windows7x64::MicrosoftVisualStudio2012

// 使用方法
//
// [./ICHashT/*.*] をフォルダごと，任意のディレクトリに配置．
// [./ICHashT/ICHashT.hpp]をインクルード．( #include "./ICHashT/ICHashT.hpp" )
// 以上．

#include <stdio.h>
#include <time.h>		// clock_t() 時間計測に必要．
#include <chrono>		// std::chrono::system_clock::now()

// #define ICHashT_DebugMode の設定は，[./ICHashT/ICHashT.hpp] にて行う必要があります．(他のファイルへ波及させるため)
// #define ReturnAssignedHash_for_debug の設定は，[./ICHashT/ICHashT.hpp] にて行う必要があります．(他のファイルへ波及させるため)
#include "./ICHashT/ICHashT.hpp"

#ifdef ICHashT_DebugMode
	#include "ICHashT_ExampleFunctions.hpp"
	#include "ICHashT_benchmark.hpp"
#endif

#ifdef ReturnAssignedHash_for_debug
	#include "ICHashT_DebugFunctions.hpp"
#endif

int main(){

	// Examples ( start ) ===================================================================================================
	// ここでは，各関数の使用例を実行します．

#if !defined(ICHashT_DebugMode) && !defined(ReturnAssignedHash_for_debug)
//	Example1();
//	Example2();
//	Example3();
#endif

	// Examples (  end  ) ===================================================================================================


	// benchmarks ( start ) =================================================================================================
	
#if defined(ICHashT_DebugMode) && !defined(ReturnAssignedHash_for_debug)
	// Load Factor の測定
	/*
	benchmark_ICHashT_MaxLoadFactor( 50 );
	printf("=============================================\n");
	benchmark_unorderd_map_MaxLoadFactor( 50 );
	//*/

	/*
	// Insert
	benchmark_ICHashT_set1(50);
	printf("=============================================\n");
	benchmark_unorderd_map_insert1(50);
	//*/

	/*
	benchmark_ICHashT_set2_StdString();
	printf("=============================================\n");
	benchmark_unorderd_map_insert2_StdString();
	//*/

	/*
	// Find, Case: 1
	benchmark_ICHashT_find1();
	printf("=============================================\n");
	benchmark_unorderd_map_find1();
	//*/

	/*
	benchmark_ICHashT_find2();
	printf("=============================================\n");
	benchmark_unorderd_map_find2();
	//*/

	/*
	benchmark_ICHashT_find3();
	printf("=============================================\n");
	benchmark_unorderd_map_find3();
	//*/

	/*
	// Find, Case: 2
	benchmark_ICHashT_find4();
	printf("=============================================\n");
	benchmark_unorderd_map_find4();
	//*/

	/*
	// Erase
	benchmark_ICHashT_erase1(50);
	printf("=============================================\n");
	benchmark_unorderd_map_erase1(50);
	//*/

	/*
	benchmark_ICHashT_find5_get_StdString();
	printf("=============================================\n");
	benchmark_unorderd_map_find5_get_StdString();
	//*/

	/*
	benchmark_ICHashT_find6_get_UINT64pointer();
	printf("=============================================\n");
	benchmark_unorderd_map_find4();
	//*/



	/*
	// Find, Case1 + Erase
//	benchmark_ICHashT_find1_and_Erase(50);
	printf("=============================================\n");
	benchmark_unorderd_map_find1_and_Erase(50);
	//*/


	/*
	// Find, Case2 + Erase
//	benchmark_ICHashT_find4_and_Erase(50);
	printf("=============================================\n");
	benchmark_unorderd_map_find4_and_Erase(50);
	//*/



#endif

	// benchmarks (  end  ) =================================================================================================

	
	// DebugFunctions ( start ) =============================================================================================
	//
	// When you use functions in "ICHashT_DebugFunctions.hpp",
	// you should define "#define ICHashT_DebugMode" and "#define ReturnAssignedHash_for_debug" in "ICHashT.hpp".

#ifdef ReturnAssignedHash_for_debug
//	ICHashT_DebugFunction_ConfirmBasicOperation();

//	ICHashT_DebugFunction_ConfirmSetProcessingNum1();		// 処理番号①
//	ICHashT_DebugFunction_ConfirmSetProcessingNum2();		// 処理番号②
//	ICHashT_DebugFunction_ConfirmSetProcessingNum3();		// 処理番号③, 重複なし
//	ICHashT_DebugFunction_ConfirmSetProcessingNum3_2();		// 処理番号③, 重複あり
//	ICHashT_DebugFunction_ConfirmSetProcessingNum4();		// 処理番号④
//	ICHashT_DebugFunction_ConfirmSetProcessingNum4_2();		// 処理番号④
//	ICHashT_DebugFunction_ConfirmSetProcessingNum5and6();	// 処理番号⑤, 処理番号⑥
//	ICHashT_DebugFunction_ConfirmSetProcessingNum7();		// 処理番号⑦
//	ICHashT_DebugFunction_ConfirmSetProcessingNum8();		// 処理番号⑧
//	ICHashT_DebugFunction_ConfirmSetProcessingNum9();		// 処理番号⑨
//	ICHashT_DebugFunction_ConfirmSetProcessingNum9_2();		// 処理番号⑨
//	ICHashT_DebugFunction_ConfirmSetProcessingNum10();		// 処理番号⑩
//	ICHashT_DebugFunction_ConfirmSetProcessingNum11();		// 処理番号⑪

//	ICHashT_DebugFunction_ConfirmEraseProcessingNum1();		// 処理番号①
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum2();		// 処理番号②
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum2_2();	// 処理番号②
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum3();		// 処理番号③
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum3_2();	// 処理番号③
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum4();		// 処理番号④
#endif

	// DebugFunctions (  end  ) =============================================================================================


	SysPauseForWin32();	// コンソールを一時停止する
	return 0;
}
