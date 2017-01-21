/* -*- coding: utf-8 -*- */
#pragma once

#include <time.h>		// clock_t() 時間計測に必要。
#include <stdlib.h>		// rand()
#include <random>		// mt19937
#include <iostream>
#include <unordered_map>

#include "./ICHashT/ICHashT.hpp"

//========================================

//void benchmark_ICHashT_MaxLoadFactor( UINT64& RandSeed );
void benchmark_ICHashT_MaxLoadFactor(int LoopNum);


//void benchmark_ICHashT_set1();
void benchmark_ICHashT_set1(int LoopNum);
void benchmark_ICHashT_set2_StdString();

void benchmark_ICHashT_find1();
void benchmark_ICHashT_find2();
void benchmark_ICHashT_find3();
void benchmark_ICHashT_find4();
void benchmark_ICHashT_find5_get_StdString();
void benchmark_ICHashT_find6_get_UINT64pointer();

//void benchmark_ICHashT_erase1();
void benchmark_ICHashT_erase1(int LoopNum);

void benchmark_ICHashT_find1_and_Erase(int LoopNum);
void benchmark_ICHashT_find4_and_Erase(int LoopNum);

//========================================

void benchmark_unorderd_map_MaxLoadFactor(int LoopNum);


//void benchmark_unorderd_map_insert1();
void benchmark_unorderd_map_insert1(int LoopNum);
void benchmark_unorderd_map_insert2_StdString();

void benchmark_unorderd_map_find1();
void benchmark_unorderd_map_find2();
void benchmark_unorderd_map_find3();
void benchmark_unorderd_map_find4();

//void benchmark_unorderd_map_erase1();
void benchmark_unorderd_map_erase1(int LoopNum);
void benchmark_unorderd_map_find5_get_StdString();

void benchmark_unorderd_map_find1_and_Erase(int LoopNum);
void benchmark_unorderd_map_find4_and_Erase(int LoopNum);
