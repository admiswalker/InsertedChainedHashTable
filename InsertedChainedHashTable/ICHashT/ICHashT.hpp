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
 *  ���{���
 *  
 *  The MIT License
 *  Copyright (c) 2017 ADMIS_Walker
 *  
 *  �ȉ��ɒ�߂�����ɏ]���C�{�\�t�g�E�F�A����ъ֘A�����̃t�@�C��
 *  �i�ȉ��u�\�t�g�E�F�A�v�j�̕������擾���邷�ׂĂ̐l�ɑ΂��C�\�t�g�E�F�A�𖳐���
 *  �Ɉ������Ƃ𖳏��ŋ����܂��D����ɂ́C�\�t�g�E�F�A�̕������g�p�C���ʁC�ύX�C
 *  �����C�f�ځC�Еz�C�T�u���C�Z���X�C�����/�܂��͔̔����錠���C����у\�t�g�E�F�A
 *  ��񋟂��鑊��ɓ������Ƃ������錠�����������Ɋ܂܂�܂��D
 *  
 *  ��L�̒��쌠�\������і{�����\�����C�\�t�g�E�F�A�̂��ׂĂ̕����܂��͏d�v�ȕ���
 *  �ɋL�ڂ�����̂Ƃ��܂��D
 *  
 *  �\�t�g�E�F�A�́u����̂܂܁v�ŁC�����ł��邩�Öقł��邩���킸�C����̕ۏ؂�
 *  �Ȃ��񋟂���܂��D�����ł����ۏ؂Ƃ́C���i���C����̖ړI�ւ̓K�����C����ь���
 *  ��N�Q�ɂ��Ă̕ۏ؂��܂݂܂����C����Ɍ��肳�����̂ł͂���܂���D ��҂܂�
 *  �͒��쌠�҂́C�_��s�ׁC�s�@�s�ׁC�܂��͂���ȊO�ł��낤�ƁC�\�t�g�E�F�A�ɋN��
 *  �܂��͊֘A���C���邢�̓\�t�g�E�F�A�̎g�p�܂��͂��̑��̈����ɂ���Đ������؂�
 *  �����C���Q�C���̑��̋`���ɂ��ĉ���̐ӔC������Ȃ����̂Ƃ��܂��D
 */

#pragma once

#include <string>

//=====================================================================================================================================================

// Debug options
#define ICHashT_DebugMode				// ���̍s���R�����g�A�E�g����Ă��Ȃ��ꍇ�́C�f�o�b�O�p�̃R�[�h�𐶐����܂��D

// ���̃I�v�V�����ł́Creturn_Hash �� return_assigned_Hash ���w�肷��D(ICHashT_DebugFunctions.cpp, ICHashT_DebugFunctions.hpp ���g�p����ۂɗL���ɂ���K�v������)
// ���ꂪ��`����Ă���ꍇ�Ɍ���AmakeHash(); �֐��́A[return_Hash] �̒l�Ƃ��āA��� [return_assigned_Hash] ��Ԃ��B
//#define ReturnAssignedHash_for_debug

// Hash table option
#define UnLimit_AllowedSumOfShift		// AllowedSumOfShift �ŁC���v�V�t�g���𐧌����Ȃ�

// Hash �֐��� FNV-1 Hash ���g�p����D(������w�肵�Ȃ��ꍇ�́Cstd::hash ���g�p�����)
#define Use_FNV1_Hash

//=====================================================================================================================================================

#ifndef Use_FNV1_Hash
	#include <functional>	//[std::hash]�ɕK�v
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

//typedef UINT64 KeyType;	//���[�U�[���K����int�^�Ƃ�char�^���g������h�����߁C�ʖ����`���Ă����D

typedef UINT64 AllScanPlace;	//���[�U�[���K����int�^�Ƃ�char�^���g������h�����߁C�ʖ����`���Ă����D
#ifdef ICHashT_DebugMode
	// �f�o�b�O�p�̐ݒ�
typedef UINT64 DumpAll_Place;	//���[�U�[���K����int�^�Ƃ�char�^���g������h�����߁C�ʖ����`���Ă����D
#endif//ICHashT_DebugMode


struct HashStruct{
	UINT64 key;					// key
	void* value;				// value
	unsigned char previous;		// �O�̗v�f�ւ̃V�t�g���D�`�F�[���̐擪�̏ꍇ��[ previous ==   0 ]�D�܂��C[ previous == 255 ]�Ƃ��邱�ƂŁC�v�f����ł��邱�Ƃ������D
	unsigned char next;			// ���̗v�f�ւ̃V�t�g���D
											// ��������C�E�V�t�g����������Ă��Ȃ��D
											// �V�t�g0�Ŏ������g�������D
											// unsigned char�^�Œ�`���Ă���̂ŁC�ő��255��̉E�V�t�g���\�D
											// �V�t�g������傫�����Ύ��قǃf�[�^�̑}���͗e�ՂɂȂ邪�C
											// �V�t�g������傫�����Ύ��قǃL���b�V���~�X�ɂ���đ��x���ቺ����\�����傫���Ȃ�D
};


typedef class InsertedChainedHashTable{
private:
	struct HashStruct* HashTable;		//malloc�Ŋm�ۂ��ꂽ�n�b�V���e�[�u���{�̂̃A�h���X���L�^����D

	UINT64 NumOfElementsInTheTable;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��D(�e�[�u���̏[�U�����v�Z����ۂɗp����D)

	UINT64 TableLength;			//��������̃e�[�u���T�C�Y
	UINT64 AllowedSumOfShift;	//�`�F�[���̍��v�V�t�g�������D
	unsigned char AllowedRightShiftNum;	//�E�V�t�g��������ő�l�̎w��D(0�`254�܂ŁD)
										//���̃T�C�Y�������C�n�b�V���l�d���ɔ����āC�B���e�[�u�����m�ۂ��Ă����D(�V�t�g�͉E�ɂ��������Ă��Ȃ����߁C�e�[�u���̈�ԍŌ�̃n�b�V���l���d�������ꍇ�ɗv�f��ۑ�����悪�������߁D)
										//[ previous == 255 ]�́C�z�񂪋�ł��鎖��\���\��ς݂̐��l�D���̂��߁C�V�t�g����[0�`254]�܂łɐ��������D
										//[ next     == 255 ]�́Cprevious��0�`254�܂łɐ�������Ă���ȏ�C�g���Ȃ��D
	UINT64 TrueTableLength;	//[ TableLength + AllowedRightShiftNum ]�ƂȂ�D

	double LoadFactor;	//�e�[�u���̎g�p���C���׌W���C(���ȗ��p��)
						//[ LoadFactor = NumOfElementsInTheTable / TrueTableLength; ]�Ōv�Z�����D
						//(���̏��C)����g�p�����ł͂Ȃ��̂ŁC��ɍŐV�̒l���L�^����Ă����ł͖����D(�K�v�ȏꍇ�̒��O�ł̂݌v�Z�D)
	void SetLoadFactor();	//�e�[�u����L���̌v�Z

	unsigned char PrimeList_Num;		//�e�[�u���T�C�Y���C�z��uSmallestPrimeList_LargerThanPower2�v�̉��Ԗڂ̗v�f�����L�^����D�uSmallestPrimeList_LargerThanPower2�v�̗v�f����64�Ȃ̂ŁCPrimeList_Num�́C0�`63�܂ŁD

	void makeHash( UINT64& key, UINT64& return_Hash, UINT64& TableLength_makeHash );
	void reHash();						// ���n�b�V�� //�n�b�V���e�[�u���̍č\���D�e�[�u���T�C�Y�����悻2�{�Ɋg�債�čč\������D(�u���悻�v�Ȃ̂́C�e�[�u���T�C�Y��f���ɂ���֌W��D)(2�{�ȏ�ł��ǂ����C2�{�ȉ����ƁC��v�f���Ƃ̉��Z�ʂɊ��Z���Ă��萔���ԂŏI���Ȃ��Ȃ�̂Ń}�Y�C�D�����͈�x�Ɋg�傷��T�C�Y���傫����Α傫���قǌ��シ��D(�������C�m�ۂ��ꂽ�v�f���g���Ȃ��̂ł���Ζ��ʂȂ̂ŁC��͂�2�{���g�傷�鎖�ɂ���D))

	void mallocTable( struct HashStruct*& HashTable_p_for_malloc, UINT64& TrueTableLength_mallocTable );
	void freeTable  ( struct HashStruct*& HashTable_p_for_free );

	void setX_key_value_0_0( struct HashStruct& HashTable_setX, UINT64& key, void*& value );														// setX() �ɕK�v
	void setX_Move_key_value( struct HashStruct& HashTable_empty, struct HashStruct& HashTable_first );												// setX() �ɕK�v
	void setX_SameProcessFor4to7( struct HashStruct*& HashTable, UINT64& first, UINT64& empty, UINT64& empty_front, UINT64& TrueTableLength_setX );	// setX() �ɕK�v
	void setX          ( char& set_function_type, UINT64& key, void*& value, void**& return_value_pp, bool& result, bool& NeedReHash, struct HashStruct*& HashTable_setX, UINT64& TableLength_setX, UINT64& TrueTableLength_setX );	// [set0]�`[set2]�֐���[set_function_type]�ɂ���Đ؂�ւ���D(�ʁX�ɏ����ƁC�o�O��������C�@�\�ύX���s���ꍇ�ɁC���Ɏ��(�Ǘ��R�X�g)��������̂ŁD)
	void setXwithReHash( char  set_function_type, UINT64& key, void*& value, void**& return_value_pp, bool& result,                   struct HashStruct*& HashTable_setX, UINT64& TableLength_setX, UINT64& TrueTableLength_setX );	// setX();�֐��Ƀ��n�b�V���@�\��t�������́D(�ċA������������邽�߁D)

	void getX          ( char  set_function_type, UINT64& key, void*& value, void**& pp_value,        bool& result );

	void eraseX        ( char  set_function_type, UINT64& key, bool& result, void*&  return_value_p );

//	bool CompareKEY( KeyType key );	//key�̔�r�����̂݊֐����D(key�̃^�C�v��ύX����ꍇ�ɁC�v���ɕύX�ł���悤�ɁC�������܂Ƃ߂�D)
									//�����Ə����΁Ckey�̌^�̕ύX���C���̊֐��ƁC[typedef UINT64 KeyType;]�̕ύX�݂̂ŉ\�ƂȂ�D�͂��D
									//inline�W�J���ƁC�ꍇ�ɂ���ēW�J����Ȃ��̂ŁC#define�}�N���̕��������̂�???
public:
	InsertedChainedHashTable();								//���̎��_�ł́C�utypedef�v����Ă��炸�C�uICHashT�v�͕s�Ȃ̂Œ��ӁD
	InsertedChainedHashTable( UINT64 TableLength_set );		//�I�[�o�[���[�h(���d��`)�����D
	~InsertedChainedHashTable();							//���̎��_�ł́C�utypedef�v����Ă��炸�C�uICHashT�v�͕s�Ȃ̂Œ��ӁD

	//�S�v�f�����p�̕ϐ��E�֐�
	AllScanPlace AllScanEnd;
	void AllScanInit     (                            AllScanPlace& AllScanNowPlace );	// AllScanNowPlace���ŏ��̓ǂݍ��ݏꏊ�ɏ���������D
	void AllScanNextShift(                            AllScanPlace& AllScanNowPlace );	// AllScanNowPlace�����̓ǂݍ��ݏꏊ�ɏ���������D
	void AllScanGetData  ( UINT64& key, void*& value, AllScanPlace& AllScanNowPlace );	// ���݂̓ǂݍ��݈ʒuAllScanNowPlace�ɂ���f�[�^��ǂݍ��ށD
		//[AllScanPlace�ϐ�]���\���̂ɂ��āCkey value����������΁C�C�e���[�^���g�����ꍇ�̂悤�ɁC[AllScanGetData();�֐�]���C[AllScanNextShift();�֐�]���ɖ��ߍ��݁C�s�������炷���Ƃ��ł��邪�C���̏ꍇ�C�����ɂ���ẮC�p�ӂ������̍\���̂���C����ɁC�ړI�̕ϐ���\���̂փf�[�^���R�s�[����K�v��������̂ŁC��߂�D

	void reHash_to_fit();				// �e�[�u���T�C�Y���[�U����50%���x�ƂȂ�悤�Ƀ��n�b�V���D(�e�[�u���T�C�Y���k�������鎖���ł���D)
										// �������C����̃e�[�u���T�C�Y���C�[�U����50%�ȉ��ŁC�ł�50%�ɋ߂�(�f���\��ɂ���I���\��)�e�[�u���T�C�Y�̏ꍇ�́C����������return;
										// �e�[�u����̗v�f��啝�ɍ폜���āC�e�[�u�����k���������ꍇ�Ȃǂɗp����D

	void Reconstruction_to_fit();		//���z�I�ȃe�[�u���T�C�Y�ƂȂ�悤�ɁC�����I�Ƀ��n�b�V���D

	void ReconstrucTheTable();			// �e�[�u���̍č\���D�����T�C�Y�̃e�[�u���Ƃ��ăe�[�u�����č\������D
										// �e�[�u���̃`�F�[���̒f�Љ����������ꍇ�ɗp����(���Ƃ�z��)�D

	void set0( UINT64& key, void*& value );												// �ł���ʓI�ȓ��������֐�
	void set1( UINT64& key, void*& value );												// ����֐�
	void set2( UINT64& key, void*& value, void**& return_value_pp, bool& result );		// ����֐�
		//set0():key�����Ɋi�[����Ă���ꍇ�́Cresult==false���ԋp����Cvalue���㏑������return;
		//set1():key�����Ɋi�[����Ă���ꍇ�́Cresult==false���ԋp����C����������return;
		//set2():key�����Ɋi�[����Ă���ꍇ�́Cresult==false���ԋp����Cvalue���擾����return;(value�ɂ͎�������Ȃ����Cvoid�|�C���^�Ȃ̂ŁC���g�̔��f�Ə����������ł���D)
		//setXwithNoReHash():���n�b�V������������܂ł̏������x�v���p�ɒ�`�����֐�
		//����������C������������΁C���̂܂ܒl����������return;
#ifdef ICHashT_DebugMode
	void setXwithNoReHash( UINT64& key, void*& value, void**& return_value_pp, bool& result, bool& NeedReHash );	//�f�o�b�O�p
	void set_for_Debug   ( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ); //�w�肵���z��ԍ��ɁC�C�ӂ̃f�[�^���Z�b�g����D
#endif//ICHashT_DebugMode

	void get0( UINT64& key, void* &    value, bool& result );				// ��ʓI�ȓ��������֐�	// �w�肵�� key �ɑΉ����� value ���擾����D
	void get1( UINT64& key, void**& pp_value, bool& result );				// ����֐�					// �w�肵�� key �ɑΉ����� value ���擾����D���̎��C�����������ł���悤�ɁC�_�u���|�C���^�œn���D

	void find( UINT64& key,                   bool& result );				// �w�肵�� key �ɑΉ����� value �����݂��邩�ǂ������m�F����D

	void erase0( UINT64& key, bool& result );								// ��ʓI�ȓ��������֐�	// key �y�� value �̍폜(�������Ώۂ�key����������Ȃ������ꍇ��false��Ԃ��D)
	void erase1( UINT64& key, bool& result, void*& return_value_p );		// ����֐�					// key �y�� value �̍폜(�������Ώۂ�key����������Ȃ������ꍇ��false��Ԃ��D)
		// erase0(); :: �w�肵��key�̗v�f���폜���邾���D
		// erase1(); :: �w�肵��key�̗v�f���폜����Ɠ����ɁC key �ƃy�A�ɂȂ��Ă��� value ���擾����D
		//              ( �f�[�^���폜����ۂɁCvalue�̐�Ƀ����������I�Ɋm�ۂ���Ă���C���������������K�v������ꍇ�C
		//                �܂��C�f�[�^���擾������ɕK���f�[�^���폜����ꍇ�C��z��D                                    )

	void AllClear();	//�e�[�u��������������D(�e�[�u����̃f�[�^�͑S�č폜�����D)

	//�e�[�u����L���̌v�Z
	UINT64 GetTableLength();		//�e�[�u�������m�F���邱�ƂŁC���n�b�V�����������Ă��邩�ǂ����C�m�F���邱�Ƃ��ł���D
	UINT64 GetTrueTableLength();	//�e�[�u�������m�F���邱�ƂŁC���n�b�V�����������Ă��邩�ǂ����C�m�F���邱�Ƃ��ł���D
	UINT64 GetNumOfElementsInTheTable();
	double GetLoadFactor();

	//=====================================================================================================================================================
#ifdef ReturnAssignedHash_for_debug

	// set(); �֐� �y�сCget(); �֐��Cerase(); �֐��̃f�o�b�O�p
	UINT64 return_assigned_Hash;	// makeHash();�֐��̕Ԃ�[return_Hash]�̒l���w�肷��B

#endif//ReturnAssignedHash_for_debug
	//=====================================================================================================================================================
#ifdef ICHashT_DebugMode
	// �f�o�b�O�p�̊֐�

	// �n�b�V���e�[�u����ŋ�̗v�f���܂߂��S�Ă̗v�f�̃_���v
	DumpAll_Place DumpAll_End;
	std::string DumpWithRange( UINT64& start, UINT64& end );
	void DumpAll_for_Debug_Init   (                                                                          DumpAll_Place& DumpAll_NowPlace );	// DumpAll_NowPlace���J�n�ꏊ�ɏ��������CDumpAll_End���I���ꏊ�ɐݒ肷��D
	void DumpAll_for_Debug_GetData( UINT64& key, void*& value, unsigned char& previous, unsigned char& next, DumpAll_Place& DumpAll_NowPlace );	// ����DumpAll_NowPlace�C�Ŏw�肳��Ă���l�̓ǂݍ��݁D
	void DumpALL();
	void DumpAllToTheText( const char* SaveDirctory );																							// �n�b�V���e�[�u�����ǂȌ`���Ńe�L�X�g�t�@�C���ɕۑ�����D

#ifndef UnLimit_AllowedSumOfShift
	UINT64 GetAllowedSumOfShift();
#endif
	unsigned char GetAllowedRightShiftNum();
#endif//ICHashT_DebugMode
	//=====================================================================================================================================================

//	void SaveTableToTheFile();		//�n�b�V���e�[�u�����o�C�i���̂܂܃t�@�C���ɕۑ�����D
//	void ReadTableFromTheFile();	//�o�C�i���̂܂ܕۑ������n�b�V���e�[�u�����t�@�C������ǂݍ��ށD
			//���₢��CSave�������Ă��Ckey�͓e���pvalue�͂�����void*�����畁�ʂɂ͖����D
}ICHashT;


//�f���\
//prime(X)��X�Ԗڂ̑f���Ƃ���Ƃ��C1<=n<=33 �ɂ��āC
// prime(X-1) < 2^n <= prime(X)
//�𖞂����f�� prime(X) ��\�ɂ��Ă���D
//
//	Smallest prime >= 2^n.
const UINT64 SmallestPrimeList_LargerThanPower2[64] = {
	//[ull]�́C[unsigned long long]�̈Ӗ�(�t���Ă����Ȃ���GCC�ł̃R���p�C���Ōx�����o��D)
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
//	18446744073709551629ull	,	// 2^64 + 13  = 18446744073709551629ull	//[UINT64]�͈̔͂𒴂���傫���Ȃ̂ŕs�D���܂�C[UINT64]�̍ő�l - �V�t�g���� ��菬�����ő�̑f�������蓖�Ă�K�v������D�c�c���C���̑f����������Ȃ��̂ŁC�p�X�D
	18446744073709551360ull		// 2^64 - 254 = 18446744073709551360ull
								// �����     �u18446744073709551360�v�ƂȂ�̂ŁC���炩�ɑf���ł͂Ȃ�(10�Ŋ����)���C��L�̒ʂ�C�f����������Ȃ��̂ŁC�P���Ɂu[UINT64]�̍ő�l - �V�t�g�����̍ő�l�v�����蓖�ĂĂ���D
								// 2^64�ɂ��ẮC��ŏ��������D(�����炭�����őf�����v�Z���Ȃ�����s�\�D)
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
 * ���z (y ��: table length, x ��: this table shows load factors)
 * (�ڍׂ́C[HyperParameters.xlsx] ���Q��)
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
