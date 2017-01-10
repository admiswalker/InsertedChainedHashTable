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

//=====================================================================================================================================================
//InsertedChainedHashTable
//=====================================================================================================================================================
#include "ICHashT.hpp"

//コンストラクタ
ICHashT::InsertedChainedHashTable(){

	PrimeList_Num        = 9;													//reHash()でアクセスされるので，必ず初期化しておく．
	
	NumOfElementsInTheTable = 0;	//テーブル上に格納されているkey valueペアの数．(テーブルの充填率を計算する際に用いる．)

	TableLength          = SmallestPrimeList_LargerThanPower2[PrimeList_Num];	//デフォルトのテーブルサイズ(1031)
#ifndef UnLimit_AllowedSumOfShift
	AllowedSumOfShift    = 254;								//合計シフト数制限	//許可される右シフトの合計値について，最大値の指定．
#endif
	AllowedRightShiftNum = GetHyperParameters( PrimeList_Num );//254;			//右シフトを許可する最大値の指定．(0～254まで．)
	TrueTableLength      = TableLength + AllowedRightShiftNum;					//実際に確保するテーブル長を計算
	
	mallocTable(HashTable,TrueTableLength);	//テーブルの確保，及び初期化
}
//コンストラクタのオーバーロード(多重定義)
ICHashT::InsertedChainedHashTable( UINT64 TableLength_set ){
	//[TableLength]が大きすぎる場合のエラー処理は省略．( SmallestPrimeList_LargerThanPower2[64]==18446744073709551360 以下である必要がある．)

	PrimeList_Num = 0;															//reHash()でアクセスされるので，必ず初期化しておく．
	for(; PrimeList_Num<64; PrimeList_Num++){ //32bit対応するのであれば，注意した方が良いのかな?(素数表も32bitならサイズを小さくした方が良い．)
		if( SmallestPrimeList_LargerThanPower2[PrimeList_Num] >= TableLength_set ){
			break;		//for文を抜ける．
		}
	}
	
	NumOfElementsInTheTable = 0;	//テーブル上に格納されているkey valueペアの数．(テーブルの充填率を計算する際に用いる．)

	TableLength          = SmallestPrimeList_LargerThanPower2[PrimeList_Num];
#ifndef UnLimit_AllowedSumOfShift
	AllowedSumOfShift    = 254;								//合計シフト数制限	//許可される右シフトの合計値について，最大値の指定．
#endif//UnLimit_AllowedSumOfShift
	AllowedRightShiftNum = GetHyperParameters( PrimeList_Num );//254;			//右シフトを許可する最大値の指定．(0～254まで．)
	TrueTableLength      = TableLength + AllowedRightShiftNum;					//実際に確保するテーブル長を計算

	mallocTable(HashTable,TrueTableLength);	//テーブルの確保，及び初期化
}
//ディストラクタ
ICHashT::~InsertedChainedHashTable(){
	freeTable(HashTable);
}

// 指定された値からハッシュ値を生成する関数: key より，[0 ～ UINT64の最大値] までの範囲の値 (ハッシュ値) を生成する．
void ICHashT::makeHash( UINT64& key, UINT64& return_Hash, UINT64& TableLength_makeHash ){

#ifdef ReturnAssignedHash_for_debug
	
	// デバッグ用の設定
	return_Hash = return_assigned_Hash;

#else //ReturnAssignedHash_for_debug
	
	// リリース用の設定
	#ifdef Use_FNV1_Hash
		// FNV-1 Hash を使用
		size_t length = sizeof(UINT64);
		unsigned char*pc_key = (unsigned char*)&key;
		fnv_1_hash_64( return_Hash, (unsigned char*&)pc_key, length );
		return_Hash = return_Hash % TableLength_makeHash;//これだと一様分布しないのでは ??? (HashValue が TableLength_makeHash より十分に大きなことを暗に仮定している)
	#else
		// std::hash を使用
		return_Hash = std::hash<UINT64>()( key );		//ハッシュ値の生成．[std::hash]を利用する．
		return_Hash = return_Hash % TableLength_makeHash;//これだと一様分布しないのでは ??? (HashValue が TableLength_makeHash より十分に大きなことを暗に仮定している)
	#endif //Use_FNV1_Hash

#endif//ReturnAssignedHash_for_debug

}



void ICHashT::mallocTable( struct HashStruct*& HashTable_p_for_malloc, UINT64& TrueTableLength_mallocTable ){

	HashTable_p_for_malloc = (HashStruct*)malloc((size_t)TrueTableLength_mallocTable*sizeof(HashStruct));	//size_tが[UINT64]より小さい場合は，ハッシュテーブルのサイズが制限される可能性がある．
																											//あと，本当は，環境ごとのsize_tのサイズを確認し，エラー処理を行った方が良い．
	//mallocに失敗した場合，falseを返すようにした方が良い．(メモリが足りない場合に失敗する．)
	printf_dbger_if_Exit( HashTable_p_for_malloc == NULL, "ERROR: Memory allocation failure.\n" );

	HashStruct HashStruct_for_init;
	HashStruct_for_init.key      = 0;
	HashStruct_for_init.value    = 0;
	HashStruct_for_init.previous = 255;	//[previous==255] 配列が空であることを示す．
	HashStruct_for_init.next     = 0;
	
	//初期化(データをNULLにしておかないと，参照したテーブルが既に使われているのか，挿入しても大丈夫なのかが分からない．)
	for(int i=0; i<TrueTableLength_mallocTable; i++){
		HashTable_p_for_malloc[i] = HashStruct_for_init;
	}
}
void ICHashT::freeTable( struct HashStruct*& HashTable_p_for_free ){
	free(HashTable_p_for_free);
}



void ICHashT::AllClear(){
	
	HashStruct HashStruct_for_init;
	HashStruct_for_init.key      = 0;
	HashStruct_for_init.value    = 0;
	HashStruct_for_init.previous = 255;	//[previous==255] 配列が空であることを示す．
	HashStruct_for_init.next     = 0;

	//初期化(データをNULLにしておかないと，参照したテーブルが既に使われているのか，挿入しても大丈夫なのかが分からない．)
	for(int i=0; i<TrueTableLength; i++){
		HashTable[i] = HashStruct_for_init;
	}
}



//全要素出力・走査→イテレータを用いて実装しようと思ったが，C++11的な，最近の技術で，信用ならないので使わない事にした．
void ICHashT::AllScanInit( AllScanPlace& AllScanNowPlace ){
#define AllScanStart 0x0000000000000000
	AllScanNowPlace = (AllScanPlace)AllScanStart;		//カーソルを初期位置にセット．
	AllScanEnd      = (AllScanPlace)TrueTableLength;
	AllScanNextShift( AllScanNowPlace );//これを一回だけ実行しておかないと，一回目のfor文で，必ず，HashTable[0]のkeyとvalueが引っかかる事になってしまう．
#undef AllScanStart
}
void ICHashT::AllScanNextShift( AllScanPlace& AllScanNowPlace ){
	for(; AllScanNowPlace < AllScanEnd; ){
		if(HashTable[AllScanNowPlace].previous != 255){//配列に要素があるかどうかの確認．
			return;//要素を発見したのでreturn;
		}
		AllScanNowPlace++;//次の要素を検索する．
	}
}
void ICHashT::AllScanGetData( UINT64& key, void*& value, AllScanPlace& AllScanNowPlace ){
	key   = HashTable[AllScanNowPlace].key;
	value = HashTable[AllScanNowPlace].value;
	AllScanNowPlace++;//for文の途中でreturnしたので，加算されていない．そのため，ここで加算しておく．
}
/*
	上記の実装は，下記のように AllScanGetData() を実行せずにループを回すと，AllScanNowPlace++; されないため，無限ループに陥るため，注意．
	-> イテレータで書き直しておくように

	// 全要素走査 ( ハッシュテーブル上に記録されているすべての key value ペアの取り出し． )
	printf("■■■ Check AllScan function. ( Start ) ■■■\n");
	{
		UINT64 counter=0;
		AllScanPlace NowPlace;
		for(ICHashT_p->AllScanInit(NowPlace); NowPlace < ICHashT_p->AllScanEnd; ICHashT_p->AllScanNextShift(NowPlace)){
			counter++;
		}
		printf("counter: %llu\n", counter);
	}
	printf("■■■ Check AllScan function. (  End  ) ■■■\n");
*/



void ICHashT::setX_key_value_0_0( struct HashStruct& HashTable, UINT64& key, void*& value ){
	HashTable.key      = key;		// データの書き込み処理 ( 前からある要素 )
	HashTable.value    = value;	// データの書き込み処理 ( 前からある要素 )
	HashTable.previous = 0;		// データの書き込み処理 ( 前からある要素 )
	HashTable.next     = 0;		// データの書き込み処理 ( 前からある要素 )
}
void ICHashT::setX_Move_key_value( struct HashStruct& HashTable_empty, struct HashStruct& HashTable_first ){
	HashTable_empty.key   = HashTable_first.key;	//データの移動処理 ( 前からある要素 )
	HashTable_empty.value = HashTable_first.value;	//データの移動処理 ( 前からある要素 )
	return;
}
void ICHashT::setX_SameProcessFor4to7( struct HashStruct*& HashTable, UINT64& first, UINT64& empty, UINT64& empty_front, UINT64& TrueTableLength_setX ){
	printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
	printf_dbger_if_Exit_Stop( HashTable[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
	setX_Move_key_value(HashTable[ empty ], HashTable[ first ]);
	HashTable[ empty ].previous = (unsigned char)( empty - empty_front );

	UINT64 ShiftOf_EmptyFront_to_Empty = empty - empty_front;
	printf_dbger_if_Exit_Stop( ShiftOf_EmptyFront_to_Empty>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftNum: %llu, empty: %llu, empty_front: %llu.", ShiftOf_EmptyFront_to_Empty, empty, empty_front );
	HashTable[ empty_front ].next     = (unsigned char)( ShiftOf_EmptyFront_to_Empty );
	HashTable[ empty       ].previous = (unsigned char)( ShiftOf_EmptyFront_to_Empty );
}
void ICHashT::setX( char& set_function_type, UINT64& key, void*& value, void**& return_value_pp, bool& result, bool& NeedReHash, struct HashStruct*& HashTable_setX, UINT64& TableLength_setX, UINT64& TrueTableLength_setX ){
	//[char set_function_type]
	//		'0'はset0
	//		'1'はset1
	//		'2'はset2

	//既に同一のkeyが登録されている場合．→valueを更新

	//[previous==255] 配列が空であることを示す．
	//[previous!=255] 配列は使用済み．ただし，ハッシュ値の重複が無いため，チェーンはされていない．
	//[next    ==  0] 要素の末尾
	//[previous==  0] slaveではない通常の要素として使われていることを示す．(必ずチェーンの先頭となる)
	//[ next == 0 && previous == 0 ] 要素数1のチェーンの先頭．
	
	//setX();関数の実行結果が，[NeedReHash==true]の場合に，リハッシュするように設計する．

	UINT64 first;							//チェーンの先頭，の意味を込めて．
	makeHash(key,first,TableLength_setX);	//ハッシュ値の生成

	if( HashTable_setX[first].previous == 255 ){ //[if( HashTable_setX[return_Hash].key == 0 ){]としないのは，[key]自身が[0]という値を取れなくなってしまうから．[if( HashTable_setX[return_Hash].value == 0 ){]としないのも同様．(今はvalueはvoid*だが，void*は単なる64bitのメモリに過ぎないので，直接値を格納する可能性もある．)
		// テーブルが空いていた場合．
		// [処理番号①]
		printf_dbger_if_Exit_Stop( first >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
		printf_dbger_if_Exit_Stop( HashTable_setX[ first ].previous != 255, "ERROR: This element is not empty!\n" );
		setX_key_value_0_0( HashTable_setX[ first ], key, value );

		NeedReHash = false; result = true; NumOfElementsInTheTable++;	//テーブル上に格納されているkey valueペアの数のカウントを加算．( 失敗している場合は，keyが重複している場合で，要素数は変化しない． )
		return;
	}


	//======================================================================================================================================================================================================================================================
	if( HashTable_setX[first].previous ==   0 ){
		// [  HashTable_setX[first] が，keyチェーンの先頭の場合．    ]
		// [  → 最もkeyに近い配列の空きスペースに，要素を格納する． ]
		// [                                                         ]
		// [  [処理番号①]，[処理番号②]，[処理番号③]               ]

		//*
		// [実装方法01::素直な実装]
		{
			UINT64 NowPlace = first;

			while(1){
				if( HashTable_setX[NowPlace].key == key ){
					// keyが一致した場合．
					// set_function_type == '0'  →  result = false; valueの上書きを行う．
					// set_function_type == '1'  →  result = false; 何も行わずにreturn．
					// set_function_type == '2'  →  result = false; valueの上書きを行わない代わりに，valueへのポインタを返す．
					if( set_function_type == '0' ){
						HashTable_setX[NowPlace].value = value;
						NeedReHash = false; result = false; return;
					}else if( set_function_type=='1' ){
						NeedReHash = false; result = false; return;
					}else{// if( set_function_type == '2' ){
						return_value_pp = &HashTable_setX[NowPlace].value;	//   return_value_pp :: &(HashTable_setX[return_Hash].value)
																			//  *return_value_pp ::   HashTable_setX[return_Hash].value == void* ←これに対してmallocとかしてメモリを割り当てる予定．
																			// **return_value_pp :: mallocで確保していれば，実体がある．
																			//[free((void*)(*return_value_pp));]←mallocで確保した物を解放したいのであれば．
						NeedReHash = false; result = false; return;								//set2():keyが既に格納されている場合は，valueを取得してreturn;(valueには手を加えないが，voidポインタなので，中身の判断と書き換えができる．)→中身を比較判定してから書き換えたい場合に便利．
					}
				}
				UINT64 NextShift = HashTable_setX[NowPlace].next;

				if(NextShift==0){break;}

				NowPlace += HashTable_setX[NowPlace].next;
			}
		}
	

		//keyの一致が無かった場合．
		//空きの走査
		{
			UINT64 last;
			{
				UINT64 NowPlace = first;

				while(1){
					unsigned char NextShift = HashTable_setX[NowPlace].next;

					if(NextShift==0){
						last = NowPlace;
						break;
					}

					for( unsigned char i=1; i<NextShift; i++ ){ //現在地[return_Hash]の1つシフトである，[return_Hash+1]から，[return_Hash+NextShift]の一つ手前である，[return_Hash+NextShift-1]までの間に，空の配列が無いかどうかを走査する．
						if( HashTable_setX[NowPlace+i].previous == 255 ){
							// 空いている配列を発見．
							// [処理番号②]
							UINT64 empty_front = NowPlace;
							UINT64 empty       = NowPlace + i;
							UINT64 empty__back = NowPlace + NextShift;

							printf_dbger_if_Exit_Stop(empty>=TrueTableLength_setX, "ERROR: Out of Range!\n");
							printf_dbger_if_Exit_Stop(HashTable_setX[empty].previous!=255, "ERROR: This element is not empty!\n");
							HashTable_setX[ empty       ].key      = key;			// データの書き込み処理
							HashTable_setX[ empty       ].value    = value;			// データの書き込み処理

							printf_dbger_if_Exit_Stop( i>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
							HashTable_setX[ empty_front ].next     = i;				// next接続を繋ぎ替え     ( 前からある要素 )
							HashTable_setX[ empty       ].previous = i;				// previous接続を設定     (   新しい要素   )
							printf_dbger_if_Exit_Stop( (NextShift - i)>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. (NextShift - i): %llu, NextShift: %llu, i: %llu.", (NextShift - i), NextShift, i );
							HashTable_setX[ empty       ].next     = NextShift - i;	// next接続を設定         (   新しい要素   )
							HashTable_setX[ empty__back ].previous = NextShift - i;	// previous接続を繋ぎ替え ( 前からある要素 )

							NeedReHash = false; result = true; NumOfElementsInTheTable++;	//テーブル上に格納されているkey valueペアの数のカウントを加算．( 失敗している場合は，keyが重複している場合で，要素数は変化しない． )
							return;
						}
					}

					NowPlace += NextShift;
				}
			}


			//keyチェーンの最後まで走査しても，チェーンの間に空きが無かった場合．
			//keyチェーンの後ろ(last以降)に，空きを探す
			{
				UINT64 ShiftLimit = AllowedRightShiftNum;

				UINT64 LimitOfTableSize = TrueTableLength_setX - last - 1;	//これの確認は，keyがチェーンされていると，無限幅シフト可能なので，必須．

				// (コンパイラにもよるが) for 文中で比較すると計算コストが増えるので，条件を減らす．
				if( ShiftLimit > LimitOfTableSize ){ ShiftLimit = LimitOfTableSize; }

			#ifndef UnLimit_AllowedSumOfShift
			//	UINT64 SumOfShift = last - first;
				UINT64 LeftSumOfShift = AllowedSumOfShift - (last - first); // 許可された合計シフト数の残り
				if( ShiftLimit > LeftSumOfShift ){ ShiftLimit = LeftSumOfShift; }
			#endif

				for( UINT64 i=1; i <= ShiftLimit; i++ ){		//[for( UINT64 i=1; (SumOfShift+i) <= AllowedRightShiftNum; i++ ){]と書くと，VisualStudioでは，毎回足し算を計算し始めてしまう．[for( UINT64 i=1; i <= (AllowedRightShiftNum-SumOfShift); i++ ){]とすれば，一回だけ引き算を行う．
					UINT64 empty = last + i;

					if( HashTable_setX[ empty ].previous == 255 ){
						// 空いている配列を発見．
						// [処理番号③]
						
						UINT64 empty_front = last;
						printf_dbger_if( empty >= TrueTableLength_setX, "ERROR: last: %llu, i: %llu, ShiftLimit: %llu, LimitOfTableSize: %llu,\n", last, i, ShiftLimit, LimitOfTableSize );
						printf_dbger_if( empty >= TrueTableLength_setX, "ERROR: AllowedSumOfShift: %llu, last: %llu, first: %llu, ShiftLimit: %llu\n", AllowedSumOfShift, last, first, ShiftLimit );
					#ifndef UnLimit_AllowedSumOfShift
						printf_dbger_if( empty >= TrueTableLength_setX, "ERROR: AllowedSumOfShift: %llu, last: %llu, first: %llu, LeftSumOfShift: %llu, ShiftLimit: %llu\n", AllowedSumOfShift, last, first, LeftSumOfShift, ShiftLimit );
					#endif
						printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
						printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );

						HashTable_setX[ empty       ].key      = key;	// データの書き込み処理 (   新しい要素   )
						HashTable_setX[ empty       ].value    = value;	// データの書き込み処理 (   新しい要素   )

						printf_dbger_if_Exit_Stop( i>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
						HashTable_setX[ empty_front ].next     = (unsigned char)i;		// next接続を繋ぎ替え   ( 前からある要素 )
						HashTable_setX[ empty       ].previous = (unsigned char)i;		// データの書き込み処理 (   新しい要素   )
						HashTable_setX[ empty       ].next     = 0;		// データの書き込み処理 (   新しい要素   )

						NeedReHash = false; result = true; NumOfElementsInTheTable++;	//テーブル上に格納されているkey valueペアの数のカウントを加算．( 失敗している場合は，keyが重複している場合で，要素数は変化しない． )
						return;
					}
				}

				//keyの挿入に失敗．( [合計シフト制限] を超過 → リハッシュの必要がある． )
				//printf_dbger("NeedReHash: 3\n");
				NeedReHash = true; result = false; return;
			}
		}
		//*/


		/*
		//(動作確認してないし，直してないから動かないハズ．)

		// [実装方法02::難解な実装]
		//空きの走査と，keyの一致を同時進行で確認する．ただし，先に空きが発見された場合は，keyの一致が空きまでしか確認されないので，最後までkeyの一致が無い事を確認してから代入している．
		//空きの走査
		{
			UINT64 last;
			UINT64 empty = 0;	//0は使われないので，0で初期化しても問題ない．//[empty]はデータ構造的に，必ず1以上の値を取る．(HashTable_setX[0]がemptyであることは，データ構造上不可能．)
			{
				UINT64 NowPlace = first;

				while(1){
					if( HashTable_setX[NowPlace].key == key ){ //keyの一致の確認．
						// keyが一致した場合．
						// set_function_type=='0'  →  result = false; valueの上書きを行う．
						// set_function_type=='1'  →  result = false; 何も行わずにreturn．
						// set_function_type=='2'  →  result = false; valueの上書きを行わない代わりに，valueへのポインタを返す．
						if( set_function_type=='0' ){
							HashTable_setX[NowPlace].value = value;
							NeedReHash = false; result = false; return;
						}else if(set_function_type=='1'){
							NeedReHash = false; result = false; return;
						}else{// if(set_function_type=='2'){
							return_value_pp = HashTable_setX[NowPlace].value;	//   return_value_pp :: &(HashTable_setX[return_Hash].value)
																				//  *return_value_pp ::   HashTable_setX[return_Hash].value == void* ←これに対してmallocとかしてメモリを割り当てる予定．
																				// **return_value_pp :: mallocで確保していれば，実体がある．
																				//[free((void*)(*return_value_pp));]←mallocで確保した物を解放したいのであれば．
							NeedReHash = false; result = false; return;								//set2():keyが既に格納されている場合は，valueを取得してreturn;(valueには手を加えないが，voidポインタなので，中身の判断と書き換えができる．)→中身を比較判定してから書き換えたい場合に便利．
						}
					}
					UINT64 NextShift = HashTable_setX[NowPlace].next;

					if(NextShift==0){
						last = NowPlace;
						break;
					}

					for( UINT64 i=1; i<NextShift; i++ ){ //現在地[return_Hash]の1つシフトである，[return_Hash+1]から，[return_Hash+NextShift]の一つ手前である，[return_Hash+NextShift-1]までの間に，空の配列が無いかどうかを走査する．
						if( HashTable_setX[NowPlace+i].previous == 255 ){
							//空いている配列を発見．

							//一致するkeyが無いことを，チェーンの最後まで確認する．
							{
								UINT64 NextPlace = NowPlace + NextShift;

								while(1){
									if( HashTable_setX[NextPlace].key == key ){
										// keyが一致した場合．
										// set_function_type=='0'  →  result = false; valueの上書きを行う．
										// set_function_type=='1'  →  result = false; 何も行わずにreturn．
										// set_function_type=='2'  →  result = false; valueの上書きを行わない代わりに，valueへのポインタを返す．
										if( set_function_type=='0' ){
											HashTable_setX[NowPlace].value = value;
											NeedReHash = false; result = false; return;
										}else if( set_function_type=='1' ){
											NeedReHash = false; result = false; return;
										}else{// if(set_function_type=='2'){
											return_value_pp = HashTable_setX[NowPlace].value;	//   return_value_pp :: &(HashTable_setX[return_Hash].value)
																								//  *return_value_pp ::   HashTable_setX[return_Hash].value == void* ←これに対してmallocとかしてメモリを割り当てる予定．
																								// **return_value_pp :: mallocで確保していれば，実体がある．
																								//[free((void*)(*return_value_pp));]←mallocで確保した物を解放したいのであれば．
											NeedReHash = false; result = false; return;								//set2():keyが既に格納されている場合は，valueを取得してreturn;(valueには手を加えないが，voidポインタなので，中身の判断と書き換えができる．)→中身を比較判定してから書き換えたい場合に便利．
										}
									}
									UINT64 NextShift = HashTable_setX[NextPlace].next;

									if(NextShift==0){break;}	//一致するkeyが無かったので，while()文を抜ける．

									NextPlace += HashTable_setX[NextPlace].next;
								}
							}

							//一致するkeyが無い事を確認したので，値を配列の空きに格納する．
							// [処理番号②]
							UINT64 empty = NowPlace + i;

							printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
							printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
							HashTable_setX[ empty                ].key      = key;				// データの書き込み処理
							HashTable_setX[ empty                ].value    = value;			// データの書き込み処理

							printf_dbger_if_Exit_Stop( i>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
							HashTable_setX[ NowPlace             ].next     = i;				// next接続を繋ぎ替え     ( 前からある要素 )

							printf_dbger_if_Exit_Stop( (NextShift - i)>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. (NextShift - i): %llu, NextShift: %llu, i: %llu.", (NextShift - i), NextShift, i );
							HashTable_setX[ empty                ].previous = i;				// previous接続を設定     (   新しい要素   )
							HashTable_setX[ empty                ].next     = NextShift - i;	// next接続を設定         (   新しい要素   )

							HashTable_setX[ NowPlace + NextShift ].previous = NextShift - i;	// previous接続を繋ぎ替え ( 前からある要素 )

							NeedReHash = false; result = true; NumOfElementsInTheTable++;	//テーブル上に格納されているkey valueペアの数のカウントを加算．( 失敗している場合は，keyが重複している場合で，要素数は変化しない． )
							return;
						}
					}

					NowPlace += HashTable_setX[NowPlace].next;
				}
			}


			//keyチェーンの最後まで走査しても，チェーンの間に空きが無かった場合．
			//keyチェーンの後ろ(last以降)に，空きを探す
			{
				UINT64 ShiftLimit = AllowedRightShiftNum;

				UINT64 LimitOfTableSize = TrueTableLength_setX - last - 1;	//これの確認は，keyがチェーンされていると，無限幅シフト可能なので，必須．

				// (コンパイラにもよるが) for 文中で比較すると計算コストが増えるので，条件を減らす．
				if( ShiftLimit > LimitOfTableSize ){ ShiftLimit = LimitOfTableSize; }

			#ifndef UnLimit_AllowedSumOfShift
			//	UINT64 SumOfShift = last - first;
				UINT64 LeftSumOfShift = AllowedSumOfShift - (last - first); // 許可された合計シフト数の残り
				if( ShiftLimit > LeftSumOfShift ){ ShiftLimit = LeftSumOfShift; }
			#endif

				for( UINT64 i=1; i <= ShiftLimit; i++ ){		//[for( UINT64 i=1; (SumOfShift+i) <= AllowedRightShiftNum; i++ ){]と書くと，VisualStudioでは，毎回足し算を計算し始めてしまう．[for( UINT64 i=1; i <= (AllowedRightShiftNum-SumOfShift); i++ ){]とすれば，一回だけ引き算を行う．
					UINT64 empty = last + i;

					if( HashTable_setX[ empty ].previous == 255 ){
						// 空いている配列を発見．
						// [処理番号③]
						printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
						printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
						HashTable_setX[ empty ].key      = key;		// データの書き込み処理 (   新しい要素   )
						HashTable_setX[ empty ].value    = value;	// データの書き込み処理 (   新しい要素   )
						HashTable_setX[ empty ].previous = i;		// データの書き込み処理 (   新しい要素   )
						HashTable_setX[ empty ].next     = 0;		// データの書き込み処理 (   新しい要素   )

						printf_dbger_if_Exit_Stop( i>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
						HashTable_setX[ last  ].next     = i;		// next接続を繋ぎ替え   ( 前からある要素 )

						NeedReHash = false; result = true; NumOfElementsInTheTable++;	//テーブル上に格納されているkey valueペアの数のカウントを加算．( 失敗している場合は，keyが重複している場合で，要素数は変化しない． )
						return;
					}
				}

				//keyの挿入に失敗．( [合計シフト制限] を超過 → リハッシュの必要がある． )
				NeedReHash = true; result = false; return;
			}
		}
		//*/
	//======================================================================================================================================================================================================================================================
	}else{
		// [  HashTable_setX[first] が，keyチェーンの先頭ではない場合．                                                                           ]
		// [  → HashTable_setX[first] に格納されているkeyチェーンを，HashTable_setX[first] 以外で，最もkeyに近い配列の空きスペースに移動させる． ]
		// [  → データを移動させて空となった HashTable_setX[first] に新しいkeyを格納する．                                                       ]
		// [                                                                                                                                      ]
		// [  [処理番号④]，[処理番号⑥]，[処理番号⑧]                                                                                            ]


		// keyチェーンを遡る方向に，配列の空きを検索し，空きを発見した場合は， HashTable_setX[first] のデータを移動させ， HashTable_setX[first] を空ける．
		// → HashTable_setX[first] に新しいkey，valueをセットする．
		//
		// keyチェーンを遡る方向に，配列の空きが無かった場合は，keyチェーンに沿う方向に，配列の空きを検索する．
		//
		// ハッシュ値が異なる時点で，一致keyが存在する事はあり得ないので，調べる必要はない．

		{
			UINT64 empty = 0;	//必ず初期化する必要がある．	//シフト値として0は使用されないので，0で初期化をしておく．
			UINT64 empty_front;
			UINT64 empty__back;

			{
				// keyチェーンを遡る方向に，配列の空きを検索．
				UINT64 NowPlace = first;

				while(1){
					UINT64 PreviousShift = HashTable_setX[NowPlace].previous;

					if( PreviousShift == 0 ){
						break;
					}

					for( UINT64 i=1; i<PreviousShift; i++ ){
						if( HashTable_setX[NowPlace-i].previous == 255 ){
							// 空いている配列を発見．
							// [処理番号④]，及び，[処理番号⑥]
							empty__back = NowPlace;
							empty       = NowPlace - i;				//最初に見つけた値が，最もkeyに近い空きとは限らないので，[ PreviousShift == 0 ]となるまで，ループを回す必要がある．
							empty_front = NowPlace - PreviousShift;
						}
					}

					NowPlace -= PreviousShift;
				}
			}

			if(empty!=0){
				// keyチェーンを遡る方向に，配列の空きを検索し，空きを発見した場合．

				// emptyが，配列の空きを示している．
				// [処理番号④]，及び，[処理番号⑥]

				// [key]--[empty_front]- (empty) -[empty__back]--[X]--[X]--[first_front]--[first](want to set key2)--[first__back]--[X]

				if( empty__back == first ){
					if( HashTable_setX[ first ].next == 0 ){
						// [処理番号④]

						setX_SameProcessFor4to7( HashTable_setX, first, empty, empty_front, TrueTableLength_setX );
					//	HashTable_setX[ empty       ].next     = 0;
						//[empty]であれば，nextは既に0に初期化されているはずなので，不要．
					}else{
						// [処理番号⑤]

						// シフト限界を超えないことを確認する
						UINT64 ShiftOf_Empty_to_FirstBack  = first + HashTable_setX[ first ].next - empty;
						if( ShiftOf_Empty_to_FirstBack > AllowedRightShiftNum ){
							//printf_dbger("NeedReHash: 5\n");
							NeedReHash = true; result = false; return;
						}

						setX_SameProcessFor4to7( HashTable_setX, first, empty, empty_front, TrueTableLength_setX );
						UINT64 first__back = first + HashTable_setX[first].next;

						printf_dbger_if_Exit_Stop( ShiftOf_Empty_to_FirstBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_Empty_to_FirstBack: %llu, first: %llu, HashTable_setX[ first ].next: %llu, empty: %llu, AllowedRightShiftNum: %llu.", ShiftOf_Empty_to_FirstBack, first, HashTable_setX[ first ].next, empty, AllowedRightShiftNum );
						HashTable_setX[ empty       ].next     = (unsigned char)( ShiftOf_Empty_to_FirstBack  );
						HashTable_setX[ first__back ].previous = (unsigned char)( ShiftOf_Empty_to_FirstBack  );
					}
				}else{
					UINT64 ShiftOf_Empty_to_EmptyBack  = empty__back - empty;
					printf_dbger_if_Exit_Stop( ShiftOf_Empty_to_EmptyBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_Empty_to_EmptyBack: %llu, empty__back: %llu, empty: %llu.", ShiftOf_Empty_to_EmptyBack, empty__back, empty );
					HashTable_setX[ empty       ].next     = (unsigned char)( ShiftOf_Empty_to_EmptyBack  );
					HashTable_setX[ empty__back ].previous = (unsigned char)( ShiftOf_Empty_to_EmptyBack  );

					UINT64 first_front = first - HashTable_setX[first].previous;

					if( HashTable_setX[first].next == 0 ){
						// [処理番号⑥]

						setX_SameProcessFor4to7( HashTable_setX, first, empty, empty_front, TrueTableLength_setX );
						HashTable_setX[ first_front ].next = 0;
					}else{
						// [処理番号⑦]

						// シフト限界を超えないことを確認する
						UINT64 ShiftOf_FirstFront_to_FirstBack = HashTable_setX[ first ].next + HashTable_setX[ first ].previous;//== first__back - first_front
						if( ShiftOf_FirstFront_to_FirstBack > AllowedRightShiftNum ){
							//printf_dbger("NeedReHash: 7\n");
							NeedReHash = true; result = false; return;
						}

						setX_SameProcessFor4to7( HashTable_setX, first, empty, empty_front, TrueTableLength_setX );
						UINT64 first__back = first + HashTable_setX[first].next;

						printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_FirstBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_FirstFront_to_FirstBack: %llu, HashTable_setX[ first ].next: %llu, HashTable_setX[ first ].previous: %llu.", ShiftOf_FirstFront_to_FirstBack, HashTable_setX[ first ].next, HashTable_setX[ first ].previous );
						HashTable_setX[ first_front ].next     = (unsigned char)( ShiftOf_FirstFront_to_FirstBack );
						HashTable_setX[ first__back ].previous = (unsigned char)( ShiftOf_FirstFront_to_FirstBack );
					}
				}
				printf_dbger_if_Exit_Stop( first >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
				setX_key_value_0_0( HashTable_setX[ first ], key, value );

				NeedReHash = false; result = true; NumOfElementsInTheTable++;	//テーブル上に格納されているkey valueペアの数のカウントを加算．( 失敗している場合は，keyが重複している場合で，要素数は変化しない． )
				return;
			}



			// 配列の空きがfirstより前に存在しなかった場合．
			// →firstより後ろの空きを探す．
			//
			// [処理番号⑧]，[処理番号⑨]，及び，[処理番号⑩]，[処理番号⑪]
			{
				UINT64 last;			//( [first] ～ [last] 間の配列に，空きが無い場合に限り，) keyチェーンの終わりを記録する．
				{
					// [first] ～ [last] 間の配列に，空きが無いか走査．
					// [処理番号⑧]，[処理番号⑨] (←空きがある場合の処理．)
					empty = 0;
					UINT64 NowPlace = first;

					while(1){
						UINT64 NextShift = HashTable_setX[NowPlace].next;

						if( NextShift == 0 ){
							last = NowPlace;	//( [first] ～ [last] までの間に，空の配列が無い場合に限り，) keyチェーンの終わりを記録する．
							break;
						}

						for( UINT64 i=1; i<NextShift; i++ ){
							if( HashTable_setX[NowPlace+i].previous == 255 ){
								// 空いている配列を発見．
								empty_front = NowPlace;
								empty       = NowPlace + i;
								empty__back = NowPlace + NextShift;
								UINT64 first_front = first - HashTable_setX[ first ].previous;
								UINT64 first__back = first + HashTable_setX[ first ].next;
								UINT64 ShiftOf_Empty_to_EmptyBack = empty__back - empty;

								if( first == empty_front ){ //[if( empty != 0 ){]の中に処理を入れないと， [処理番号⑩] のパターンの場合に，ここの[empty_front]が初期化されないまま使用される事になってしまう．
									// [処理番号⑨]

									// シフト限界を超えないことを確認する
									UINT64 ShiftOf_FirstFront_to_Empty = empty - first_front;
									if( ShiftOf_FirstFront_to_Empty > AllowedRightShiftNum ){
										printf_dbger("NeedReHash: 9\n");
										NeedReHash = true; result = false; return;
									}

									printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
									printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
									setX_Move_key_value(HashTable_setX[ empty ], HashTable_setX[ first ]);

									printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_Empty > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_FirstFront_to_Empty: %llu, empty: %llu, first_front: %llu.", ShiftOf_FirstFront_to_Empty, empty, first_front );
									HashTable_setX[ first_front ].next     = (unsigned char)( ShiftOf_FirstFront_to_Empty );		//     next接続を繋ぎ替え ( 前からある要素 )
									HashTable_setX[ empty       ].previous = (unsigned char)( ShiftOf_FirstFront_to_Empty );		// previous接続を設定     (   新しい要素   )
								}else{
									// [処理番号⑧]

									// シフト限界を超えないことを確認する
									UINT64 ShiftOf_FirstFront_to_FirstBack = first__back - first_front;
									if( ShiftOf_FirstFront_to_FirstBack > AllowedRightShiftNum ){
										printf_dbger("NeedReHash: 8\n");
										NeedReHash = true; result = false; return;
									}

									printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
									printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
									setX_Move_key_value(HashTable_setX[ empty ], HashTable_setX[ first ]);

									printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_FirstBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_FirstFront_to_FirstBack: %llu, first__back: %llu, first_front: %llu.", ShiftOf_FirstFront_to_FirstBack, first__back, first_front );
									HashTable_setX[ first_front ].next     = (unsigned char)( ShiftOf_FirstFront_to_FirstBack );	//     next接続を繋ぎ替え ( 前からある要素 )
									HashTable_setX[ first__back ].previous = (unsigned char)( ShiftOf_FirstFront_to_FirstBack );	// previous接続を繋ぎ替え ( 前からある要素 )

									UINT64 ShiftOf_EmptyFront_to_Empty = empty - empty_front;
									printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_FirstBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_EmptyFront_to_Empty: %llu, empty: %llu, empty_front: %llu.", ShiftOf_EmptyFront_to_Empty, empty, empty_front );
									HashTable_setX[ empty_front ].next     = (unsigned char)( ShiftOf_EmptyFront_to_Empty );		//     next接続を繋ぎ替え ( 前からある要素 )
									HashTable_setX[ empty       ].previous = (unsigned char)( ShiftOf_EmptyFront_to_Empty );
								}
								printf_dbger_if_Exit_Stop( ShiftOf_Empty_to_EmptyBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_EmptyFront_to_Empty: %llu, empty__back: %llu, empty: %llu.", ShiftOf_Empty_to_EmptyBack, empty__back, empty );
								HashTable_setX[ empty       ].next     = (unsigned char)( ShiftOf_Empty_to_EmptyBack  );		//     next接続を設定     (   新しい要素   )
								HashTable_setX[ empty__back ].previous = (unsigned char)( ShiftOf_Empty_to_EmptyBack  );		// previous接続を繋ぎ替え ( 前からある要素 )

								printf_dbger_if_Exit_Stop( first >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
								printf_dbger_if_Exit_Stop( HashTable_setX[ first ].previous == 255, "ERROR: This element is not empty!\n" );
								setX_key_value_0_0( HashTable_setX[ first ], key, value );

								NeedReHash = false; result = true; NumOfElementsInTheTable++;	//テーブル上に格納されているkey valueペアの数のカウントを加算．( 失敗している場合は，keyが重複している場合で，要素数は変化しない． )
								return;
								// =======================
							}
						}

						NowPlace += NextShift;
					}
				}
				{
					// [first] ～ [last] 間の配列に，空きが無い場合．
					// →[last]より後ろの空きを走査する．
					// [処理番号⑩]，[処理番号⑪]

					if( HashTable_setX[ first ].next == 0 ){
						//[処理番号⑪]

						// FirstFront から First までのシフト数が AllowedRightShiftNum を超えないようにする．
						UINT64 ShiftLimit = AllowedRightShiftNum - HashTable_setX[ first ].previous;

						// これの確認は，keyがチェーンされていると，無限幅シフト可能なので，必須．
						UINT64 LimitOfTableSize = TrueTableLength_setX - last - 1;

						// (コンパイラにもよるが) for 文中で比較すると計算コストが増えるので，条件を減らす．
						if( ShiftLimit > LimitOfTableSize ){ ShiftLimit = LimitOfTableSize; }

					#ifndef UnLimit_AllowedSumOfShift
					//	UINT64 SumOfShift = last - first;
						UINT64 LeftSumOfShift = AllowedSumOfShift - (last - first); // 許可された合計シフト数の残り
						if( ShiftLimit > LeftSumOfShift ){ ShiftLimit = LeftSumOfShift; }
					#endif

						for( UINT64 i=1; i <= ShiftLimit; i++ ){
							UINT64 empty = last + i;
							if( HashTable_setX[ empty ].previous == 255 ){
								setX_Move_key_value(HashTable_setX[ empty ], HashTable_setX[ first ]);

								UINT64 first_front = first - HashTable_setX[ first ].previous;
								UINT64                                                  ShiftOf_FirstFront_to_Empty = empty - first_front;
								printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_Empty > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_FirstFront_to_Empty: %llu, empty: %llu, first_front: %llu.", ShiftOf_FirstFront_to_Empty, empty, first_front );
								HashTable_setX[ first_front ].next     = (unsigned char)ShiftOf_FirstFront_to_Empty;	//     next接続を繋ぎ替え ( 前からある要素 )
								HashTable_setX[ empty       ].previous = (unsigned char)ShiftOf_FirstFront_to_Empty;	// previous接続を繋ぎ替え ( 前からある要素 )
							//	HashTable_setX[ empty       ].next     = 0;												// previous接続を繋ぎ替え ( 前からある要素 )
								//[empty]であれば，nextは既に0に初期化されているはずなので，不要．
								
								printf_dbger_if_Exit_Stop( first >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
							//	printf_dbger_if_Exit_Stop( HashTable_setX[ first ].previous != 255, "ERROR: This element is not empty!\n" );
								setX_key_value_0_0( HashTable_setX[ first ], key, value );

								NeedReHash = false; result = true; NumOfElementsInTheTable++;	//テーブル上に格納されているkey valueペアの数のカウントを加算．( 失敗している場合は，keyが重複している場合で，要素数は変化しない． )
								return;
							}
						}
						//printf_dbger("NeedReHash: 11\n");
					}else{
						//[処理番号⑩]

					//	UINT64 ShiftOf_FirstFront_to_FirstBack = first__back - first_front;
					//                                         = (first + HashTable_setX[ first ].next) - (first - HashTable_setX[ first ].previous)
					//                                         = (      + HashTable_setX[ first ].next) - (      - HashTable_setX[ first ].previous)
					//                                         = HashTable_setX[ first ].next + HashTable_setX[ first ].previous
						UINT64 ShiftOf_FirstFront_to_FirstBack = HashTable_setX[ first ].next + HashTable_setX[ first ].previous;
						if( ShiftOf_FirstFront_to_FirstBack > AllowedRightShiftNum ){
							NeedReHash = true; result = false; return;
						}

						UINT64 ShiftLimit = AllowedRightShiftNum;

						// これの確認は，keyがチェーンされていると，無限幅シフト可能なので，必須．
						UINT64 LimitOfTableSize = TrueTableLength_setX - last - 1;

						// (コンパイラにもよるが) for 文中で比較すると計算コストが増えるので，条件を減らす．
						if( ShiftLimit > LimitOfTableSize ){ ShiftLimit = LimitOfTableSize; }

					#ifndef UnLimit_AllowedSumOfShift
					//	UINT64 SumOfShift = last - first;
						UINT64 LeftSumOfShift = AllowedSumOfShift - (last - first); // 許可された合計シフト数の残り
						if( ShiftLimit > LeftSumOfShift ){ ShiftLimit = LeftSumOfShift; }
					#endif

						for( UINT64 i=1; i <= ShiftLimit; i++ ){
							UINT64 empty = last + i;
							if( HashTable_setX[ empty ].previous == 255 ){
								setX_Move_key_value(HashTable_setX[ empty ], HashTable_setX[ first ]);
								
								UINT64 first_front = first - HashTable_setX[ first ].previous;
								UINT64 first__back = first + HashTable_setX[ first ].next;
								printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_FirstBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_FirstFront_to_FirstBack: %llu, HashTable_setX[ first ].next: %llu, HashTable_setX[ first ].previous: %llu.", ShiftOf_FirstFront_to_FirstBack, HashTable_setX[ first ].next, HashTable_setX[ first ].previous );
								HashTable_setX[ first_front ].next     = (unsigned char)ShiftOf_FirstFront_to_FirstBack;	//     next接続を繋ぎ替え ( 前からある要素 )
								HashTable_setX[ first__back ].previous = (unsigned char)ShiftOf_FirstFront_to_FirstBack;	// previous接続を繋ぎ替え ( 前からある要素 )
								
								printf_dbger_if_Exit_Stop( i > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
								HashTable_setX[ last  ].next     = (unsigned char)i;													//     next接続を繋ぎ替え ( 前からある要素 )
								HashTable_setX[ empty ].previous = (unsigned char)i;													// previous接続を設定     (   新しい要素   )
							//	HashTable_setX[ empty ].next     = 0;													//     next接続を設定     (   新しい要素   )
								//[empty]であれば，nextは既に0に初期化されているはずなので，不要．
								
								printf_dbger_if_Exit_Stop( first >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
							//	printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
								setX_key_value_0_0( HashTable_setX[ first ], key, value );

								NeedReHash = false; result = true; NumOfElementsInTheTable++;	//テーブル上に格納されているkey valueペアの数のカウントを加算．( 失敗している場合は，keyが重複している場合で，要素数は変化しない． )
								return;
							}
						}
						//printf_dbger("NeedReHash: 10\n");
					}
					NeedReHash = true; result = false; return;
				}
			}
		}

	}
	//======================================================================================================================================================================================================================================================
}
void ICHashT::setXwithReHash( char set_function_type, UINT64& key, void*& value, void**& return_value_pp, bool& result, struct HashStruct*& HashTable_setX, UINT64& TableLength_setX, UINT64& TrueTableLength_setX ){

	bool NeedReHash;

ReSet:
	setX( set_function_type, key, value, return_value_pp, result, NeedReHash, HashTable, TableLength, TrueTableLength );
	if( NeedReHash == true ){
		reHash();
		goto ReSet;		//リハッシュした場合は，もう一度key,valueの挿入を試みる必要がある．(do-while()を用いると，比較が一回多くなるので，用いない．)
	}
}
void ICHashT::set0( UINT64& key, void*& value ){
	void** return_value_pp;	//ダミー(実際には使われない．)
	bool   result;			//ダミー(実際には使われない．)
	setXwithReHash( '0', key, value, return_value_pp, result, HashTable, TableLength, TrueTableLength );
}
void ICHashT::set1( UINT64& key, void*& value ){
	void** return_value_pp;	//ダミー(実際には使われない．)
	bool   result;			//ダミー(実際には使われない．)
	setXwithReHash( '1', key, value, return_value_pp, result, HashTable, TableLength, TrueTableLength );
}
void ICHashT::set2( UINT64& key, void*& value, void**& return_value_pp, bool& result ){
	setXwithReHash( '2', key, value, return_value_pp, result, HashTable, TableLength, TrueTableLength );
}
//=====================================================================================================================================================
#ifdef ICHashT_DebugMode
	// デバッグ用の設定
void ICHashT::setXwithNoReHash( UINT64& key, void*& value, void**& return_value_pp, bool& result, bool& NeedReHash ){
	char set_function_type = '2';
	setX( set_function_type, key, value, return_value_pp, result, NeedReHash, HashTable, TableLength, TrueTableLength );
}
void ICHashT::set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ){ //指定した配列番号に，任意のデータをセットする．
	HashTable[ TableElementNum ].key      = key;		// データの書き込み処理
	HashTable[ TableElementNum ].value    = value;		// データの書き込み処理
	HashTable[ TableElementNum ].previous = previous;	// データの書き込み処理
	HashTable[ TableElementNum ].next     = next;		// データの書き込み処理
}
#endif//ICHashT_DebugMode
//=====================================================================================================================================================


void ICHashT::getX( char set_function_type, UINT64& key, void*& value, void**& pp_value, bool& result ){//keyおよびvalueが共に0を取る場合に，値の有無を確認できないので，[bool& result]で[true]or[false]を返す．
	UINT64 first;				//チェーンの先頭，の意味を込めて．
	makeHash(key,first,TableLength);	//ハッシュ値の生成
	UINT64 PreviousShift = HashTable[first].previous;
	if( PreviousShift != 0 ){
		//NotFound(該当データ無し)
		result = false; return;
	}

	{
		UINT64 NextShift;
		UINT64 TagetPlace = first;

		do{
			if( HashTable[ TagetPlace ].key == key ){//keyの一致を確認
				//keyが一致．
				if(set_function_type=='0'){
					value     =  HashTable[ TagetPlace ].value;
				}else{//if(set_function_type=='1'){
					pp_value  = &HashTable[ TagetPlace ].value;
				}
				result = true; return;
			}
			NextShift = HashTable[ TagetPlace ].next;
			TagetPlace += NextShift;				//次の要素へ移動する量を計算．
		}while( NextShift != 0 );					//チェーンの末尾に到達したかどうか．
		//チェーンの末尾に到達．

		result = false; return;//最後まで探しても無かったので，[false]
	}
}
void ICHashT::get0( UINT64& key, void* & value, bool& result ){
	void** pp_value;	//ダミー(実際には使われない．)
	getX( '0', key, value, pp_value, result );
}
void ICHashT::get1( UINT64& key, void**& pp_value, bool& result ){
	void* value;		//ダミー(実際には使われない．)
	getX( '1', key, value, pp_value, result );
}


//find関数はget関数と全く同じ動作だが，valueを返さない．
void ICHashT::find( UINT64& key, bool& result ){
	UINT64 TagetPlace;				//チェーンの先頭，の意味を込めて．
	makeHash(key,TagetPlace,TableLength);	//ハッシュ値の生成
	UINT64 PreviousShift = HashTable[TagetPlace].previous;
	if( PreviousShift != 0 ){
		//NotFound(該当データ無し)
		result = false; return;
	}

	{
		UINT64 NextShift;

		do{
			if( HashTable[ TagetPlace ].key == key ){//keyの一致を確認
				//keyが一致．
//				value  = HashTable[ TagetPlace ].value;
				result = true; return;
			}
			NextShift = HashTable[ TagetPlace ].next;
			TagetPlace += NextShift;				//次の要素へ移動する量を計算．
		}while( NextShift != 0 );					//チェーンの末尾に到達したかどうか．
		//チェーンの末尾に到達．

		result = false; return;//最後まで探しても無かったので，[false]
	}
}


void ICHashT::eraseX( char set_function_type, UINT64& key, bool& result, void*& return_value_p ){
	//要素がチェーンされていない場合(要素数1)，あるいは，チェーンの一番後ろの要素を削除する場合．  →  そのまま削除するだけ．
	//チェーンの一番後ろ以外の要素を削除する場合．  →  一番後ろの要素を，本来削除したかった要素へ上書きする．その上で，一番後ろの要素を，削除する．

	//テーブル自体の整理整頓はsetX()関数に一任することにするので，erase()関数においては，これ以上の処理の実装は行わない．(root値が設定されている場合を除く．root_solver();)

	UINT64 first;				//チェーンの先頭head
	UINT64  last;				//チェーンの末尾tail
	
	UINT64 delete_object;		//削除対象
	
//	unsigned char ChainLength = 1;		//チェーンの長さ


	//keyの検索
	makeHash(key,first,TableLength);//ハッシュ値の生成


	//keyが絶対に存在しない場合は [return false;]
	if( HashTable[first].previous!=0 ){
		// [previous==255] :: 配列が空(no data)
		// [previous!=  0] :: データはあるが，チェーンの先頭ではない．
		// [previous==  0] :: チェーンの先頭．
		result = false; return;
	}
	// HashTable[first] が，長さ1以上のチェーンの先頭である事を確認．


	//削除対象の要素までチェーンの検索を進める．
	delete_object = first;	//初期化
	while( HashTable[delete_object].key != key ){ //keyが一致するまでループを回し，削除対象を検索する．
		UINT64 NextShift = HashTable[delete_object].next;
		if( NextShift == 0 ){
			result = false; return;//最後まで一致するkeyが存在しなかったので， [return false;]
		}
//		ChainLength++;
		delete_object += NextShift;
	}
	

	// [チェーンの長さの計測] ，及び，[チェーンの最後] と [チェーンの最後から二番目] の位置を確認．
	last = delete_object;	//初期化
	while(1){ //チェーンの最後までループを回す．
		UINT64 NextShift = HashTable[last].next;
		if( NextShift == 0 ){
			break;
		}
//		ChainLength++;
		last += NextShift;
	}
	//ここまでで，処理に必要なデータ収集終了．下準備が完了．


	HashStruct HashStruct_for_init;
	HashStruct_for_init.key      = 0;
	HashStruct_for_init.value    = 0;
	HashStruct_for_init.previous = 255;	//[root==255] 配列が空であることを示す．
	HashStruct_for_init.next     = 0;


	/*
	if( last == first ){ //削除対象が配列の一番最後かどうかの確認．
		//要素数1の場合．(配列の先頭が，配列の最後)
		//[処理番号①]
		HashTable[last] = HashStruct_for_init;

		NumOfElementsInTheTable--;	//テーブル上に格納されているkey valueペアの数のカウントを減算．
	//	SetLoadFactor();		//使わない値を毎回計算させるのも……．
		result = true; return;
	}


	//以下，要素数2以上の場合．

	if( last == delete_object ){ //削除対象が配列の一番最後かどうかの確認．
		//削除対象が配列の一番最後．(かつ，少なくとも要素数2以上)
		UINT64 NextShift     = HashTable[last].next;
		UINT64 PreviousShift = HashTable[last].previous;

		//[処理番号②]
		HashTable[last-PreviousShift].next = 0;		//next接続を切断
		HashTable[last] = HashStruct_for_init;		//要素を初期化

		NumOfElementsInTheTable--;	//テーブル上に格納されているkey valueペアの数のカウントを減算．
	//	SetLoadFactor();		//使わない値を毎回計算させるのも……．
		result = true; return;
	}
	*/

	
	//上記の，[処理番号①]，及び，[処理番号②] の処理も，ここの処理を行えば，同様に処理されてしまう．
	//従って，上記はコメントアウト．(if文と(場合によっては)無駄なメモリコピーと，総合的にどちらが速いのか……．)
	{
		UINT64 PreviousShift = HashTable[last].previous;

		// [処理番号③]，及び，[処理番号④]
		HashTable[last-PreviousShift].next = 0;					//next接続を切断

		HashTable[delete_object].key   = HashTable[last].key;	//データの移動処理
		if( set_function_type == '0' ){
			// set_function_type == '0'
			// 何もしない．
		}else{
			// set_function_type == '1'
			return_value_p = HashTable[delete_object].value;	//メモリの解放用．あるいは，削除と同時に削除先に動的に保存されているデータの取得を行う．
		}
		HashTable[delete_object].value = HashTable[last].value;	//データの移動処理

		HashTable[last] = HashStruct_for_init;		//要素を初期化

		NumOfElementsInTheTable--;	//テーブル上に格納されているkey valueペアの数のカウントを減算．
	//	SetLoadFactor();		//使わない値を毎回計算させるのも……．
		result = true; return;
	}
}
void ICHashT::erase0( UINT64& key, bool& result ){
	void* return_value_p;	//ダミー(実際には使われない．)
	eraseX( '0', key, result, return_value_p );
}
void ICHashT::erase1( UINT64& key, bool& result, void*& return_value_p ){
	eraseX( '1', key, result, return_value_p );
}


void ICHashT::reHash(){

#ifdef ICHashT_DebugMode
	// デバッグ用の設定
//	printf( "■■■reHash■■■\n");	//for debug
//	printf( "LoadFactor: %lf\n", GetLoadFactor() );
//	printf( "\n");
//	SysPauseForWin32();
#endif//ICHashT_DebugMode

ReReHash:

	PrimeList_Num++;																//一つ上のテーブルサイズを指定する．
//	if(PrimeList_Num>63){return false;}
	UINT64 TableLength_new     = SmallestPrimeList_LargerThanPower2[PrimeList_Num];	//見かけ上のテーブルサイズ
	                                               AllowedRightShiftNum = GetHyperParameters( PrimeList_Num );	//右シフトを許可する最大値の指定．(0～254まで．)
	UINT64 TrueTableLength_new = TableLength_new + AllowedRightShiftNum;

	struct HashStruct* HashTable_new;
	mallocTable(HashTable_new,TrueTableLength_new);	//新しいテーブルの確保，及び初期化

	NumOfElementsInTheTable = 0; // 下記の入れ替えで加算されてしまうので，初期化する．( 古いバージョンでは， setX(); 内で加算をしていなかったため，リハッシュ時の NumOfElementsInTheTable の加算コストはなかったが，かわりに，関数の外で加算するかどうかの比較コストが発生していた． )

	//全要素について，ハッシュ値を再計算し，新しいテーブルにコピーし，配置し直す．

	//全要素走査
	AllScanPlace NowPlace;
	for(AllScanInit(NowPlace); NowPlace < AllScanEnd; AllScanNextShift(NowPlace)){
		UINT64 key;
		void* value;
		AllScanGetData(key,value,NowPlace);
//			printf("key::%lld\n"           ,            key );
//			printf("value(Address)::%llX\n",          value );

		//要素を新しい配列へ書き込み
		void** return_value_pp;	//ダミー(実際には使われない．)
		bool   result;
		bool   NeedReHash;
		char   set_function_type = '1';
		setX( set_function_type, key, value, return_value_pp, result, NeedReHash, HashTable_new, TableLength_new, TrueTableLength_new );		//setXwithReHash()関数を使うと，再帰的な処理が発生するので，決して使わない事．
		if( NeedReHash == true ){
			//もしもハッシュ値とkeyが重複し，seX()がfalseを返すような場合があれば，無限にリハッシュしようとするので，注意．

			//setX()が更なるリハッシュを要求してきた場合
			//→現在新しく確保したテーブルへのコピーを中止し，さらに倍サイズのテーブルへのコピーへと操作を変更する．

			free( HashTable_new );			//テーブルの解放
			goto ReReHash;					//gotoで書いた方が，比較が一回少ないので．do{}while(result==false);はやめる．
		}
	}

	freeTable(HashTable);					//古いテーブルの解放
	HashTable       = HashTable_new;		//入れ替え
	TableLength     = TableLength_new;		//入れ替え
	TrueTableLength = TrueTableLength_new;	//入れ替え
}


UINT64 ICHashT::GetNumOfElementsInTheTable(){ return this->NumOfElementsInTheTable; }

//テーブル占有率の計算
void ICHashT::SetLoadFactor(){
	this->LoadFactor = (double)((double)NumOfElementsInTheTable / (double)TrueTableLength);
//	printf_dbger("LoadFactor: %lf, NumOfElementsInTheTable: %llu, TrueTableLength: %llu\n", LoadFactor, NumOfElementsInTheTable, TrueTableLength);
}
double ICHashT::GetLoadFactor(){
	SetLoadFactor();
	return LoadFactor;
}

// 外部からの参照用
UINT64 ICHashT::GetTableLength(){ return this->TableLength; }
UINT64 ICHashT::GetTrueTableLength(){ return this->TrueTableLength; }


void ICHashT::reHash_to_fit(){

	//座席占有率(充填率)の計算．
//	SetLoadFactor();

	//理想とするテーブルサイズを計算．
		//    [理想テーブル占有率] == [実際にテーブル上に存在する key value ペアの要素数] / [理想テーブル長]
		// ⇔   IdealLoadFactor    ==               NumOfElementsInTheTable               / IdealTableLength
		// ⇔   IdealTableLength   ==               NumOfElementsInTheTable               / IdealLoadFactor
		//より，
	double IdealLoadFactor = 0.5;	//理想テーブル占有率 ( 取りあえず50%に設定しておく． )
	UINT64 IdealTableLength = (UINT64)( ((double)NumOfElementsInTheTable) / IdealLoadFactor );

	//[理想テーブル占有率]が50%以下となり，かつ，最も50%に近いテーブルサイズを，素数表の中から選ぶ．
	unsigned char PrimeList_Num_New = 0;															//reHash()でアクセスされるので，必ず初期化しておく．
	for(; PrimeList_Num_New<64; PrimeList_Num_New++){ //32bit対応するのであれば，注意した方が良いのかな?(素数表も32bitならサイズを小さくした方が良い．)
		if( SmallestPrimeList_LargerThanPower2[PrimeList_Num_New] >= IdealTableLength ){
			break;		//for文を抜ける．
		}
	}

	if( PrimeList_Num_New == PrimeList_Num ){
		//現状のテーブルサイズは理想的なサイズなため，reHash();関数を実行する必要は無い．従って，リハッシュせずにreturn．
		return;
	}else{
		PrimeList_Num = PrimeList_Num_New;	//理想とするテーブルサイズを指定する．
	}

	ReconstrucTheTable();	//強制リハッシュ
}


void ICHashT::Reconstruction_to_fit(){

	//座席利用率の計算．
//	SetLoadFactor();

	//理想とするテーブルサイズを計算．
		//    [理想座席利用率] == [実際にテーブル上に存在する key value ペアの要素数] / [理想テーブル長]
		// ⇔ IdealLoadFactor  ==               NumOfElementsInTheTable               / IdealTableLength
		// ⇔ IdealTableLength ==               NumOfElementsInTheTable               / IdealLoadFactor
		//より，
	double IdealLoadFactor = 0.5;	//理想座席利用率 ( 取りあえず50%に設定しておく． )
	UINT64 IdealTableLength = (UINT64)( ((double)NumOfElementsInTheTable) / IdealLoadFactor );

	//[理想座席利用率]が50%以下となり，かつ，最も50%に近いテーブルサイズを，素数表の中から選ぶ．
	unsigned char PrimeList_Num_New = 0;															//reHash()でアクセスされるので，必ず初期化しておく．
	for(; PrimeList_Num_New<65; PrimeList_Num_New++){ //32bit対応するのであれば，注意した方が良いのかな?(素数表も32bitならサイズを小さくした方が良い．)
		if( SmallestPrimeList_LargerThanPower2[PrimeList_Num_New] >= IdealTableLength ){
			break;		//for文を抜ける．
		}
	}
	if(PrimeList_Num_New>=65){
		printf_dbger("ERROR: Because of the hash table length will be over the range of 64 bits address, no operation will be done.\n");
		return; // 何もせずに return
	}

	PrimeList_Num = PrimeList_Num_New;	//理想とするテーブルサイズを指定する．

	ReconstrucTheTable();	//強制リハッシュ
}


void ICHashT::ReconstrucTheTable(){
	PrimeList_Num--;	//reHash();関数は，一つ上のテーブルサイズを指定しようとするので，先に一つ引いておく．
	reHash();			//リハッシュ
}


unsigned char GetHyperParameters( unsigned char& PrimeList_Num ){
	if( PrimeList_Num < sizeof(HyperParameters) ){
		return HyperParameters[PrimeList_Num];
	}else{
		return 254;
	}
}

//=====================================================================================================================================================
#ifdef ICHashT_DebugMode
	// デバッグ用の設定

//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	//全要素出力・走査→イテレータを用いて実装しようと思ったが，C++11的な，最近の技術で，信用ならないので使わない事にした．
void ICHashT::DumpAll_for_Debug_Init( DumpAll_Place& DumpAll_NowPlace ){
#define DumpAll_Start 0x0000000000000000
	DumpAll_NowPlace  = (DumpAll_Place)DumpAll_Start;		//カーソルを初期位置にセット．
	this->DumpAll_End = (DumpAll_Place)TrueTableLength;
#undef AllScanStart
}
void ICHashT::DumpAll_for_Debug_GetData( UINT64& key, void*& value, unsigned char& previous, unsigned char& next, DumpAll_Place& DumpAll_NowPlace ){
	key      = HashTable[DumpAll_NowPlace].key;
	value    = HashTable[DumpAll_NowPlace].value;
	previous = HashTable[DumpAll_NowPlace].previous;
	next     = HashTable[DumpAll_NowPlace].next;
}
std::string ICHashT::DumpWithRange( UINT64& start, UINT64& end ){

	std::string StrBuf;

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	DumpAll_Place DumpAll_NowPlace = start;
	this->DumpAll_End              = end + 1;
	for(; DumpAll_NowPlace < this->DumpAll_End; DumpAll_NowPlace++ ){
//	for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End; DumpAll_NowPlace++){
//	for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End && DumpAll_NowPlace<10; DumpAll_NowPlace++){//全て表示させると邪魔なので．
		UINT64 key;
		void* value3;
		unsigned char previous;
		unsigned char next;
		this->DumpAll_for_Debug_GetData( key, value3, previous, next, DumpAll_NowPlace );
		StrBuf += str_sprintf( "配列番号:%lld\n",        DumpAll_NowPlace );
		StrBuf += str_sprintf( "           key::%llu\n", key              );
		StrBuf += str_sprintf( "value(Address)::%llX\n", (UINT64)value3   );
		StrBuf += str_sprintf( "      previous::%u\n",   previous         );
		StrBuf += str_sprintf( "          next::%u\n",   next             );
//		StrBuf += str_sprintf( "value(Entity )::%c\n",   *((char*)value3) );
		StrBuf += str_sprintf( "\n" );
	}

	return StrBuf;
}
void ICHashT::DumpALL(){
	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	printf("■■■ Dump All ( Start ) ■■■\n");
	{
		DumpAll_Place DumpAll_NowPlace;
		for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End; DumpAll_NowPlace++){
//		for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End && DumpAll_NowPlace<10; DumpAll_NowPlace++){//全て表示させると邪魔なので．
			UINT64 key;
			void* value3;
			unsigned char previous;
			unsigned char next;
			this->DumpAll_for_Debug_GetData(key,value3,previous,next,DumpAll_NowPlace);
			printf("配列番号:%lld\n"       , DumpAll_NowPlace );
			printf("           key::%llu\n",              key );
			printf("value(Address)::%llX\n",   (UINT64)value3 );
			printf("      previous::%u\n"  ,         previous );
			printf("          next::%u\n"  ,             next );
//			printf("value(Entity )::%c\n"  ,  *((char*)value3));
			printf("\n");
		}
	}
	printf("■■■ Dump All (  End  ) ■■■\n");
}
void ICHashT::DumpAllToTheText( const char* SaveDirctory){

	class FileWriter* FW =  new FileWriter(SaveDirctory);	//出力先のファイル名を指定

	std::string StrBuf;

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	DumpAll_Place DumpAll_NowPlace;
	for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End; DumpAll_NowPlace++){
//	for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End && DumpAll_NowPlace<10; DumpAll_NowPlace++){//全て表示させると邪魔なので．
		UINT64 key;
		void* value3;
		unsigned char previous;
		unsigned char next;
		this->DumpAll_for_Debug_GetData(key,value3,previous,next,DumpAll_NowPlace);
		StrBuf  = str_sprintf("ArrayNum:%llu\n", DumpAll_NowPlace );
		StrBuf += str_sprintf("           key::%llu\n",              key );
		StrBuf += str_sprintf("value(Address)::%llX\n",   (UINT64)value3 );
		StrBuf += str_sprintf("      previous::%u\n"  ,         previous );
		StrBuf += str_sprintf("          next::%u\n"  ,             next );
	//	StrBuf += str_sprintf("value(Entity )::%c\n"  ,  *((char*)value3));
		StrBuf += "\n";
		FW->FWrite(StrBuf.c_str());	//出力
	}

	delete FW;
}


#ifndef UnLimit_AllowedSumOfShift
UINT64 ICHashT::GetAllowedSumOfShift(){ return this->AllowedSumOfShift; }
#endif
unsigned char ICHashT::GetAllowedRightShiftNum(){ return this->AllowedRightShiftNum; }


#endif//ICHashT_DebugMode
//=====================================================================================================================================================

//keyに文字列など様々な型を使いたいなら，まずは比較部分を切り離した方がいいかな??

//bool CompareKEY( TableNum, KeyType key ){
//	if( HashTable[TableNum] = key ){
//		return true;
//	}
//	return false;
//}

