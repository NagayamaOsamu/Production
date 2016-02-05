/*===========================================================================================
概　略：基本的なメッシュの処理をするクラスの関数定義
作成日：9月3日
更新日：9月3日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "BasicMesh.h"


//コンストラクタ
BasicMesh::BasicMesh()
{
	//初期化
	pMesh = 0;
}

//デストラクタ
BasicMesh::~BasicMesh()
{
	//LP型の解放
	SAFE_RELEASE(pMesh);
}

//機能：立方体のメッシュを作る
//引数：作成したいメッシュの種類
//戻値：成功か失敗か
HRESULT BasicMesh::Create(DWORD type)
{
	HRESULT hr;		//メッシュを作成したときの成功か失敗を入れる変数

	switch (type)
	{
		//立方体が選ばれた時
	case BMESH_BOX:

		//立方体のメッシュを作成(成功・失敗を変数に入れる)
		hr = D3DXCreateBox(g_pDevice, 1, 1, 1, &pMesh, NULL);					//引数は(デバイスオブジェクト,幅,高さ,奥行き,メッシュのアドレス,NULL)

		break;

		//球体が選ばれた時
	case BMESH_SPHERE:

		//球体のメッシュを作成
		hr = D3DXCreateSphere(g_pDevice, 1, 24, 24, &pMesh, NULL);				//引数は(デバイスオブジェクト,半径,角の数,分割数,,メッシュのアドレス,NULL)

		break;

		//円柱が選ばれた時
	case BMESH_CYLINDER:

		//円柱のメッシュを作成
		hr = D3DXCreateCylinder(g_pDevice, 1, 1, 3, 24, 1, &pMesh, NULL);		//引数は(デバイスオブジェクト,手前の半径,奥の半径,長さ,分割数,スタック数,メッシュのアドレス,NULL)

		break;

		//トーラス(ドーナッツ状)が選ばれた時
	case BMESH_TORUS:

		//トーラスのメッシュ作成
		hr = D3DXCreateTorus(g_pDevice, 0.5, 1, 12, 24, &pMesh, NULL);			//引数は(デバイスオブジェクト,穴の半径,外部の半径,辺の数,角の数,メッシュのアドレス,NULL)

		break;

		//ティーポットが選ばれた時
	case BMESH_TEAPOT:

		//ティーポットのメッシュ作成
		hr = D3DXCreateTeapot(g_pDevice, &pMesh, NULL);							//引数は(デバイスオブジェクト,メッシュのアドレス,NULL)

		break;
	}
	
	//メッシュの作成に失敗したら
	if (FAILED(hr))
	{
		//メッセージボックスで知らせる
		MessageBox(0, "メッシュの作成に失敗しました", "BasicMesh", MB_OK);

		return E_FAIL;		//失敗を返す
	}


	InitMaterial();			//マテリアルの設定をする関数の呼び出し

	return S_OK;			//成功を返す
}

//機能：マテリアルの設定
//引数：なし
//戻値：失敗か成功か
HRESULT BasicMesh::InitMaterial()
{
	//構造体を全て初期化
	ZeroMemory(&material, sizeof(D3DMATERIAL9));

	//ディフューズ(拡散反射)のRGB値とアルファ値の設定
	//ディフューズとは何色の光をどのくらい反射しているかをあらわす
	material.Diffuse.r = 1.0f;		//赤色の光の強さ
	material.Diffuse.g = 1.0f;		//緑色の光の強さ
	material.Diffuse.b = 1.0f;		//青色の光の強さ
	material.Diffuse.a = 1.0f;		//透明度

	//アンビエント(環境光)を反射させる設定
	material.Ambient.r = 1.0f;		//赤色の光
	material.Ambient.g = 1.0f;		//緑色の光
	material.Ambient.b = 1.0f;		//青色の光
	material.Ambient.a = 1.0f;		//透明度

	return S_OK;			//成功を返す
}

//機能：描画処理
//引数：ワールド行列
//戻値：失敗か成功か
HRESULT BasicMesh::Draw(D3DXMATRIX* matWorld)
{
	//テクスチャを貼れなくする
	if (FAILED(g_pDevice->SetTexture(0, NULL)))
	{
		//処理に失敗したらメッセージボックスで知らせる
		MessageBox(0, "テクスチャのリセットに失敗しました", "BasicMesh", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//ワールド行列変換
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		//設定に失敗したらメッセージボックスで知らせる
		MessageBox(0, "ワールド行列の設定に失敗しました", "BasicMesh", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	
	//マテリアルの設定のセット
	if (FAILED(g_pDevice->SetMaterial(&material)))
	{
		//設定に失敗したらメッセージボックスで知らせる
		MessageBox(0, "マテリアルの設定に失敗しました", "BasicMesh", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//立方体の描画(グループ分けをしない)
	if (FAILED(pMesh->DrawSubset(0)))				//引数はグループ(サブセット)を設定する
	{
		//描画に失敗したらメッセージボックスで知らせる
		MessageBox(0, "メッシュの描画に失敗しました", "BasicMesh", MB_OK);

		return E_FAIL;		//失敗を返す
	}
	return S_OK;			//成功を返す
}


