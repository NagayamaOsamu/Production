/*===========================================================================================
概　略：ステージエディタークラスの処理をまとめたクラスヘッダー作成
作成日：1月12日
更新日：1月12日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include <list>			//リスト
#include "UnitBase.h"	//ユニットベースクラス
#include "Fbx.h"		//FBXクラス
#include "Sprite.h"		//スプライトクラス

//多重定義防止処理
#pragma once

//定数宣言
enum					//ステージブロックごとに定数で名前を付ける
{
	BLOCK_START,		//スタート
	BLOCK_STRAIGHT,		//直線
	BLOCK_BRANCH,		//T字路
	BLOCK_UP,			//上り坂
	BLOCK_DOWN,			//下り坂
	BLOCK_GOAL,			//ゴール
	BLOCK_STOP,			//行き止まり
	BLOCK_MAX			//最大ブロック数
};

//ユニットベースクラスを継承
class StageEditor : public UnitBase
{
	//クラスのインスタンス作成
	Sprite* chipWindow;			//チップウィンドウ(置きたいブロックを選択する部分)
	Sprite* frame;				//どのチップを選らんでいるか分かるようにする枠

	//各ステージブロックの出口の情報をまとめた構造体
	struct ExitState
	{
		D3DXVECTOR3 pos;		//位置
		float		angle;		//角度

	};

	//ステージブロックの設定をまとめた構造体
	struct
	{
		Fbx stage;							//ステージのモデル

		//出口の情報をまとめた配列
		std::vector<ExitState> exitList;	//出口が複数あるので動的配列にする	

	}blocksState[BLOCK_MAX];

	//メンバ変数
	std::list<DWORD> stagePartsList;			//ステージブロックを格納していく
	std::list<D3DXMATRIX> blocksMatList;		//ステージブロックごとの行列
	D3DXVECTOR3		cameraPos;					//カメラの位置
	D3DXVECTOR3		cameraTarget;				//カメラの角度
	int				selectChip;					//選ばれているチップ
	POINT			mouseTapPos;				//マウスのクリック位置
	POINT			mouseDownPos;				//マウスのクリックが押されてる間の位置
	D3DXVECTOR3		branchPos;					//T字路ブロックの位置
	float			branchAngle;				//T字路ブロックの角度
	BOOL			isChipWin;					//マウスカーソルがチップウィンドウにいないときにFALSEにする
	std::list<DWORD>::iterator itr;				//リストstagePartsListのイテレータ
	std::list<D3DXMATRIX>::iterator matItr;			//リストblocksPosのイテレータ

public:
	StageEditor();			//コンストラクタ
	~StageEditor();			//デストラクタ

	HRESULT Load();			//読み込み処理
	HRESULT Update();		//更新処理
	HRESULT Render();		//描画処理
};

