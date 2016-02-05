/*===========================================================================================
概　略：プレイヤーの処理をまとめたクラスヘッダー
作成日：10月15日
更新日：10月15日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "UnitBase.h"	//ユニットベースクラス
#include "Fbx.h"		//Fbxクラス
#include "Sprite.h"		//スプライトクラス

//定数宣言
#define QUOT 90;		//90度

//多重定義防止処理
#pragma once

//ユニットベースクラスを継承する
class Player : public UnitBase
{
	//-----------privateメンバ--------------

	//クラス変数
	Fbx*	player;		//プレイヤーのモデル用のFbxクラスのインスタンス作成
	Sprite* lifeMark[5];	//ライフの画像用のSpriteクラスのインスタンス作成

	//メンバ変数
	D3DXVECTOR3 move;			//移動量
	BOOL		isStage;		//プレイヤーがステージに当たっているか判定
	float		dirX;			//x軸の回転量
	float		dirZ;			//z軸の回転量
	DWORD		blockId;		//ステージの種類
	D3DXMATRIX  matRX;			//x軸に回転させる行列
	D3DXMATRIX	matRZ;			//z軸に回転させる行列
	D3DXVECTOR3 normal;			//各ステージブロックのの法線
	D3DXVECTOR3 normalVec;		//すべりベクトルで使うベクトル(常に真下に飛ばす)
	int			life;			//体力
	int			timeCount;		//時間をカウント
	BOOL		isSprikHit;		//障害物と衝突したかどうか
	BOOL		isDamege;		//ダメージを受けたかどうか
	BOOL		isDown;			//ステージからおちているかどうか
	float		downTime;		//ステージから落ちている時間

	//メンバ関数
	HRESULT Move();				//移動処理

	HRESULT frontMove();		//前移動処理
	HRESULT backMove();			//後移動処理
	HRESULT leftMove();			//左移動処理
	HRESULT rightMove();		//右移動処理

	HRESULT frontFriction();	//前摩擦処理
	HRESULT backFriction();		//後摩擦処理
	HRESULT leftFriction();		//左摩擦処理
	HRESULT rightFriction();	//右摩擦処理

	HRESULT Rotation();			//回転処理
	HRESULT MoveCamera();		//カメラの移動処理


	//------------publicメンバ--------------
public:

	//メンバ関数
	 Player();			//コンストラクタ
	~Player();			//デストラクタ

	HRESULT Load();						//読み込み処理
	HRESULT Update();					//更新処理
	HRESULT Hit(UnitBase* pTarget);		//衝突判定処理
	HRESULT Render();					//描画処理

};

