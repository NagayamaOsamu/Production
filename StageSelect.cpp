/*===========================================================================================
概　略：ステージの選択をする処理をまとめたクラスの関数定義
作成日：1月28日
更新日：1月28日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "StageSelect.h"
#include "Stage.h"		//ステージクラス
#include "StageData.h"	//ステージデータクラス
#include <string.h>

//コンストラクタ
StageSelect::StageSelect()
{
	//クラスの初期化
	backGrround = NULL;		//背景画像
	arrow		= NULL;		//矢印画像
	text		= NULL;		//テキスト画像
	num		    = NULL;		//ステージ番号画像

	//メンバ変数の初期化
	selectStage = 0;		//今選んでいるステージ
}

//デストラクタ
StageSelect::~StageSelect()
{
	//クラスの解放
	SAFE_DELETE(backGrround);		//背景画像
	SAFE_DELETE(arrow);				//矢印画像
	SAFE_DELETE(text);				//テキスト画像
	SAFE_DELETE(num);				//ステージ番号画像
}


//機能：読み込み処理
//引数：なし
//戻値：成功か失敗か
HRESULT StageSelect::Load()
{
	//クラスの動的作成
	backGrround = new Sprite;		//背景画像
	arrow		= new Sprite;		//矢印画像
	text		= new Sprite;		//テキスト画像
	num			= new Sprite;		//ステージ番号画像

	//背景画像の読み込み
	if (FAILED(backGrround->Load("Asset\\Sprite\\ST_SelectBack.bmp")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "選択画面の背景画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//矢印画像の読み込み
	if (FAILED(arrow->Load("Asset\\Sprite\\Arrow.png")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "矢印画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//テキスト画像の読み込み
	if (FAILED(text->Load("Asset\\Sprite\\StageText.png")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "テキスト画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//ステージ番号画像の読み込み
	if (FAILED(num->Load("Asset\\Sprite\\StageTextNum.png")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "ステージ番号画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	return S_OK;			//成功を返す
}

//機能：更新処理
//引数：なし
//戻値：成功か失敗か
HRESULT StageSelect::Update()
{
	//十字上キーが押されたら
	if (g_pInput->IsKeyTap(DIK_UP))
	{
		selectStage--;		//ステージ選択変数をカウントダウン
	}

	//十字下キーが押されたら
	if (g_pInput->IsKeyTap(DIK_DOWN))
	{
		selectStage++;		//ステージ選択変数をカウントアップ
	}

	//現在のカレントディレクトリのフォルダ名を覚えておく処理
	char  defaultCurrentDir[MAX_PATH];							//デフォルトのフォルダ名を入れる変数
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);			//現在のカレントディレクトリをGetCurrentDirectory関数で調べ変数に代入
	//この処理以降は↑で取得したフォルダ名のフォルダだけを使うするようにする
	SetCurrentDirectory(defaultCurrentDir);

	

	//変数selectStageが0でかつエンターキーが押されたとき
	if (selectStage == 0 && g_pInput->IsKeyTap(DIK_RETURN))
	{

		//ファイルを開く
		HANDLE hFile;
		hFile = CreateFile(
			"sample1.stage",		//ファイル名
			GENERIC_READ,			//アクセスモード（読み込み用）
			0,						//共有（なし）
			NULL,					//セキュリティ属性
			OPEN_EXISTING,			//開く方法（ファイルがなければ失敗）
			FILE_ATTRIBUTE_NORMAL,	//属性とフラグ（設定なし）
			NULL
			);

		//開けなかったら中断
		if (hFile == 0) return S_OK;

		//ファイルサイズの取得
		DWORD fileSize = GetFileSize(hFile, &fileSize);

		//ファイルサイズ分メモリを確保
		char* data;
		data = new char[fileSize];

		//全部読み込む
		DWORD dwBytes = 0;	//読み込み位置
		ReadFile(
			hFile,					//ファイルハンドル
			data,					//データを入れる変数
			fileSize,				//読み込むサイズ
			&dwBytes,				//読み込んだサイズ（読み込み位置）
			NULL					//オーバーラップ度構造体（今回は使わない）
			);

		//ファイルハンドルの解放
		CloseHandle(hFile);

		
		int index = 0;									//ファイルの場所を示す変数
		int arraySize = GetComma(data, &index);			//ファイルにあるデータの数を取得

		int i = 0;

		//データの数だけループ
		while (i < arraySize)
		{
			//ファイルデータは「ブロックの種類(整数)　,(カンマ)」で区切られてるのでカンマごとに区切って配列に格納する
			stagePartsArray.push_back(GetComma(data, &index));
			StageData::GetInst()->data[i] = stagePartsArray[i];
			i++;

		}

		//最後をお知らせするために'\0'をいれる
		StageData::GetInst()->data[i] = 99;
		//デフォルトのディレクトリに戻す
		SetCurrentDirectory(defaultCurrentDir);

		
		SAFE_DELETE(data);			//変数dataを解放


		g_gameScene = SC_PLAY;					//プレイシーンに移行
	}

	//変数selectStageがでかつエンターキーが押されたとき
	if (selectStage == 1 && g_pInput->IsKeyTap(DIK_RETURN))
	{

		//ファイルを開く
		HANDLE hFile;
		hFile = CreateFile(
			"sample2.stage",		//ファイル名
			GENERIC_READ,			//アクセスモード（読み込み用）
			0,						//共有（なし）
			NULL,					//セキュリティ属性
			OPEN_EXISTING,			//開く方法（ファイルがなければ失敗）
			FILE_ATTRIBUTE_NORMAL,	//属性とフラグ（設定なし）
			NULL
			);

		//開けなかったら中断
		if (hFile == 0) return S_OK;

		//ファイルサイズの取得
		DWORD fileSize = GetFileSize(hFile, &fileSize);

		//ファイルサイズ分メモリを確保
		char* data;
		data = new char[fileSize];

		//全部読み込む
		DWORD dwBytes = 0;	//読み込み位置
		ReadFile(
			hFile,					//ファイルハンドル
			data,					//データを入れる変数
			fileSize,				//読み込むサイズ
			&dwBytes,				//読み込んだサイズ（読み込み位置）
			NULL					//オーバーラップ度構造体（今回は使わない）
			);

		//ファイルハンドルの解放
		CloseHandle(hFile);


		int index = 0;									//ファイルの場所を示す変数
		int arraySize = GetComma(data, &index);			//ファイルにあるデータの数を取得

		//データの数だけループ
		for (int i = 0; i < arraySize; i++)
		{
			//ファイルデータは「ブロックの種類(整数)　,(カンマ)」で区切られてるのでカンマごとに区切って配列に格納する
			stagePartsArray.push_back(GetComma(data, &index));
		}

		//デフォルトのディレクトリに戻す
		SetCurrentDirectory(defaultCurrentDir);

		//変数dataを解放
		SAFE_DELETE(data);

		g_gameScene = SC_PLAY;		//プレイシーンに移行
	}

	return S_OK;			//成功を返す
}

//機能：ステージファイルのデータをカンマ区切りで取得する
//引数：data[]　ファイルのデータ
//引数：index　 読み込む開始位置
//戻値：カンマ前の数値
int StageSelect::GetComma(char data[], int* index)
{
	//変数準備
	char chr[256];	//コンマ区切りのデータを入れる変数
	int i = 0;		//変数chrの要素		

	//「,」まで一文字ずつ変数に入れる
	while (data[*index] != ',')
	{
		chr[i] = data[*index];		//文字列を代入
		i++;						//カウントアップ
		(*index)++;					//次の位置に
	}

	//最後に「\0」を付ける
	chr[i] = '\0';		//配列の最後をお知らせ
	i++;				//次の文字に
	(*index)++;			//次の位置に

	//整数に変換
	int value;			//変換後のデータを入れる変数
	value = atoi(chr);	//atoi関数を使って文字列を数値に変換

	//数値に変換した値を返す
	return value;
}

//機能：描画処理
//引数：なし
//戻値：成功か失敗か
HRESULT StageSelect::Render()
{
	//-----------------スプライトの描画--------------------
	SpriteData data;		////スプライトの情報をまとめた構造体の準備

	//背景画像の描画
	backGrround->Draw(&data);

	//矢印の位置を指定
	data.pos.x = 200;
	data.pos.y = (float)(150 + (selectStage * 100));

	//矢印の画像を描画
	arrow->Draw(&data);

	//作られているステージの数だけループ
	for (int i = 0; i < 2; i++)
	{

		//テキストの位置を指定
		data.pos.x = 300;
		data.pos.y = (float)(130 + (i * 100));

		//テキスト描画
		text->Draw(&data);

		////ステージ番号の位置を指定
		//data.pos.x = 1000;
		//data.pos.y = 250 + i * 200;
		//data.size.x = 140;
		//data.cut.x = 0 + i* 140;

		////ステージ番号を描画
		//num->Draw(&data);
	}

	//ステージ番号の位置を指定
	data.pos.x = 900;
	data.pos.y = 130;
	data.size.x = 140;
	data.cut.x = 140;

	//ステージ番号を描画
	num->Draw(&data);

	////ステージ番号の位置を指定
	data.pos.x = 900;
	data.pos.y = 230;
	data.size.x = 140;
	data.cut.x = 280;

	//ステージ番号を描画
	num->Draw(&data);

	return S_OK;			//成功を返す
}
