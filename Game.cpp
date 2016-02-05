/*=======================================================================================================
概　略：ゲーム全体の流れを扱う処理
作成日：6月26日
更新日：6月26日
制作者：永山治夢
=======================================================================================================*/

//---------------インクルード-----------------------
#include "Game.h"

//各シーンのインクルード
#include "TitleScene.h"			//タイトルシーン
#include "StageSelectScene.h"	//ステージ選択シーン
#include "StageEditorScene.h"	//ステージ作成シーン
#include "PlayScene.h"			//プレイシーン
#include "ClearScene.h"			//クリアシーン
#include "GameOverScene.h"		//ゲームオーバーシーン

//カメラクラス
#include "Camera.h"

//----------------グローバル変数----------------
GAME_SCENE		  g_gameScene;		//現在のゲームシーン
LPDIRECT3DDEVICE9 g_pDevice;		//Direct3Dデバイスオブジェクト
Input*			  g_pInput;			//入力処理オブジェクト
Camera*           g_pCamera;		//カメラオブジェクト  

//――――――――――――――――――――――
//	コンストラクタ
//――――――――――――――――――――――
Game::Game()
{
	//各シーンオブジェクトの生成
	scene[SC_TITLE]		   =	new TitleScene;			//タイトルシーン
	scene[SC_STAGE_SELECT] =    new StageSelectScene;	//ステージ選択シーン
	scene[SC_STAGE_EDITOR] =    new StageEditorScene;	//ステージ作成シーン
	scene[SC_PLAY]		   =	new PlayScene;			//プレイシーン
	scene[SC_CLEAR]		   =	new ClearScene;			//クリアシーン
	scene[SC_GAMEOVER]	   =	new GameOverScene;		//ゲームオーバーシーン

	//最初はタイトルシーン
	g_gameScene = SC_STAGE_SELECT;

	//カメラクラスを動的作成
	g_pCamera = new Camera;
}

//――――――――――――――――――――――
//	デストラクタ
//――――――――――――――――――――――
Game::~Game()
{
	for (int i = 0; i < SC_MAX; i++)
	{
		delete scene[i];
	}

	//DirectX開放
	SAFE_DELETE(g_pCamera);		//カメラクラス
	SAFE_RELEASE(g_pDevice);	//デバイスオブジェクト
	SAFE_RELEASE(pD3d);			//Direct3Dオブジェクト
}

//――――――――――――――――――――――
// Direct3D初期化
//――――――――――――――――――――――
HRESULT Game::InitD3d(HWND hWnd)
{
	// 「Direct3D」オブジェクトの作成
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))		//Direct3Dを作成して失敗してないか判定
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);		//失敗していたらメッセージボックスで失敗を表示

		return E_FAIL;		//失敗したことを返す
	}


	// 「DIRECT3Dデバイス」オブジェクトの作成(構造体)
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));								//初期化
	d3dpp.BackBufferFormat =			   D3DFMT_UNKNOWN;			//バックバッファのフォーマット
	d3dpp.BackBufferCount =				   1;						//用意するバックバッファの数
	d3dpp.SwapEffect =					   D3DSWAPEFFECT_DISCARD;	//スワップ
	d3dpp.Windowed =					   TRUE;					//ディスプレイ全体に絵を表示させたいときはFALSE
	d3dpp.EnableAutoDepthStencil =		   TRUE;					//Zバッファ(奥行き）を使う(TURE)か使わない(FALSE)か
	d3dpp.AutoDepthStencilFormat =		   D3DFMT_D16;				//バッファのフォーマット

	//ゲーム画面の作成
	if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice)))				//環境によってはDirect3Dの作成に失敗する可能性があるためif文にする
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);

		//パフォーマンスを落として再度作成
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice)))			//それでも失敗する可能性がある
		{
			//失敗を知らせる
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;			//失敗を返す
		}
	}
	
	//ライトをOffにする
	//if (FAILED(LigtOff()))
	//{
	//	return E_FAIL;				//失敗を返す
	//}
	
	//カリングモードの設定を行う
	//if (FAILED(CullingConfig()))
	//{
	//	return E_FAIL;				//失敗を返す
	//}

	//アルファブレンドの設定を行う
	if (FAILED(AlphaConfig()))
	{
		return E_FAIL;				//失敗を返す
	}

	//テクスチャ行列の設定を行う
	if (FAILED(TexMatConfig()))
	{
		return E_FAIL;				//失敗を返す
	}

	//ライトの初期化設定
	if (FAILED(InitLight()))
	{
		return E_FAIL;		//失敗を返す
	}

	
	return S_OK;				//成功を返す

}


//――――――――――――――――――――――
//	読み込み処理
//――――――――――――――――――――――
HRESULT Game::Load()
{	
	//全てのシーンの読み込み
	for (int i = 0; i < SC_MAX; i++)
	{
		//どこかで失敗していたら
		if (FAILED(scene[i]->Load()))
		{
			return E_FAIL;	//失敗を返す（Main関数に）
		}
	}
	return S_OK;
}

//――――――――――――――――――――――
//	衝突判定
//――――――――――――――――――――――
HRESULT Game::Hit()
{
	//どこかで失敗していたら
	if (FAILED(scene[g_gameScene]->Hit()))
	{
		return E_FAIL;	//失敗していたら
	}
	return S_OK;	//成功を変えす
}

//――――――――――――――――――――――
//	更新処理
//――――――――――――――――――――――
HRESULT Game::Update()
{
	//入力処理の更新処理（キーの状態を調べる）
	if (g_pInput)
	{
		//インプットクラスが存在しているときだけ呼ぶ
		g_pInput->Update();
	}

	//どこかで失敗していたら
	if (FAILED(scene[g_gameScene]->Update()))
	{
		return E_FAIL;	//失敗を返す
	}

	//カメラクラスの更新処理も呼び出す
	if (FAILED(g_pCamera->Update()))
	{
		return E_FAIL;	//失敗を返す
	}

	return S_OK;		//成功を返す
}

//――――――――――――――――――――――
//	描画処理
//――――――――――――――――――――――
HRESULT Game::Render()
{
	//画面をクリア(XRGBで画面の色を青　ZBUFFERでzバッファもクリアにしている）
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pDevice->BeginScene()))		//DirectXだとここで失敗するときもあるので成功したか判定する
	{

		//ゲーム画面の描画
		if (FAILED(scene[g_gameScene]->Render()))	//どこかで失敗していたら
		{
			return E_FAIL;	//失敗を返す
		}

		//描画終了
		g_pDevice->EndScene();
		//BeginSceneとEndSceneはBeginPaintとEndPaintのようなイメージでよい
	}
	
	//フリップ
	//Direct3Dでは2枚の画面（フロントバッファ、バックバッファ）を用意してバックバッファのほうに画像を描画する
	//全ての画像を描画したら、バックバッファとフロントバッファを交換するこれをフリップという
	g_pDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;	//成功を返す
}

//機能：ライトをOffにする
//引数：なし
//戻値：成功か失敗か
HRESULT Game::LigtOff()
{
	//ライトをOFF状態にする
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))			//第1引数が「何を」第2引数が「どうするか」になっている
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "ライトの設定に失敗しました", "エラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}

	return S_OK;				//成功を返す

}

//機能：カリングモードの設定
//引数：なし
//戻値：成功か失敗か
HRESULT Game::CullingConfig()
{
	//カリングモード(ポリゴンの裏面を表示しないこと)をOFFにする
	if (FAILED(g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "カリングモードの設定に失敗しました", "エラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}

	return S_OK;				//成功を返す
}

//機能：アルファブレンドの設定を行う
//引数：なし
//戻値：成功か失敗か
HRESULT Game::AlphaConfig()
{
	//アルファブレンドを有効にして半透明の部分をそのまま半透明に表示させる
	//SetRendeerStateを3回呼んで設定する
	if (FAILED(g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)		||			//1つ目のでアルファブレンドの設定
			   g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) ||			//2つ目で今から使うテクスチャ等の透明度を決める
			   g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))		//3つ目で背景の色を2つ目の透明度の逆の透明度にする
	{
		//失敗したらメッセージボックスで表示
		MessageBox(0, "アルファブレンドの設定に失敗しました", "エラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}

	return S_OK;				//成功を返す
}

//機能：テクスチャ行列の設定を行う
//引数：なし
//戻値：成功か失敗か
HRESULT Game::TexMatConfig()
{
	//テクスチャ行列(テクスチャの切り取りなどを行えるようにする)を使えるようにする
	if (FAILED(g_pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2)))		//第2引数は「何を」、第3引数は「どうするか」
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "テクスチャステートの設定に失敗しました", "エラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}

	return S_OK;				//成功を返す

}

//機能：ライトの初期設定
//引数：なし
//戻値：失敗か成功か
HRESULT Game::InitLight()
{
	
	D3DLIGHT9 light;							//ライトの設定をまとめた構造体の準備
	ZeroMemory(&light, sizeof(D3DLIGHT9));		//構造体を初期化

	//ライトの設定
	light.Type = D3DLIGHT_DIRECTIONAL;						//ライトの種類をディレクショナルライト(平行ライト)
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 1.0f);		//ライトの向きの

	//ライトの色(強さ)を０～１の範囲で(１を超えても良い)
	light.Diffuse.r = 1.0f;									//赤				
	light.Diffuse.g = 1.0f;									//緑
	light.Diffuse.b = 1.0f;									//青

	//アンビエントの設定(一律で明るくするライトの設定)
	light.Ambient.r = 0.4f;									//赤
	light.Ambient.g = 0.4f;									//緑
	light.Ambient.b = 0.4f;									//青

	//↑で設定した値でライトを作成
	if (FAILED(g_pDevice->SetLight(0, &light)))				//引数は(ライトの番号(何個目のライトか),構造体のアドレス)
	{
		//設定に失敗したらメッセージボックスで知らせる
		MessageBox(0, "ライトをセットできませんでした", "Game", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//ライトの設定をONにする
	if (FAILED(g_pDevice->LightEnable(0, TRUE)))
	{
		//ライトをONできなかったらメッセージボックスで知らせる
		MessageBox(0, "ライトを有効にできませんでした", "Game", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	return S_OK;		//成功を返す
}