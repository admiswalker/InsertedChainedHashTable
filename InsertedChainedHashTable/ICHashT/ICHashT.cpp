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

//=====================================================================================================================================================
//InsertedChainedHashTable
//=====================================================================================================================================================
#include "ICHashT.hpp"

//�R���X�g���N�^
ICHashT::InsertedChainedHashTable(){

	PrimeList_Num        = 9;													//reHash()�ŃA�N�Z�X�����̂ŁC�K�����������Ă����D
	
	NumOfElementsInTheTable = 0;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��D(�e�[�u���̏[�U�����v�Z����ۂɗp����D)

	TableLength          = SmallestPrimeList_LargerThanPower2[PrimeList_Num];	//�f�t�H���g�̃e�[�u���T�C�Y(1031)
#ifndef UnLimit_AllowedSumOfShift
	AllowedSumOfShift    = 254;								//���v�V�t�g������	//�������E�V�t�g�̍��v�l�ɂ��āC�ő�l�̎w��D
#endif
	AllowedRightShiftNum = GetHyperParameters( PrimeList_Num );//254;			//�E�V�t�g��������ő�l�̎w��D(0�`254�܂ŁD)
	TrueTableLength      = TableLength + AllowedRightShiftNum;					//���ۂɊm�ۂ���e�[�u�������v�Z
	
	mallocTable(HashTable,TrueTableLength);	//�e�[�u���̊m�ہC�y�я�����
}
//�R���X�g���N�^�̃I�[�o�[���[�h(���d��`)
ICHashT::InsertedChainedHashTable( UINT64 TableLength_set ){
	//[TableLength]���傫������ꍇ�̃G���[�����͏ȗ��D( SmallestPrimeList_LargerThanPower2[64]==18446744073709551360 �ȉ��ł���K�v������D)

	PrimeList_Num = 0;															//reHash()�ŃA�N�Z�X�����̂ŁC�K�����������Ă����D
	for(; PrimeList_Num<64; PrimeList_Num++){ //32bit�Ή�����̂ł���΁C���ӂ��������ǂ��̂���?(�f���\��32bit�Ȃ�T�C�Y�����������������ǂ��D)
		if( SmallestPrimeList_LargerThanPower2[PrimeList_Num] >= TableLength_set ){
			break;		//for���𔲂���D
		}
	}
	
	NumOfElementsInTheTable = 0;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��D(�e�[�u���̏[�U�����v�Z����ۂɗp����D)

	TableLength          = SmallestPrimeList_LargerThanPower2[PrimeList_Num];
#ifndef UnLimit_AllowedSumOfShift
	AllowedSumOfShift    = 254;								//���v�V�t�g������	//�������E�V�t�g�̍��v�l�ɂ��āC�ő�l�̎w��D
#endif//UnLimit_AllowedSumOfShift
	AllowedRightShiftNum = GetHyperParameters( PrimeList_Num );//254;			//�E�V�t�g��������ő�l�̎w��D(0�`254�܂ŁD)
	TrueTableLength      = TableLength + AllowedRightShiftNum;					//���ۂɊm�ۂ���e�[�u�������v�Z

	mallocTable(HashTable,TrueTableLength);	//�e�[�u���̊m�ہC�y�я�����
}
//�f�B�X�g���N�^
ICHashT::~InsertedChainedHashTable(){
	freeTable(HashTable);
}

// �w�肳�ꂽ�l����n�b�V���l�𐶐�����֐�: key ���C[0 �` UINT64�̍ő�l] �܂ł͈̔͂̒l (�n�b�V���l) �𐶐�����D
void ICHashT::makeHash( UINT64& key, UINT64& return_Hash, UINT64& TableLength_makeHash ){

#ifdef ReturnAssignedHash_for_debug
	
	// �f�o�b�O�p�̐ݒ�
	return_Hash = return_assigned_Hash;

#else //ReturnAssignedHash_for_debug
	
	// �����[�X�p�̐ݒ�
	#ifdef Use_FNV1_Hash
		// FNV-1 Hash ���g�p
		size_t length = sizeof(UINT64);
		unsigned char*pc_key = (unsigned char*)&key;
		fnv_1_hash_64( return_Hash, (unsigned char*&)pc_key, length );
		return_Hash = return_Hash % TableLength_makeHash;//���ꂾ�ƈ�l���z���Ȃ��̂ł� ??? (HashValue �� TableLength_makeHash ���\���ɑ傫�Ȃ��Ƃ��Âɉ��肵�Ă���)
	#else
		// std::hash ���g�p
		return_Hash = std::hash<UINT64>()( key );		//�n�b�V���l�̐����D[std::hash]�𗘗p����D
		return_Hash = return_Hash % TableLength_makeHash;//���ꂾ�ƈ�l���z���Ȃ��̂ł� ??? (HashValue �� TableLength_makeHash ���\���ɑ傫�Ȃ��Ƃ��Âɉ��肵�Ă���)
	#endif //Use_FNV1_Hash

#endif//ReturnAssignedHash_for_debug

}



void ICHashT::mallocTable( struct HashStruct*& HashTable_p_for_malloc, UINT64& TrueTableLength_mallocTable ){

	HashTable_p_for_malloc = (HashStruct*)malloc((size_t)TrueTableLength_mallocTable*sizeof(HashStruct));	//size_t��[UINT64]��菬�����ꍇ�́C�n�b�V���e�[�u���̃T�C�Y�����������\��������D
																											//���ƁC�{���́C�����Ƃ�size_t�̃T�C�Y���m�F���C�G���[�������s���������ǂ��D
	//malloc�Ɏ��s�����ꍇ�Cfalse��Ԃ��悤�ɂ��������ǂ��D(������������Ȃ��ꍇ�Ɏ��s����D)
	printf_dbger_if_Exit( HashTable_p_for_malloc == NULL, "ERROR: Memory allocation failure.\n" );

	HashStruct HashStruct_for_init;
	HashStruct_for_init.key      = 0;
	HashStruct_for_init.value    = 0;
	HashStruct_for_init.previous = 255;	//[previous==255] �z�񂪋�ł��邱�Ƃ������D
	HashStruct_for_init.next     = 0;
	
	//������(�f�[�^��NULL�ɂ��Ă����Ȃ��ƁC�Q�Ƃ����e�[�u�������Ɏg���Ă���̂��C�}�����Ă����v�Ȃ̂���������Ȃ��D)
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
	HashStruct_for_init.previous = 255;	//[previous==255] �z�񂪋�ł��邱�Ƃ������D
	HashStruct_for_init.next     = 0;

	//������(�f�[�^��NULL�ɂ��Ă����Ȃ��ƁC�Q�Ƃ����e�[�u�������Ɏg���Ă���̂��C�}�����Ă����v�Ȃ̂���������Ȃ��D)
	for(int i=0; i<TrueTableLength; i++){
		HashTable[i] = HashStruct_for_init;
	}
}



//�S�v�f�o�́E�������C�e���[�^��p���Ď������悤�Ǝv�������CC++11�I�ȁC�ŋ߂̋Z�p�ŁC�M�p�Ȃ�Ȃ��̂Ŏg��Ȃ����ɂ����D
void ICHashT::AllScanInit( AllScanPlace& AllScanNowPlace ){
#define AllScanStart 0x0000000000000000
	AllScanNowPlace = (AllScanPlace)AllScanStart;		//�J�[�\���������ʒu�ɃZ�b�g�D
	AllScanEnd      = (AllScanPlace)TrueTableLength;
	AllScanNextShift( AllScanNowPlace );//�������񂾂����s���Ă����Ȃ��ƁC���ڂ�for���ŁC�K���CHashTable[0]��key��value�����������鎖�ɂȂ��Ă��܂��D
#undef AllScanStart
}
void ICHashT::AllScanNextShift( AllScanPlace& AllScanNowPlace ){
	for(; AllScanNowPlace < AllScanEnd; ){
		if(HashTable[AllScanNowPlace].previous != 255){//�z��ɗv�f�����邩�ǂ����̊m�F�D
			return;//�v�f�𔭌������̂�return;
		}
		AllScanNowPlace++;//���̗v�f����������D
	}
}
void ICHashT::AllScanGetData( UINT64& key, void*& value, AllScanPlace& AllScanNowPlace ){
	key   = HashTable[AllScanNowPlace].key;
	value = HashTable[AllScanNowPlace].value;
	AllScanNowPlace++;//for���̓r����return�����̂ŁC���Z����Ă��Ȃ��D���̂��߁C�����ŉ��Z���Ă����D
}
/*
	��L�̎����́C���L�̂悤�� AllScanGetData() �����s�����Ƀ��[�v���񂷂ƁCAllScanNowPlace++; ����Ȃ����߁C�������[�v�Ɋׂ邽�߁C���ӁD
	-> �C�e���[�^�ŏ��������Ă����悤��

	// �S�v�f���� ( �n�b�V���e�[�u����ɋL�^����Ă��邷�ׂĂ� key value �y�A�̎��o���D )
	printf("������ Check AllScan function. ( Start ) ������\n");
	{
		UINT64 counter=0;
		AllScanPlace NowPlace;
		for(ICHashT_p->AllScanInit(NowPlace); NowPlace < ICHashT_p->AllScanEnd; ICHashT_p->AllScanNextShift(NowPlace)){
			counter++;
		}
		printf("counter: %llu\n", counter);
	}
	printf("������ Check AllScan function. (  End  ) ������\n");
*/



void ICHashT::setX_key_value_0_0( struct HashStruct& HashTable, UINT64& key, void*& value ){
	HashTable.key      = key;		// �f�[�^�̏������ݏ��� ( �O���炠��v�f )
	HashTable.value    = value;	// �f�[�^�̏������ݏ��� ( �O���炠��v�f )
	HashTable.previous = 0;		// �f�[�^�̏������ݏ��� ( �O���炠��v�f )
	HashTable.next     = 0;		// �f�[�^�̏������ݏ��� ( �O���炠��v�f )
}
void ICHashT::setX_Move_key_value( struct HashStruct& HashTable_empty, struct HashStruct& HashTable_first ){
	HashTable_empty.key   = HashTable_first.key;	//�f�[�^�̈ړ����� ( �O���炠��v�f )
	HashTable_empty.value = HashTable_first.value;	//�f�[�^�̈ړ����� ( �O���炠��v�f )
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
	//		'0'��set0
	//		'1'��set1
	//		'2'��set2

	//���ɓ����key���o�^����Ă���ꍇ�D��value���X�V

	//[previous==255] �z�񂪋�ł��邱�Ƃ������D
	//[previous!=255] �z��͎g�p�ς݁D�������C�n�b�V���l�̏d�����������߁C�`�F�[���͂���Ă��Ȃ��D
	//[next    ==  0] �v�f�̖���
	//[previous==  0] slave�ł͂Ȃ��ʏ�̗v�f�Ƃ��Ďg���Ă��邱�Ƃ������D(�K���`�F�[���̐擪�ƂȂ�)
	//[ next == 0 && previous == 0 ] �v�f��1�̃`�F�[���̐擪�D
	
	//setX();�֐��̎��s���ʂ��C[NeedReHash==true]�̏ꍇ�ɁC���n�b�V������悤�ɐ݌v����D

	UINT64 first;							//�`�F�[���̐擪�C�̈Ӗ������߂āD
	makeHash(key,first,TableLength_setX);	//�n�b�V���l�̐���

	if( HashTable_setX[first].previous == 255 ){ //[if( HashTable_setX[return_Hash].key == 0 ){]�Ƃ��Ȃ��̂́C[key]���g��[0]�Ƃ����l�����Ȃ��Ȃ��Ă��܂�����D[if( HashTable_setX[return_Hash].value == 0 ){]�Ƃ��Ȃ��̂����l�D(����value��void*�����Cvoid*�͒P�Ȃ�64bit�̃������ɉ߂��Ȃ��̂ŁC���ڒl���i�[����\��������D)
		// �e�[�u�����󂢂Ă����ꍇ�D
		// [�����ԍ��@]
		printf_dbger_if_Exit_Stop( first >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
		printf_dbger_if_Exit_Stop( HashTable_setX[ first ].previous != 255, "ERROR: This element is not empty!\n" );
		setX_key_value_0_0( HashTable_setX[ first ], key, value );

		NeedReHash = false; result = true; NumOfElementsInTheTable++;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D( ���s���Ă���ꍇ�́Ckey���d�����Ă���ꍇ�ŁC�v�f���͕ω����Ȃ��D )
		return;
	}


	//======================================================================================================================================================================================================================================================
	if( HashTable_setX[first].previous ==   0 ){
		// [  HashTable_setX[first] ���Ckey�`�F�[���̐擪�̏ꍇ�D    ]
		// [  �� �ł�key�ɋ߂��z��̋󂫃X�y�[�X�ɁC�v�f���i�[����D ]
		// [                                                         ]
		// [  [�����ԍ��@]�C[�����ԍ��A]�C[�����ԍ��B]               ]

		//*
		// [�������@01::�f���Ȏ���]
		{
			UINT64 NowPlace = first;

			while(1){
				if( HashTable_setX[NowPlace].key == key ){
					// key����v�����ꍇ�D
					// set_function_type == '0'  ��  result = false; value�̏㏑�����s���D
					// set_function_type == '1'  ��  result = false; �����s�킸��return�D
					// set_function_type == '2'  ��  result = false; value�̏㏑�����s��Ȃ�����ɁCvalue�ւ̃|�C���^��Ԃ��D
					if( set_function_type == '0' ){
						HashTable_setX[NowPlace].value = value;
						NeedReHash = false; result = false; return;
					}else if( set_function_type=='1' ){
						NeedReHash = false; result = false; return;
					}else{// if( set_function_type == '2' ){
						return_value_pp = &HashTable_setX[NowPlace].value;	//   return_value_pp :: &(HashTable_setX[return_Hash].value)
																			//  *return_value_pp ::   HashTable_setX[return_Hash].value == void* ������ɑ΂���malloc�Ƃ����ă����������蓖�Ă�\��D
																			// **return_value_pp :: malloc�Ŋm�ۂ��Ă���΁C���̂�����D
																			//[free((void*)(*return_value_pp));]��malloc�Ŋm�ۂ�����������������̂ł���΁D
						NeedReHash = false; result = false; return;								//set2():key�����Ɋi�[����Ă���ꍇ�́Cvalue���擾����return;(value�ɂ͎�������Ȃ����Cvoid�|�C���^�Ȃ̂ŁC���g�̔��f�Ə����������ł���D)�����g���r���肵�Ă��珑�����������ꍇ�ɕ֗��D
					}
				}
				UINT64 NextShift = HashTable_setX[NowPlace].next;

				if(NextShift==0){break;}

				NowPlace += HashTable_setX[NowPlace].next;
			}
		}
	

		//key�̈�v�����������ꍇ�D
		//�󂫂̑���
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

					for( unsigned char i=1; i<NextShift; i++ ){ //���ݒn[return_Hash]��1�V�t�g�ł���C[return_Hash+1]����C[return_Hash+NextShift]�̈��O�ł���C[return_Hash+NextShift-1]�܂ł̊ԂɁC��̔z�񂪖������ǂ����𑖍�����D
						if( HashTable_setX[NowPlace+i].previous == 255 ){
							// �󂢂Ă���z��𔭌��D
							// [�����ԍ��A]
							UINT64 empty_front = NowPlace;
							UINT64 empty       = NowPlace + i;
							UINT64 empty__back = NowPlace + NextShift;

							printf_dbger_if_Exit_Stop(empty>=TrueTableLength_setX, "ERROR: Out of Range!\n");
							printf_dbger_if_Exit_Stop(HashTable_setX[empty].previous!=255, "ERROR: This element is not empty!\n");
							HashTable_setX[ empty       ].key      = key;			// �f�[�^�̏������ݏ���
							HashTable_setX[ empty       ].value    = value;			// �f�[�^�̏������ݏ���

							printf_dbger_if_Exit_Stop( i>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
							HashTable_setX[ empty_front ].next     = i;				// next�ڑ����q���ւ�     ( �O���炠��v�f )
							HashTable_setX[ empty       ].previous = i;				// previous�ڑ���ݒ�     (   �V�����v�f   )
							printf_dbger_if_Exit_Stop( (NextShift - i)>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. (NextShift - i): %llu, NextShift: %llu, i: %llu.", (NextShift - i), NextShift, i );
							HashTable_setX[ empty       ].next     = NextShift - i;	// next�ڑ���ݒ�         (   �V�����v�f   )
							HashTable_setX[ empty__back ].previous = NextShift - i;	// previous�ڑ����q���ւ� ( �O���炠��v�f )

							NeedReHash = false; result = true; NumOfElementsInTheTable++;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D( ���s���Ă���ꍇ�́Ckey���d�����Ă���ꍇ�ŁC�v�f���͕ω����Ȃ��D )
							return;
						}
					}

					NowPlace += NextShift;
				}
			}


			//key�`�F�[���̍Ō�܂ő������Ă��C�`�F�[���̊Ԃɋ󂫂����������ꍇ�D
			//key�`�F�[���̌��(last�ȍ~)�ɁC�󂫂�T��
			{
				UINT64 ShiftLimit = AllowedRightShiftNum;

				UINT64 LimitOfTableSize = TrueTableLength_setX - last - 1;	//����̊m�F�́Ckey���`�F�[������Ă���ƁC�������V�t�g�\�Ȃ̂ŁC�K�{�D

				// (�R���p�C���ɂ���邪) for �����Ŕ�r����ƌv�Z�R�X�g��������̂ŁC���������炷�D
				if( ShiftLimit > LimitOfTableSize ){ ShiftLimit = LimitOfTableSize; }

			#ifndef UnLimit_AllowedSumOfShift
			//	UINT64 SumOfShift = last - first;
				UINT64 LeftSumOfShift = AllowedSumOfShift - (last - first); // �����ꂽ���v�V�t�g���̎c��
				if( ShiftLimit > LeftSumOfShift ){ ShiftLimit = LeftSumOfShift; }
			#endif

				for( UINT64 i=1; i <= ShiftLimit; i++ ){		//[for( UINT64 i=1; (SumOfShift+i) <= AllowedRightShiftNum; i++ ){]�Ə����ƁCVisualStudio�ł́C���񑫂��Z���v�Z���n�߂Ă��܂��D[for( UINT64 i=1; i <= (AllowedRightShiftNum-SumOfShift); i++ ){]�Ƃ���΁C��񂾂������Z���s���D
					UINT64 empty = last + i;

					if( HashTable_setX[ empty ].previous == 255 ){
						// �󂢂Ă���z��𔭌��D
						// [�����ԍ��B]
						
						UINT64 empty_front = last;
						printf_dbger_if( empty >= TrueTableLength_setX, "ERROR: last: %llu, i: %llu, ShiftLimit: %llu, LimitOfTableSize: %llu,\n", last, i, ShiftLimit, LimitOfTableSize );
						printf_dbger_if( empty >= TrueTableLength_setX, "ERROR: AllowedSumOfShift: %llu, last: %llu, first: %llu, ShiftLimit: %llu\n", AllowedSumOfShift, last, first, ShiftLimit );
					#ifndef UnLimit_AllowedSumOfShift
						printf_dbger_if( empty >= TrueTableLength_setX, "ERROR: AllowedSumOfShift: %llu, last: %llu, first: %llu, LeftSumOfShift: %llu, ShiftLimit: %llu\n", AllowedSumOfShift, last, first, LeftSumOfShift, ShiftLimit );
					#endif
						printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
						printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );

						HashTable_setX[ empty       ].key      = key;	// �f�[�^�̏������ݏ��� (   �V�����v�f   )
						HashTable_setX[ empty       ].value    = value;	// �f�[�^�̏������ݏ��� (   �V�����v�f   )

						printf_dbger_if_Exit_Stop( i>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
						HashTable_setX[ empty_front ].next     = (unsigned char)i;		// next�ڑ����q���ւ�   ( �O���炠��v�f )
						HashTable_setX[ empty       ].previous = (unsigned char)i;		// �f�[�^�̏������ݏ��� (   �V�����v�f   )
						HashTable_setX[ empty       ].next     = 0;		// �f�[�^�̏������ݏ��� (   �V�����v�f   )

						NeedReHash = false; result = true; NumOfElementsInTheTable++;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D( ���s���Ă���ꍇ�́Ckey���d�����Ă���ꍇ�ŁC�v�f���͕ω����Ȃ��D )
						return;
					}
				}

				//key�̑}���Ɏ��s�D( [���v�V�t�g����] �𒴉� �� ���n�b�V���̕K�v������D )
				//printf_dbger("NeedReHash: 3\n");
				NeedReHash = true; result = false; return;
			}
		}
		//*/


		/*
		//(����m�F���ĂȂ����C�����ĂȂ����瓮���Ȃ��n�Y�D)

		// [�������@02::����Ȏ���]
		//�󂫂̑����ƁCkey�̈�v�𓯎��i�s�Ŋm�F����D�������C��ɋ󂫂��������ꂽ�ꍇ�́Ckey�̈�v���󂫂܂ł����m�F����Ȃ��̂ŁC�Ō�܂�key�̈�v�����������m�F���Ă��������Ă���D
		//�󂫂̑���
		{
			UINT64 last;
			UINT64 empty = 0;	//0�͎g���Ȃ��̂ŁC0�ŏ��������Ă����Ȃ��D//[empty]�̓f�[�^�\���I�ɁC�K��1�ȏ�̒l�����D(HashTable_setX[0]��empty�ł��邱�Ƃ́C�f�[�^�\����s�\�D)
			{
				UINT64 NowPlace = first;

				while(1){
					if( HashTable_setX[NowPlace].key == key ){ //key�̈�v�̊m�F�D
						// key����v�����ꍇ�D
						// set_function_type=='0'  ��  result = false; value�̏㏑�����s���D
						// set_function_type=='1'  ��  result = false; �����s�킸��return�D
						// set_function_type=='2'  ��  result = false; value�̏㏑�����s��Ȃ�����ɁCvalue�ւ̃|�C���^��Ԃ��D
						if( set_function_type=='0' ){
							HashTable_setX[NowPlace].value = value;
							NeedReHash = false; result = false; return;
						}else if(set_function_type=='1'){
							NeedReHash = false; result = false; return;
						}else{// if(set_function_type=='2'){
							return_value_pp = HashTable_setX[NowPlace].value;	//   return_value_pp :: &(HashTable_setX[return_Hash].value)
																				//  *return_value_pp ::   HashTable_setX[return_Hash].value == void* ������ɑ΂���malloc�Ƃ����ă����������蓖�Ă�\��D
																				// **return_value_pp :: malloc�Ŋm�ۂ��Ă���΁C���̂�����D
																				//[free((void*)(*return_value_pp));]��malloc�Ŋm�ۂ�����������������̂ł���΁D
							NeedReHash = false; result = false; return;								//set2():key�����Ɋi�[����Ă���ꍇ�́Cvalue���擾����return;(value�ɂ͎�������Ȃ����Cvoid�|�C���^�Ȃ̂ŁC���g�̔��f�Ə����������ł���D)�����g���r���肵�Ă��珑�����������ꍇ�ɕ֗��D
						}
					}
					UINT64 NextShift = HashTable_setX[NowPlace].next;

					if(NextShift==0){
						last = NowPlace;
						break;
					}

					for( UINT64 i=1; i<NextShift; i++ ){ //���ݒn[return_Hash]��1�V�t�g�ł���C[return_Hash+1]����C[return_Hash+NextShift]�̈��O�ł���C[return_Hash+NextShift-1]�܂ł̊ԂɁC��̔z�񂪖������ǂ����𑖍�����D
						if( HashTable_setX[NowPlace+i].previous == 255 ){
							//�󂢂Ă���z��𔭌��D

							//��v����key���������Ƃ��C�`�F�[���̍Ō�܂Ŋm�F����D
							{
								UINT64 NextPlace = NowPlace + NextShift;

								while(1){
									if( HashTable_setX[NextPlace].key == key ){
										// key����v�����ꍇ�D
										// set_function_type=='0'  ��  result = false; value�̏㏑�����s���D
										// set_function_type=='1'  ��  result = false; �����s�킸��return�D
										// set_function_type=='2'  ��  result = false; value�̏㏑�����s��Ȃ�����ɁCvalue�ւ̃|�C���^��Ԃ��D
										if( set_function_type=='0' ){
											HashTable_setX[NowPlace].value = value;
											NeedReHash = false; result = false; return;
										}else if( set_function_type=='1' ){
											NeedReHash = false; result = false; return;
										}else{// if(set_function_type=='2'){
											return_value_pp = HashTable_setX[NowPlace].value;	//   return_value_pp :: &(HashTable_setX[return_Hash].value)
																								//  *return_value_pp ::   HashTable_setX[return_Hash].value == void* ������ɑ΂���malloc�Ƃ����ă����������蓖�Ă�\��D
																								// **return_value_pp :: malloc�Ŋm�ۂ��Ă���΁C���̂�����D
																								//[free((void*)(*return_value_pp));]��malloc�Ŋm�ۂ�����������������̂ł���΁D
											NeedReHash = false; result = false; return;								//set2():key�����Ɋi�[����Ă���ꍇ�́Cvalue���擾����return;(value�ɂ͎�������Ȃ����Cvoid�|�C���^�Ȃ̂ŁC���g�̔��f�Ə����������ł���D)�����g���r���肵�Ă��珑�����������ꍇ�ɕ֗��D
										}
									}
									UINT64 NextShift = HashTable_setX[NextPlace].next;

									if(NextShift==0){break;}	//��v����key�����������̂ŁCwhile()���𔲂���D

									NextPlace += HashTable_setX[NextPlace].next;
								}
							}

							//��v����key�����������m�F�����̂ŁC�l��z��̋󂫂Ɋi�[����D
							// [�����ԍ��A]
							UINT64 empty = NowPlace + i;

							printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
							printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
							HashTable_setX[ empty                ].key      = key;				// �f�[�^�̏������ݏ���
							HashTable_setX[ empty                ].value    = value;			// �f�[�^�̏������ݏ���

							printf_dbger_if_Exit_Stop( i>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
							HashTable_setX[ NowPlace             ].next     = i;				// next�ڑ����q���ւ�     ( �O���炠��v�f )

							printf_dbger_if_Exit_Stop( (NextShift - i)>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. (NextShift - i): %llu, NextShift: %llu, i: %llu.", (NextShift - i), NextShift, i );
							HashTable_setX[ empty                ].previous = i;				// previous�ڑ���ݒ�     (   �V�����v�f   )
							HashTable_setX[ empty                ].next     = NextShift - i;	// next�ڑ���ݒ�         (   �V�����v�f   )

							HashTable_setX[ NowPlace + NextShift ].previous = NextShift - i;	// previous�ڑ����q���ւ� ( �O���炠��v�f )

							NeedReHash = false; result = true; NumOfElementsInTheTable++;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D( ���s���Ă���ꍇ�́Ckey���d�����Ă���ꍇ�ŁC�v�f���͕ω����Ȃ��D )
							return;
						}
					}

					NowPlace += HashTable_setX[NowPlace].next;
				}
			}


			//key�`�F�[���̍Ō�܂ő������Ă��C�`�F�[���̊Ԃɋ󂫂����������ꍇ�D
			//key�`�F�[���̌��(last�ȍ~)�ɁC�󂫂�T��
			{
				UINT64 ShiftLimit = AllowedRightShiftNum;

				UINT64 LimitOfTableSize = TrueTableLength_setX - last - 1;	//����̊m�F�́Ckey���`�F�[������Ă���ƁC�������V�t�g�\�Ȃ̂ŁC�K�{�D

				// (�R���p�C���ɂ���邪) for �����Ŕ�r����ƌv�Z�R�X�g��������̂ŁC���������炷�D
				if( ShiftLimit > LimitOfTableSize ){ ShiftLimit = LimitOfTableSize; }

			#ifndef UnLimit_AllowedSumOfShift
			//	UINT64 SumOfShift = last - first;
				UINT64 LeftSumOfShift = AllowedSumOfShift - (last - first); // �����ꂽ���v�V�t�g���̎c��
				if( ShiftLimit > LeftSumOfShift ){ ShiftLimit = LeftSumOfShift; }
			#endif

				for( UINT64 i=1; i <= ShiftLimit; i++ ){		//[for( UINT64 i=1; (SumOfShift+i) <= AllowedRightShiftNum; i++ ){]�Ə����ƁCVisualStudio�ł́C���񑫂��Z���v�Z���n�߂Ă��܂��D[for( UINT64 i=1; i <= (AllowedRightShiftNum-SumOfShift); i++ ){]�Ƃ���΁C��񂾂������Z���s���D
					UINT64 empty = last + i;

					if( HashTable_setX[ empty ].previous == 255 ){
						// �󂢂Ă���z��𔭌��D
						// [�����ԍ��B]
						printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
						printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
						HashTable_setX[ empty ].key      = key;		// �f�[�^�̏������ݏ��� (   �V�����v�f   )
						HashTable_setX[ empty ].value    = value;	// �f�[�^�̏������ݏ��� (   �V�����v�f   )
						HashTable_setX[ empty ].previous = i;		// �f�[�^�̏������ݏ��� (   �V�����v�f   )
						HashTable_setX[ empty ].next     = 0;		// �f�[�^�̏������ݏ��� (   �V�����v�f   )

						printf_dbger_if_Exit_Stop( i>254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
						HashTable_setX[ last  ].next     = i;		// next�ڑ����q���ւ�   ( �O���炠��v�f )

						NeedReHash = false; result = true; NumOfElementsInTheTable++;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D( ���s���Ă���ꍇ�́Ckey���d�����Ă���ꍇ�ŁC�v�f���͕ω����Ȃ��D )
						return;
					}
				}

				//key�̑}���Ɏ��s�D( [���v�V�t�g����] �𒴉� �� ���n�b�V���̕K�v������D )
				NeedReHash = true; result = false; return;
			}
		}
		//*/
	//======================================================================================================================================================================================================================================================
	}else{
		// [  HashTable_setX[first] ���Ckey�`�F�[���̐擪�ł͂Ȃ��ꍇ�D                                                                           ]
		// [  �� HashTable_setX[first] �Ɋi�[����Ă���key�`�F�[�����CHashTable_setX[first] �ȊO�ŁC�ł�key�ɋ߂��z��̋󂫃X�y�[�X�Ɉړ�������D ]
		// [  �� �f�[�^���ړ������ċ�ƂȂ��� HashTable_setX[first] �ɐV����key���i�[����D                                                       ]
		// [                                                                                                                                      ]
		// [  [�����ԍ��C]�C[�����ԍ��E]�C[�����ԍ��G]                                                                                            ]


		// key�`�F�[����k������ɁC�z��̋󂫂��������C�󂫂𔭌������ꍇ�́C HashTable_setX[first] �̃f�[�^���ړ������C HashTable_setX[first] ���󂯂�D
		// �� HashTable_setX[first] �ɐV����key�Cvalue���Z�b�g����D
		//
		// key�`�F�[����k������ɁC�z��̋󂫂����������ꍇ�́Ckey�`�F�[���ɉ��������ɁC�z��̋󂫂���������D
		//
		// �n�b�V���l���قȂ鎞�_�ŁC��vkey�����݂��鎖�͂��蓾�Ȃ��̂ŁC���ׂ�K�v�͂Ȃ��D

		{
			UINT64 empty = 0;	//�K������������K�v������D	//�V�t�g�l�Ƃ���0�͎g�p����Ȃ��̂ŁC0�ŏ����������Ă����D
			UINT64 empty_front;
			UINT64 empty__back;

			{
				// key�`�F�[����k������ɁC�z��̋󂫂������D
				UINT64 NowPlace = first;

				while(1){
					UINT64 PreviousShift = HashTable_setX[NowPlace].previous;

					if( PreviousShift == 0 ){
						break;
					}

					for( UINT64 i=1; i<PreviousShift; i++ ){
						if( HashTable_setX[NowPlace-i].previous == 255 ){
							// �󂢂Ă���z��𔭌��D
							// [�����ԍ��C]�C�y�сC[�����ԍ��E]
							empty__back = NowPlace;
							empty       = NowPlace - i;				//�ŏ��Ɍ������l���C�ł�key�ɋ߂��󂫂Ƃ͌���Ȃ��̂ŁC[ PreviousShift == 0 ]�ƂȂ�܂ŁC���[�v���񂷕K�v������D
							empty_front = NowPlace - PreviousShift;
						}
					}

					NowPlace -= PreviousShift;
				}
			}

			if(empty!=0){
				// key�`�F�[����k������ɁC�z��̋󂫂��������C�󂫂𔭌������ꍇ�D

				// empty���C�z��̋󂫂������Ă���D
				// [�����ԍ��C]�C�y�сC[�����ԍ��E]

				// [key]--[empty_front]- (empty) -[empty__back]--[X]--[X]--[first_front]--[first](want to set key2)--[first__back]--[X]

				if( empty__back == first ){
					if( HashTable_setX[ first ].next == 0 ){
						// [�����ԍ��C]

						setX_SameProcessFor4to7( HashTable_setX, first, empty, empty_front, TrueTableLength_setX );
					//	HashTable_setX[ empty       ].next     = 0;
						//[empty]�ł���΁Cnext�͊���0�ɏ���������Ă���͂��Ȃ̂ŁC�s�v�D
					}else{
						// [�����ԍ��D]

						// �V�t�g���E�𒴂��Ȃ����Ƃ��m�F����
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
						// [�����ԍ��E]

						setX_SameProcessFor4to7( HashTable_setX, first, empty, empty_front, TrueTableLength_setX );
						HashTable_setX[ first_front ].next = 0;
					}else{
						// [�����ԍ��F]

						// �V�t�g���E�𒴂��Ȃ����Ƃ��m�F����
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

				NeedReHash = false; result = true; NumOfElementsInTheTable++;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D( ���s���Ă���ꍇ�́Ckey���d�����Ă���ꍇ�ŁC�v�f���͕ω����Ȃ��D )
				return;
			}



			// �z��̋󂫂�first���O�ɑ��݂��Ȃ������ꍇ�D
			// ��first�����̋󂫂�T���D
			//
			// [�����ԍ��G]�C[�����ԍ��H]�C�y�сC[�����ԍ��I]�C[�����ԍ��J]
			{
				UINT64 last;			//( [first] �` [last] �Ԃ̔z��ɁC�󂫂������ꍇ�Ɍ���C) key�`�F�[���̏I�����L�^����D
				{
					// [first] �` [last] �Ԃ̔z��ɁC�󂫂������������D
					// [�����ԍ��G]�C[�����ԍ��H] (���󂫂�����ꍇ�̏����D)
					empty = 0;
					UINT64 NowPlace = first;

					while(1){
						UINT64 NextShift = HashTable_setX[NowPlace].next;

						if( NextShift == 0 ){
							last = NowPlace;	//( [first] �` [last] �܂ł̊ԂɁC��̔z�񂪖����ꍇ�Ɍ���C) key�`�F�[���̏I�����L�^����D
							break;
						}

						for( UINT64 i=1; i<NextShift; i++ ){
							if( HashTable_setX[NowPlace+i].previous == 255 ){
								// �󂢂Ă���z��𔭌��D
								empty_front = NowPlace;
								empty       = NowPlace + i;
								empty__back = NowPlace + NextShift;
								UINT64 first_front = first - HashTable_setX[ first ].previous;
								UINT64 first__back = first + HashTable_setX[ first ].next;
								UINT64 ShiftOf_Empty_to_EmptyBack = empty__back - empty;

								if( first == empty_front ){ //[if( empty != 0 ){]�̒��ɏ��������Ȃ��ƁC [�����ԍ��I] �̃p�^�[���̏ꍇ�ɁC������[empty_front]������������Ȃ��܂܎g�p����鎖�ɂȂ��Ă��܂��D
									// [�����ԍ��H]

									// �V�t�g���E�𒴂��Ȃ����Ƃ��m�F����
									UINT64 ShiftOf_FirstFront_to_Empty = empty - first_front;
									if( ShiftOf_FirstFront_to_Empty > AllowedRightShiftNum ){
										printf_dbger("NeedReHash: 9\n");
										NeedReHash = true; result = false; return;
									}

									printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
									printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
									setX_Move_key_value(HashTable_setX[ empty ], HashTable_setX[ first ]);

									printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_Empty > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_FirstFront_to_Empty: %llu, empty: %llu, first_front: %llu.", ShiftOf_FirstFront_to_Empty, empty, first_front );
									HashTable_setX[ first_front ].next     = (unsigned char)( ShiftOf_FirstFront_to_Empty );		//     next�ڑ����q���ւ� ( �O���炠��v�f )
									HashTable_setX[ empty       ].previous = (unsigned char)( ShiftOf_FirstFront_to_Empty );		// previous�ڑ���ݒ�     (   �V�����v�f   )
								}else{
									// [�����ԍ��G]

									// �V�t�g���E�𒴂��Ȃ����Ƃ��m�F����
									UINT64 ShiftOf_FirstFront_to_FirstBack = first__back - first_front;
									if( ShiftOf_FirstFront_to_FirstBack > AllowedRightShiftNum ){
										printf_dbger("NeedReHash: 8\n");
										NeedReHash = true; result = false; return;
									}

									printf_dbger_if_Exit_Stop( empty >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
									printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
									setX_Move_key_value(HashTable_setX[ empty ], HashTable_setX[ first ]);

									printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_FirstBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_FirstFront_to_FirstBack: %llu, first__back: %llu, first_front: %llu.", ShiftOf_FirstFront_to_FirstBack, first__back, first_front );
									HashTable_setX[ first_front ].next     = (unsigned char)( ShiftOf_FirstFront_to_FirstBack );	//     next�ڑ����q���ւ� ( �O���炠��v�f )
									HashTable_setX[ first__back ].previous = (unsigned char)( ShiftOf_FirstFront_to_FirstBack );	// previous�ڑ����q���ւ� ( �O���炠��v�f )

									UINT64 ShiftOf_EmptyFront_to_Empty = empty - empty_front;
									printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_FirstBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_EmptyFront_to_Empty: %llu, empty: %llu, empty_front: %llu.", ShiftOf_EmptyFront_to_Empty, empty, empty_front );
									HashTable_setX[ empty_front ].next     = (unsigned char)( ShiftOf_EmptyFront_to_Empty );		//     next�ڑ����q���ւ� ( �O���炠��v�f )
									HashTable_setX[ empty       ].previous = (unsigned char)( ShiftOf_EmptyFront_to_Empty );
								}
								printf_dbger_if_Exit_Stop( ShiftOf_Empty_to_EmptyBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_EmptyFront_to_Empty: %llu, empty__back: %llu, empty: %llu.", ShiftOf_Empty_to_EmptyBack, empty__back, empty );
								HashTable_setX[ empty       ].next     = (unsigned char)( ShiftOf_Empty_to_EmptyBack  );		//     next�ڑ���ݒ�     (   �V�����v�f   )
								HashTable_setX[ empty__back ].previous = (unsigned char)( ShiftOf_Empty_to_EmptyBack  );		// previous�ڑ����q���ւ� ( �O���炠��v�f )

								printf_dbger_if_Exit_Stop( first >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
								printf_dbger_if_Exit_Stop( HashTable_setX[ first ].previous == 255, "ERROR: This element is not empty!\n" );
								setX_key_value_0_0( HashTable_setX[ first ], key, value );

								NeedReHash = false; result = true; NumOfElementsInTheTable++;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D( ���s���Ă���ꍇ�́Ckey���d�����Ă���ꍇ�ŁC�v�f���͕ω����Ȃ��D )
								return;
								// =======================
							}
						}

						NowPlace += NextShift;
					}
				}
				{
					// [first] �` [last] �Ԃ̔z��ɁC�󂫂������ꍇ�D
					// ��[last]�����̋󂫂𑖍�����D
					// [�����ԍ��I]�C[�����ԍ��J]

					if( HashTable_setX[ first ].next == 0 ){
						//[�����ԍ��J]

						// FirstFront ���� First �܂ł̃V�t�g���� AllowedRightShiftNum �𒴂��Ȃ��悤�ɂ���D
						UINT64 ShiftLimit = AllowedRightShiftNum - HashTable_setX[ first ].previous;

						// ����̊m�F�́Ckey���`�F�[������Ă���ƁC�������V�t�g�\�Ȃ̂ŁC�K�{�D
						UINT64 LimitOfTableSize = TrueTableLength_setX - last - 1;

						// (�R���p�C���ɂ���邪) for �����Ŕ�r����ƌv�Z�R�X�g��������̂ŁC���������炷�D
						if( ShiftLimit > LimitOfTableSize ){ ShiftLimit = LimitOfTableSize; }

					#ifndef UnLimit_AllowedSumOfShift
					//	UINT64 SumOfShift = last - first;
						UINT64 LeftSumOfShift = AllowedSumOfShift - (last - first); // �����ꂽ���v�V�t�g���̎c��
						if( ShiftLimit > LeftSumOfShift ){ ShiftLimit = LeftSumOfShift; }
					#endif

						for( UINT64 i=1; i <= ShiftLimit; i++ ){
							UINT64 empty = last + i;
							if( HashTable_setX[ empty ].previous == 255 ){
								setX_Move_key_value(HashTable_setX[ empty ], HashTable_setX[ first ]);

								UINT64 first_front = first - HashTable_setX[ first ].previous;
								UINT64                                                  ShiftOf_FirstFront_to_Empty = empty - first_front;
								printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_Empty > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_FirstFront_to_Empty: %llu, empty: %llu, first_front: %llu.", ShiftOf_FirstFront_to_Empty, empty, first_front );
								HashTable_setX[ first_front ].next     = (unsigned char)ShiftOf_FirstFront_to_Empty;	//     next�ڑ����q���ւ� ( �O���炠��v�f )
								HashTable_setX[ empty       ].previous = (unsigned char)ShiftOf_FirstFront_to_Empty;	// previous�ڑ����q���ւ� ( �O���炠��v�f )
							//	HashTable_setX[ empty       ].next     = 0;												// previous�ڑ����q���ւ� ( �O���炠��v�f )
								//[empty]�ł���΁Cnext�͊���0�ɏ���������Ă���͂��Ȃ̂ŁC�s�v�D
								
								printf_dbger_if_Exit_Stop( first >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
							//	printf_dbger_if_Exit_Stop( HashTable_setX[ first ].previous != 255, "ERROR: This element is not empty!\n" );
								setX_key_value_0_0( HashTable_setX[ first ], key, value );

								NeedReHash = false; result = true; NumOfElementsInTheTable++;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D( ���s���Ă���ꍇ�́Ckey���d�����Ă���ꍇ�ŁC�v�f���͕ω����Ȃ��D )
								return;
							}
						}
						//printf_dbger("NeedReHash: 11\n");
					}else{
						//[�����ԍ��I]

					//	UINT64 ShiftOf_FirstFront_to_FirstBack = first__back - first_front;
					//                                         = (first + HashTable_setX[ first ].next) - (first - HashTable_setX[ first ].previous)
					//                                         = (      + HashTable_setX[ first ].next) - (      - HashTable_setX[ first ].previous)
					//                                         = HashTable_setX[ first ].next + HashTable_setX[ first ].previous
						UINT64 ShiftOf_FirstFront_to_FirstBack = HashTable_setX[ first ].next + HashTable_setX[ first ].previous;
						if( ShiftOf_FirstFront_to_FirstBack > AllowedRightShiftNum ){
							NeedReHash = true; result = false; return;
						}

						UINT64 ShiftLimit = AllowedRightShiftNum;

						// ����̊m�F�́Ckey���`�F�[������Ă���ƁC�������V�t�g�\�Ȃ̂ŁC�K�{�D
						UINT64 LimitOfTableSize = TrueTableLength_setX - last - 1;

						// (�R���p�C���ɂ���邪) for �����Ŕ�r����ƌv�Z�R�X�g��������̂ŁC���������炷�D
						if( ShiftLimit > LimitOfTableSize ){ ShiftLimit = LimitOfTableSize; }

					#ifndef UnLimit_AllowedSumOfShift
					//	UINT64 SumOfShift = last - first;
						UINT64 LeftSumOfShift = AllowedSumOfShift - (last - first); // �����ꂽ���v�V�t�g���̎c��
						if( ShiftLimit > LeftSumOfShift ){ ShiftLimit = LeftSumOfShift; }
					#endif

						for( UINT64 i=1; i <= ShiftLimit; i++ ){
							UINT64 empty = last + i;
							if( HashTable_setX[ empty ].previous == 255 ){
								setX_Move_key_value(HashTable_setX[ empty ], HashTable_setX[ first ]);
								
								UINT64 first_front = first - HashTable_setX[ first ].previous;
								UINT64 first__back = first + HashTable_setX[ first ].next;
								printf_dbger_if_Exit_Stop( ShiftOf_FirstFront_to_FirstBack > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. ShiftOf_FirstFront_to_FirstBack: %llu, HashTable_setX[ first ].next: %llu, HashTable_setX[ first ].previous: %llu.", ShiftOf_FirstFront_to_FirstBack, HashTable_setX[ first ].next, HashTable_setX[ first ].previous );
								HashTable_setX[ first_front ].next     = (unsigned char)ShiftOf_FirstFront_to_FirstBack;	//     next�ڑ����q���ւ� ( �O���炠��v�f )
								HashTable_setX[ first__back ].previous = (unsigned char)ShiftOf_FirstFront_to_FirstBack;	// previous�ڑ����q���ւ� ( �O���炠��v�f )
								
								printf_dbger_if_Exit_Stop( i > 254, "ERROR: ShiftNum is over the AllowedRightShiftNum. i: %llu.", i );
								HashTable_setX[ last  ].next     = (unsigned char)i;													//     next�ڑ����q���ւ� ( �O���炠��v�f )
								HashTable_setX[ empty ].previous = (unsigned char)i;													// previous�ڑ���ݒ�     (   �V�����v�f   )
							//	HashTable_setX[ empty ].next     = 0;													//     next�ڑ���ݒ�     (   �V�����v�f   )
								//[empty]�ł���΁Cnext�͊���0�ɏ���������Ă���͂��Ȃ̂ŁC�s�v�D
								
								printf_dbger_if_Exit_Stop( first >= TrueTableLength_setX, "ERROR: Out of Range!\n" );
							//	printf_dbger_if_Exit_Stop( HashTable_setX[ empty ].previous != 255, "ERROR: This element is not empty!\n" );
								setX_key_value_0_0( HashTable_setX[ first ], key, value );

								NeedReHash = false; result = true; NumOfElementsInTheTable++;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D( ���s���Ă���ꍇ�́Ckey���d�����Ă���ꍇ�ŁC�v�f���͕ω����Ȃ��D )
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
		goto ReSet;		//���n�b�V�������ꍇ�́C������xkey,value�̑}�������݂�K�v������D(do-while()��p����ƁC��r����񑽂��Ȃ�̂ŁC�p���Ȃ��D)
	}
}
void ICHashT::set0( UINT64& key, void*& value ){
	void** return_value_pp;	//�_�~�[(���ۂɂ͎g���Ȃ��D)
	bool   result;			//�_�~�[(���ۂɂ͎g���Ȃ��D)
	setXwithReHash( '0', key, value, return_value_pp, result, HashTable, TableLength, TrueTableLength );
}
void ICHashT::set1( UINT64& key, void*& value ){
	void** return_value_pp;	//�_�~�[(���ۂɂ͎g���Ȃ��D)
	bool   result;			//�_�~�[(���ۂɂ͎g���Ȃ��D)
	setXwithReHash( '1', key, value, return_value_pp, result, HashTable, TableLength, TrueTableLength );
}
void ICHashT::set2( UINT64& key, void*& value, void**& return_value_pp, bool& result ){
	setXwithReHash( '2', key, value, return_value_pp, result, HashTable, TableLength, TrueTableLength );
}
//=====================================================================================================================================================
#ifdef ICHashT_DebugMode
	// �f�o�b�O�p�̐ݒ�
void ICHashT::setXwithNoReHash( UINT64& key, void*& value, void**& return_value_pp, bool& result, bool& NeedReHash ){
	char set_function_type = '2';
	setX( set_function_type, key, value, return_value_pp, result, NeedReHash, HashTable, TableLength, TrueTableLength );
}
void ICHashT::set_for_Debug( UINT64 TableElementNum, UINT64 key, void* value, unsigned char previous, unsigned char next ){ //�w�肵���z��ԍ��ɁC�C�ӂ̃f�[�^���Z�b�g����D
	HashTable[ TableElementNum ].key      = key;		// �f�[�^�̏������ݏ���
	HashTable[ TableElementNum ].value    = value;		// �f�[�^�̏������ݏ���
	HashTable[ TableElementNum ].previous = previous;	// �f�[�^�̏������ݏ���
	HashTable[ TableElementNum ].next     = next;		// �f�[�^�̏������ݏ���
}
#endif//ICHashT_DebugMode
//=====================================================================================================================================================


void ICHashT::getX( char set_function_type, UINT64& key, void*& value, void**& pp_value, bool& result ){//key�����value������0�����ꍇ�ɁC�l�̗L�����m�F�ł��Ȃ��̂ŁC[bool& result]��[true]or[false]��Ԃ��D
	UINT64 first;				//�`�F�[���̐擪�C�̈Ӗ������߂āD
	makeHash(key,first,TableLength);	//�n�b�V���l�̐���
	UINT64 PreviousShift = HashTable[first].previous;
	if( PreviousShift != 0 ){
		//NotFound(�Y���f�[�^����)
		result = false; return;
	}

	{
		UINT64 NextShift;
		UINT64 TagetPlace = first;

		do{
			if( HashTable[ TagetPlace ].key == key ){//key�̈�v���m�F
				//key����v�D
				if(set_function_type=='0'){
					value     =  HashTable[ TagetPlace ].value;
				}else{//if(set_function_type=='1'){
					pp_value  = &HashTable[ TagetPlace ].value;
				}
				result = true; return;
			}
			NextShift = HashTable[ TagetPlace ].next;
			TagetPlace += NextShift;				//���̗v�f�ֈړ�����ʂ��v�Z�D
		}while( NextShift != 0 );					//�`�F�[���̖����ɓ��B�������ǂ����D
		//�`�F�[���̖����ɓ��B�D

		result = false; return;//�Ō�܂ŒT���Ă����������̂ŁC[false]
	}
}
void ICHashT::get0( UINT64& key, void* & value, bool& result ){
	void** pp_value;	//�_�~�[(���ۂɂ͎g���Ȃ��D)
	getX( '0', key, value, pp_value, result );
}
void ICHashT::get1( UINT64& key, void**& pp_value, bool& result ){
	void* value;		//�_�~�[(���ۂɂ͎g���Ȃ��D)
	getX( '1', key, value, pp_value, result );
}


//find�֐���get�֐��ƑS���������삾���Cvalue��Ԃ��Ȃ��D
void ICHashT::find( UINT64& key, bool& result ){
	UINT64 TagetPlace;				//�`�F�[���̐擪�C�̈Ӗ������߂āD
	makeHash(key,TagetPlace,TableLength);	//�n�b�V���l�̐���
	UINT64 PreviousShift = HashTable[TagetPlace].previous;
	if( PreviousShift != 0 ){
		//NotFound(�Y���f�[�^����)
		result = false; return;
	}

	{
		UINT64 NextShift;

		do{
			if( HashTable[ TagetPlace ].key == key ){//key�̈�v���m�F
				//key����v�D
//				value  = HashTable[ TagetPlace ].value;
				result = true; return;
			}
			NextShift = HashTable[ TagetPlace ].next;
			TagetPlace += NextShift;				//���̗v�f�ֈړ�����ʂ��v�Z�D
		}while( NextShift != 0 );					//�`�F�[���̖����ɓ��B�������ǂ����D
		//�`�F�[���̖����ɓ��B�D

		result = false; return;//�Ō�܂ŒT���Ă����������̂ŁC[false]
	}
}


void ICHashT::eraseX( char set_function_type, UINT64& key, bool& result, void*& return_value_p ){
	//�v�f���`�F�[������Ă��Ȃ��ꍇ(�v�f��1)�C���邢�́C�`�F�[���̈�Ԍ��̗v�f���폜����ꍇ�D  ��  ���̂܂܍폜���邾���D
	//�`�F�[���̈�Ԍ��ȊO�̗v�f���폜����ꍇ�D  ��  ��Ԍ��̗v�f���C�{���폜�����������v�f�֏㏑������D���̏�ŁC��Ԍ��̗v�f���C�폜����D

	//�e�[�u�����̂̐������ڂ�setX()�֐��Ɉ�C���邱�Ƃɂ���̂ŁCerase()�֐��ɂ����ẮC����ȏ�̏����̎����͍s��Ȃ��D(root�l���ݒ肳��Ă���ꍇ�������Droot_solver();)

	UINT64 first;				//�`�F�[���̐擪head
	UINT64  last;				//�`�F�[���̖���tail
	
	UINT64 delete_object;		//�폜�Ώ�
	
//	unsigned char ChainLength = 1;		//�`�F�[���̒���


	//key�̌���
	makeHash(key,first,TableLength);//�n�b�V���l�̐���


	//key����΂ɑ��݂��Ȃ��ꍇ�� [return false;]
	if( HashTable[first].previous!=0 ){
		// [previous==255] :: �z�񂪋�(no data)
		// [previous!=  0] :: �f�[�^�͂��邪�C�`�F�[���̐擪�ł͂Ȃ��D
		// [previous==  0] :: �`�F�[���̐擪�D
		result = false; return;
	}
	// HashTable[first] ���C����1�ȏ�̃`�F�[���̐擪�ł��鎖���m�F�D


	//�폜�Ώۂ̗v�f�܂Ń`�F�[���̌�����i�߂�D
	delete_object = first;	//������
	while( HashTable[delete_object].key != key ){ //key����v����܂Ń��[�v���񂵁C�폜�Ώۂ���������D
		UINT64 NextShift = HashTable[delete_object].next;
		if( NextShift == 0 ){
			result = false; return;//�Ō�܂ň�v����key�����݂��Ȃ������̂ŁC [return false;]
		}
//		ChainLength++;
		delete_object += NextShift;
	}
	

	// [�`�F�[���̒����̌v��] �C�y�сC[�`�F�[���̍Ō�] �� [�`�F�[���̍Ōォ���Ԗ�] �̈ʒu���m�F�D
	last = delete_object;	//������
	while(1){ //�`�F�[���̍Ō�܂Ń��[�v���񂷁D
		UINT64 NextShift = HashTable[last].next;
		if( NextShift == 0 ){
			break;
		}
//		ChainLength++;
		last += NextShift;
	}
	//�����܂łŁC�����ɕK�v�ȃf�[�^���W�I���D�������������D


	HashStruct HashStruct_for_init;
	HashStruct_for_init.key      = 0;
	HashStruct_for_init.value    = 0;
	HashStruct_for_init.previous = 255;	//[root==255] �z�񂪋�ł��邱�Ƃ������D
	HashStruct_for_init.next     = 0;


	/*
	if( last == first ){ //�폜�Ώۂ��z��̈�ԍŌォ�ǂ����̊m�F�D
		//�v�f��1�̏ꍇ�D(�z��̐擪���C�z��̍Ō�)
		//[�����ԍ��@]
		HashTable[last] = HashStruct_for_init;

		NumOfElementsInTheTable--;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D
	//	SetLoadFactor();		//�g��Ȃ��l�𖈉�v�Z������̂��c�c�D
		result = true; return;
	}


	//�ȉ��C�v�f��2�ȏ�̏ꍇ�D

	if( last == delete_object ){ //�폜�Ώۂ��z��̈�ԍŌォ�ǂ����̊m�F�D
		//�폜�Ώۂ��z��̈�ԍŌ�D(���C���Ȃ��Ƃ��v�f��2�ȏ�)
		UINT64 NextShift     = HashTable[last].next;
		UINT64 PreviousShift = HashTable[last].previous;

		//[�����ԍ��A]
		HashTable[last-PreviousShift].next = 0;		//next�ڑ���ؒf
		HashTable[last] = HashStruct_for_init;		//�v�f��������

		NumOfElementsInTheTable--;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D
	//	SetLoadFactor();		//�g��Ȃ��l�𖈉�v�Z������̂��c�c�D
		result = true; return;
	}
	*/

	
	//��L�́C[�����ԍ��@]�C�y�сC[�����ԍ��A] �̏������C�����̏������s���΁C���l�ɏ�������Ă��܂��D
	//�]���āC��L�̓R�����g�A�E�g�D(if����(�ꍇ�ɂ���Ă�)���ʂȃ������R�s�[�ƁC�����I�ɂǂ��炪�����̂��c�c�D)
	{
		UINT64 PreviousShift = HashTable[last].previous;

		// [�����ԍ��B]�C�y�сC[�����ԍ��C]
		HashTable[last-PreviousShift].next = 0;					//next�ڑ���ؒf

		HashTable[delete_object].key   = HashTable[last].key;	//�f�[�^�̈ړ�����
		if( set_function_type == '0' ){
			// set_function_type == '0'
			// �������Ȃ��D
		}else{
			// set_function_type == '1'
			return_value_p = HashTable[delete_object].value;	//�������̉���p�D���邢�́C�폜�Ɠ����ɍ폜��ɓ��I�ɕۑ�����Ă���f�[�^�̎擾���s���D
		}
		HashTable[delete_object].value = HashTable[last].value;	//�f�[�^�̈ړ�����

		HashTable[last] = HashStruct_for_init;		//�v�f��������

		NumOfElementsInTheTable--;	//�e�[�u����Ɋi�[����Ă���key value�y�A�̐��̃J�E���g�����Z�D
	//	SetLoadFactor();		//�g��Ȃ��l�𖈉�v�Z������̂��c�c�D
		result = true; return;
	}
}
void ICHashT::erase0( UINT64& key, bool& result ){
	void* return_value_p;	//�_�~�[(���ۂɂ͎g���Ȃ��D)
	eraseX( '0', key, result, return_value_p );
}
void ICHashT::erase1( UINT64& key, bool& result, void*& return_value_p ){
	eraseX( '1', key, result, return_value_p );
}


void ICHashT::reHash(){

#ifdef ICHashT_DebugMode
	// �f�o�b�O�p�̐ݒ�
//	printf( "������reHash������\n");	//for debug
//	printf( "LoadFactor: %lf\n", GetLoadFactor() );
//	printf( "\n");
//	SysPauseForWin32();
#endif//ICHashT_DebugMode

ReReHash:

	PrimeList_Num++;																//���̃e�[�u���T�C�Y���w�肷��D
//	if(PrimeList_Num>63){return false;}
	UINT64 TableLength_new     = SmallestPrimeList_LargerThanPower2[PrimeList_Num];	//��������̃e�[�u���T�C�Y
	                                               AllowedRightShiftNum = GetHyperParameters( PrimeList_Num );	//�E�V�t�g��������ő�l�̎w��D(0�`254�܂ŁD)
	UINT64 TrueTableLength_new = TableLength_new + AllowedRightShiftNum;

	struct HashStruct* HashTable_new;
	mallocTable(HashTable_new,TrueTableLength_new);	//�V�����e�[�u���̊m�ہC�y�я�����

	NumOfElementsInTheTable = 0; // ���L�̓���ւ��ŉ��Z����Ă��܂��̂ŁC����������D( �Â��o�[�W�����ł́C setX(); ���ŉ��Z�����Ă��Ȃ��������߁C���n�b�V������ NumOfElementsInTheTable �̉��Z�R�X�g�͂Ȃ��������C�����ɁC�֐��̊O�ŉ��Z���邩�ǂ����̔�r�R�X�g���������Ă����D )

	//�S�v�f�ɂ��āC�n�b�V���l���Čv�Z���C�V�����e�[�u���ɃR�s�[���C�z�u�������D

	//�S�v�f����
	AllScanPlace NowPlace;
	for(AllScanInit(NowPlace); NowPlace < AllScanEnd; AllScanNextShift(NowPlace)){
		UINT64 key;
		void* value;
		AllScanGetData(key,value,NowPlace);
//			printf("key::%lld\n"           ,            key );
//			printf("value(Address)::%llX\n",          value );

		//�v�f��V�����z��֏�������
		void** return_value_pp;	//�_�~�[(���ۂɂ͎g���Ȃ��D)
		bool   result;
		bool   NeedReHash;
		char   set_function_type = '1';
		setX( set_function_type, key, value, return_value_pp, result, NeedReHash, HashTable_new, TableLength_new, TrueTableLength_new );		//setXwithReHash()�֐����g���ƁC�ċA�I�ȏ�������������̂ŁC�����Ďg��Ȃ����D
		if( NeedReHash == true ){
			//�������n�b�V���l��key���d�����CseX()��false��Ԃ��悤�ȏꍇ������΁C�����Ƀ��n�b�V�����悤�Ƃ���̂ŁC���ӁD

			//setX()���X�Ȃ郊�n�b�V����v�����Ă����ꍇ
			//�����ݐV�����m�ۂ����e�[�u���ւ̃R�s�[�𒆎~���C����ɔ{�T�C�Y�̃e�[�u���ւ̃R�s�[�ւƑ����ύX����D

			free( HashTable_new );			//�e�[�u���̉��
			goto ReReHash;					//goto�ŏ����������C��r����񏭂Ȃ��̂ŁDdo{}while(result==false);�͂�߂�D
		}
	}

	freeTable(HashTable);					//�Â��e�[�u���̉��
	HashTable       = HashTable_new;		//����ւ�
	TableLength     = TableLength_new;		//����ւ�
	TrueTableLength = TrueTableLength_new;	//����ւ�
}


UINT64 ICHashT::GetNumOfElementsInTheTable(){ return this->NumOfElementsInTheTable; }

//�e�[�u����L���̌v�Z
void ICHashT::SetLoadFactor(){
	this->LoadFactor = (double)((double)NumOfElementsInTheTable / (double)TrueTableLength);
//	printf_dbger("LoadFactor: %lf, NumOfElementsInTheTable: %llu, TrueTableLength: %llu\n", LoadFactor, NumOfElementsInTheTable, TrueTableLength);
}
double ICHashT::GetLoadFactor(){
	SetLoadFactor();
	return LoadFactor;
}

// �O������̎Q�Ɨp
UINT64 ICHashT::GetTableLength(){ return this->TableLength; }
UINT64 ICHashT::GetTrueTableLength(){ return this->TrueTableLength; }


void ICHashT::reHash_to_fit(){

	//���Ȑ�L��(�[�U��)�̌v�Z�D
//	SetLoadFactor();

	//���z�Ƃ���e�[�u���T�C�Y���v�Z�D
		//    [���z�e�[�u����L��] == [���ۂɃe�[�u����ɑ��݂��� key value �y�A�̗v�f��] / [���z�e�[�u����]
		// ��   IdealLoadFactor    ==               NumOfElementsInTheTable               / IdealTableLength
		// ��   IdealTableLength   ==               NumOfElementsInTheTable               / IdealLoadFactor
		//���C
	double IdealLoadFactor = 0.5;	//���z�e�[�u����L�� ( ��肠����50%�ɐݒ肵�Ă����D )
	UINT64 IdealTableLength = (UINT64)( ((double)NumOfElementsInTheTable) / IdealLoadFactor );

	//[���z�e�[�u����L��]��50%�ȉ��ƂȂ�C���C�ł�50%�ɋ߂��e�[�u���T�C�Y���C�f���\�̒�����I�ԁD
	unsigned char PrimeList_Num_New = 0;															//reHash()�ŃA�N�Z�X�����̂ŁC�K�����������Ă����D
	for(; PrimeList_Num_New<64; PrimeList_Num_New++){ //32bit�Ή�����̂ł���΁C���ӂ��������ǂ��̂���?(�f���\��32bit�Ȃ�T�C�Y�����������������ǂ��D)
		if( SmallestPrimeList_LargerThanPower2[PrimeList_Num_New] >= IdealTableLength ){
			break;		//for���𔲂���D
		}
	}

	if( PrimeList_Num_New == PrimeList_Num ){
		//����̃e�[�u���T�C�Y�͗��z�I�ȃT�C�Y�Ȃ��߁CreHash();�֐������s����K�v�͖����D�]���āC���n�b�V��������return�D
		return;
	}else{
		PrimeList_Num = PrimeList_Num_New;	//���z�Ƃ���e�[�u���T�C�Y���w�肷��D
	}

	ReconstrucTheTable();	//�������n�b�V��
}


void ICHashT::Reconstruction_to_fit(){

	//���ȗ��p���̌v�Z�D
//	SetLoadFactor();

	//���z�Ƃ���e�[�u���T�C�Y���v�Z�D
		//    [���z���ȗ��p��] == [���ۂɃe�[�u����ɑ��݂��� key value �y�A�̗v�f��] / [���z�e�[�u����]
		// �� IdealLoadFactor  ==               NumOfElementsInTheTable               / IdealTableLength
		// �� IdealTableLength ==               NumOfElementsInTheTable               / IdealLoadFactor
		//���C
	double IdealLoadFactor = 0.5;	//���z���ȗ��p�� ( ��肠����50%�ɐݒ肵�Ă����D )
	UINT64 IdealTableLength = (UINT64)( ((double)NumOfElementsInTheTable) / IdealLoadFactor );

	//[���z���ȗ��p��]��50%�ȉ��ƂȂ�C���C�ł�50%�ɋ߂��e�[�u���T�C�Y���C�f���\�̒�����I�ԁD
	unsigned char PrimeList_Num_New = 0;															//reHash()�ŃA�N�Z�X�����̂ŁC�K�����������Ă����D
	for(; PrimeList_Num_New<65; PrimeList_Num_New++){ //32bit�Ή�����̂ł���΁C���ӂ��������ǂ��̂���?(�f���\��32bit�Ȃ�T�C�Y�����������������ǂ��D)
		if( SmallestPrimeList_LargerThanPower2[PrimeList_Num_New] >= IdealTableLength ){
			break;		//for���𔲂���D
		}
	}
	if(PrimeList_Num_New>=65){
		printf_dbger("ERROR: Because of the hash table length will be over the range of 64 bits address, no operation will be done.\n");
		return; // ���������� return
	}

	PrimeList_Num = PrimeList_Num_New;	//���z�Ƃ���e�[�u���T�C�Y���w�肷��D

	ReconstrucTheTable();	//�������n�b�V��
}


void ICHashT::ReconstrucTheTable(){
	PrimeList_Num--;	//reHash();�֐��́C���̃e�[�u���T�C�Y���w�肵�悤�Ƃ���̂ŁC��Ɉ�����Ă����D
	reHash();			//���n�b�V��
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
	// �f�o�b�O�p�̐ݒ�

//�n�b�V���e�[�u����ŋ�̗v�f���܂߂��S�Ă̗v�f�̃_���v
	//�S�v�f�o�́E�������C�e���[�^��p���Ď������悤�Ǝv�������CC++11�I�ȁC�ŋ߂̋Z�p�ŁC�M�p�Ȃ�Ȃ��̂Ŏg��Ȃ����ɂ����D
void ICHashT::DumpAll_for_Debug_Init( DumpAll_Place& DumpAll_NowPlace ){
#define DumpAll_Start 0x0000000000000000
	DumpAll_NowPlace  = (DumpAll_Place)DumpAll_Start;		//�J�[�\���������ʒu�ɃZ�b�g�D
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

	//�n�b�V���e�[�u����ŋ�̗v�f���܂߂��S�Ă̗v�f�̃_���v
	DumpAll_Place DumpAll_NowPlace = start;
	this->DumpAll_End              = end + 1;
	for(; DumpAll_NowPlace < this->DumpAll_End; DumpAll_NowPlace++ ){
//	for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End; DumpAll_NowPlace++){
//	for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End && DumpAll_NowPlace<10; DumpAll_NowPlace++){//�S�ĕ\��������Ǝז��Ȃ̂ŁD
		UINT64 key;
		void* value3;
		unsigned char previous;
		unsigned char next;
		this->DumpAll_for_Debug_GetData( key, value3, previous, next, DumpAll_NowPlace );
		StrBuf += str_sprintf( "�z��ԍ�:%lld\n",        DumpAll_NowPlace );
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
	//�n�b�V���e�[�u����ŋ�̗v�f���܂߂��S�Ă̗v�f�̃_���v
	printf("������ Dump All ( Start ) ������\n");
	{
		DumpAll_Place DumpAll_NowPlace;
		for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End; DumpAll_NowPlace++){
//		for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End && DumpAll_NowPlace<10; DumpAll_NowPlace++){//�S�ĕ\��������Ǝז��Ȃ̂ŁD
			UINT64 key;
			void* value3;
			unsigned char previous;
			unsigned char next;
			this->DumpAll_for_Debug_GetData(key,value3,previous,next,DumpAll_NowPlace);
			printf("�z��ԍ�:%lld\n"       , DumpAll_NowPlace );
			printf("           key::%llu\n",              key );
			printf("value(Address)::%llX\n",   (UINT64)value3 );
			printf("      previous::%u\n"  ,         previous );
			printf("          next::%u\n"  ,             next );
//			printf("value(Entity )::%c\n"  ,  *((char*)value3));
			printf("\n");
		}
	}
	printf("������ Dump All (  End  ) ������\n");
}
void ICHashT::DumpAllToTheText( const char* SaveDirctory){

	class FileWriter* FW =  new FileWriter(SaveDirctory);	//�o�͐�̃t�@�C�������w��

	std::string StrBuf;

	//�n�b�V���e�[�u����ŋ�̗v�f���܂߂��S�Ă̗v�f�̃_���v
	DumpAll_Place DumpAll_NowPlace;
	for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End; DumpAll_NowPlace++){
//	for(this->DumpAll_for_Debug_Init(DumpAll_NowPlace); DumpAll_NowPlace < this->DumpAll_End && DumpAll_NowPlace<10; DumpAll_NowPlace++){//�S�ĕ\��������Ǝז��Ȃ̂ŁD
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
		FW->FWrite(StrBuf.c_str());	//�o��
	}

	delete FW;
}


#ifndef UnLimit_AllowedSumOfShift
UINT64 ICHashT::GetAllowedSumOfShift(){ return this->AllowedSumOfShift; }
#endif
unsigned char ICHashT::GetAllowedRightShiftNum(){ return this->AllowedRightShiftNum; }


#endif//ICHashT_DebugMode
//=====================================================================================================================================================

//key�ɕ�����ȂǗl�X�Ȍ^���g�������Ȃ�C�܂��͔�r������؂藣����������������??

//bool CompareKEY( TableNum, KeyType key ){
//	if( HashTable[TableNum] = key ){
//		return true;
//	}
//	return false;
//}

