/*===========================================================================================
概　略：ゲームの音を鳴らす処理
作成日：7月3日
更新日：7月3日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Effect2D.h"

//コンストラクタ
Effect2D::Effect2D()
{
	//初期化
	sprite = 0;
}

//デストラクタ
Effect2D::~Effect2D()
{
	//リストの解放
	dataList.clear();

	//スプライトクラス削除
	SAFE_DELETE(sprite);
}

//機能：読み込み処理
//引数：fileName	ファイルの名前
//引数：sz			画像のサイズ
//引数：sp			描画速度
//戻値：成功か失敗か
HRESULT Effect2D::Load(char* fileName,D3DXVECTOR2 sz,float sp)
{
	//スプライトクラスを動的作成
	sprite = new Sprite;

	//サイズと描画スピードを入れる
	size = sz;
	speed = sp;

	sprite->Load(fileName);		//エフェクト用の画像の読み込み
	
	return S_OK;				//成功を返す
}

//機能：描画処理
//引数：なし
//戻値：成功か失敗か
HRESULT Effect2D::Draw()
{	
	SpriteData data;			//スプライトクラスの情報を入れる構造体の準備

	//エフェクトをアニメショーンして表示させる
	data.size = size;


	//dataListのイテレータ作成
	std::list<EffectData>::iterator dL_i;

	for (dL_i = dataList.begin(); dL_i != dataList.end();)				//配列の数だけループ
	{
		//位置情報
		data.pos.x = (*dL_i).position.x;	//イテレータが指しているpotisionの番地
		data.pos.y = (*dL_i).position.y;	//dL_i->position.yでも可能

		//切り抜き位置情報
		data.cut.x = (int)(*dL_i).anime * data.size.x;	//anime変数をint型にして位置がずれるのを防ぐ
		data.cut.y = 0;

		sprite->Draw(&data);								//入れたデータをもとに描画

		(*dL_i).anime += speed;							//カウントアップ（小数点で計算カウントアップして描画速度を遅くする）

		//animeが最後まで描画したら（画像の全体の幅　/　1マスのサイズ と比較する）
		if (sprite->GetSize().x / size.x < (*dL_i).anime)
		{
			//エフェクトの削除
			dL_i = dataList.erase(dL_i);

			//エフェクトを消して次のイテレータを入れたのでインクリメントをしないで終わる
			continue;
		}

		//エフェクトを削除しなかった時だけインクリメント
		dL_i++;
	}

	return S_OK;				//成功を返す


	/*	if (sprite->GetSize().x / size.x < (*dL_i).anime)
	{
		
		dL_i = dataList.erase(dL_i);

	}

	else
	{
		dL_i++;
	}
	の書き方でも良い
	*/

}

//機能：エフェクト画像を追加する
//引数：pos　描画位置
//戻値：成功か失敗か
HRESULT Effect2D::Add(D3DXVECTOR3 pos)
{
	//構造体作成
	EffectData d;

	//引数を元にデータ変更
	d.position = pos;
	d.anime = 0;

	//配列に追加
	dataList.push_back(d);

	//成功を返す
	return S_OK;
}