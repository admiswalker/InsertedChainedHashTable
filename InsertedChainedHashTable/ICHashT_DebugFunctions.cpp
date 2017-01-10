/* -*- coding: utf-8 -*- */
// このファイルでは，デバッグ初期に用いたデバッグ関数の使用方法を示す．

#include "ICHashT_DebugFunctions.hpp"
#ifdef ReturnAssignedHash_for_debug

//============================================================================================================================================================================================

//ここから，デバック関数の基本操作に関する試験を開始．


void ICHashT_DebugFunction_ConfirmBasicOperation(){

	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================
	
	//特に意味の無い，基本動作の確認．
	key=1;
	value1 = (void*)0x0000000000000001;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);
	
	key=2;
	value1 = (void*)0x0000000000000002;
	ICHashTable.return_assigned_Hash = 2;
	ICHashTable.set1(key,value1);
	
	key=3;
	value1 = (void*)0x0000000000000003;
	ICHashTable.return_assigned_Hash = 4;
	ICHashTable.set1(key,value1);
	
	key=4;
	value1 = (void*)0x0000000000000004;
	ICHashTable.return_assigned_Hash = 6;
	ICHashTable.set1(key,value1);

	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

//ここまでで，デバック関数の基本操作に関する試験を終了．
//============================================================================================================================================================================================

//ここから，要素挿入に関する試験を開始．


void ICHashT_DebugFunction_ConfirmSetProcessingNum1(){

	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================
	
	// set();関数が正常に作動しているか確かめていく．
	// [処理番号①]の確認
	
	key=1;
	value1 = (void*)0x0000000000000001;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum2(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// [処理番号②]の確認

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 3);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
	ICHashTable.set_for_Debug( 6, 6, (void*)6, 3, 1);
	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	key=99;
	value1 = (void*)0x0000000000000099;
	ICHashTable.return_assigned_Hash = 1;	// 格納先の座標:	// [処理番号②]
	ICHashTable.set1(key,value1);			//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum3(){

	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// [処理番号③]の確認．
	// keyの重複なし．
	key=1;
	value1 = (void*)0x0000000000000001;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);
	
	key=2;
	value1 = (void*)0x0000000000000002;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);
	
	key=3;
	value1 = (void*)0x0000000000000003;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);
	
	key=4;
	value1 = (void*)0x0000000000000004;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum3_2(){

	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// [処理番号③]の確認．
	// keyの重複あり．
	key=1;
	value1 = (void*)0x0000000000000001;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);
	
	key=2;
	value1 = (void*)0x0000000000000002;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);
	
	key=3;
	value1 = (void*)0x0000000000000003;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);
	
	key=4;
	value1 = (void*)0x0000000000000004;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);

	key=3;
	value1 = (void*)0x0000000000000005;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set1(key,value1);							//この挿入操作が正常に行われている事を確認したい．

	key=4;
	value1 = (void*)0x0000000000000006;
	ICHashTable.return_assigned_Hash = 0;
	ICHashTable.set0(key,value1);			// 注意!!! set0();							//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum4(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 3);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
	ICHashTable.set_for_Debug( 6, 6, (void*)6, 3, 0);
//	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 0);

	key=99;
	value1 = (void*)0x0000000000000099;
	ICHashTable.return_assigned_Hash = 6;	// 格納先の座標:	// [処理番号④]
	ICHashTable.set1(key,value1);			//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum4_2(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 1);
	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 2);
//	ICHashTable.set_for_Debug( 6, 6, (void*)6, 1, 1);
	ICHashTable.set_for_Debug( 7, 7, (void*)7, 2, 0);

	key=99;
	value1 = (void*)0x0000000000000099;
	ICHashTable.return_assigned_Hash = 7;	// 格納先の座標:	// [処理番号④]
	ICHashTable.set1(key,value1);			//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum5and6(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// [処理番号⑤]，[処理番号⑥]の確認．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 3);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
	ICHashTable.set_for_Debug( 6, 6, (void*)6, 3, 1);
	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 0);

	key=99;
	value1 = (void*)0x0000000000000099;
	ICHashTable.return_assigned_Hash = 7;	// 格納先の座標:	// [処理番号⑥]
//	ICHashTable.return_assigned_Hash = 6;	// 格納先の座標:	// [処理番号⑤]
	ICHashTable.set1(key,value1);							//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum7(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 3);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
	ICHashTable.set_for_Debug( 6, 6, (void*)6, 3, 1);
	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	key=99;
	value1 = (void*)0x0000000000000099;
	ICHashTable.return_assigned_Hash = 8;	// 格納先の座標:	// [処理番号⑦]
	ICHashTable.set1(key,value1);			//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum8(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 3);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
	ICHashTable.set_for_Debug( 6, 6, (void*)6, 3, 1);
	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	key=99;
	value1 = (void*)0x0000000000000099;
	ICHashTable.return_assigned_Hash = 2;	// 格納先の座標:	// [処理番号⑧]
	ICHashTable.set1(key,value1);			//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum9(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	//その①
	// set();関数が正常に作動しているか確かめていく．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 3);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
	ICHashTable.set_for_Debug( 6, 6, (void*)6, 3, 1);
	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	key=99;
	value1 = (void*)0x0000000000000099;
	ICHashTable.return_assigned_Hash = 3;	// 格納先の座標:	// [処理番号⑨]
	ICHashTable.set1(key,value1);							//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum9_2(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	//その②
	// set();関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 2);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
	ICHashTable.set_for_Debug( 5, 5, (void*)5, 2, 1);
	ICHashTable.set_for_Debug( 6, 6, (void*)6, 1, 1);
	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	key=99;
	value1 = (void*)0x0000000000000099;
	ICHashTable.return_assigned_Hash = 3;	// 格納先の座標:	// [処理番号⑨]
	ICHashTable.set1(key,value1);			//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum10(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// [処理番号⑩]の確認．
	// keyの重複あり．
	key=1;
	value1 = (void*)0x0000000000000001;
	ICHashTable.return_assigned_Hash = 0;	// 格納先の座標:0
	ICHashTable.set1(key,value1);
	
	key=2;
	value1 = (void*)0x0000000000000002;
	ICHashTable.return_assigned_Hash = 0;	// 格納先の座標:1
	ICHashTable.set1(key,value1);
	
	key=3;
	value1 = (void*)0x0000000000000003;
	ICHashTable.return_assigned_Hash = 0;	// 格納先の座標:2
	ICHashTable.set1(key,value1);
	
	key=4;
	value1 = (void*)0x0000000000000004;
	ICHashTable.return_assigned_Hash = 0;	// 格納先の座標:3
	ICHashTable.set1(key,value1);

	key=5;
	value1 = (void*)0x0000000000000005;
	ICHashTable.return_assigned_Hash = 1;	// 格納先の座標:1 // 格納先の座標:2 // ただし，格納先の座標:3 の場合は，[処理番号⑪]となるので，処理内容が異なる．
	ICHashTable.set1(key,value1);			//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmSetProcessingNum11(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// set();関数が正常に作動しているか確かめていく．
	// [処理番号⑪]の確認．
	// keyの重複あり．
	key=1;
	value1 = (void*)0x0000000000000001;
	ICHashTable.return_assigned_Hash = 0;	// 格納先の座標:0
	ICHashTable.set1(key,value1);
	
	key=2;
	value1 = (void*)0x0000000000000002;
	ICHashTable.return_assigned_Hash = 0;	// 格納先の座標:1
	ICHashTable.set1(key,value1);
	
	key=3;
	value1 = (void*)0x0000000000000003;
	ICHashTable.return_assigned_Hash = 0;	// 格納先の座標:2
	ICHashTable.set1(key,value1);
	
	key=4;
	value1 = (void*)0x0000000000000004;
	ICHashTable.return_assigned_Hash = 0;	// 格納先の座標:3
	ICHashTable.set1(key,value1);

	key=5;
	value1 = (void*)0x0000000000000005;
	ICHashTable.return_assigned_Hash = 3;	// 格納先の座標:3
	ICHashTable.set1(key,value1);			//この挿入操作が正常に行われている事を確認したい．

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

//ここまでで，要素挿入に関する試験を終了．
//============================================================================================================================================================================================

//ここから，要素削除に関する試験を開始．


void ICHashT_DebugFunction_ConfirmEraseProcessingNum1(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// [処理番号①]の確認．
	// erase();関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 0);
//	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
//	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 1);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
//	ICHashTable.set_for_Debug( 6, 6, (void*)6, 1, 1);
//	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
//	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
//	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	printf("■■■Check erase(); function.■■■\n");
	bool result;
	key = 1;
	ICHashTable.return_assigned_Hash = 1;
	ICHashTable.erase0(key,result);
	printf("%s\n", result ? "true" : "false");
	printf("\n");

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmEraseProcessingNum2(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// [処理番号②]の確認．
	// erase関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 0);
//	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 1);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
//	ICHashTable.set_for_Debug( 6, 6, (void*)6, 1, 1);
//	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
//	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
//	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	printf("■■■Check erase(); function.■■■\n");
	bool result;
	key = 2;
	ICHashTable.return_assigned_Hash = 1;
	ICHashTable.erase0(key,result);
	printf("%s\n", result ? "true" : "false");
	printf("\n");

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmEraseProcessingNum2_2(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// [処理番号②]の確認．その2
	// erase関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 0);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
//	ICHashTable.set_for_Debug( 6, 6, (void*)6, 1, 1);
//	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
//	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
//	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	printf("■■■Check erase(); function.■■■\n");
	bool result;
	key = 3;
	ICHashTable.return_assigned_Hash = 1;
	ICHashTable.erase0(key,result);
	printf("%s\n", result ? "true" : "false");
	printf("\n");

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmEraseProcessingNum3(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// [処理番号③]の確認．
	// erase関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 0);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
//	ICHashTable.set_for_Debug( 6, 6, (void*)6, 1, 1);
//	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
//	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
//	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	printf("■■■Check erase(); function.■■■\n");
	bool result;
	key = 1;
	ICHashTable.return_assigned_Hash = 1;
	ICHashTable.erase0(key,result);
	printf("%s\n", result ? "true" : "false");
	printf("\n");

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmEraseProcessingNum3_2(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// [処理番号③]の確認．その2
	// erase関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 0);
//	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 1);
//	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
//	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
//	ICHashTable.set_for_Debug( 6, 6, (void*)6, 1, 1);
//	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
//	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
//	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	printf("■■■Check erase(); function.■■■\n");
	bool result;
	key = 1;
	ICHashTable.return_assigned_Hash = 1;
	ICHashTable.erase0(key,result);
	printf("%s\n", result ? "true" : "false");
	printf("\n");

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

void ICHashT_DebugFunction_ConfirmEraseProcessingNum4(){
	
	//=============================================
	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// 挿入する key の用意
	UINT64 key;
	void* value1;
	//=============================================

	// [処理番号④]の確認．
	// erase関数が正常に作動しているか確かめていく．
	// keyの重複あり．

	//void set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //指定した配列番号に，任意のデータをセットする．

//	ICHashTable.set_for_Debug( 0, 0, (void*)0, 0, 1);
	ICHashTable.set_for_Debug( 1, 1, (void*)1, 0, 1);
	ICHashTable.set_for_Debug( 2, 2, (void*)2, 1, 1);
	ICHashTable.set_for_Debug( 3, 3, (void*)3, 1, 1);
	ICHashTable.set_for_Debug( 4, 4, (void*)4, 1, 1);
	ICHashTable.set_for_Debug( 5, 5, (void*)5, 1, 1);
	ICHashTable.set_for_Debug( 6, 6, (void*)6, 1, 1);
	ICHashTable.set_for_Debug( 7, 7, (void*)7, 1, 1);
	ICHashTable.set_for_Debug( 8, 8, (void*)8, 1, 1);
	ICHashTable.set_for_Debug( 9, 9, (void*)9, 1, 0);

	printf("■■■Check erase(); function.■■■\n");
	bool result;
	key = 3;
	ICHashTable.return_assigned_Hash = 1;
	ICHashTable.erase0(key,result);
	printf("%s\n", result ? "true" : "false");
	printf("\n");

	//ハッシュテーブル上で空の要素も含めた全ての要素のダンプ
	UINT64 start=0, end=10;
	printf("■■■ DumpWithRange ( Start ) ■■■\n");
	printf((ICHashTable.DumpWithRange( start, end )).c_str());
	printf("■■■ DumpWithRange (  End  ) ■■■\n");
}

//ここまでで，要素削除に関する試験を終了．
//============================================================================================================================================================================================
#endif
