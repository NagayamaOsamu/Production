/*===========================================================================================
概　略：プロジェクト全体で必要なものをまとめたファイル
作成日：6月26日
更新日：6月26日
制作者：永山治夢
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include <windows.h>
#include <d3dx9.h>
#include "Input.h"


//----------ライブラリファイルのロード--------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//-------------------定数宣言------------------
//ウィンドウサイズ
#define WINDOW_WIDTH	1280	//ウィンドウの幅
#define WINDOW_HEIGHT	 720    //ウィンドウ高さ
#define ENEMY_LIMIT		  5		//敵の最大数
#define CHAR_SIZE		 64		//自機と敵のサイズ

//-------------------マクロ----------------------
#define SAFE_DELETE(p) {delete(p);	(p) = NULL;}						//動的に作ったモノを安全にデリートする
#define SAFE_DELETE_ARRAY(p) {delete[] (p);  (p) = NULL;}				//配列用
#define SAFE_RELEASE(p) {if(p != NULL){(p)->Release(); (p) = NULL;}}	//LP型用

//-------------------グローバル変数------------------
//シーン
enum GAME_SCENE
{
	SC_TITLE,			//タイトルシーン
	SC_STAGE_SELECT,	//ステージ選択シーン
	SC_STAGE_EDITOR,	//ステージ作成シーン
	SC_PLAY,			//プレイシーン
	SC_CLEAR,			//クリアシーン
	SC_GAMEOVER,		//ゲームオーバーシーン
	SC_MAX
};
extern GAME_SCENE	g_gameScene;		//実態はGame.cppで宣言

//Direct3Dデバイスオブジェクト
extern LPDIRECT3DDEVICE9 g_pDevice;

//入力処理オブジェクト
extern Input* g_pInput;	//クラスを作るときは基本的にポインタにした方が良い

//カメラオブジェクト
class Camera;				//カメラクラスでGlobal.hをインクルードしているのでこのヘッダーでCameraクラスをインクルードできないのでクラスの宣言をする
extern Camera* g_pCamera;	//↑で宣言しておけばポインタは使えるのでポインタで宣言