/*===========================================================================================
概　略：スプライト扱う処理
作成日：6月26日
更新日：6月26日
制作者：永山治夢
=============================================================================================*/
//インクルード
#include "Sprite.h"

//コンストラクタ
Sprite::Sprite()
{

}

//デストラクタ
Sprite::~Sprite()
{
	//解放処理
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pSprite);
}

//-------------------------------------
//	機能：画像の読み込み処理
//	引数：画像ファイルの名前
//	戻値：成功か失敗したことを返す
//--------------------------------------
HRESULT Sprite::Load(char* fileName)
{
	//「スプライトオブジェクトの作成]
	if (FAILED(D3DXCreateSprite(g_pDevice, &pSprite)))
	{
		//失敗したらメッセージボックスで表示
		MessageBox(0, "スプライトの作成に失敗しました", "エラー", MB_OK);

		return E_FAIL;		//失敗したことを返す(シーンのユニットなどで失敗しているとき)
	}

	//「テクスチャオブジェクト」の作成
	if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &pTexture)))				//第1引数にDirect３Dデバイス、第2引数にファイル名を
	{																								//指定するとテクスチャが作られ、
		//失敗したらどの画像が読み込めなかったか表示												//最後の引数に指定した変数に、完成したテクスチャがはいる
		MessageBox(0, "テクスチャの作成に失敗しました", fileName, MB_OK);

		return E_FAIL;	//失敗したことを返す(シーンのユニットなどで失敗しているとき)
	}

	//テクスチャのサイズを取得
	D3DSURFACE_DESC d3dds;					//テクスチャの情報を扱う構造体
	pTexture->GetLevelDesc(0, &d3dds);		//テクスチャの情報を取得
	texsize.x = (FLOAT)d3dds.Width;				//幅のサイズ取得
	texsize.y = (FLOAT)d3dds.Height;				//高さのサイズ取得

	return S_OK;	//成功したことを返す

}

//------------------------------------
//	機能：画像の描画
//	引数：なし
//	戻値：なし
//------------------------------------
void Sprite::Draw(SpriteData* data)
{
	//基準点
	D3DXVECTOR3  center = data->center;
	center.z = 0;	//SpriteData構造体のcenterはD3DXVECTOR2型なのでz軸が存在しないのでこの処理を追加しないとz軸の値が変になる	

	//サイズを省略した場合は画像サイズを使う
	if (data->size.x == -999) data->size.x = texsize.x;
	if (data->size.y == -999) data->size.y = texsize.y;

	//基準点が初期値のままだったら
	if (center.x == -999 && center.y == -999)
	{
		center.x = data->size.x / 2;	//画像の中心にする
		center.y = data->size.y / 2;
	}

	//拡大行列
	D3DXMATRIX matScale;	//拡大行列用変数作成


	//拡大行列作成
	D3DXMatrixScaling(&matScale, data->scale.x,data->scale.y, 1);


	//回転行列
	D3DXMATRIX matRotate;		//回転行列用変数作成
	//回転行列作成
	D3DXMatrixRotationZ(&matRotate, D3DXToRadian(data->angle));		//D3DXToRadianで角度の単位をラジアンに変換できる

	//移動行列
	D3DXMATRIX matTrans;	//移動用行列変数作成
	//移動行列作成
	D3DXMatrixTranslation(&matTrans, data->pos.x + center.x  * data->scale.x, data->pos.y + center.y  * data->scale.y, 0);	//基準点と拡大縮小を掛けると基準点を
																															//ずらした分を直すことができる

	//先に拡大行列を作っているため他の行列を作っても普通は使えない
	//このときは行列を「掛け算」することで合成できる
	D3DXMATRIX matWorld = matScale * matRotate * matTrans;		//行列掛け算


	//スプライトに行列を適用
	pSprite->SetTransform(&matWorld);

	//基準点をさらに移動
	center.x += data->pos.x;
	center.y += data->pos.y;

	//描画始め
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);		//ALPHABLENDは和訳するとALPHAは透明度,BLENDは混ぜる

	//描画位置(z軸は使わない)
	D3DXVECTOR3 position = data->pos;		//D3DXVECTOR3はPOINT関数と似ているx軸,y軸,に加えてz軸がある

	//切り抜き位置
	RECT cut = { (LONG)data->cut.x, (LONG)data->cut.y, (LONG)data->cut.x + (LONG)data->size.x, (LONG)data->cut.y + (LONG)data->size.y };		//左上と右下の指定

	//描画(引数は左から貼り付けるテクスチャ,切抜き範囲,基準点,表示位置,色(透明度,赤,緑,青))	//色の変更は画像の元々の色を何％使うかになる
	pSprite->Draw(pTexture, &cut, &center, &position, D3DCOLOR_ARGB(data->color.a, data->color.r, data->color.g, data->color.b));			

	//描画終わり
	pSprite->End();
}




