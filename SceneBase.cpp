/*===========================================================================================
概　略：各シーンの親クラス
作成日：6月26日
更新日：6月26日
制作者：永山治夢
=============================================================================================*/

//---------------インクルード-----------------------
#include "SceneBase.h"


//――――――――――――――――――――――
//	デストラクタ
//――――――――――――――――――――――
SceneBase::~SceneBase()
{
	//登場したユニットを全て開放
	for (WORD i = 0; i < unit.size(); i++)
	{
		delete unit[i];
	}
}

//――――――――――――――――――――――
//	衝突判定
//――――――――――――――――――――――
HRESULT SceneBase::Hit()
{
	//全ユニットの総当り
	for (WORD i = 0; i < unit.size(); i++)
	{
		for (WORD j = 0; j < unit.size(); j++)
		{
			//自分同士はパス
			if (i == j)
			{
				continue;
			}

			//ユニット同士の衝突判定
			if (FAILED(unit[i]->Hit(unit[j])))	//失敗していたら
			{
				return E_FAIL;	//失敗を返す
			}
		}
	}
	return S_OK;
}


//――――――――――――――――――――――
//	機能：画像の読み込みを行う
//	引数：なし
//	戻値：成功か失敗を返す
//――――――――――――――――――――――
HRESULT SceneBase::Load()
{
	//全ユニットを処理する
	for (WORD i = 0; i < unit.size(); i++)
	{
		//どこかのシーンで失敗していたら
		if (FAILED(unit[i]->Load()))
		{
			return E_FAIL;	//失敗を返す(ゲームクラスに)
		}
	
	}
	return S_OK;	//成功を返す
}

//――――――――――――――――――――――
//	更新処理
//――――――――――――――――――――――
HRESULT SceneBase::Update()
{
	//全ユニットを処理する
	for (WORD i = 0; i < unit.size(); i++)
	{
		if (FAILED(unit[i]->Update()))	//継承先で失敗していたら
		{
			return E_FAIL;	//失敗を返す
		}
	}
	return S_OK;
}

//――――――――――――――――――――――
//	描画処理
//――――――――――――――――――――――
HRESULT SceneBase::Render()
{
	//全ユニットを処理する
	for (WORD i = 0; i < unit.size(); i++)
	{
		//継承先で失敗していたら
		if (FAILED(unit[i]->Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}