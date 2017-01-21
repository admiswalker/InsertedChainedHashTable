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

#pragma once	//�C���N���[�h�K�[�h

#include <stdio.h>
#include <stdlib.h>
#include "SysPauseForWin32.h"
#include "FileDef.hpp"

#ifdef _WIN32 //�o�̓t�@�C���̉��s�R�[�h���w�肷��
	#define LineFeedCode "\r\n"
#else
	#define LineFeedCode "\r\n"
//	#define LineFeedCode "\n"
#endif

class FileWriter{
public:
	FileWriter(const char* FileName);
	~FileWriter();

	FILE* fp;

	int FWrite(const char* str);

private:
};

/*
 * How to use this.

	// �񖾎��I�� delete
	{
		std::string SaveDirctory = "./example.txt";
		class FileWriter FW((char*)SaveDirctory.c_str());	//�o�͐�̃t�@�C�������w��

		std::string str = "Write example text.";
		FW.FWrite(str.c_str());	//�o��
	} // when you over the scope of FW, destructor will be called.

	// �����I�� delete
	{
		std::string SaveDirctory = "./example.txt";
		class FileWriter* FW = new FileWriter((char*)SaveDirctory.c_str());	//�o�͐�̃t�@�C�������w��

		std::string str = "Write example text.";
		FW->FWrite(str.c_str());	//�o��
		delete FW;
	}
 *
 */
