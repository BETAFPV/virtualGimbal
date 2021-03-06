/*************************************************************************
*  Software License Agreement (BSD 3-Clause License)
*  
*  Copyright (c) 2019, Yoshiaki Sato
*  All rights reserved.
*  
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*  
*  1. Redistributions of source code must retain the above copyright notice, this
*     list of conditions and the following disclaimer.
*  
*  2. Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
*  
*  3. Neither the name of the copyright holder nor the names of its
*     contributors may be used to endorse or promote products derived from
*     this software without specific prior written permission.
*  
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/
#ifndef _MINIREAD_H_
#define _MINIREAD_H_
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mINIRead.hpp"
//#include <opencv2/opencv.hpp>
#define LINE_LENGTH_MAX 256 // 1行の長さの上限
#define DAT_NUM_MAX 128     // ファイル名の長さの上限

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {						//!<チェッカーボードの交差数設定構造体
	double X;							//!<各チェッカーボード上のX方向の交差数[個]
	double Y;							//!<各チェッカーボード上のY方向の交差数[個]
}strNumberOfInnerCorners;

typedef struct {//!< チェスボード設定構造体。要素は必ずdouble
	double NumberOfCaptureImage;			//!<キャリブレーション時に取得する画像枚数。大きな枚数を設定するとキャリブレーション精度が向上するが、キャリブレーションに時間がかかる。20枚程度が良い。
	double SizeOfQuadsX_mm;					//!<チェッカーボードをなす四角形のX方向の寸法[mm]
	double SizeOfQuadsY_mm;					//!<チェッカーボードをなす四角形のY方向の寸法[mm]
	strNumberOfInnerCorners NumberOfInnerCorners;			//!<各チェッカーボード上のX,Y方向の交差数[個]
}strCheckerBoardParams;



extern strCheckerBoardParams Dcbp;


extern const int  INICheckerBoardParamNum;
extern const char *INICheckerBoardValueNames[];


#ifdef __cplusplus
}
#endif

//テンプレート関数の宣言はここに書く

/**
* @brief 設定iniファイルを読み込みます
* @param [in] FileName		iniファイルの名前。例:"Settings.ini"
* @param [in] Num			読み込む設定項目の個数
* @param [in] NameOfValues	読み込む設定項目の名前
* @param [out] Values		iniファイルから読み取った値を書き込む構造体。構造体の要素はすべてdoubleで定義のこと。
* @retval 0で正常
**/
template<typename T_n>
int ReadINIs(const char FileName[], int Num, const char *NameOfValues[], T_n &Values){
	FILE *fp;
	char line[LINE_LENGTH_MAX];
	int n;
	double *valuesd = (double*)(void*)&Values;
	// テキストファイルを読み込み用に開く
	if (!(fp = fopen(FileName, "r"))){
		// ファイルオープンに失敗
		printf("File Open Error. \"%s\" is not found.\n", FileName);
		return 1;
	}

	n = 0;
	// ファイルから１行ずつ line に読み込む
	//    テキストを読み終えたら while ループら抜ける
	while (fgets(line, LINE_LENGTH_MAX, fp))
	{
		int i = 0;
		for (i = 0; i<Num; i++){
			if ((strstr(line, &(NameOfValues[i][0])) != NULL) && (strstr(line, &(NameOfValues[i][0]))<strstr(line, "//"))){//入力された名前があれば
				char *ch = line;
				ch = strchr(line, '=') + 1;
				valuesd[i] = atof(ch);
				n++;
				break;
			}
		}
	}
	printf("Number of Loaded Parameters=%d\n", n);
	int i;
	for (i = 0; i<Num; i++) printf("%s\t:%f\n", NameOfValues[i], valuesd[i]);
	fclose(fp);
	if (n != Num){
		printf("The Number of Parameters Error. Check format of \"%s\".\n", FileName);
		return 1;//異常終了
	}
	return 0;
}



#endif //_MINIREAD_H_
