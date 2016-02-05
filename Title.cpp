/*===========================================================================================
概　略：タイトル画面での処理をまとめたクラスの関数定義
作成日：12月21日
更新日：12月21日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Title.h"

//コンストラクタ
Title::Title()
{
	//クラスの初期化
	title	   = NULL;		//タイトル画像
	wait	   = NULL;		//待機中の画像
	modeSelect = NULL;		//モードセレクトに使う画像
	arrow	   = NULL;		//矢印

	//メンバ変数の初期化
	state = WAIT_STATE;		//最初は待機状態
	mode = SELECT_MODE;		//最初はステージ選択モード
}

//デストラクタ
Title::~Title()
{
	//クラスの解放
	SAFE_DELETE(title);			//タイトル画像
	SAFE_DELETE(wait);			//待機中の画像
	SAFE_DELETE(modeSelect);	//モードセレクトに使う画像
	SAFE_DELETE(arrow);			//矢印画像
}


//機能：読み込み処理
//引数：なし
//戻値：成功か失敗か
HRESULT Title::Load()
{
	//クラスの動的作成
	title	   = new Sprite;		//タイトル画像
	wait	   = new Sprite;		//待機中の画像
	modeSelect = new Sprite;		//モードセレクトに使う画像
	arrow	   = new Sprite;		//矢印

	//タイトル画像の読み込み
	if (FAILED(title->Load("Asset\\Sprite\\title.bmp")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "タイトル画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}

	//待機中の画像の読み込み
	if (FAILED(wait->Load("Asset\\Sprite\\Wait.png")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "モーでセレクト画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}

	//モードセレクトに使う画像の読み込み
	if (FAILED(modeSelect->Load("Asset\\Sprite\\ModeSelect.png")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "モーでセレクト画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}

	if (FAILED(arrow->Load("Asset\\Sprite\\Arrow.png")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "矢印画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}

	return S_OK;				//成功を返す
}

//機能：更新処理
//引数：なし
//戻値：成功か失敗か
HRESULT Title::Update()
{
	//画面の状態毎に処理
	switch (state)
	{
		//待機状態のとき
	case WAIT_STATE:

		//スペースキーが押されたら
		if (g_pInput->IsKeyTap(DIK_SPACE))
		{
			state = MODESELECT_STATE;		//モードセレクト状態にする
		}

		break;

		//モードセレクト状態のとき
	case MODESELECT_STATE:

		//バックスペースキーが押されたら
		if (g_pInput->IsKeyTap(DIK_BACK))
		{
			state = WAIT_STATE;		//待機状態に戻す
		}

		//ステージ選択モードのとき
		if (mode == SELECT_MODE)
		{
			//十字上キーと十字下キーのどちらかが押されたとき
			if (g_pInput->IsKeyTap(DIK_UP) || g_pInput->IsKeyTap(DIK_DOWN))
			{
				mode = CREATE_MODE;		//ステージ作成モードにする
			}

			//エンターキーが押されたら
			else if (g_pInput->IsKeyTap(DIK_RETURN))
			{
				g_gameScene = SC_STAGE_SELECT;		//ステージ選択シーンにする
			}
		}

		//ステージ作成モードのとき
		else
		{
			//十字上キーと十字下キーのどちらかが押されたとき
			if (g_pInput->IsKeyTap(DIK_UP) || g_pInput->IsKeyTap(DIK_DOWN))
			{
				mode = SELECT_MODE;		//ステージ選択モードにする
			}

			//エンターキーが押されたら
			else if (g_pInput->IsKeyTap(DIK_RETURN))
			{
				g_gameScene = SC_STAGE_EDITOR;		//ステージ作成シーンにする
			}
		}

		break;
	}

	return S_OK;				//成功を返す
}

//機能：描画処理
//引数：なし
//戻値：成功か失敗か
HRESULT Title::Render()
{
	//タイトル画像の描画
	SpriteData data;				//スプライトの情報をまとめた構造体の準備
	title->Draw(&data);				//タイトル画像は情報を変えずに描画

	//画面の状態毎に処理
	switch (state)
	{
		//待機状態のとき
	case WAIT_STATE:

		//待機中の画像の読み込み
		data.pos.y = WINDOW_HEIGHT / 2;		//画面の半分より下にする
		wait->Draw(&data);					//描画

		break;

		//モード選択状態のとき
	case MODESELECT_STATE:

		//モードセレクト画像の描画
		data.pos.y = WINDOW_HEIGHT / 2;		//画面の半分より下にする
		modeSelect->Draw(&data);			//描画

		//選んでいるモードごとに位置を変えて矢印を描画
		if (mode == SELECT_MODE)		//ステージ選択モードのとき
		{
			//位置を変更
			data.pos.x = 250;
			data.pos.y = WINDOW_HEIGHT / 2 + 50;
			arrow->Draw(&data);		//描画
		}

		//ステージ作成モードのとき
		else
		{
			//位置を変更
			data.pos.x = 250;
			data.pos.y = WINDOW_HEIGHT / 2 + 220;
			arrow->Draw(&data);		//描画
		}
	}
	return S_OK;				//成功を返す
}