/*===========================================================================================
概　略：ゲームのエフェクト処理
作成日：7月3日
更新日：7月3日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Sprite.h"
#include <list>

//多重定義防止処理
#pragma once

class Effect2D
{
	Sprite* sprite;			//スプライトクラスのポインタ作成
	float		speed;		//描画速度
	D3DXVECTOR2 size;		//画像の１コマのサイズを入れる

	//１つの画像でも複数使う可能性がある変数を構造体にまとめる
	struct EffectData
	{
		float		anime;		//アニメーション用の変数
		D3DXVECTOR3 position;	//描画位置
	};

	//構造体を動的配列にする
	std::list<EffectData> dataList;

public:
	Effect2D();		//コンストラクタ
	~Effect2D();	//デストラクタ

	HRESULT Load(char* fileName,D3DXVECTOR2 sz, float a_sp);	//読み込み処理
	HRESULT Draw();					//描画処理
	HRESULT Add(D3DXVECTOR3 pos);					//エフェクトを追加する関数
};

