/*===========================================================================================
概　略：ゲームの音を鳴らす処理
作成日：6月26日
更新日：6月26日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Global.h"
#include <xact3.h>

//多重定義防止処理
#pragma once

class Audio
{
	//メンバ変数
	IXACT3Engine*		xactEngine;		//XACTエンジンの情報を入れる変数
	IXACT3WaveBank*		waveBank;		//波形の情報を入れる
	IXACT3SoundBank*   soundBank;		//音を鳴らす鳴らさないかを入れる
	void*			   mapWaveBank;		//wavbankの情報を入れる（void*は何でも入れられるものと考えてよい）
	void*			   soundBankData;	//soundbankの情報を入れる

	//メンバ関数（private）
	HRESULT InitEngine();	//XACTエンジンの準備
	HRESULT LoadWaveBank(char* fileName);		//xwbファイルの読み込み
	HRESULT LoadSoundBank(char* name);			//xsbファイルの読み込み

public:
	Audio();	//コンストラクタ
	~Audio();	//デストラクタ

	HRESULT Load(char* waveBankFileName, char* soundBankFileName);	//音を鳴らすのに必要な関数の読み込み

	HRESULT Audio::Play(char* cueName);								//音を再生させる
	HRESULT Audio::Stop(char* cueName);								//音を停止させる
};

