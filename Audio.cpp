/*===========================================================================================
概　略：ゲームの音を鳴らすクラスの関数定義
作成日：6月26日
更新日：6月26日
制作者：永山治夢
=============================================================================================*/

#include "Audio.h"

//コンストラクタ
Audio::Audio()
{

}

//デストラクタ
Audio::~Audio()
{
	//XACTシャットダウン
	xactEngine->ShutDown();


	//サウンドバンク開放
	SAFE_DELETE_ARRAY(soundBankData);

	//ウェーブバンク開放
	UnmapViewOfFile(mapWaveBank);

	//エンジン開放
	SAFE_RELEASE(xactEngine);

	//COMライブラリ開放
	CoUninitialize();
}

//機能：XACTエンジンの準備
//引数：なし
//戻値：失敗か成功か
HRESULT Audio::InitEngine()
{
	//COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//エンジン作成
	if (FAILED(XACT3CreateEngine(0, &xactEngine)))
	{
		MessageBox(0, "XACTエンジンの作成に失敗しました", "エラー", MB_OK);
		return E_FAIL;		//失敗を返す
	}

	//パラメータ設定
	XACT_RUNTIME_PARAMETERS xactParam = { 0 };
	xactParam.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	if (FAILED(xactEngine->Initialize(&xactParam)))
	{

		MessageBox(0, "XACTエンジンのパラメータ設定に失敗しました", "エラー", MB_OK);
		return E_FAIL;		//失敗を返す
	}

	return S_OK;	//成功を返す
}

//機能：xwbファイルを読み込み
//引数：ファイル名
//戻値：失敗か成功か
HRESULT Audio::LoadWaveBank(char* fileName)
{
	//ファイルを開く
	HANDLE  hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	//ファイルサイズを調べる
	DWORD  fileSize = GetFileSize(hFile,&fileSize);

	//マッピング(「仮想アドレス空間」というところにデータをコピーしなければならない。（そういう仕様）)
	HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, fileSize, NULL);
	mapWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

	//ウェーブバンク作成
	if (FAILED(xactEngine->CreateInMemoryWaveBank(mapWaveBank, fileSize, 0, 0, &waveBank)))
	{
		//失敗したことをメッセージボックスで表示
		MessageBox(0, "ウェーブバンク作成に失敗しました", fileName, MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//ハンドルを片付ける
	CloseHandle(hMapFile);
	CloseHandle(hFile);

	return S_OK;		//成功を返す
}

//機能：xsbファイルの読み込み
//引数：ファイル名
//戻値：失敗か成功か
HRESULT Audio::LoadSoundBank(char* fileName)
{
	//ファイルを開く
	HANDLE  hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	//ファイルサイズを調べる
	DWORD fileSize = GetFileSize(hFile, &fileSize);

	//データを全部読む
	soundBankData = new BYTE[fileSize];
	DWORD  byteRead;
	ReadFile(hFile, soundBankData, fileSize, &byteRead, NULL);

	//サウンドバンク作成
	if (FAILED(xactEngine->CreateSoundBank(soundBankData, fileSize, 0, 0, &soundBank)))
	{
		MessageBox(0, "サウンドバンク作成に失敗しました", fileName, MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//ハンドルを片付ける
	CloseHandle(hFile);

	return S_OK;		//成功を返す
}

//機能：音を鳴らすのに必要な関数の読み込み
//引数：waveBankFileName　xwbファイルの名前
//引数：soundBankFileName　xsbファイルの名前
//戻値：失敗か成功か
HRESULT Audio::Load(char* waveBankFileName, char* soundBankFileName)
{
	//XACTエンジンの作成
	if (FAILED(InitEngine()))
	{
		return E_FAIL;	//失敗を返す
	}

	//WaveBank読み込み
	if (FAILED(LoadWaveBank(waveBankFileName)))
	{
		return E_FAIL;	//失敗を返す
	}

	//SoundBank読み込み
	if (FAILED(LoadSoundBank(soundBankFileName)))
	{
		return E_FAIL;	//失敗を返す
	}

	return S_OK;		//成功を返す
}

//機能：音を再生させる
//引数：cueName　キューの(ツールで指定した時の)名前
//戻値：成功か失敗か
HRESULT Audio::Play(char* cueName)
{
	//キューの番号を取得
	XACTINDEX cueIndex = soundBank->GetCueIndex(cueName);

	//再生
	if (FAILED(soundBank->Play(cueIndex, 0, 0, NULL)))
	{
		return E_FAIL;		//失敗を返す
	}
	return S_OK;			//成功を返す
}

//機能：音を停止させる
//引数：キューの名前
//戻値：成功か失敗か
HRESULT Audio::Stop(char* cueName)
{
	//キューの番号を取得
	XACTINDEX cueIndex = soundBank->GetCueIndex(cueName);

	//停止
	if (FAILED(soundBank->Stop(cueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE)))
	{
		return E_FAIL;		//失敗を返す
	}
	return S_OK;			//成功を返す
}
