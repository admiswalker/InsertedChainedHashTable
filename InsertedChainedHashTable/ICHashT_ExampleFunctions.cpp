/* -*- coding: utf-8 -*- */
#include "ICHashT_ExampleFunctions.hpp"


// このファイルでは，Inserted Chained Hash Table の基本的な使い方を説明します．

// 主に set0, set1, set2 関数に関して説明する
void Example1(){

	// 初期化 (静的確保) (ハッシュテーブルは，スコープを外れるまで保持される)
	UINT64 TableLength = 1024;
//	ICHashT ICHashTable;				//初期化する．( 何も指定せずに初期化すると，テーブルサイズ 約1024 で初期化される． )
	ICHashT ICHashTable(TableLength);	//テーブルサイズを指定して初期化する．

	// ハッシュテーブルの現在サイズを取得
	printf( "    TableLength :: %llu\n", ICHashTable.GetTableLength()     );	// 実際に確保されているメモリ長から，シフト幅分を引いた長さ．
	printf( "TrueTableLength :: %llu\n", ICHashTable.GetTrueTableLength() );	// 実際に確保されているメモリ長
	printf( "LoadFactor: %lf\n", ICHashTable.GetLoadFactor() );					// Load Factor (座席占有率) の取得
	printf("\n");

	// 挿入する key の用意
	UINT64 key0 = 6519;		// この値は UINT64 の範囲内の整数であること以外に意味はありません	// [UINT64]を満たす値をkeyとして使用可能
	UINT64 key1 = 16519;	// この値に UINT64 の範囲内の整数であること以外に意味はありません	// [UINT64]を満たす値をkeyとして使用可能
	
	void** return_value_pp;
	bool   result;
	
	//===========================================================================================================
	// set0(), set1(), set2() 関数により, key, value ペアをハッシュテーブルに挿入する

	char set_value0_char[] = "set0";
	char set_value0_char_2[] = "set0(); 関数は，既に key が格納されている場合，value を書き換える．";
	void* set_value0 = set_value0_char;
	void* set_value0_2 = set_value0_char_2;

	char set_value1_char[] = "set1";
	char set_value1_char_2[] = "set1(); 関数は，既に key が格納されている場合，挿入しない．(Value を書き換えない)";
	void* set_value1 = set_value0_char;
	void* set_value1_2 = set_value1_char_2;
	
	char set_value2_char[] = "set2_キーが既に登録されている場合は，valueを書き換える．";
	void*  set_value2 = set_value2_char;
	
	printf("■■■ Check set0(); function. ( Start ) ■■■\n");

	ICHashTable.set0( key0, set_value0 );	// set0(); 関数は，既に key が格納されている場合，value を上書きして return; する．
	ICHashTable.set0( key0, set_value0_2 );	// set0(); 関数は，既に key が格納されている場合，value を上書きして return; する．
	
	printf("■■■ Check set0(); function. (  end  ) ■■■\n");
	printf("\n");


	printf("■■■ Check set1(); function. ( Start ) ■■■\n");
	
	ICHashTable.set1( key1, set_value1 );	// set1(); 関数は，まだ key が格納されていない場合，挿入する．
	ICHashTable.set1( key1, set_value1_2 );	// set1(); 関数は，既に key が格納されている場合，挿入しない．(Value を書き換えない)
	
	printf("■■■ Check set1(); function. (  end  ) ■■■\n");
	printf("\n");

	
	printf("■■■ Check set2(); function. ( Start ) ■■■\n");

	ICHashTable.set2( key1, set_value2, return_value_pp, result );	// set2(); 関数は，既に key が格納されている場合，value を取得して result を書き込んで return; (value に手は加えないが，voidポインタなので，中身の判断と書き換えができる)
	if( result==false ){
		printf( "%s\n", (char*)(*((char**)(void**)return_value_pp)) );

		//値が気に入らない場合は，書き換える．
		char set_value2_char_2[] = "set2によりvalueを書き換えてみた場合．";		//値の書き込み先である，[set_value2_char_2]のサイズを越えないように注意．
		*return_value_pp = (void**)&set_value2_char_2;
		printf("%s\n", (char*)*((char**)return_value_pp));
	}

	printf("■■■ Check set2(); function. (  end  ) ■■■\n");
	printf("\n");

	//===========================================================================================================
	// get0, get1 関数により, ハッシュテーブルに挿入済みの key, value ペアを取得する
	
	printf("■■■ Check get0(); function. ( Start ) ■■■\n");

	void* get0_value;
	ICHashTable.get0( key0, get0_value, result );
	printf("%s\n", result ? "true" : "false");
	if(result){
		printf("%s\n", ((char*)get0_value));
	}
	
	printf("■■■ Check get0(); function. (  end  ) ■■■\n");
	printf("\n");

	//===========================================================================================================
	// 全要素走査 ( ハッシュテーブル上に記録されているすべての key value ペアの取り出し． )

	printf("■■■ Check AllScan function. ( Start ) ■■■\n");
	{
		AllScanPlace NowPlace;
		for(ICHashTable.AllScanInit(NowPlace); NowPlace < ICHashTable.AllScanEnd; ICHashTable.AllScanNextShift(NowPlace)){
			UINT64 key;
			void* value;
			ICHashTable.AllScanGetData(key,value,NowPlace);
			printf("           key::%lld\n",            key );
			printf("value(Address)::%llX\n",          value );
			printf("value(Entity )::%s\n"  ,  ((char*)value));
			printf("\n");
		}
	}
	printf("■■■ Check AllScan function. (  end  ) ■■■\n");
	printf("\n");

	//===========================================================================================================
	// erase0, erase1 関数により, ハッシュテーブルに挿入済みの key, value ペアを削除する

	printf("■■■ Check erase0(); function. ( Start ) ■■■\n");

	ICHashTable.erase0( key0, result );
	printf("%s\n", result ? "true" : "false");	// 削除に成功した場合はtrue．
	
	printf("■■■ Check erase0(); function. (  end  ) ■■■\n");
	printf("\n");


	printf("■■■ Check erase0(); function. ( Start ) ■■■\n");
	
	ICHashTable.erase0( key0, result );
	printf("%s\n", result ? "true" : "false");	//削除しようとした要素がテーブル上に存在しない場合はfalseが帰って来る．
	
	printf("■■■ Check erase0(); function. (  end  ) ■■■\n");
	printf("\n");

	//===========================================================================================================
	// ハッシュテーブルに記録されている全てのkey-valueペアを削除する．(本当に消すだけなので，value の void* の先にメモリ確保していた場合は，メモリが解放されず，メモリリークする)

	printf("■■■ Check AllClear(); function. ( Start ) ■■■\n");
	ICHashTable.AllClear();
	printf("■■■ Check AllClear(); function. (  end  ) ■■■\n");
	printf("\n");

	//===========================================================================================================
	// 全要素走査 ( ハッシュテーブル上に記録されているすべての key value ペアの取り出し． )

	printf("■■■ Check AllScan function. ( Start ) ■■■\n");
	{
		AllScanPlace NowPlace;
		for(ICHashTable.AllScanInit(NowPlace); NowPlace < ICHashTable.AllScanEnd; ICHashTable.AllScanNextShift(NowPlace)){
			UINT64 key;
			void* value;
			ICHashTable.AllScanGetData(key,value,NowPlace);
			printf("           key::%lld\n",            key );
			printf("value(Address)::%llX\n",          value );
			printf("value(Entity )::%s\n"  ,  ((char*)value));
			printf("\n");
		}
	}
	printf("■■■ Check AllScan function. (  end  ) ■■■\n");
	printf("\n");
	
	//===========================================================================================================
	
//	ICHashTable.reHash_to_fit();		// テーブルが最適なサイズになるように強制的にリハッシュ．(ただし，現状が最適なサイズであると判断された場合は，何もしない．)//テーブルサイズを縮小させることも可能．(デーブルサイズは，LoadFactorが50%以下かつ最も50%に近いサイズが選択される．)
//	ICHashTable.ReconstrucTheTable();	// 全く同じテーブルサイズで再構成する．(強制的にリハッシュ．)(ただし，同じテーブルサイズでの再構成に失敗した場合は，再構成に成功するまで，テーブルサイズを大きくする．)	//テーブルの要素の大幅な削除や追加を行い，データの断片化が激しい場合を想定．

	//===========================================================================================================
	
	return;
}

// 主に get0, get1, find 関数に関して説明する
void Example2(){

	// 初期化 (動的確保) (ハッシュテーブルは，明示的に delete するまで保持される)
	ICHashT* pICHashT;
	pICHashT = new ICHashT(1024);		//ハッシュテーブルを動的に確保する．

	std::string* str = new std::string;
	*str = "ABCDEF保存テスト";
	printf("%s\n",(*str).c_str());
	
	//要素の挿入操作
	unsigned __int64 key = 1656;
	pICHashT->set1( key, (void*&)str );		//set1():keyが既に格納されている場合は，valueを上書きしてreturn;

	//要素の [get1();関数] による取得操作
	void** pp_value;
	bool result;
	pICHashT->get1( key, pp_value, result );
	printf("%s\n", result ? "true" : "false");
	printf("%s\n",((std::string*)*pp_value)->c_str() );
	
	//要素の [get1();関数] による書き換え操作
//	((std::string)*(std::string*)*pp_value) = "要素の書き換え．";	//←これだと書き換えられないので，注意．(コンパイラの問題?なぜ?)
//	(std::string)(*(std::string*)*pp_value) = "要素の書き換え．";	//←これだと書き換えられないので，注意．(コンパイラの問題?なぜ?)
	*((std::string*)*pp_value) = "要素の書き換え．";
	printf("%s\n",((std::string*)*pp_value)->c_str() );
	
	//要素の [get0();関数] による取得操作
	void* value;
	pICHashT->get0( key, value, result );
	printf("%s\n", result ? "true" : "false");
	printf("%s\n",((std::string*)value)->c_str() );
	
	delete (std::string*)*pp_value;	//メモリの解放

	//要素の [get1();関数] の [] に対する特徴は，要素をまるごと入れ替えられること．
	std::string* str_new = new std::string;
	*str_new = "全く新しく確保した要素．";
	*pp_value = (void**)str_new;

	//要素の [get0();関数] による取得操作
//	void* value;
	pICHashT->get0( key, value, result );
	printf("%s\n", result ? "true" : "false");
	printf("%s\n",((std::string*)value)->c_str() );
	*((std::string*)value) = "要素の書き換え．その2";

	//要素の [get0();関数] による取得操作
	void* value2;
	pICHashT->get0( key, value2, result );
	printf("%s\n", result ? "true" : "false");
	printf("%s\n",((std::string*)value2)->c_str() );

	// どちらの方法でもメモリを解放可能．
	delete (std::string*)value;	//メモリの解放
//	delete (std::string*)*pp_value;	//メモリの解放

	// key の存在を確認
	pICHashT->find( key, result );
	printf("%s\n", result ? "true" : "false");

	delete pICHashT;	// ハッシュテーブルの解放
}

// 主に erase0, erase1 関数に関して説明する
void Example3(){

	ICHashT* pICHashT;
	pICHashT = new ICHashT(1024);		//ハッシュテーブルを動的に確保する．

	std::string* str = new std::string;
	*str = "ABCDEF保存テスト";
	printf("%s\n",(*str).c_str());

	//要素の挿入操作
	unsigned __int64 key = 1656;
	pICHashT->set1( key, (void*&)str );		//set1():keyが既に格納されている場合は，valueを上書きしてreturn;

	//要素の [erase1();関数] による削除操作
	void* value_p;
	bool result;
	pICHashT->erase1( key, result, value_p );
	printf("%s\n", result ? "true" : "false");
	if( result ){
		printf("%s\n",((std::string*)value_p)->c_str() );
	}

	delete (std::string*)value_p;	//メモリの解放
	delete pICHashT;
}
