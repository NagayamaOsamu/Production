/*===========================================================================================
概　略：2D画像(スプライト)を扱うクラスヘッダー
作成日：6月26日
更新日：6月26日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Global.h"

//多重定義防止処理
#pragma once

//画像の表示に使うデータ
struct SpriteData
{
		D3DXVECTOR3 pos;      //位置
		D3DXVECTOR2 size;     //サイズ
		D3DXVECTOR2 cut;      //切り抜き位置
		D3DXVECTOR2 center;   //基準点
		D3DXVECTOR2 scale;	  //拡大率	
		float		angle;		//角度

		//色
		struct
		{
			int a, r, g, b;
		}color;

		//構造体作成
		SpriteData()
		{
			pos    = D3DXVECTOR3(0, 0, 0);				//描画位置
			size   = D3DXVECTOR2(-999,-999);			//サイズ(省略したい場合は-999で指定する)
			cut    = D3DXVECTOR2(0, 0);					//切抜き位置
			center = D3DXVECTOR2(-999,-999);			//基準点
			color  = { 255, 255, 255, 255 };			//透明度と色
			scale  = D3DXVECTOR2(1,1);					//拡大率
			angle  = 0;									//角度
		}
};

class Sprite
{
	//スプライトオブジェクト
	LPD3DXSPRITE pSprite;

	//テクスチャオブジェクト
	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャのサイズ
	D3DXVECTOR2 texsize;
public:
	Sprite();		//コンストラクタ
	~Sprite();		//デストラクタ

	//読み込み処理
	HRESULT Load(char* fileName);

	//描画処理
	void Draw(SpriteData* data);

	//テクスチャのサイズを渡すアクセル関数
	D3DXVECTOR2 GetSize(){ return texsize;}
};

