/*===========================================================================================
概　略：ステージのデータを格納するためのクラスヘッダー(.cppはない)
作成日：2月2日
更新日：2月2日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include <Windows.h>

//多重定義防止処理
#pragma once

//シングルトン(インスタンスが一つした生成されないようにする)を使う
class StageData
{
	static StageData* inst;			//このクラスのインスタンス

	//コンストラクタ
	StageData();	//nullを入れる

public:

	 DWORD data[100];					//ステージのデータを入れる

	//このクラスのインスタンスを渡す関数
	static StageData* GetInst()
	{
		//インスタンスの中身がnullだったら
		if (inst == nullptr)
		{
			inst = new StageData;		//ステージのインスタンスを入れる
		}
		return inst;					//インスタンスを返す
	}

	//インスタンスの解放関数
	static void Free()
	{
		//削除
		delete inst;
	}
};

