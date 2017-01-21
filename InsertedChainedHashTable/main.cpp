/* -*- coding: utf-8 -*- */
// InsertedChainedHashTable_Project2 ( �u���ߍ��ݎ��v�́u����Ȃ��^�v�́u�n�b�V���e�[�u���v�J���v��2 )
// ����::64bit���Ɉˑ�(key�̃T�C�Y�ƃn�b�V���l�̐����̒����𒲐����邱�Ƃ�32bit���ɂ��Ή��͂ł���͂��D)
// �J����::Windows7x64::MicrosoftVisualStudio2012

// �g�p���@
//
// [./ICHashT/*.*] ���t�H���_���ƁC�C�ӂ̃f�B���N�g���ɔz�u�D
// [./ICHashT/ICHashT.hpp]���C���N���[�h�D( #include "./ICHashT/ICHashT.hpp" )
// �ȏ�D

#include <stdio.h>
#include <time.h>		// clock_t() ���Ԍv���ɕK�v�D
#include <chrono>		// std::chrono::system_clock::now()

// #define ICHashT_DebugMode �̐ݒ�́C[./ICHashT/ICHashT.hpp] �ɂčs���K�v������܂��D(���̃t�@�C���֔g�y�����邽��)
// #define ReturnAssignedHash_for_debug �̐ݒ�́C[./ICHashT/ICHashT.hpp] �ɂčs���K�v������܂��D(���̃t�@�C���֔g�y�����邽��)
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
	// �����ł́C�e�֐��̎g�p������s���܂��D

#if !defined(ICHashT_DebugMode) && !defined(ReturnAssignedHash_for_debug)
//	Example1();
//	Example2();
//	Example3();
#endif

	// Examples (  end  ) ===================================================================================================


	// benchmarks ( start ) =================================================================================================
	
#if defined(ICHashT_DebugMode) && !defined(ReturnAssignedHash_for_debug)
	// Load Factor �̑���
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

//	ICHashT_DebugFunction_ConfirmSetProcessingNum1();		// �����ԍ��@
//	ICHashT_DebugFunction_ConfirmSetProcessingNum2();		// �����ԍ��A
//	ICHashT_DebugFunction_ConfirmSetProcessingNum3();		// �����ԍ��B, �d���Ȃ�
//	ICHashT_DebugFunction_ConfirmSetProcessingNum3_2();		// �����ԍ��B, �d������
//	ICHashT_DebugFunction_ConfirmSetProcessingNum4();		// �����ԍ��C
//	ICHashT_DebugFunction_ConfirmSetProcessingNum4_2();		// �����ԍ��C
//	ICHashT_DebugFunction_ConfirmSetProcessingNum5and6();	// �����ԍ��D, �����ԍ��E
//	ICHashT_DebugFunction_ConfirmSetProcessingNum7();		// �����ԍ��F
//	ICHashT_DebugFunction_ConfirmSetProcessingNum8();		// �����ԍ��G
//	ICHashT_DebugFunction_ConfirmSetProcessingNum9();		// �����ԍ��H
//	ICHashT_DebugFunction_ConfirmSetProcessingNum9_2();		// �����ԍ��H
//	ICHashT_DebugFunction_ConfirmSetProcessingNum10();		// �����ԍ��I
//	ICHashT_DebugFunction_ConfirmSetProcessingNum11();		// �����ԍ��J

//	ICHashT_DebugFunction_ConfirmEraseProcessingNum1();		// �����ԍ��@
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum2();		// �����ԍ��A
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum2_2();	// �����ԍ��A
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum3();		// �����ԍ��B
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum3_2();	// �����ԍ��B
//	ICHashT_DebugFunction_ConfirmEraseProcessingNum4();		// �����ԍ��C
#endif

	// DebugFunctions (  end  ) =============================================================================================


	SysPauseForWin32();	// �R���\�[�����ꎞ��~����
	return 0;
}
