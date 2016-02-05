/*===========================================================================================
概　略：ゲーム全体の流れを扱う処理
作成日：6月26日
更新日：6月26日
制作者：永山治夢
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include "Global.h"
#include "SceneBase.h"

//―――――――――――――――――――――
//	ゲームクラス
//―――――――――――――――――――――
class Game
{
	//Direct3Dオブジェクト
	LPDIRECT3D9 pD3d;		//LPが頭についていると「＊」がなくてもポインタになる(解放する必要がある）

	//シーンオブジェクト
	SceneBase* scene[SC_MAX];

	//private関数
	HRESULT LigtOff();		//ライトの設定

	HRESULT CullingConfig();	//カリングモードの設定

	HRESULT AlphaConfig();		//アルファブレンドの設定

	HRESULT TexMatConfig();		//テクスチャ行列の設定

	HRESULT Game::InitLight();	//ライトの初期設定

public:
	//コンストラクタ
	Game();

	//デストラクタ
	~Game();

	//DirectX初期化
	HRESULT InitD3d(HWND);

	//読み込み処理
	HRESULT Load();

	//衝突判定処理
	HRESULT Hit();

	//更新処理
	HRESULT Update();

	//描画処理
	//引数：hdc	デバイスコンテキストハンドル
	HRESULT Render();
};

