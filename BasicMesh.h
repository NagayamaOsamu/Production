/*===========================================================================================
概　略：基本的なメッシュ(３Ｄポリゴンの物体)の処理をするクラスのヘッダー定義
作成日：9月3日
更新日：9月3日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Global.h"


//各形状(メッシュ)に名前を付ける
enum
{
	BMESH_BOX,			//立方体
	BMESH_SPHERE,		//球体
	BMESH_CYLINDER,		//円柱
	BMESH_TORUS,		//トーラス
	BMESH_TEAPOT		//ティーポット
};

//多重定義防止処理
#pragma once

class BasicMesh
{
	LPD3DXMESH   pMesh;		//立方体のメッシュを入れる変数
	D3DMATERIAL9 material;	//マテリアルの設定項目をまとめた変数（構造体）

public:
	 BasicMesh();		//コンストラクタ
	~BasicMesh();		//デストラクタ


	HRESULT Create(DWORD type);					//立方体のメッシュを作る
	HRESULT InitMaterial();						//マテリアルの設定をする
	HRESULT Draw(D3DXMATRIX* matWorld);			//描画処理

};

