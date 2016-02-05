/*===========================================================================================
概　略：カメラの設定をするクラスの関数定義
作成日：8月27日
更新日：8月27日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Camera.h"

//コンストラクタ
Camera::Camera()
{
	//ビュー行列用の変数に初期値を代入
	pos	   =	D3DXVECTOR3(0, 3, -10);		//カメラの位置
	target =	D3DXVECTOR3(0, 0, 0);		//焦点位置
	upVec  =	D3DXVECTOR3(0, 1, 0);		//上方向

	//射影行列用の変数に初期値を代入
	angle = 60;												//画角
	aspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;			//アスペクト比
	nearClip = 0.1f;										//近クリッピング	これは０にしないほうがいい
	farClip = 1000.0f;										//遠クリッピング

}

//デストラクタ
Camera::~Camera()
{

}

//機能：ビュー行列(カメラの位置や向き)を作成する
//引数：なし
//戻値：失敗か成功か
HRESULT Camera::SetView()
{
	//ビュー行列を作成(引数は全てアドレスで、行列を入れる変数,カメラの位置,焦点位置,上方向)
	D3DXMatrixLookAtLH(&view, &pos, &target, &upVec);

	//ビュー行列をセット
	if (FAILED(g_pDevice->SetTransform(D3DTS_VIEW, &view)))					//SetTransformでセット、この関数は別の行列がセットされるまでその行列がセットされ続ける(第１引数に行列の種類を書く)
	{
		//行列のセットに失敗したらメッセージボックスで失敗を知らせる
		MessageBox(0, "ビュー行列の設定に失敗しました", "Camera", MB_OK);

		return E_FAIL;	//失敗を返す
	}

	return S_OK;		//成功を返す
}

//機能：射影行列(画角やアスペクト比などの設定)を作成する
//引数：なし
//戻値：失敗か成功か
HRESULT Camera::SetProjection()
{
	//射影行列の作成(引数は、行列を入れる変数(アドレス),画角(ラジアンに変換),アスペクト比,近クリッピング面までの距離,遠クリッピング面までの距離)
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(angle), aspect, nearClip, farClip);

	//射影行列のセット
	if (FAILED(g_pDevice->SetTransform(D3DTS_PROJECTION, &proj)))
	{
		//行列のセットに失敗したらメッセージボックスで失敗を知らせる
		MessageBox(0, "射影行列の設定に失敗しました", "Camera", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	return S_OK;			//成功を返す
}

//機能：更新処理
//引数：なし
//戻値：失敗か成功か
HRESULT Camera::Update()
{
	//ビュー行列作成関数を呼ぶ
	if (FAILED(SetView()))
	{
		//失敗していたら失敗を返す
		return E_FAIL;		
	}
	
	//射影行列作成関数を呼ぶ
	if (FAILED(SetProjection()))
	{
		//失敗していたら失敗を返す
		return E_FAIL;
	}

		return S_OK;		//成功を返す
}