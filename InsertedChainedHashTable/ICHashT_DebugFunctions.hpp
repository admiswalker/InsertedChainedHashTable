/* -*- coding: utf-8 -*- */
#pragma once

#include "./ICHashT/ICHashT.hpp"
#ifdef ReturnAssignedHash_for_debug

void ICHashT_DebugFunction_ConfirmBasicOperation();

void ICHashT_DebugFunction_ConfirmSetProcessingNum1();		// 処理番号①
void ICHashT_DebugFunction_ConfirmSetProcessingNum2();		// 処理番号②
void ICHashT_DebugFunction_ConfirmSetProcessingNum3();		// 処理番号③, 重複なし
void ICHashT_DebugFunction_ConfirmSetProcessingNum3_2();	// 処理番号③, 重複あり
void ICHashT_DebugFunction_ConfirmSetProcessingNum4();		// 処理番号④
void ICHashT_DebugFunction_ConfirmSetProcessingNum4_2();	// 処理番号④
void ICHashT_DebugFunction_ConfirmSetProcessingNum5and6();	// 処理番号⑤, 処理番号⑥
void ICHashT_DebugFunction_ConfirmSetProcessingNum7();		// 処理番号⑦
void ICHashT_DebugFunction_ConfirmSetProcessingNum8();		// 処理番号⑧
void ICHashT_DebugFunction_ConfirmSetProcessingNum9();		// 処理番号⑨
void ICHashT_DebugFunction_ConfirmSetProcessingNum9_2();	// 処理番号⑨
void ICHashT_DebugFunction_ConfirmSetProcessingNum10();		// 処理番号⑩
void ICHashT_DebugFunction_ConfirmSetProcessingNum11();		// 処理番号⑪

void ICHashT_DebugFunction_ConfirmEraseProcessingNum1();	// 処理番号①
void ICHashT_DebugFunction_ConfirmEraseProcessingNum2();	// 処理番号②
void ICHashT_DebugFunction_ConfirmEraseProcessingNum2_2();	// 処理番号②
void ICHashT_DebugFunction_ConfirmEraseProcessingNum3();	// 処理番号③
void ICHashT_DebugFunction_ConfirmEraseProcessingNum3_2();	// 処理番号③
void ICHashT_DebugFunction_ConfirmEraseProcessingNum4();	// 処理番号④
#endif
