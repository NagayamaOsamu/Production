/*===========================================================================================
概　略：ゲームオーバー画面での処理をまとめたクラスのヘッダー作成
作成日：12月21日
更新日：12月21日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "UnitBase.h"
#include "Sprite.h"

//多重定義防止処理
#pragma once

//ユニットベースクラスを継承
class GameOver	:	public UnitBase
{
	//クラスのインスタンス作成
	Sprite* gameOver;			//ゲームオーバーの画像

public:
	GameOver();			//コンストラクタ
	~GameOver();		//デストラクタ

	HRESULT Load();		//読み込み処理
	HRESULT	Update();	//更新処理
	HRESULT	Render();	//描画処理
};

