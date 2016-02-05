/*===========================================================================================
概　略：カメラの設定をするヘッダー
作成日：8月27日
更新日：8月27日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Global.h"

//多重定義防止処理
#pragma once

class Camera
{
	//-------------ビュー行列で使う変数-------------
	D3DXVECTOR3     pos;     //カメラの位置
	D3DXVECTOR3		target;  //焦点位置
	D3DXVECTOR3     upVec;   //上方向

	//-------------射影行列で使う変数-----------------
	float angle;        //画角
	float aspect;      //アスペクト比
	float nearClip;    //近クリッピング
	float farClip;     //遠クリッピング

	//------------行列を入れる変数-----------------
	D3DXMATRIX		view;    //ビュー行列
	D3DXMATRIX		proj;	 //射影行列
	
	//-----------行列作成する関数-------------------
	HRESULT SetView();			//ビュー行列を作成する
	HRESULT SetProjection();	//射影行列を作成する

public:

	 Camera();				//コンストラクタ
	~Camera();				//デストラクタ

	HRESULT Update();		//更新処理

	//----------------ビュー行列で使う変数に値を渡すアクセス関数(全てインライン関数)-------------
	void SetPos	  (D3DXVECTOR3 v){ pos	  = v; }		 //カメラの位置
	void SetTarget(D3DXVECTOR3 v){ target = v; }		 //焦点位置
	void SetUp	  (D3DXVECTOR3 v){ upVec  = v; }		 //上方向

	//----------------射影行列で使う変数に値を渡すアクセス関数(全てインライン関数)--------------
	void SetAngle	(float f){ angle	= f; }			//画角
	void SetAspect	(float f){ aspect	= f; }			//アスペクト比
	void SetNearClip(float f){ nearClip = f; }			//近クリッピング
	void SetFarClip	(float f){ farClip	= f; }			//遠クリッピング

	//---------------各行列を渡すアクセス関数(全てインライン関数)--------------------------------
	D3DXMATRIX* GetView(){ return &view; }				//ビュー行列
	D3DXMATRIX* GetProj(){ return &proj; }				//射影行列


};

