/*===========================================================================================
概　略：ステージの選択をする処理をまとめたクラスのヘッダー
作成日：1月28日
更新日：1月28日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "UnitBase.h"		//ユニットベースクラス
#include "Sprite.h"			//スプライトクラス
#include <vector>			//ベクター

//多重定義防止処理
#pragma once

//ユニットベースクラスを継承
class StageSelect : public UnitBase
{
	//クラスのインスタンス作成
	Sprite* backGrround;		//背景画像
	Sprite* arrow;				//矢印画像
	Sprite* text;				//テキスト画像
	Sprite* num;				//ステージ番号

	//メンバ変数
	int selectStage;						//今選んでいるステージ
	std::vector<DWORD> stagePartsArray;		//ステージのブロックの種類を格納する

	//プライベート関数
	int GetComma(char data[], int* index);		//読み込んだステージファイルをカンマ区切りでデータを取得

public:
	StageSelect();			//コンストラクタ
	~StageSelect();			//デストラクタ

	HRESULT Load();			//読み込み処理
	HRESULT Update();		//更新処理
	HRESULT Render();		//描画処理
};

