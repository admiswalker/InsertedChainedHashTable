#include "ICHashT_benchmark.hpp"

void benchmark_ICHashT_MaxLoadFactor_insert( ICHashT*& ICHashT_p, UINT64& TableLength, UINT64& SetNum, UINT64& NumOfElements, UINT64& RandSeed );
void benchmark_ICHashT_MaxLoadFactor_insert( ICHashT*& ICHashT_p, UINT64& TableLength, UINT64& SetNum, UINT64& NumOfElements, UINT64& RandSeed ){

	init_genrand64(RandSeed);

	//挿入処理
	for(UINT64 i=0;; i++){

		void** return_value_pp;	//ダミー(実際には使われない．)
		bool   result;
		bool   NeedReHash;
		//ICHashT_p->setXwithNoReHash( i, (void*&)i, return_value_pp, result, NeedReHash );
		UINT64 key = genrand64_int64();
		ICHashT_p->setXwithNoReHash( key, (void*&)i, return_value_pp, result, NeedReHash );
		if(NeedReHash==true){
			SetNum = i; // NumOfElements = ICHashT_p->NumOfElementsInTheTable;
			break;
		}
		if(result==false){
			printf("ERROR: result==false\n");
			SysPauseForWin32();
			break;
		}
		/*
		//if(TableLength==2053 && i==1675 ){ // after insert 1676, key: 1081 become to be not found.
		if(TableLength==11 && i==18 ){ // after insert 238329, key: 77388 become to be not found.
			std::string SaveDirctory = "./TrueTableLength11_AfterInsert18.txt";
			ICHashT_p->DumpAllToTheText(SaveDirctory.c_str());
		}*/
		/*
		//for debug
		UINT64 key=5;
		if(TableLength==11 && i>=key ){ // after insert 1676, key: 1081 become to be not found.
			//printf("i: %llu.\n", i);
			ICHashT_p->find(key, result);
			if(result==false){
				printf("after insert %llu, key: %llu become not to be found.\n", i, key);
				//DumpALL(ICHashT_p);
				std::string SaveDirctory = "./TrueTableLength11_AfterInsert19.txt";
				ICHashT_p->DumpAllToTheText(SaveDirctory.c_str());
				SysPauseForWin32();
				break;
			}
		}//*/
	}

	/*
	//for debug
	for(UINT64 key=0; key<TableLength && key<SetNum; key++){
		bool result;
		ICHashT_p->find(key, result);
		if(result==false){
			printf("%llu is not found.\n", key);
			// = "./TrueTableLength11_AfterInsert18.txt";
			std::string SaveDirctory = str_sprintf("./ERROR_Key_%llu_is_not_found._TrueTableLength_is_%llu.txt", key, ICHashT_p->GetTrueTableLength() );
			ICHashT_p->DumpAllToTheText(SaveDirctory.c_str());
			SysPauseForWin32();
		}
	}
	//*/
}

void benchmark_ICHashT_MaxLoadFactor( UINT64& RandSeed ){

//	UINT64 TableLength = 59652324;	//およそ1GByteのテーブルサイズ．// 1 GByte = 1024 MByte = 1024^2 kByte = 1024^3 Byte, 1024^3 Byte/18[Byte/要素] = 1073741824/18 = 59652323.55555556 ≒ 59652324
									//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

	int PrimeList_Limit = 25;	// サイズ 1 GByte までのテーブルについて，LoadFactor の最大値を計測

//	printf("PrimeListNum, ApparentTableLength, ( ElementsInTheTable / TrueTableLength ) * 100 = MaxLoadFactor [%%]\n");
	printf("TrueTableLength, MaxLoadFactor [%%]\n");
	for(UINT64 i=0; i<=PrimeList_Limit; i++){
		UINT64 TableLength = SmallestPrimeList_LargerThanPower2[i];

		ICHashT* ICHashT_p = new ICHashT(TableLength);	//ハッシュテーブルを動的に確保する．

		UINT64 SetNum, NumOfElements;
		benchmark_ICHashT_MaxLoadFactor_insert( ICHashT_p, TableLength, SetNum, NumOfElements, RandSeed );
		UINT64 TrueTableLength = ICHashT_p->GetTrueTableLength();
//		printf( "          %2llu,            %8llu, (          %8llu  /        %8llu ) * 100 =        %6.2lf [%%]\n", i, TableLength, SetNum, TrueTableLength, 100*((double)SetNum)/((double)TrueTableLength) );
		printf( "%llu, %lf\n", TrueTableLength, 100*((double)SetNum)/((double)TrueTableLength) );
//		printf( "NumOfElements: %8llu\n", NumOfElements );//for debug
		
		delete ICHashT_p;	//ハッシュテーブルの解放．
	}
}

//===================================================================

void benchmark_ICHashT_set1(){
	// set1();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	//処理
	clock_t start, end;
	double lf_now, lf_before=0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		end = clock();
		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}

	delete pICHashT;	//ハッシュテーブルの解放．
}

// value に std::string をポインタで接続した場合
void benchmark_ICHashT_set2_StdString(){
	// set1();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	//処理
	clock_t start, end;
	double lf_now, lf_before=0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			std::string* str = new std::string;
			*str = "ABCDEF_SaveTest";
			pICHashT->set1(key,(void*&)str);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		end = clock();
		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}


	// 解放処理
	{
		AllScanPlace NowPlace;
		for(pICHashT->AllScanInit(NowPlace); NowPlace < pICHashT->AllScanEnd; pICHashT->AllScanNextShift(NowPlace)){
			UINT64 key;
			void* value;
			pICHashT->AllScanGetData(key,value,NowPlace);
//			printf("           key::%lld\n",            key );
//			printf("value(Address)::%llX\n",          value );
//			printf("value(Entity )::%s\n"  ,  ((char*)value));
//			printf("\n");
//			if(NowPlace<50){
//				printf("%s\n",((std::string*)value)->c_str());
//			}
			delete (std::string*)value;	//メモリの解放
		}
	}
	delete pICHashT;	//ハッシュテーブルの解放．
}

void benchmark_ICHashT_find1(){
	// find();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000*10; p++){
			UINT64 key = genrand64_int64();
			bool result;
			pICHashT->find( key, result );
			if(result){
				counter++;
			}
		}
		end = clock();
		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pICHashT;	//ハッシュテーブルの解放．
}

void benchmark_ICHashT_find2(){
	// find();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = genrand64_int64();
//			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			UINT64 key = 100000*q + p;
			pICHashT->set1(key,(void*&)key);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000*200; p++){
//			UINT64 key = genrand64_int64();
			bool result;
//			pICHashT->find( key, result );
			pICHashT->find(  p , result );
			if(result){
				counter++;
			}
		}
		end = clock();
		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pICHashT;	//ハッシュテーブルの解放．
}

void benchmark_ICHashT_find3(){
	// find();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = genrand64_int64();
//			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			UINT64 key = 100000*q + p;
			pICHashT->set1(key,(void*&)key);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = genrand64_int64();
			bool result;
//			pICHashT->find( key, result );
			UINT64 buf = (100000*q + p);
			pICHashT->find( buf, result );
			if(result){
				counter++;
			}
		}
		end = clock();
		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pICHashT;	//ハッシュテーブルの解放．
}

void benchmark_ICHashT_find4(){
	// find();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	printf("Table Size: %llu\n", pICHashT->GetTrueTableLength());
	
	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = genrand64_int64();
//			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			UINT64 key = 100000*q + p;
			pICHashT->set1(key,(void*&)key);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<(100000*(q+1)); p++){
			//UINT64 key = genrand64_int64();
//			UINT64 key = (UINT64)(((double)genrand64_int64()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
			bool result;
			pICHashT->find( key, result );
//			UINT64 buf = (100000*q + p);
//			pICHashT->find( buf, result );
			if(result){
				counter++;
			}
		}
		end = clock();
		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pICHashT;	//ハッシュテーブルの解放．
}


void benchmark_ICHashT_find5_get_StdString(){
	// find();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	//処理
	UINT64 OptimizationGuard_buf=0;
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = 100000*q + p;
			std::string* str = new std::string;
			*str = "ABCDEF_SaveTest";
			pICHashT->set1(key,(void*&)str);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}

		start = clock();	//計測開始
		for(UINT64 p=0; p<(100000*(q+1)); p++){
			//UINT64 key = genrand64_int64();
//			UINT64 key = (UINT64)(((double)genrand64_int64()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
			bool result;
			std::string* str;
			pICHashT->get0( key, (void*&)str, result );
			if(result==true){
				//char buf = ((char*)(str->c_str()))[0];
				OptimizationGuard_buf += ((char*)(str->c_str()))[0];
				//printf("%c\n", buf);
				//printf("%s\n",((std::string*)str)->c_str() );
			}
//			if(q=0&&p<5){
//				printf("AA%c\n", buf);
//			}
		}
		end = clock();

		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("OptimizationGuard_buf: %llu\n", OptimizationGuard_buf);

	// 解放処理
	{
		AllScanPlace NowPlace;
		for(pICHashT->AllScanInit(NowPlace); NowPlace < pICHashT->AllScanEnd; pICHashT->AllScanNextShift(NowPlace)){
			UINT64 key;
			void* value;
			pICHashT->AllScanGetData(key,value,NowPlace);
//			printf("           key::%lld\n",            key );
//			printf("value(Address)::%llX\n",          value );
//			printf("value(Entity )::%s\n"  ,  ((char*)value));
//			printf("\n");
//			if(NowPlace<50){
//				printf("%s\n",((std::string*)value)->c_str());
//			}
			delete (std::string*)value;	//メモリの解放
		}
	}
	delete pICHashT;	//ハッシュテーブルの解放．
}


void benchmark_ICHashT_find6_get_UINT64pointer(){
	// find();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	//処理
	UINT64 OptimizationGuard_buf=0;
	clock_t start, end, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = 100000*q + p;
			//std::string* str = new std::string;
			//*str = "ABCDEF_SaveTest";
			UINT64* point_uint64 = (UINT64*)malloc(1*sizeof(UINT64));
			*point_uint64 = 314159;
			pICHashT->set1(key,(void*&)point_uint64);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}

		start = clock();	//計測開始
		for(UINT64 p=0; p<(100000*(q+1)); p++){
			//UINT64 key = genrand64_int64();
//			UINT64 key = (UINT64)(((double)genrand64_int64()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
			bool result;
			//std::string* str;
			UINT64* point_uint64;
			pICHashT->get0( key, (void*&)point_uint64, result );
			if(result==true){
				//char buf = ((char*)(str->c_str()))[0];
				OptimizationGuard_buf += *point_uint64;
				//printf("%llu\n", *point_uint64);
				//printf("%s\n",((std::string*)str)->c_str() );
			}
//			if(q=0&&p<5){
//				printf("AA%c\n", buf);
//			}
		}
		end = clock();

		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("OptimizationGuard_buf: %llu\n", OptimizationGuard_buf);

	// 解放処理
	{
		AllScanPlace NowPlace;
		for(pICHashT->AllScanInit(NowPlace); NowPlace < pICHashT->AllScanEnd; pICHashT->AllScanNextShift(NowPlace)){
			UINT64 key;
			void* value;
			pICHashT->AllScanGetData(key,value,NowPlace);
//			printf("           key::%lld\n",            key );
//			printf("value(Address)::%llX\n",          value );
//			printf("value(Entity )::%s\n"  ,  ((char*)value));
//			printf("\n");
//			if(NowPlace<50){
//				printf("%s\n",((std::string*)value)->c_str());
//			}
//			delete (std::string*)value;	//メモリの解放
			free(value);
		}
	}
	delete pICHashT;	//ハッシュテーブルの解放．
}


void benchmark_ICHashT_erase1(){
	// find();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	UINT64 MaxLF_q;// load factor の最大値を記録しておく
	init_genrand64(0);
	// LoadFactor の最大値を計測======================================================================================================
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");MaxLF_q=q;break;}
	//	printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	delete pICHashT;	//ハッシュテーブルの解放．

	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	init_genrand64(0);
	lf_before=0;
	// MaxLF_q まで挿入する===========================================================================================================
	for(UINT64 q=0; q<MaxLF_q; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
	}
	printf("end\n");

	init_genrand64(0);
	// 削除処理============================================================================================================================
	for(UINT64 q=0; q<MaxLF_q; q++){
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			bool result=true;
			pICHashT->erase0( key, result );
//			if(result==false){
//				printf("%s\n", result ? "true" : "false");				//削除に成功した場合はtrue．
//				printf("q: %llu, p: %llu\n", q, p );
//				printf("\n");
//				SysPauseForWin32();
//			}
		//	pICHashT->find( key, result );
		}
		end = clock();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, pICHashT->GetLoadFactor() );
	}

	delete pICHashT;	//ハッシュテーブルの解放．
}


//===========================================================================================================
//===========================================================================================================
//===========================================================================================================

void benchmark_unorderd_map_insert1(){

	//http://www.cplusplus.com/reference/unordered_map/unordered_map/load_factor/
	std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

	std::cout << "size = " << pUnOrderedMap->size() << std::endl;
	std::cout << "bucket_count = " << pUnOrderedMap->bucket_count() << std::endl;
	std::cout << "load_factor = " << pUnOrderedMap->load_factor() << std::endl;
	std::cout << "max_load_factor = " << pUnOrderedMap->max_load_factor() << std::endl;
	std::cout << std::endl;

	printf( "index, ProcessTime, LoadFactor\n");
	
	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
		}
		end = clock();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}

	delete pUnOrderedMap;
}

void benchmark_unorderd_map_insert2_StdString(){

	//http://www.cplusplus.com/reference/unordered_map/unordered_map/load_factor/
	std::unordered_map<UINT64, std::string> *pUnOrderedMap = new std::unordered_map<UINT64, std::string>(10000000);

	std::cout << "size = " << pUnOrderedMap->size() << std::endl;
	std::cout << "bucket_count = " << pUnOrderedMap->bucket_count() << std::endl;
	std::cout << "load_factor = " << pUnOrderedMap->load_factor() << std::endl;
	std::cout << "max_load_factor = " << pUnOrderedMap->max_load_factor() << std::endl;
	std::cout << std::endl;

	printf( "index, ProcessTime, LoadFactor\n");
	
	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
//			std::string* str = new std::string;
//			*str = "ABCDEF_SaveTest";

//			std::string str = "ABCDEF_SaveTest";
//			(*pUnOrderedMap)[key] = str;	//ハッシュテーブルに (key, value) ペアを格納

			(*pUnOrderedMap)[key] = "ABCDEF_SaveTest";	//ハッシュテーブルに (key, value) ペアを格納
		}
		end = clock();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}

	delete pUnOrderedMap;
}

void benchmark_unorderd_map_find1(){
	// find();関数，ベンチマーク

	std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

	printf( "index, ProcessTime, LoadFactor\n");
	
	//処理
	clock_t start, end, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000*10; p++){
			UINT64 key = genrand64_int64();
			bool result;
			auto itr = pUnOrderedMap->find( key );
			if(itr!=pUnOrderedMap->end()){
				counter++;
			}
		}
		end = clock();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pUnOrderedMap;	//ハッシュテーブルの解放．
}


void benchmark_unorderd_map_find2(){
	// find();関数，ベンチマーク

	std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

	printf( "index, ProcessTime, LoadFactor\n");
	
	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
//	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = genrand64_int64();
//			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			UINT64 key = 100000*q + p;
			(*pUnOrderedMap)[key] = key;	//ハッシュテーブルに (key, value) ペアを格納
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000*200; p++){
//			UINT64 key = genrand64_int64();
			auto itr = pUnOrderedMap->find(  p  );
			if(itr!=pUnOrderedMap->end()){
				counter++;
			}
		}
		end = clock();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pUnOrderedMap;	//ハッシュテーブルの解放．
}

void benchmark_unorderd_map_find3(){
	// find();関数，ベンチマーク

	std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

	printf( "index, ProcessTime, LoadFactor\n");
	
	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
//	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = genrand64_int64();
//			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			UINT64 key = 100000*q + p;
			(*pUnOrderedMap)[key] = key;	//ハッシュテーブルに (key, value) ペアを格納
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = genrand64_int64();
			auto itr = pUnOrderedMap->find(  (100000*q + p)  );
			if(itr!=pUnOrderedMap->end()){
				counter++;
			}
		}
		end = clock();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pUnOrderedMap;	//ハッシュテーブルの解放．
}

void benchmark_unorderd_map_find4(){
	// find();関数，ベンチマーク

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

	printf( "index, ProcessTime, LoadFactor\n");
	
	std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);
	printf("Table Size: %llu\n", pUnOrderedMap->bucket_count());
//	ICHashT* pICHashT;
//	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = genrand64_int64();
//			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			UINT64 key = 100000*q + p;
			(*pUnOrderedMap)[key] = key;	//ハッシュテーブルに (key, value) ペアを格納
		}
//		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = 100000*q + p;
//			pICHashT->set1(key,(void*&)key);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
//		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<(100000*(q+1)); p++){
			//UINT64 key = genrand64_int64();
//			UINT64 key = (UINT64)(((double)genrand64_int64()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
//			bool result;
//			pICHashT->find( key, result );
			auto itr = pUnOrderedMap->find(  key  );
			if(itr!=pUnOrderedMap->end()){
			}
//			UINT64 buf = (100000*q + p);
//			pICHashT->find( buf, result );
//			if(result){
//				counter++;
//			}
		}
		end = clock();
//		lf_now = pICHashT->GetLoadFactor();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}

//	delete pICHashT;	//ハッシュテーブルの解放．
	delete pUnOrderedMap;	//ハッシュテーブルの解放
}


void benchmark_unorderd_map_erase1(){
	// find();関数，ベンチマーク

	std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

	printf( "index, ProcessTime, LoadFactor\n");
	
	UINT64 MaxLF_q;// load factor の最大値を記録しておく
	init_genrand64(0);
	// LoadFactor の最大値を計測======================================================================================================
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
		}
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");MaxLF_q=q;break;}
	//	printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	delete pUnOrderedMap;	//ハッシュテーブルの解放

	pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);
	init_genrand64(0);
	lf_before=0;
	// MaxLF_q まで挿入する===========================================================================================================
	for(UINT64 q=0; q<MaxLF_q; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
		}
	}
	printf("end\n");

	init_genrand64(0);
	// 削除処理============================================================================================================================
	for(UINT64 q=0; q<MaxLF_q; q++){
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = genrand64_int64();
			bool result=true;
			pUnOrderedMap->erase( key );
//			if(result==false){
//				printf("%s\n", result ? "true" : "false");				//削除に成功した場合はtrue．
//				printf("q: %llu, p: %llu\n", q, p );
//				printf("\n");
//				SysPauseForWin32();
//			}
		//	pICHashT->find( key, result );
		}
		end = clock();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, pUnOrderedMap->load_factor() );
	}

	delete pUnOrderedMap;	//ハッシュテーブルの解放
}

void benchmark_unorderd_map_find5_get_StdString(){
	// find();関数，ベンチマーク

	std::unordered_map<UINT64, std::string> *pUnOrderedMap = new std::unordered_map<UINT64, std::string>(10000000);

	printf( "index, ProcessTime, LoadFactor\n");
	
	//処理
	UINT64 OptimizationGuard_buf=0;
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	init_genrand64(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = 100000*q + p;
			//std::string* str = new std::string;
			//*str = "ABCDEF_SaveTest";
			//pICHashT->set1(key,(void*&)str);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			(*pUnOrderedMap)[key] = "ABCDEF_SaveTest";
		}

		start = clock();	//計測開始
		for(UINT64 p=0; p<(100000*(q+1)); p++){
			//UINT64 key = genrand64_int64();
//			UINT64 key = (UINT64)(((double)genrand64_int64()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
			//bool result;
			//std::string* str;
			//pICHashT->get0( key, (void*&)str, result );
			auto itr = pUnOrderedMap->find(key);
			if(itr!=pUnOrderedMap->end()){
				//char buf = ((char*)(str->c_str()))[0];
				OptimizationGuard_buf += ((char*)(itr->second.c_str()))[0];
				//printf("%c\n", ((char*)(itr->second.c_str()))[0]);
				//printf("%s\n",((std::string*)str)->c_str() );
			}
//			if(q=0&&p<5){
//				printf("AA%c\n", buf);
//			}
		}
		end = clock();

		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%d, %llu, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);
	printf("OptimizationGuard_buf: %llu\n", OptimizationGuard_buf);

	delete pUnOrderedMap;	//ハッシュテーブルの解放．
}
