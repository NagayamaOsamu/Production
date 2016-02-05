/*===========================================================================================
概　略：ステージの処理をまとめたクラスヘッダー
作成日：10月15日
更新日：10月15日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "UnitBase.h"	//ユニットベースクラス
#include "Fbx.h"		//Fbxクラス
#include <vector>		//動的配列

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
class Stage : public UnitBase
{
	//------------構造体-----------------

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


	//各ノード情報をまとめた構造体
	struct Node
	{
		D3DXMATRIX matrix;							//各ブロックごとの行列
		DWORD type;									//ブロックの種類(直線、T字路、上り・下り坂)
		std::vector<Node*> childNode;				//ブロックが続く数が複数あるので動的配列にする
		D3DXVECTOR3 pos;							//位置
		std::vector<D3DXVECTOR3> sprikPos;			//障害物の位置
	};

	//-------------メンバ変数----------------

	char flg;

	//障害物のモデル変数
	Fbx* sprik;

	//ノード変数
	Node startNode;									//開始ノード	
	DWORD nodeID;									//ノードの番号（開始点から何個目のノードか）

	//プレイヤーの位置情報
	D3DXVECTOR3 playerPos;							//プレイヤーの位置

	//ステージブロック変数
	DWORD stageBlock;								//ステージブロックの種類

	//衝突処理で使う変数
	D3DXVECTOR3 blocksNormal;						//ステージブロックの法線
	BOOL		isSprikHit;							//障害物と衝突したらtrueをしていないときはfalseにする
	BOOL		isPlayerDown;						//プレイヤーがステージから落ちているかどうか

	//-----------private関数--------------------
	void Create(D3DXVECTOR3 pos, float angle, Node* pNode);				//ステージ作成
	HRESULT BlocksRender(D3DXVECTOR3 pos, float angle, Node* pNode);	//パーツ描画
	void Clear(Node* pNode);											//ノードの削除
	HRESULT Hit(Node* pNode);											//ステージブロックとプレイヤーの衝突判定


public:
	Stage();		//コンストラクタ
	~Stage();		//デストラクタ

	HRESULT Load();						//読み込み処理
	HRESULT Update();					//更新処理
	HRESULT Hit(UnitBase* pTarget);		//衝突判定処理
	HRESULT Render();					//描画処理

	//-----------アクセス関数------------------
	float		GetPlayerPos()	 { return playerPos.y; }		//プレイヤーとステージブロックの衝突位置を渡す
	D3DXVECTOR3 GetBlocksNormal(){ return blocksNormal; }		//ステージブロックの法線を渡す
	BOOL		GetIsSprikHit()	 { return isSprikHit; }			//障害物とプレイヤーが衝突したかどうかを返す
	BOOL		GetPlayerDown(){ return isPlayerDown; }			//プレイヤーがステージから落ちているかどうかを返す
};

