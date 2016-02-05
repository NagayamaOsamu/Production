/*===========================================================================================
概　略：２Ｄの四角形を作るクラスの関数定義
作成日：9月1日
更新日：9月1日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Quad.h"

//コンストラクタ
Quad::Quad()
{
	//初期化
	pTexture = 0;
}

//デストラクタ
Quad::~Quad()
{
	//テクスチャの解放
	SAFE_RELEASE(pTexture);
}

//機能：読み込み処理
//引数：ファイル名
//戻値：成功か失敗か
HRESULT Quad::Load(char* fileName)
{
	//「テクスチャオブジェクト」の作成
	if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &pTexture)))				//第1引数にDirect３Dデバイス、第2引数にファイル名を
	{																								//指定するとテクスチャが作られ、
		//失敗したらどの画像が読み込めなかったか表示												//最後の引数に指定した変数に、完成したテクスチャがはいる
		MessageBox(0, "テクスチャの作成に失敗しました", fileName, MB_OK);

		return E_FAIL;	//失敗したことを返す(シーンのユニットなどで失敗しているとき)
	}

	return S_OK;	//成功したことを返す
}

//機能：四角形ポリゴンの描画
//引数：ワールド行列
//戻値：失敗か成功か
HRESULT Quad::Draw(D3DXMATRIX* matWorld, D3DXMATRIX* matTex)
{
	//頂点ストリームを入れる変数を構造体で作成
	struct Vertex
	{
		D3DXVECTOR3   pos;		//位置情報
		DWORD		color;		//色
		D3DXVECTOR2    uv;		//テクスチャのUV座標(頂点ごとにテクスチャのどの部分を貼り付けるか)
	};

	//四角形作るに当たってDirectXだとポリゴンは三角形しか存在しない
	//なので三角形を2つ作りそれを合体させて四角形にする
	//それ以外にはD3DPT_TRIANGLESTRIPに指定する
	//LISTは離れた複数の図形を描くものに対し
	//STRIPは繋がった図形を描くものなので頂点を４つ指定するだけで四角形を書ける

	//配列を作って初期値を代入
	Vertex point[4] =
	{
		D3DXVECTOR3(-0.5, 0.5, 0)   , D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 0),       //左上
		D3DXVECTOR3( 0.5 , 0.5 , 0) , D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 0),       //右上
		D3DXVECTOR3(-0.5 , -0.5, 0) , D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 1),       //左下
		D3DXVECTOR3( 0.5 , -0.5 ,0) , D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 1),       //右下
	};

	//四角形の情報を伝える
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)))		//引数は ｜ を使うことで複数の情報を設定できる
	{
		//失敗したらメッセージボックスで失敗を知らせる
		MessageBox(0, "頂点ストリームの設定に失敗しました", "Line", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//引数の行列をワールド行列に変換する
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		//変換に失敗したらメッセージボックスで知らせる
		MessageBox(0, "ワールド行列の設定に失敗しました", "Quad", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//テクスチャのセット
	if (FAILED(g_pDevice->SetTexture(0, pTexture)))
	{
		//セットに失敗したらメッセージボックスで知らせる
		MessageBox(0, "テクスチャのセットに失敗しました", "Quad", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//テクスチャ行列が空でないときだけ行列の設定を行う
	if (matTex != 0)
	{

		//テクスチャ行列のセット
		if (FAILED(g_pDevice->SetTransform(D3DTS_TEXTURE0, matTex)))
		{
			//失敗したらメッセージボックスで知らせる
			MessageBox(0, "テクスチャ行列の設定に失敗しました", "Quad", MB_OK);

			return E_FAIL;		//失敗を返す
		}
	}

	//ライトをOffにする
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "ライトの設定に失敗しました", "Quad", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//四角形の描画
	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, point, sizeof(Vertex))))		//引数は、描画する種類,表示する数,頂点情報,１頂点のデータサイズ
	{
		//描画に失敗したらメッセージボックスで知らせる
		MessageBox(0, "ポリゴンの描画に失敗しました", "Quad", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//ライトをONにする
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "ライトの設定に失敗しました", "Quad", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	return S_OK;			//成功を返す
}