/*===========================================================================================
概　略：ゲームオーバー画面での処理をまとめたクラスの関数定義
作成日：12月21日
更新日：12月21日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "GameOver.h"

//コンストラクタ
GameOver::GameOver()
{
	//クラスの初期化
	gameOver = NULL;
}

//デストラクタ
GameOver::~GameOver()
{
	//クラスの解放
	SAFE_DELETE(gameOver);
}

//機能：読み込み処理
//引数：なし
//戻値：成功か失敗か
HRESULT GameOver::Load()
{
	//クラスを動的に作成
	gameOver = new Sprite;

	//ゲームオーバー画像の読み込み
	if (FAILED(gameOver->Load("Asset\\Sprite\\GAMEOVER.bmp")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "ゲームオーバー画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	return S_OK;			//成功を返す
}

//機能：更新処理
//引数：なし
//戻値：成功か失敗か
HRESULT GameOver::Update()
{
	//スペースキーが押されたらタイトルへ戻る
	/*if (g_pInput->IsKeyTap(DIK_SPACE))
		g_gameScene = SC_TITLE;*/

	return S_OK;			//成功を返す
}

//機能：描画処理
//引数：なし
//戻値：成功か失敗か
HRESULT GameOver::Render()
{
	//ゲームオーバー画像を描画
	SpriteData data;			//画像の情報をまとめた構造体の準備
	//今回は情報を変更せずにそのまま描画
	gameOver->Draw(&data);

	return S_OK;			//成功を返す
}