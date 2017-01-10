/* -*- coding: utf-8 -*- */
/*
 *  Developer name : ADMIS_Walker
 *  Blog           : https://admiswalker.blogspot.jp/
 *  Profile        : https://admiswalker.blogspot.jp/p/profile.html
 *
 *
 *  The MIT License (MIT)
 *  
 *  Copyright (c) 2017 ADMIS_Walker
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 *
 *  日本語訳
 *  
 *  The MIT License
 *  Copyright (c) 2017 ADMIS_Walker
 *  
 *  以下に定める条件に従い，本ソフトウェアおよび関連文書のファイル
 *  （以下「ソフトウェア」）の複製を取得するすべての人に対し，ソフトウェアを無制限
 *  に扱うことを無償で許可します．これには，ソフトウェアの複製を使用，複写，変更，
 *  結合，掲載，頒布，サブライセンス，および/または販売する権利，およびソフトウェア
 *  を提供する相手に同じことを許可する権利も無制限に含まれます．
 *  
 *  上記の著作権表示および本許諾表示を，ソフトウェアのすべての複製または重要な部分
 *  に記載するものとします．
 *  
 *  ソフトウェアは「現状のまま」で，明示であるか暗黙であるかを問わず，何らの保証も
 *  なく提供されます．ここでいう保証とは，商品性，特定の目的への適合性，および権利
 *  非侵害についての保証も含みますが，それに限定されるものではありません． 作者また
 *  は著作権者は，契約行為，不法行為，またはそれ以外であろうと，ソフトウェアに起因
 *  または関連し，あるいはソフトウェアの使用またはその他の扱いによって生じる一切の
 *  請求，損害，その他の義務について何らの責任も負わないものとします．
 */

#pragma once

#include <string>

//=====================================================================================================================================================

// Debug options
#define ICHashT_DebugMode				// この行がコメントアウトされていない場合は，デバッグ用のコードを生成します．

// このオプションでは，return_Hash に return_assigned_Hash を指定する．(ICHashT_DebugFunctions.cpp, ICHashT_DebugFunctions.hpp を使用する際に有効にする必要がある)
// これが定義されている場合に限り、makeHash(); 関数は、[return_Hash] の値として、常に [return_assigned_Hash] を返す。
//#define ReturnAssignedHash_for_debug

// Hash table option
#define UnLimit_AllowedSumOfShift		// AllowedSumOfShift で，合計シフト幅を制限しない

// Hash 関数に FNV-1 Hash を使用する．(これを指定しない場合は，std::hash が使用される)
#define Use_FNV1_Hash

//=====================================================================================================================================================

#ifndef Use_FNV1_Hash
	#include <functional>	//[std::hash]に必要
#endif

#include "FNV_Hash.hpp"
#include "printf_dbger.h"

#ifdef ICHashT_DebugMode
	#include "FileWriter.hpp"
	#include "str_sprintf.hpp"
#endif

#ifdef _WIN32
	typedef unsigned __int64 UINT64;
#else
	typedef unsigned long long UINT64;
#endif

//typedef UINT64 KeyType;	//ユーザーが適当にint型とかchar型を使う事を防ぐため，別名を定義しておく．

typedef UINT64 AllScanPlace;	//ユーザーが適当にint型とかchar型を使う事を防ぐため，別名を定義しておく．
#ifdef ICHashT_DebugMode
	// デバッグ用の設定
typedef UINT64 DumpAll_Place;	//ユーザーが適当にint型とかchar型を使う事を防ぐため，別名を定義しておく．
#endif//ICHashT_DebugMode


struct HashStruct{
	UINT64 key;					// key
	void* value;				// value
	unsigned char previous;		// 前の要素へのシフト数．チェーンの先頭の場合は[ previous ==   0 ]．また，[ previous == 255 ]とすることで，要素が空であることを示す．
	unsigned char next;			// 次の要素へのシフト数．
											// いずれも，右シフトしか許可されていない．
											// シフト0で自分自身を示す．
											// unsigned char型で定義しているので，最大で255回の右シフトが可能．
											// シフト許可幅を大きく取れば取るほどデータの挿入は容易になるが，
											// シフト許可幅を大きく取れば取るほどキャッシュミスによって速度が低下する可能性が大きくなる．
};


typedef class InsertedChainedHashTable{
private:
	struct HashStruct* HashTable;		//mallocで確保されたハッシュテーブル本体のアドレスを記録する．

	UINT64 NumOfElementsInTheTable;	//テーブル上に格納されているkey valueペアの数．(テーブルの充填率を計算する際に用いる．)

	UINT64 TableLength;			//見かけ上のテーブルサイズ
	UINT64 AllowedSumOfShift;	//チェーンの合計シフト数制限．
	unsigned char AllowedRightShiftNum;	//右シフトを許可する最大値の指定．(0～254まで．)
										//このサイズ分だけ，ハッシュ値重複に備えて，隠しテーブルを確保しておく．(シフトは右にしか許可していないため，テーブルの一番最後のハッシュ値が重複した場合に要素を保存する先が無いため．)
										//[ previous == 255 ]は，配列が空である事を表す予約済みの数値．そのため，シフト数は[0～254]までに制限される．
										//[ next     == 255 ]は，previousが0～254までに制限されている以上，使われない．
	UINT64 TrueTableLength;	//[ TableLength + AllowedRightShiftNum ]となる．

	double LoadFactor;	//テーブルの使用率，負荷係数，(座席利用率)
						//[ LoadFactor = NumOfElementsInTheTable / TrueTableLength; ]で計算される．
						//(今の所，)毎回使用する訳ではないので，常に最新の値が記録されている訳では無い．(必要な場合の直前でのみ計算．)
	void SetLoadFactor();	//テーブル占有率の計算

	unsigned char PrimeList_Num;		//テーブルサイズが，配列「SmallestPrimeList_LargerThanPower2」の何番目の要素かを記録する．「SmallestPrimeList_LargerThanPower2」の要素数が64なので，PrimeList_Numは，0～63まで．

	void makeHash( UINT64& key, UINT64& return_Hash, UINT64& TableLength_makeHash );
	void reHash();						// リハッシュ //ハッシュテーブルの再構成．テーブルサイズをおよそ2倍に拡大して再構成する．(「およそ」なのは，テーブルサイズを素数にする関係上．)(2倍以上でも良いが，2倍以下だと，一要素ごとの演算量に換算しても定数時間で終わらなくなるのでマズイ．効率は一度に拡大するサイズが大きければ大きいほど向上する．(ただし，確保された要素が使われないのであれば無駄なので，やはり2倍ずつ拡大する事にする．))

	void mallocTable( struct HashStruct*& HashTable_p_for_malloc, UINT64& TrueTableLength_mallocTable );
	void freeTable  ( struct HashStruct*& HashTable_p_for_free );

	void setX_key_value_0_0( struct HashStruct& HashTable_setX, UINT64& key, void*& value );														// setX() に必要
	void setX_Move_key_value( struct HashStruct& HashTable_empty, struct HashStruct& HashTable_first );												// setX() に必要
	void setX_SameProcessFor4to7( struct HashStruct*& HashTable, UINT64& first, UINT64& empty, UINT64& empty_front, UINT64& TrueTableLength_setX );	// setX() に必要
	void setX          ( char& set_function_type, UINT64& key, void*& value, void**& return_value_pp, bool& result, bool& NeedReHash, struct HashStruct*& HashTable_setX, UINT64& TableLength_setX, UINT64& TrueTableLength_setX );	// [set0]～[set2]関数を[set_function_type]によって切り替える．(別々に書くと，バグ発生時や，機能変更を行う場合に，非常に手間(管理コスト)が増えるので．)
	void setXwithReHash( char  set_function_type, UINT64& key, void*& value, void**& return_value_pp, bool& result,                   struct HashStruct*& HashTable_setX, UINT64& TableLength_setX, UINT64& TrueTableLength_setX );	// setX();関数にリハッシュ機能を付けたもの．(再帰処理を回避するため．)

	void getX          ( char  set_function_type, UINT64& key, void*& value, void**& pp_value,        bool& result );

	void eraseX        ( char  set_function_type, UINT64& key, bool& result, void*&  return_value_p );

//	bool CompareKEY( KeyType key );	//keyの比較部分のみ関数化．(keyのタイプを変更する場合に，迅速に変更できるように，処理をまとめる．)
									//ちゃんと書けば，keyの型の変更が，この関数と，[typedef UINT64 KeyType;]の変更のみで可能となる．はず．
									//inline展開だと，場合によって展開されないので，#defineマクロの方がいいのか???
public:
	InsertedChainedHashTable();								//この時点では，「typedef」されておらず，「ICHashT」は不可なので注意．
	InsertedChainedHashTable( UINT64 TableLength_set );		//オーバーロード(多重定義)される．
	~InsertedChainedHashTable();							//この時点では，「typedef」されておらず，「ICHashT」は不可なので注意．

	//全要素走査用の変数・関数
	AllScanPlace AllScanEnd;
	void AllScanInit     (                            AllScanPlace& AllScanNowPlace );	// AllScanNowPlaceを最初の読み込み場所に初期化する．
	void AllScanNextShift(                            AllScanPlace& AllScanNowPlace );	// AllScanNowPlaceを次の読み込み場所に初期化する．
	void AllScanGetData  ( UINT64& key, void*& value, AllScanPlace& AllScanNowPlace );	// 現在の読み込み位置AllScanNowPlaceにあるデータを読み込む．
		//[AllScanPlace変数]を構造体にして，key valueを持たせれば，イテレータを使った場合のように，[AllScanGetData();関数]を，[AllScanNextShift();関数]内に埋め込み，行数を減らすこともできるが，その場合，実装によっては，用意したその構造体から，さらに，目的の変数や構造体へデータをコピーする必要が生じるので，やめる．

	void reHash_to_fit();				// テーブルサイズが充填率約50%程度となるようにリハッシュ．(テーブルサイズを縮小させる事もできる．)
										// ただし，現状のテーブルサイズが，充填率が50%以下で，最も50%に近い(素数表上にあり選択可能な)テーブルサイズの場合は，何もせずにreturn;
										// テーブル上の要素を大幅に削除して，テーブルを縮小したい場合などに用いる．

	void Reconstruction_to_fit();		//理想的なテーブルサイズとなるように，強制的にリハッシュ．

	void ReconstrucTheTable();			// テーブルの再構成．同じサイズのテーブルとしてテーブルを再構成する．
										// テーブルのチェーンの断片化が激しい場合に用いる(ことを想定)．

	void set0( UINT64& key, void*& value );												// 最も一般的な動作をする関数
	void set1( UINT64& key, void*& value );												// 特殊関数
	void set2( UINT64& key, void*& value, void**& return_value_pp, bool& result );		// 特殊関数
		//set0():keyが既に格納されている場合は，result==falseが返却され，valueを上書きしてreturn;
		//set1():keyが既に格納されている場合は，result==falseが返却され，何もせずにreturn;
		//set2():keyが既に格納されている場合は，result==falseが返却され，valueを取得してreturn;(valueには手を加えないが，voidポインタなので，中身の判断と書き換えができる．)
		//setXwithNoReHash():リハッシュが発生するまでの処理速度計測用に定義した関数
		//→いずれも，何事も無ければ，そのまま値を書き込みreturn;
#ifdef ICHashT_DebugMode
	void setXwithNoReHash( UINT64& key, void*& value, void**& return_value_pp, bool& result, bool& NeedReHash );	//デバッグ用
	void set_for_Debug   ( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．
#endif//ICHashT_DebugMode

	void get0( UINT64& key, void* &    value, bool& result );				// 一般的な動作をする関数	// 指定した key に対応する value を取得する．
	void get1( UINT64& key, void**& pp_value, bool& result );				// 特殊関数					// 指定した key に対応する value を取得する．この時，書き換えができるように，ダブルポインタで渡す．

	void find( UINT64& key,                   bool& result );				// 指定した key に対応する value が存在するかどうかを確認する．

	void erase0( UINT64& key, bool& result );								// 一般的な動作をする関数	// key 及び value の削除(もしも対象のkeyが発見されなかった場合はfalseを返す．)
	void erase1( UINT64& key, bool& result, void*& return_value_p );		// 特殊関数					// key 及び value の削除(もしも対象のkeyが発見されなかった場合はfalseを返す．)
		// erase0(); :: 指定したkeyの要素を削除するだけ．
		// erase1(); :: 指定したkeyの要素を削除すると同時に， key とペアになっていた value を取得する．
		//              ( データを削除する際に，valueの先にメモリが動的に確保されており，メモリを解放する必要がある場合，
		//                また，データを取得した後に必ずデータを削除する場合，を想定．                                    )

	void AllClear();	//テーブルを初期化する．(テーブル上のデータは全て削除される．)

	//テーブル占有率の計算
	UINT64 GetTableLength();		//テーブル長を確認することで，リハッシュが発生しているかどうか，確認することができる．
	UINT64 GetTrueTableLength();	//テーブル長を確認することで，リハッシュが発生しているかどうか，確認することができる．
	UINT64 GetNumOfElementsInTheTable();
	double GetLoadFactor();

	//=====================================================================================================================================================
#ifdef ReturnAssignedHash_for_debug

	// set(); 関数 及び，get(); 関数，erase(); 関数のデバッグ用
	UINT64 return_assigned_Hash;	// makeHash();関数の返す[return_Hash]の値を指定する。

#endif//ReturnAssignedHash_for_debug
	//=====================================================================================================================================================
#ifdef ICHashT_DebugMode
	// デバッグ用の関数

	// ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	DumpAll_Place DumpAll_End;
	std::string DumpWithRange( UINT64& start, UINT64& end );
	void DumpAll_for_Debug_Init   (                                                                          DumpAll_Place& DumpAll_NowPlace );	// DumpAll_NowPlaceを開始場所に初期化し，DumpAll_Endを終了場所に設定する．
	void DumpAll_for_Debug_GetData( UINT64& key, void*& value, unsigned char& previous, unsigned char& next, DumpAll_Place& DumpAll_NowPlace );	// 現在DumpAll_NowPlace，で指定されている値の読み込み．
	void DumpALL();
	void DumpAllToTheText( const char* SaveDirctory );																							// ハッシュテーブルを可読な形式でテキストファイルに保存する．

#ifndef UnLimit_AllowedSumOfShift
	UINT64 GetAllowedSumOfShift();
#endif
	unsigned char GetAllowedRightShiftNum();
#endif//ICHashT_DebugMode
	//=====================================================================================================================================================

//	void SaveTableToTheFile();		//ハッシュテーブルをバイナリのままファイルに保存する．
//	void ReadTableFromTheFile();	//バイナリのまま保存したハッシュテーブルをファイルから読み込む．
			//いやいや，Saveしたくても，keyは兎も角valueはただのvoid*だから普通には無理．
}ICHashT;


//素数表
//prime(X)をX番目の素数とするとき，1<=n<=33 について，
// prime(X-1) < 2^n <= prime(X)
//を満たす素数 prime(X) を表にしている．
//
//	Smallest prime >= 2^n.
const UINT64 SmallestPrimeList_LargerThanPower2[64] = {
	//[ull]は，[unsigned long long]の意味(付けておかないとGCCでのコンパイルで警告が出る．)
	2ull					,	// 2^ 1 + 0   = 2ull
	5ull					,	// 2^ 2 + 1   = 5ull
	11ull					,	// 2^ 3 + 3   = 11ull
	17ull					,	// 2^ 4 + 1   = 17ull
	37ull					,	// 2^ 5 + 5   = 37ull
	67ull					,	// 2^ 6 + 3   = 67ull
	131ull					,	// 2^ 7 + 3   = 131ull
	257ull					,	// 2^ 8 + 1   = 257ull
	521ull					,	// 2^ 9 + 9   = 521ull
	1031ull					,	// 2^10 + 7   = 1031ull
	2053ull					,	// 2^11 + 5   = 2053ull
	4099ull					,	// 2^12 + 3   = 4099ull
	8209ull					,	// 2^13 + 17  = 8209ull
	16411ull				,	// 2^14 + 27  = 16411ull
	32771ull				,	// 2^15 + 3   = 32771ull
	65537ull				,	// 2^16 + 1   = 65537ull
	131101ull				,	// 2^17 + 29  = 131101ull
	262147ull				,	// 2^18 + 3   = 262147ull
	524309ull				,	// 2^19 + 21  = 524309ull
	1048583ull				,	// 2^20 + 7   = 1048583ull
	2097169ull				,	// 2^21 + 17  = 2097169ull
	4194319ull				,	// 2^22 + 15  = 4194319ull
	8388617ull				,	// 2^23 + 9   = 8388617ull
	16777259ull				,	// 2^24 + 43  = 16777259ull
	33554467ull				,	// 2^25 + 35  = 33554467ull
	67108879ull				,	// 2^26 + 15  = 67108879ull		//<- Table size is about 1GByte.
	134217757ull			,	// 2^27 + 29  = 134217757ull
	268435459ull			,	// 2^28 + 3   = 268435459ull
	536870923ull			,	// 2^29 + 11  = 536870923ull
	1073741827ull			,	// 2^30 + 85  = 1073741827ull
	2147483659ull			,	// 2^31 + 11  = 2147483659ull
	4294967311ull			,	// 2^32 + 15  = 4294967311ull
	8589934609ull			,	// 2^33 + 17  = 8589934609ull
	17179869209ull			,	// 2^34 + 25  = 17179869209ull
	34359738421ull			,	// 2^35 + 53  = 34359738421ull
	68719476767ull			,	// 2^36 + 31  = 68719476767ull
	137438953481ull			,	// 2^37 + 9   = 137438953481ull
	274877906951ull			,	// 2^38 + 7   = 274877906951ull
	549755813911ull			,	// 2^39 + 23  = 549755813911ull
	1099511627791ull		,	// 2^40 + 15  = 1099511627791ull
	2199023255579ull		,	// 2^41 + 27  = 2199023255579ull
	4398046511119ull		,	// 2^42 + 15  = 4398046511119ull
	8796093022237ull		,	// 2^43 + 29  = 8796093022237ull
	17592186044423ull		,	// 2^44 + 7   = 17592186044423ull
	35184372088891ull		,	// 2^45 + 59  = 35184372088891ull
	70368744177679ull		,	// 2^46 + 15  = 70368744177679ull
	140737488355333ull		,	// 2^47 + 5   = 140737488355333ull
	281474976710677ull		,	// 2^48 + 21  = 281474976710677ull
	562949953421381ull		,	// 2^49 + 69  = 562949953421381ull
	1125899906842679ull		,	// 2^50 + 55  = 1125899906842679ull
	2251799813685269ull		,	// 2^51 + 21  = 2251799813685269ull
	4503599627370517ull		,	// 2^52 + 21  = 4503599627370517ull
	9007199254740997ull		,	// 2^53 + 5   = 9007199254740997ull
	18014398509482143ull	,	// 2^54 + 159 = 18014398509482143ull
	36028797018963971ull	,	// 2^55 + 3   = 36028797018963971ull
	72057594037928017ull	,	// 2^56 + 81  = 72057594037928017ull
	144115188075855881ull	,	// 2^57 + 9   = 144115188075855881ull
	288230376151711813ull	,	// 2^58 + 69  = 288230376151711813ull
	576460752303423619ull	,	// 2^59 + 131 = 576460752303423619ull
	1152921504606847009ull	,	// 2^60 + 33  = 1152921504606847009ull
	2305843009213693967ull	,	// 2^61 + 15  = 2305843009213693967ull
	4611686018427388039ull	,	// 2^62 + 135 = 4611686018427388039ull
	9223372036854775837ull	,	// 2^63 + 29  = 9223372036854775837ull
//	18446744073709551629ull	,	// 2^64 + 13  = 18446744073709551629ull	//[UINT64]の範囲を超える大きさなので不可．→つまり，[UINT64]の最大値 - シフト許可幅 より小さい最大の素数を割り当てる必要がある．……が，その素数が分からないので，パス．
	18446744073709551360ull		// 2^64 - 254 = 18446744073709551360ull
								// これは     「18446744073709551360」となるので，明らかに素数ではない(10で割れる)が，上記の通り，素数が分からないので，単純に「[UINT64]の最大値 - シフト許可幅の最大値」を割り当てている．
								// 2^64については，後で書き直す．(おそらく自分で素数を計算しない限り不可能．)
};


const unsigned char HyperParameters[16] = {
	5,		// 2
	10,		// 5
	25,		// 11
	40,		// 17
	40,		// 37
	20,		// 67
	20,		// 131
	20,		// 257
	10,		// 521
	75,		// 1031
	60,		// 2053
	160,	// 4099
	195,	// 8209
	205,	// 16411
	205,	// 32771
	205		// 65537
};
unsigned char GetHyperParameters( unsigned char& PrimeList_Num );
/*
 * 分布 (y 軸: table length, x 軸: this table shows load factors)
 * (詳細は，[HyperParameters.xlsx] を参照)
 *
 * *
 *   *
 *       *
 *            *
 * *                  *        *
 *    *
 *       *
 *    *                 
 * *                          *
 *           *    *
 *      *
 *   *
 * *
 * 
 */
