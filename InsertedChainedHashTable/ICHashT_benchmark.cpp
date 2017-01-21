#include "ICHashT_benchmark.hpp"

void benchmark_ICHashT_MaxLoadFactor_insert( ICHashT*& ICHashT_p, UINT64& TableLength, UINT64& SetNum, UINT64& NumOfElements, UINT64& RandSeed );
void benchmark_ICHashT_MaxLoadFactor_insert( ICHashT*& ICHashT_p, UINT64& TableLength, UINT64& SetNum, UINT64& NumOfElements, UINT64& RandSeed ){

	std::mt19937_64* pMT = new std::mt19937_64(RandSeed);

	//挿入処理
	for(UINT64 i=0;; i++){

		void** return_value_pp;	//ダミー(実際には使われない．)
		bool   result;
		bool   NeedReHash;
		//ICHashT_p->setXwithNoReHash( i, (void*&)i, return_value_pp, result, NeedReHash );
		UINT64 key = (*pMT)();
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
	}

	delete pMT;
}

void benchmark_ICHashT_MaxLoadFactor(int LoopNum){

	std::string SaveDir = R"(./ResultOfBenchmarks/benchmark_ICHashT_MaxLoadFactor.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("TrueTableLength, MaxLoadFactor [%%]%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	// 素数表より(67108879+254要素)で，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
	int PrimeList_Limit = 25;	// サイズ 1 GByte までのテーブルについて，LoadFactor の最大値を計測

	for(int i=0; i<LoopNum; i++){
			UINT64 RandSeed = i;

	//	printf("PrimeListNum, ApparentTableLength, ( ElementsInTheTable / TrueTableLength ) * 100 = MaxLoadFactor [%%]\n");
	//	printf("TrueTableLength, MaxLoadFactor [%%]\n");
		for(UINT64 i=0; i<=PrimeList_Limit; i++){
			UINT64 TableLength = SmallestPrimeList_LargerThanPower2[i];

			ICHashT* ICHashT_p = new ICHashT(TableLength);	//ハッシュテーブルを動的に確保する．

			UINT64 SetNum, NumOfElements;
			benchmark_ICHashT_MaxLoadFactor_insert( ICHashT_p, TableLength, SetNum, NumOfElements, RandSeed );
			UINT64 TrueTableLength = ICHashT_p->GetTrueTableLength();
	//		printf( "          %2llu,            %8llu, (          %8llu  /        %8llu ) * 100 =        %6.2lf [%%]\n", i, TableLength, SetNum, TrueTableLength, 100*((double)SetNum)/((double)TrueTableLength) );
	//		printf( "%llu, %lf\n", TrueTableLength, 100*((double)SetNum)/((double)TrueTableLength) );
			PrintBuf = str_sprintf("%llu, %lf%s", TrueTableLength, 100*((double)SetNum)/((double)TrueTableLength), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		
			delete ICHashT_p;	//ハッシュテーブルの解放．
		}
		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
}

//===================================================================

void benchmark_ICHashT_set1(int LoopNum){

	std::string SaveDir = R"(./ResultOfBenchmarks/benchmark_ICHashT_set1.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("index, LoadFactor, ProcessTime%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	for(int i=0; i<LoopNum; i++){
		// 素数表より(67108879+254要素)で，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
		UINT64 TableLength = 10000000;

		ICHashT* pICHashT;
		pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
		std::mt19937_64* pMT = new std::mt19937_64(0);

		//処理
		clock_t start, end;
		double lf_now, lf_before=0;
		for(UINT64 q=0; q<200; q++){
			start = clock();	//計測開始
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			}
			end = clock();
			lf_now = pICHashT->GetLoadFactor();
			if(lf_now<lf_before){printf("rehash\n");break;}
		
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());

			lf_before = lf_now;
		}

		delete pMT;
		delete pICHashT;	//ハッシュテーブルの解放．

		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
}

// value に std::string をポインタで接続した場合
void benchmark_ICHashT_set2_StdString(int LoopNum){

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	clock_t start, end;
	double lf_now, lf_before=0;
	for(UINT64 q=0; q<200; q++){
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = (*pMT)();
			std::string* str = new std::string;
			*str = "ABCDEF_SaveTest";
			pICHashT->set1(key,(void*&)str);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		end = clock();
		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
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

	delete pMT;
	delete pICHashT;	//ハッシュテーブルの解放．
}

void benchmark_ICHashT_find1(){

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = (*pMT)();
			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000*10; p++){
			UINT64 key = (*pMT)();
			bool result;
			pICHashT->find( key, result );
			if(result){
				counter++;
			}
		}
		end = clock();
		lf_now = pICHashT->GetLoadFactor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pMT;
	delete pICHashT;	//ハッシュテーブルの解放．
}

void benchmark_ICHashT_find2(){

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
//	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = (*pMT)();
//			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			UINT64 key = 100000*q + p;
			pICHashT->set1(key,(void*&)key);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000*200; p++){
//			UINT64 key = (*pMT)();
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
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

//	delete pMT;
	delete pICHashT;	//ハッシュテーブルの解放．
}

void benchmark_ICHashT_find3(){

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

//	UINT64 TableLength = 8388608-254;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 8388617+254;	//67109133
//	int lfLimit = 100;

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
//	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = (*pMT)();
//			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			UINT64 key = 100000*q + p;
			pICHashT->set1(key,(void*&)key);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = (*pMT)();
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
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

//	delete pMT;
	delete pICHashT;	//ハッシュテーブルの解放．
}

void benchmark_ICHashT_find4(){

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	printf("Table Size: %llu\n", pICHashT->GetTrueTableLength());
	
	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	clock_t start, end;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = genrand64_int64();
//			pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			UINT64 key = 100000*q + p;
			pICHashT->set1(key,(void*&)key);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<(100000*(q+1)); p++){
			UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
//			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
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
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pMT;
	delete pICHashT;	//ハッシュテーブルの解放．
}


void benchmark_ICHashT_find5_get_StdString(){

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	UINT64 OptimizationGuard_buf=0;
	clock_t start, end;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = 100000*q + p;
			std::string* str = new std::string;
			*str = "ABCDEF_SaveTest";
			pICHashT->set1(key,(void*&)str);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
		}

		start = clock();	//計測開始
		for(UINT64 p=0; p<(100000*(q+1)); p++){
			UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
//			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
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
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
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

	delete pMT;
	delete pICHashT;	//ハッシュテーブルの解放．
}


void benchmark_ICHashT_find6_get_UINT64pointer(){

	UINT64 TableLength = 10000000;//59652324;	//およそ1GByteのテーブルサイズ．//素数表より実際には(67108879+254要素)より，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
//	UINT64 TrueTableLength = 67108879+254;	//67109133

	printf( "index, ProcessTime, LoadFactor\n");
	
	ICHashT* pICHashT;
	pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
	
	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	UINT64 OptimizationGuard_buf=0;
	clock_t start, end;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
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
			UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
//			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
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
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
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
	delete pMT;
	delete pICHashT;	//ハッシュテーブルの解放．
}


void benchmark_ICHashT_erase1(int LoopNum){

	std::string SaveDir = R"(./ResultOfBenchmarks/benchmark_ICHashT_erase1.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("index, LoadFactor, ProcessTime%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	for(int i=0; i<LoopNum; i++){
		// 素数表より(67108879+254要素)で，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
		UINT64 TableLength = 10000000;
	
		UINT64 MaxLF_q;// load factor の最大値を記録しておく
		// LoadFactor の最大値を計測======================================================================================================
		ICHashT* pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		std::mt19937_64* pMT = new std::mt19937_64(0);

		clock_t start, end;
		double lf_now, lf_before=0;
		for(UINT64 q=0; q<200; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			}
			lf_now = pICHashT->GetLoadFactor();
			if(lf_now<lf_before){printf("rehash\n");MaxLF_q=q;break;}
		//	printf( "%d, %llu, %lf\n", q, end - start, lf_now );
			lf_before = lf_now;
		}
		delete pICHashT;	//ハッシュテーブルの解放．
		delete pMT;

		pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		pMT = new std::mt19937_64(0);
		lf_before=0;
		// MaxLF_q まで挿入する===========================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			}
		}
		delete pMT;
		printf("end\n");

		pMT = new std::mt19937_64(0);
		// 削除処理============================================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			start = clock();	//計測開始
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
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
			
			lf_now = pICHashT->GetLoadFactor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		delete pICHashT;	//ハッシュテーブルの解放．

		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
}



void benchmark_ICHashT_find1_and_Erase(int LoopNum){
	
	std::string SaveDir = R"(./ResultOfBenchmarks/benchmark_ICHashT_find1_and_Erase.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("index, LoadFactor, ProcessTime%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	for(int i=0; i<LoopNum; i++){
		// 素数表より(67108879+254要素)で，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
		UINT64 TableLength = 10000000;

		UINT64 MaxLF_q;// load factor の最大値を記録しておく

		std::mt19937_64* pMT = new std::mt19937_64(0);
		// LoadFactor の最大値を計測======================================================================================================
		ICHashT* pICHashT;
		pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		clock_t start, end;
		double lf_now, lf_before=0;
		for(UINT64 q=0; q<200; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			}
			lf_now = pICHashT->GetLoadFactor();
			if(lf_now<lf_before){printf("rehash\n");MaxLF_q=q;break;}
		//	printf( "%d, %llu, %lf\n", q, end - start, lf_now );
			lf_before = lf_now;
		}
		delete pMT;
		delete pICHashT;	//ハッシュテーブルの解放．

		pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		pMT = new std::mt19937_64(0);
		lf_before=0;
		UINT64 counter = 0;
		// MaxLF_q まで挿入する===========================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			}


			start = clock();	// 計測開始
			for(UINT64 p=0; p<100000*10; p++){
				UINT64 key = (*pMT)();
				bool result;
				pICHashT->find( key, result );
				if(result){
					counter++;
				}
			}
			end = clock();


			lf_now = pICHashT->GetLoadFactor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		printf("end\n");

		pMT = new std::mt19937_64(0);
		// 削除処理============================================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				bool result=true;
				pICHashT->erase0( key, result );
			}


			start = clock();	//計測開始
			for(UINT64 p=0; p<100000*10; p++){
				UINT64 key = (*pMT)();
				bool result;
				pICHashT->find( key, result );
				if(result){
					counter++;
				}
			}
			end = clock();


			lf_now = pICHashT->GetLoadFactor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		delete pICHashT;	//ハッシュテーブルの解放

		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
}



void benchmark_ICHashT_find4_and_Erase(int LoopNum){
	
	std::string SaveDir = R"(./ResultOfBenchmarks/benchmark_ICHashT_find4_and_Erase.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("index, LoadFactor, ProcessTime%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	for(int i=0; i<LoopNum; i++){
		// 素数表より(67108879+254要素)で，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
		UINT64 TableLength = 10000000;

		UINT64 MaxLF_q;// load factor の最大値を記録しておく

		std::mt19937_64* pMT = new std::mt19937_64(0);
		// LoadFactor の最大値を計測======================================================================================================
		ICHashT* pICHashT;
		pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		clock_t start, end;
		double lf_now, lf_before=0;
		for(UINT64 q=0; q<200; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			}
			lf_now = pICHashT->GetLoadFactor();
			if(lf_now<lf_before){printf("rehash\n");MaxLF_q=q;break;}
		//	printf( "%d, %llu, %lf\n", q, end - start, lf_now );
			lf_before = lf_now;
		}
		delete pMT;
		delete pICHashT;	//ハッシュテーブルの解放．

		pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		pMT = new std::mt19937_64(0);
		lf_before=0;
		UINT64 counter = 0;
		// MaxLF_q まで挿入する===========================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				//UINT64 key = (*pMT)();
				UINT64 key = 100000*q + p;
				pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
			}


			start = clock();	//計測開始
	//		for(UINT64 p=0; p<100000*10; p++){
			for(UINT64 p=0; p<(100000*(q+1)); p++){
				//UINT64 key = (*pMT)();
				UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
				bool result;
				pICHashT->find( key, result );
				if(result){
					counter++;
				}
			}
			end = clock();


			lf_now = pICHashT->GetLoadFactor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		printf("end\n");

		pMT = new std::mt19937_64(0);
		// 削除処理============================================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				//UINT64 key = (*pMT)();
				UINT64 key = 100000*q + p;
				bool result=true;
				pICHashT->erase0( key, result );
			}


			start = clock();	//計測開始
			//for(UINT64 p=0; p<100000*10; p++){
			//for(UINT64 p=0; p<(100000*(q+1)); p++){
			for(UINT64 p=(100000*(q+1)); p<(100000*(MaxLF_q)); p++){
				//UINT64 key = (*pMT)();
				//UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
				UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(MaxLF_q-(q+1)))) + (100000*(q+1));
				bool result;
				pICHashT->find( key, result );
				if(result){
					counter++;
				}
			}
			end = clock();


			lf_now = pICHashT->GetLoadFactor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		delete pICHashT;	//ハッシュテーブルの解放

		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
}


//===========================================================================================================
//===========================================================================================================
//===========================================================================================================

void benchmark_unorderd_map_MaxLoadFactor(int LoopNum){

	std::string SaveDir = R"(./ResultOfBenchmarks/unorderd_map_MaxLoadFactor.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("TrueTableLength, MaxLoadFactor [%%]%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	for(int i=0; i<LoopNum; i++){

		UINT64 RandSeed = LoopNum;
		std::mt19937_64* pMT = new std::mt19937_64(RandSeed);

		std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(1);
		
		// 挿入処理
		double lf_now, lf_before=0;
		//for(UINT64 i=0; i<10000000; i++){
		for(UINT64 i=0; i<  30000000; i++){
			UINT64 key = (*pMT)();
			(*pUnOrderedMap)[key] = i;	//ハッシュテーブルに (key, value) ペアを格納
			lf_now = pUnOrderedMap->load_factor();
			if(lf_now<lf_before){
//				PrintBuf = str_sprintf("%llu, %lf%s", i, lf_before, LineFeedCode);
				PrintBuf = str_sprintf("%llu, %lf%s", (pUnOrderedMap->bucket_count())-1, lf_before, LineFeedCode);
				printf(PrintBuf.c_str());
				FW.FWrite(PrintBuf.c_str());
				//break;
			}
			lf_before = lf_now;
		}

		delete pUnOrderedMap;
		delete pMT;

		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
}






void benchmark_unorderd_map_insert1(int LoopNum){

	std::string SaveDir = R"(./ResultOfBenchmarks/benchmark_unorderd_map_insert1.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("index, LoadFactor, ProcessTime%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	for(int i=0; i<LoopNum; i++){
		//http://www.cplusplus.com/reference/unordered_map/unordered_map/load_factor/
		std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

		std::cout << "size = " << pUnOrderedMap->size() << std::endl;
		std::cout << "bucket_count = " << pUnOrderedMap->bucket_count() << std::endl;
		std::cout << "load_factor = " << pUnOrderedMap->load_factor() << std::endl;
		std::cout << "max_load_factor = " << pUnOrderedMap->max_load_factor() << std::endl;
		std::cout << std::endl;
	
		//処理
		clock_t start, end;
		double lf_now, lf_before=0;
		std::mt19937_64* pMT = new std::mt19937_64(0);
		for(UINT64 q=0; q<200; q++){
			start = clock();	//計測開始
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			}
			end = clock();
			lf_now = pUnOrderedMap->load_factor();
			if(lf_now<lf_before){printf("rehash\n");break;}
			
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());

			lf_before = lf_now;
		}
		delete pMT;
		delete pUnOrderedMap;

		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
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
	
	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	clock_t start, end;
	double lf_now, lf_before=0;
	for(UINT64 q=0; q<200; q++){
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = (*pMT)();
//			std::string* str = new std::string;
//			*str = "ABCDEF_SaveTest";

//			std::string str = "ABCDEF_SaveTest";
//			(*pUnOrderedMap)[key] = str;	//ハッシュテーブルに (key, value) ペアを格納

			(*pUnOrderedMap)[key] = "ABCDEF_SaveTest";	//ハッシュテーブルに (key, value) ペアを格納
		}
		end = clock();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	delete pMT;
	delete pUnOrderedMap;
}

void benchmark_unorderd_map_find1(){
	// find();関数，ベンチマーク

	std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

	printf( "index, ProcessTime, LoadFactor\n");
	
	//処理
	clock_t start, end;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	std::mt19937_64 mt(0);
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
			UINT64 key = mt();
			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000*10; p++){
			UINT64 key = mt();
			//bool result;
			auto itr = pUnOrderedMap->find( key );
			if(itr!=pUnOrderedMap->end()){
				counter++;
			}
		}
		end = clock();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

	delete pUnOrderedMap;	//ハッシュテーブルの解放．
}


void benchmark_unorderd_map_find2(){
	// find();関数，ベンチマーク

	std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

	printf( "index, ProcessTime, LoadFactor\n");
	
//	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	clock_t start, end;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = (*pMT)();
//			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			UINT64 key = 100000*q + p;
			(*pUnOrderedMap)[key] = key;	//ハッシュテーブルに (key, value) ペアを格納
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000*200; p++){
//			UINT64 key = (*pMT)();
			auto itr = pUnOrderedMap->find(  p  );
			if(itr!=pUnOrderedMap->end()){
				counter++;
			}
		}
		end = clock();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

//	delete pMT;
	delete pUnOrderedMap;	//ハッシュテーブルの解放．
}

void benchmark_unorderd_map_find3(){
	// find();関数，ベンチマーク

	std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

	printf( "index, ProcessTime, LoadFactor\n");
	
//	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	clock_t start, end;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = (*pMT)();
//			(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			UINT64 key = 100000*q + p;
			(*pUnOrderedMap)[key] = key;	//ハッシュテーブルに (key, value) ペアを格納
		}
		start = clock();	//計測開始
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = (*pMT)();
			auto itr = pUnOrderedMap->find(  (100000*q + p)  );
			if(itr!=pUnOrderedMap->end()){
				counter++;
			}
		}
		end = clock();
		lf_now = pUnOrderedMap->load_factor();
		if(lf_now<lf_before){printf("rehash\n");break;}
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);

//	delete pMT;
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
	
	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	clock_t start, end;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
	for(UINT64 q=0; q<200; q++){
		for(UINT64 p=0; p<100000; p++){
//			UINT64 key = mt();
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
			UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
//			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
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
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}

	delete pMT;
//	delete pICHashT;	//ハッシュテーブルの解放．
	delete pUnOrderedMap;	//ハッシュテーブルの解放
}


void benchmark_unorderd_map_erase1(int LoopNum){
	// find();関数，ベンチマーク

	std::string SaveDir = R"(./ResultOfBenchmarks/benchmark_unorderd_map_erase1.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("index, LoadFactor, ProcessTime%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	for(int i=0; i<LoopNum; i++){
		std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);

		UINT64 MaxLF_q;// load factor の最大値を記録しておく
		std::mt19937_64* pMT = new std::mt19937_64(0);
		// LoadFactor の最大値を計測======================================================================================================
		clock_t start, end;
		double lf_now, lf_before=0;
		for(UINT64 q=0; q<200; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			}
			lf_now = pUnOrderedMap->load_factor();
			if(lf_now<lf_before){printf("rehash\n");MaxLF_q=q;break;}
		//	printf( "%d, %llu, %lf\n", q, end - start, lf_now );
			lf_before = lf_now;
		}
		delete pMT;
		delete pUnOrderedMap;	//ハッシュテーブルの解放

		pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(10000000);
		pMT = new std::mt19937_64(0);
		lf_before=0;
		// MaxLF_q まで挿入する===========================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			}
		}
		delete pMT;
		printf("end\n");

		pMT = new std::mt19937_64(0);
		// 削除処理============================================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			start = clock();	//計測開始
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
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
			
			lf_now = pUnOrderedMap->load_factor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		delete pUnOrderedMap;	//ハッシュテーブルの解放

		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
}

void benchmark_unorderd_map_find5_get_StdString(){
	// find();関数，ベンチマーク

	std::unordered_map<UINT64, std::string> *pUnOrderedMap = new std::unordered_map<UINT64, std::string>(10000000);

	printf( "index, ProcessTime, LoadFactor\n");
	
	std::mt19937_64* pMT = new std::mt19937_64(0);

	//処理
	UINT64 OptimizationGuard_buf=0;
	clock_t start, end;//, now;
	double lf_now, lf_before=0;
	UINT64 counter = 0;
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
			UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
//			UINT64 key = round_half_to_even<UINT64>(((double)genrand64_real1())*(double)(100000*(q+1)));
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
		printf( "%llu, %ld, %lf\n", q, end - start, lf_now );
		lf_before = lf_now;
	}
	printf("find num: %llu\n", counter);
	printf("OptimizationGuard_buf: %llu\n", OptimizationGuard_buf);

	delete pMT;
	delete pUnOrderedMap;	//ハッシュテーブルの解放．
}


void benchmark_unorderd_map_find1_and_Erase(int LoopNum){
	
	std::string SaveDir = R"(./ResultOfBenchmarks/benchmark_unorderd_map_find1_and_Erase.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("index, LoadFactor, ProcessTime%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	for(int i=0; i<LoopNum; i++){
		// 素数表より(67108879+254要素)で，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
		UINT64 TableLength = 10000000;

		UINT64 MaxLF_q;// load factor の最大値を記録しておく

		std::mt19937_64* pMT = new std::mt19937_64(0);
		// LoadFactor の最大値を計測======================================================================================================
		//ICHashT* pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(TableLength);
		clock_t start, end;//, now;
		double lf_now, lf_before=0;
		for(UINT64 q=0; q<200; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				//pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
				(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			}
			//lf_now = pICHashT->GetLoadFactor();
			lf_now = pUnOrderedMap->load_factor();
			if(lf_now<lf_before){printf("rehash\n");MaxLF_q=q;break;}
		//	printf( "%d, %llu, %lf\n", q, end - start, lf_now );
			lf_before = lf_now;
		}
		delete pMT;
		//delete pICHashT;	//ハッシュテーブルの解放．
		delete pUnOrderedMap;	//ハッシュテーブルの解放

		//pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(TableLength);
		pMT = new std::mt19937_64(0);
		lf_before=0;
		UINT64 counter = 0;
		// MaxLF_q まで挿入する===========================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				//pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
				(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			}


			start = clock();	//計測開始
			for(UINT64 p=0; p<100000*10; p++){
				UINT64 key = (*pMT)();
				//bool result;
				//pICHashT->find( key, result );
				//if(result){
				//	counter++;
				//}
				auto itr = pUnOrderedMap->find(  key  );
				if(itr!=pUnOrderedMap->end()){
					counter++;
				}
			}
			end = clock();


			//lf_now = pICHashT->GetLoadFactor();
			lf_now = pUnOrderedMap->load_factor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		printf("end\n");

		pMT = new std::mt19937_64(0);
		// 削除処理============================================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				bool result=true;
				//pICHashT->erase0( key, result );
				pUnOrderedMap->erase( key );
			}


			start = clock();	//計測開始
			for(UINT64 p=0; p<100000*10; p++){
				UINT64 key = (*pMT)();
				//bool result;
				//pICHashT->find( key, result );
				//if(result){
				//	counter++;
				//}
				auto itr = pUnOrderedMap->find(  key  );
				if(itr!=pUnOrderedMap->end()){
					counter++;
				}
			}
			end = clock();


			//lf_now = pICHashT->GetLoadFactor();
			lf_now = pUnOrderedMap->load_factor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		//delete pICHashT;	//ハッシュテーブルの解放
		delete pUnOrderedMap;	//ハッシュテーブルの解放

		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
}

void benchmark_unorderd_map_find4_and_Erase(int LoopNum){
	
	std::string SaveDir = R"(./ResultOfBenchmarks/benchmark_unorderd_map_find4_and_Erase.csv)";
	class FileWriter FW(SaveDir.c_str());	//出力先のファイル名を指定

	std::string PrintBuf = str_sprintf("index, LoadFactor, ProcessTime%s", LineFeedCode);
	printf("%s", PrintBuf.c_str());
	FW.FWrite(PrintBuf.c_str());

	for(int i=0; i<LoopNum; i++){
		// 素数表より(67108879+254要素)で，(67108879+254要素)*18[Byte/要素]==1207964394Byte==1179652.728515625kByte==1152.00461769104MByte==1.125004509463906GByte
		UINT64 TableLength = 10000000;

		UINT64 MaxLF_q;// load factor の最大値を記録しておく

		std::mt19937_64* pMT = new std::mt19937_64(0);
		// LoadFactor の最大値を計測======================================================================================================
		//ICHashT* pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		std::unordered_map<UINT64, UINT64> *pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(TableLength);

		clock_t start, end;//, now;
		double lf_now, lf_before=0;
		for(UINT64 q=0; q<200; q++){
			for(UINT64 p=0; p<100000; p++){
				UINT64 key = (*pMT)();
				//pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
				(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			}
			//lf_now = pICHashT->GetLoadFactor();
			lf_now = pUnOrderedMap->load_factor();
			if(lf_now<lf_before){printf("rehash\n");MaxLF_q=q;break;}
		//	printf( "%d, %llu, %lf\n", q, end - start, lf_now );
			lf_before = lf_now;
		}
		delete pMT;
		//delete pICHashT;	//ハッシュテーブルの解放．
		delete pUnOrderedMap;	//ハッシュテーブルの解放

		//pICHashT = new ICHashT(TableLength);		//ハッシュテーブルを動的に確保する．
		pUnOrderedMap = new std::unordered_map<UINT64, UINT64>(TableLength);

		pMT = new std::mt19937_64(0);
		lf_before=0;
		UINT64 counter = 0;
		// MaxLF_q まで挿入する===========================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				//UINT64 key = (*pMT)();
				UINT64 key = 100000*q + p;
				//pICHashT->set1(key,(void*&)p);				//set1():keyが既に格納されている場合は，valueを上書きしてreturn;
				(*pUnOrderedMap)[key] = p;	//ハッシュテーブルに (key, value) ペアを格納
			}


			start = clock();	//計測開始
	//		for(UINT64 p=0; p<100000*10; p++){
			for(UINT64 p=0; p<(100000*(q+1)); p++){
				//UINT64 key = (*pMT)();
				UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
				//bool result;
				//pICHashT->find( key, result );
				//if(result){
				//	counter++;
				//}
				auto itr = pUnOrderedMap->find(  key  );
				if(itr!=pUnOrderedMap->end()){
					counter++;
				}
			}
			end = clock();


			//lf_now = pICHashT->GetLoadFactor();
			lf_now = pUnOrderedMap->load_factor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		printf("end\n");

		pMT = new std::mt19937_64(0);
		// 削除処理============================================================================================================================
		for(UINT64 q=0; q<MaxLF_q; q++){
			for(UINT64 p=0; p<100000; p++){
				//UINT64 key = (*pMT)();
				UINT64 key = 100000*q + p;
				//bool result=true;
				//pICHashT->erase0( key, result );
				pUnOrderedMap->erase( key );
			}


			start = clock();	//計測開始
			//for(UINT64 p=0; p<100000*10; p++){
			for(UINT64 p=(100000*(q+1)); p<(100000*(MaxLF_q)); p++){
				//UINT64 key = (*pMT)();
				//UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(q+1)));
				UINT64 key = (UINT64)(((double)(*pMT)()/(double)((UINT64)0xFFFFFFFFFFFFFFFF))*(double)(100000*(MaxLF_q-(q+1)))) + (100000*(q+1));
				//bool result;
				//pICHashT->find( key, result );
				//if(result){
				//	counter++;
				//}
				auto itr = pUnOrderedMap->find(  key  );
				if(itr!=pUnOrderedMap->end()){
					counter++;
				}
			}
			end = clock();


			//lf_now = pICHashT->GetLoadFactor();
			lf_now = pUnOrderedMap->load_factor();
			PrintBuf = str_sprintf("%d, %lf, %ld%s",  q, lf_now, (clock_t)(end - start), LineFeedCode);
			printf(PrintBuf.c_str());
			FW.FWrite(PrintBuf.c_str());
		}
		delete pMT;
		//delete pICHashT;	//ハッシュテーブルの解放
		delete pUnOrderedMap;	//ハッシュテーブルの解放

		PrintBuf = str_sprintf("==========================================================================%s", LineFeedCode);
		printf(PrintBuf.c_str());
		FW.FWrite(PrintBuf.c_str());
	}
}
