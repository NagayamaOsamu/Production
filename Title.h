/*===========================================================================================
概　略：タイトル画面での処理をまとめたクラスヘッダー作成
作成日：12月21日
更新日：12月21日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "UnitBase.h"
#include "Sprite.h"

//多重定義防止処理
#pragma once

//画面の状態を定数で宣言
enum
{
	WAIT_STATE,				//待機中
	MODESELECT_STATE,		//モードセレクト
	MAX_STATE				//最大値
};

//モードセレクト状態のときどのモードを選んでいるかを定数で宣言
enum
{
	SELECT_MODE,			//ステージ選択モード
	CREATE_MODE,			//ステージ作成モード
	MAX_MODE				//最大値
};

//ユニットベースクラスを継承
class Title	: public UnitBase
{
	//クラスのインスタンス作成
	Sprite* title;			//タイトル画像
	Sprite* wait;			//待機中の画像
	Sprite* modeSelect;		//モードセレクトに使う画像
	Sprite* arrow;			//矢印画像

	//メンバ変数
	int state;				//画面の状態
	int mode;				//選んでいるモード

public:
	Title();			//コンストラクタ
	~Title();			//デストラクタ

	HRESULT Load();		//読み込み処理
	HRESULT	Update();	//更新処理
	HRESULT	Render();	//描画処理
};

