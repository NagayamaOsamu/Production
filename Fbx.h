/*===========================================================================================
概　略：Mayaで作成したモデル(Fbx形式)を描画するクラスヘッダー
作成日：9月10日
更新日：10月5日
制作者：永山治夢
=============================================================================================*/

//--------------------インクルード------------------
#include "Global.h"
#include <fbxsdk.h>	
#include <vector>

//--------------------ライブラリ-----------------
#pragma comment(lib,"libfbxsdk-mt.lib")

//多重定義防止処理
#pragma once

//レイキャスト用構造体(D3DXIntersetTri関数で必要になるデータをまとめる)
struct RayCastData
{
	D3DXVECTOR3 start;	//レイ発射位置
	D3DXVECTOR3 dir;	//レイの向きベクトル
	float		dist;	//衝突点までの距離
	D3DXVECTOR3 normal;	//衝突点の法線
	D3DXVECTOR3 pos;	//衝突位置
	BOOL		hit;	//レイが当たったか
	D3DXMATRIX	matrix;	//モデルの行列
};

class Fbx
{
	//FBXファイルを開くのに必要な変数
	FbxManager*					manager;					//マネージャ(FBX機能を管理する)
	FbxImporter*				importer;					//インポータ(ファイルを開くのに必要)
	FbxScene*					scene;						//シーン(開いたファイルを管理する)

	//メッシュの頂点バッファを作るのに必要な変数
	int							vertexCount;				//頂点の数
	IDirect3DVertexBuffer9      *pVertexBuffer;				//作った頂点バッファを入れる変数

	//インデックスバッファを作るのに必要な変数
	int polygonCount;										//ポリゴン数
	int indexCount;											//インデックス数
	IDirect3DIndexBuffer9    **pIndexBuffer;				//作ったインデックスバッファを入れる変数のポインタのポインタ(ポインタ変数のアドレスを格納する変数)

	//マテリアル作成に必要な変数
	int					  materialCount;					//マテリアルの個数
	D3DMATERIAL9          *pMaterial;						//マテリアルの配列を作るのに必要なポインタ
	int*				  polygonCountOfMaterial;			//マテリアル毎のポリゴン数を入れるポインタ

	//テクスチャを貼るのに必要な変数
	LPDIRECT3DTEXTURE9       *pTexture;						//テクスチャを入れるポインタ

	//レイキャスト処理で使う処理
	D3DXMATRIX lastWorldMatrix;								//最後に使った行列を記憶させる変数
	std::vector<D3DXMATRIX> lastMatrix;
	
	
	//メッシュに必要な頂点情報を構造体に入れる
	struct Vertex
	{
		D3DXVECTOR3 pos;	//位置情報
		D3DXVECTOR3 normal;	//法線情報(法線は頂点情報だがポリゴン毎に決まっている)
		D3DXVECTOR2						uv;						//UV座標
	};

	//プライベート関数
	HRESULT      CheckNode(FbxNode* pNode);		//メッシュノードを探す関数
	HRESULT      CheckMesh(FbxMesh* pMesh);		//メッシュの頂点情報を取り出す関数

	//バッファ作成
	HRESULT		CreateVertexBuf(FbxMesh* pMesh);				//頂点バッファ作成
	HRESULT		CreateindexBuf(FbxMesh* pMesh);					//インデックスバッファ作成

public:
	 Fbx();		//コンストラクタ
	~Fbx();		//デストラクタ

	HRESULT Load(char* fileName);			//読み込み処理
	HRESULT Draw(D3DXMATRIX* matWorld);		//描画処理
	BOOL	RayCast(RayCastData *data);		//レイキャスト処理

	BOOL		HitRay(D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist,D3DXMATRIX matW);
	BOOL		HitRay(D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist, D3DXVECTOR3* normal, D3DXMATRIX matW);
	//BOOL		HitRay(MeshFBX * pMesh D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist, D3DXVECTOR3* normal);

	BOOL HitMouseRay(D3DXMATRIX matW);		//マウス座標とFbxの衝突判定
};

