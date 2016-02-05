/*===========================================================================================
概　略：ステージエディタークラスの関数定義
作成日：1月12日
更新日：1月12日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "StageEditor.h"
#include "Camera.h"			//カメラクラス

//コンストラクタ
StageEditor::StageEditor()
{
	//各パーツの出口の位置を設定しておく（この辺は外部ファイルから読み込めるとカッコイイ）
	ExitState exit;

	//スタートの設定
	exit.pos = D3DXVECTOR3(0, 0, 24);		//位置
	exit.angle = 0;							//角度
	blocksState[BLOCK_START].exitList.push_back(exit);		//情報を配列に格納

	//直線の設定
	exit.pos = D3DXVECTOR3(0, 0, 24);		//位置
	exit.angle = 0;							//角度
	blocksState[BLOCK_STRAIGHT].exitList.push_back(exit);	//情報を配列に格納

	//T字路(出口が2つあるので2つ作る)

	exit.pos = D3DXVECTOR3(24, 0, 6);		//位置
	exit.angle = 90;						//角度
	blocksState[BLOCK_BRANCH].exitList.push_back(exit);	//情報を配列に格納

	exit.pos = D3DXVECTOR3(-24, 0, 6);		//位置
	exit.angle = 180;						//角度
	blocksState[BLOCK_BRANCH].exitList.push_back(exit);	//情報を配列に格納

	//上り坂
	exit.pos = D3DXVECTOR3(0, 8, 22);		//位置
	exit.angle = 0;							//角度
	blocksState[BLOCK_UP].exitList.push_back(exit);	//情報を配列に格納

	//下り坂
	exit.pos = D3DXVECTOR3(0, -8, 24);		//位置
	exit.angle = 0;							//角度
	blocksState[BLOCK_DOWN].exitList.push_back(exit);	//情報を配列に格納

	//行き止まり
	//設定することない



	//クラスの初期化
	chipWindow = NULL;							//チップウィンドウ
	frame = NULL;							//枠

	//メンバ変数の初期化
	stagePartsList.push_back(BLOCK_START);		//最初のブロックはスタート
	cameraPos		= D3DXVECTOR3(-10, 100,-1);	//カメラの位置
	cameraTarget	= D3DXVECTOR3(-10,0,0);		//カメラの角度
	selectChip		= BLOCK_STRAIGHT;			//最初は直線のチップ
	mouseTapPos.x	= 0;						//マウスクリックしたx位置
	mouseTapPos.y	= 0;						//マウスクリックしたy位置
	mouseDownPos.x	= 0;						//マウスが押されてる間のx位置
	mouseDownPos.y  = 0;						//マウスが押されてる間のy位置
	branchPos		= D3DXVECTOR3(0, 0, 0);		//T字路の位置
	branchAngle		= 0;						//T字路の角度
	isChipWin		= FALSE;					//マウスカーソルがチップウィンドウにいるか判定
}

//デストラクタ
StageEditor::~StageEditor()
{
	//クラスの解放
	SAFE_DELETE(chipWindow);		//チップウィンドウ
	SAFE_DELETE(frame);				//枠

}

//機能：読み込み処理
//引数：なし
//戻値：成功か失敗か
HRESULT StageEditor::Load()
{
	//クラスの動的作成
	chipWindow = new Sprite;		//チップウィンドウ
	frame	   = new Sprite;		//枠

	//----------------------ステージブロックの読み込み--------------------------------

	//ステージブロックの読み込み
	if (FAILED(blocksState[BLOCK_START].stage.Load("Asset\\Stage\\Start.fbx")))		//スタートブロック
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "スタートの読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	if (FAILED(blocksState[BLOCK_STRAIGHT].stage.Load("Asset\\Stage\\Staight.fbx")))		//直線ブロック
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "直線の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	if (FAILED(blocksState[BLOCK_BRANCH].stage.Load("Asset\\Stage\\Branch.fbx")))		//T字路ブロック
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "T字路の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	if (FAILED(blocksState[BLOCK_UP].stage.Load("Asset\\Stage\\Up.fbx")))		//上り坂ブロック
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "上り坂の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	if (FAILED(blocksState[BLOCK_DOWN].stage.Load("Asset\\Stage\\Down.fbx")))		//下り坂ブロック
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "下り坂の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}


	if (FAILED(blocksState[BLOCK_GOAL].stage.Load("Asset\\Stage\\Goal.fbx")))		//ゴール		
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "ゴールの読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	if (FAILED(blocksState[BLOCK_STOP].stage.Load("Asset\\Stage\\Stop.fbx")))		//行き止まり
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "行き止まりの読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//----------------------スプライトの読み込み----------------------------

	//チップウィンドウ画像の読み込み
	if (FAILED(chipWindow->Load("Asset\\Sprite\\ChipWindow.png")))
	{
		//失敗したらメッセージウィンドウで知らせる
		MessageBox(0, "チップウィンドウ画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}

	//枠の画像の読み込み
	if (FAILED(frame->Load("Asset\\Sprite\\Frame.png")))
	{
		//失敗したらメッセージウィンドウで知らせる
		MessageBox(0, "枠の画像の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;			//失敗を返す
	}


	return S_OK;				//成功を返す
}

//機能：更新処理
//引数：なし
//戻値：成功か失敗か
HRESULT StageEditor::Update()
{
	//カメラの位置と角度を変更
	g_pCamera->SetPos(cameraPos);				//位置
	g_pCamera->SetTarget(cameraTarget);

	//マウスで左クリックされたとき
	if (g_pInput->IsMouseTap(0))
	{
		//クリックされた位置の取得
		mouseTapPos = g_pInput->GetMousePos();
		
		//マウスの位置がチップウィンドウの直線の位置にあるとき
		if ( (  mouseTapPos.x >= 0  &&  mouseTapPos.x <= 250  ) && ( mouseTapPos.y >= 0 && mouseTapPos.y <= 250 ) )
		{
			selectChip = BLOCK_STRAIGHT;		//直線チップの選択
			isChipWin = TRUE;					//マウスカーソルの位置フラグをTRUEに
			
		}

		//T字路のとき
		else if ( ( mouseTapPos.x > 250 && mouseTapPos.x <= 500 ) && (mouseTapPos.y >= 0 && mouseTapPos.y <= 250 ) )
		{
			selectChip = BLOCK_BRANCH;			//T字路チップの選択
			isChipWin = TRUE;					//マウスカーソルの位置フラグをTRUEに
		}

		//上り坂のとき
		else if ( ( mouseTapPos.x >= 0 && mouseTapPos.x <= 250 ) && ( mouseTapPos.y > 250 && mouseTapPos.y <= 500 ) )
		{
			selectChip = BLOCK_UP;				//上り坂チップの選択
			isChipWin = TRUE;					//マウスカーソルの位置フラグをTRUEに
		}

		//下り坂のとき
		else if ( ( mouseTapPos.x > 250 && mouseTapPos.x <= 500) && (mouseTapPos.y > 250 && mouseTapPos.y <= 500 ) )
		{
			selectChip = BLOCK_DOWN;			//下り坂チップの選択
			isChipWin = TRUE;					//マウスカーソルの位置フラグをTRUEに
		}

		//ゴールのとき
		else if ( ( mouseTapPos.x >= 0 && mouseTapPos.x <= 250 ) && ( mouseTapPos.y > 500 && mouseTapPos.y <= 750 ) )
		{
			selectChip = BLOCK_GOAL;			//ゴールチップの選択
			isChipWin = TRUE;					//マウスカーソルの位置フラグをTRUEに
		}

		//ストップのとき
		else if ( ( mouseTapPos.x > 250 && mouseTapPos.x <= 500 ) && ( mouseTapPos.y > 500 && mouseTapPos.y <= 750 ) )
		{
			selectChip = BLOCK_STOP;		//ストップの選択
			isChipWin  = TRUE;				//マウスカーソルの位置フラグをTRUEに
		}

		//チップウィンドウでないとき
		else
		{
			isChipWin = FALSE;			//マウスカーソルの位置フラグをFALSEに
		}


		//マウスカーソルの位置フラグがTRUEのとき
		if (isChipWin)
		{
			stagePartsList.push_back(selectChip);		//選ばれているチップをリストに追加
		}
	}

	//チップウィンドウ外で左クリックされ続けている間
	else if ( ( !isChipWin ) && ( g_pInput->IsMousePush(0) ) )
	{
		//マウスの位置を取得
		mouseDownPos = g_pInput->GetMousePos();

		//マウスタップされた位置からどれだけドラックされたかを求める
		float mouseMoveX = (float)(mouseDownPos.x - mouseTapPos.x) / 100;		//x方向
		float mouseMoveZ = (float)(mouseDownPos.y - mouseTapPos.y) / 100;		//y方向

		//------------カメラの移動をマウスがドラッグされた分だけ移動させるようにする--------------

		//タップした位置から左にマウスがあるとき
		if (mouseTapPos.x > mouseDownPos.x)
		{
			//カメラを左方向を移動
			cameraPos.x += mouseMoveX;
			cameraTarget.x += mouseMoveX;
		}

		//右のとき
		else if (mouseTapPos.x < mouseDownPos.x)
		{
			//カメラを右方向に移動
			cameraPos.x += mouseMoveX;
			cameraTarget.x += mouseMoveX;
		}

		//上のとき
		 if (mouseTapPos.y < mouseDownPos.y)
		{
			//カメラを上方向に移動
			 cameraPos.z    += mouseMoveZ;
			 cameraTarget.z += mouseMoveZ;
		}

		//下のとき
		else if (mouseTapPos.y > mouseDownPos.y)
		{
			//カメラを下方向に移動
			cameraPos.z += mouseMoveZ;
			cameraTarget.z += mouseMoveZ;
		}
	}

	//マウスの左クリックが離れたら
	else if (g_pInput->IsMouseRelease(0))
	{
		mouseDownPos = mouseTapPos;
	}

	//右クリックされたとき
	if (g_pInput->IsMouseTap(1))
	{
		//行列用のイテレータに初期値を代入
		matItr = blocksMatList.begin();

		//ステージのパーツ数分だけループ
		for (itr = stagePartsList.begin(); itr != stagePartsList.end();itr++)
		{
			//クリックした位置とステージブロックの衝突判定
			BOOL isHit;
			isHit = blocksState[*itr].stage.HitMouseRay(*matItr);

			//スタート位置以外と衝突したとき
			if (isHit && (*itr != BLOCK_START))
			{
				//ステージブロックと行列を削除
				stagePartsList.erase(itr);		//ステージブロック
				blocksMatList.erase(matItr);	//行列

				break;
			}

			//行列用リストが最後以外のとき
			if (matItr != blocksMatList.end())
				matItr++;						//次のイテレータに
		}
	}

	//Sキーが押されたら
	if (g_pInput->IsKeyTap(DIK_S))
	{
		//stagePartsListを保存
		char szFile[MAX_PATH] = "stage.stage";		//ファイル名を入れる変数（初期の名前をいれておく）

		//「ファイルを保存」ダイアログの設定
		OPENFILENAME ofn;		//名前をつけて保存ダイアログの設定用構造体
		ZeroMemory(&ofn, sizeof(ofn));									//構造体の初期化
		ofn.lStructSize = sizeof(OPENFILENAME);							//構造体のサイズ
		ofn.lpstrFilter = TEXT("ステージデータ(*.stage)\0*.stage\0")	//_____	ファイルの種類
						  TEXT("全てのファイル(*.*)\0*.*\0\0");			//_____|_
		ofn.lpstrFile	= szFile;											//ファイル名
		ofn.nMaxFile	= MAX_PATH;										//パスの最大文字数
		ofn.Flags		=  OFN_OVERWRITEPROMPT;								//フラグ（同名ファイルが存在したら上書き保存するようにする）
		ofn.lpstrDefExt = "stage";										//デフォルト拡張子

		//「ファイルを保存」ダイアログを表示
		BOOL selFile;		//ダイアログのどこのボタンが押されたか判断する変数

		selFile = GetSaveFileName(&ofn);		//ダイアログ表示

		//キャンセルしたら中断
		if (selFile == FALSE)	
			return S_OK;

		//ファイル作成
		HANDLE hFile;				//ファイルのハンドル
		hFile = CreateFile(
			szFile,					//ファイル名
			GENERIC_WRITE,			//アクセスモード（書き込み用)
			0,						//共有（なし）
			NULL,					//セキュリティ属性（継承しない）
			CREATE_ALWAYS,			//作成方法（設定なし）
			FILE_ATTRIBUTE_NORMAL,	//属性とフラグ（設定なし）
			NULL					//拡張属性（なし）
			);

		//stagePartsListのサイズを文字列として格納する
		char buf[256];										//文字列を入れる配列
		wsprintf(buf, "%d,", stagePartsList.size());	//文字列に変換

		//幅と高さをファイルに書き込む
		DWORD dwBytes = 0;	//書き込み位置
		WriteFile(
			hFile,				//ファイルハンドル
			buf,				//保存するデータ
			(DWORD)strlen(buf),	//書き込むサイズ
			&dwBytes,			//書き込んだサイズを入れる変数
			NULL
			);

		//stagePartsListの数だけループ
		for (itr = stagePartsList.begin(); itr != stagePartsList.end();itr++)
		{
			//各ステージブロックを「,」で区切って書き込む
			wsprintf(buf, "%d,", *itr);
			WriteFile(hFile, buf, (DWORD)strlen(buf), &dwBytes, NULL);
		}

		//ファイルを閉じる
		CloseHandle(hFile);
	}

	//選ばれているチップごとの処理
	switch (selectChip)
	{
		//直線のチップのとき
	case BLOCK_STRAIGHT:

		//十字右キーか左キーが押されたとき
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_BRANCH;		//T字路のチップを選択している状態にする
		}

		//十字下キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_UP;		//上り坂チップを選択している状態にする
		}

		//十字上キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_GOAL;		//ゴールのチップを選択している状態にする
		}

		break;

		//T字路のチップのとき
	case BLOCK_BRANCH:

		//十字右キーか左キーが押されたとき
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_STRAIGHT;		//直線のチップを選択している状態にする
		}

		//十字下キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_DOWN;			//下り坂チップを選択している状態にする
		}

		break;

		//上り坂のチップのとき
	case BLOCK_UP:

		//十字右キーか左キーが押されたとき
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_DOWN;			//下り坂のチップを選択している状態にする
		}

		//十字下キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_GOAL;			//上り坂チップを選択している状態にする
		}

		//十字上キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_STRAIGHT;		//直線のチップを選択している状態にする
		}

		break;

		//下り坂のチップのとき
	case BLOCK_DOWN:

		//十字右キーか左キーが押されたとき
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_UP;				//上り坂のチップを選択している状態にする
		}

		//十字下キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_STOP;			//行き止まりを選択している状態にする
		}

		//十字上キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_BRANCH;			//T字路のチップを選択している状態にする
		}

		break;

		//ゴールのチップのとき
	case BLOCK_GOAL:

		//十字右キーか左キーが押されたとき
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_STOP;			//行き止まりを選択している状態にする
		}

		//十字下キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_STRAIGHT;			//直線のチップを選択している状態にする
		}

		//十字上キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_UP;				//上り坂のチップを選択している状態にする
		}


		break;

		//行き止まりのとき
	case BLOCK_STOP:

		//十字右キーか左キーが押されたとき
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_GOAL;			//下り坂のチップを選択している状態にする
		}

		//十字下キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_BRANCH;			//直線のチップを選択している状態にする
		}

		//十字上キーが押されたとき
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_BRANCH;				//上り坂のチップを選択している状態にする
		}


		break;

	}

	//Enterキーが押されたら
	if (g_pInput->IsKeyTap(DIK_RETURN))
	{
		//選ばれているチップをリストに追加
		stagePartsList.push_back(selectChip);


		//カメラを移動
		cameraPos.z += 10;
		cameraTarget.z += 10;
	}

	return S_OK;				//成功を返す
}

//機能：描画処理
//引数：なし
//戻値：成功か失敗か
HRESULT StageEditor::Render()
{
	//--------------------スプライトの描画-------------------

	//スプライトの情報をまとめた構造体の準備
	SpriteData data;

	//チップウィンドウの描画
	chipWindow->Draw(&data);		//情報は変えずに描画

	//選んでいるチップごとに枠の描画
	switch (selectChip)
	{
		//直線のチップのとき
	case BLOCK_STRAIGHT:
		
		data.pos = D3DXVECTOR3(-5, -4, 0);	//枠の位置をチップウィンドウの左上に

		break;

		//T字路のチップのとき
	case BLOCK_BRANCH:

		data.pos = D3DXVECTOR3(270, -4, 0);	//枠の位置をチップウィンドウの右上に

		break;

		//上り坂のチップのとき
	case BLOCK_UP:

		data.pos = D3DXVECTOR3(-5, 250, 0);	//枠の位置をチップウィンドウの左真ん中に

		break;

		//下り坂のチップのとき
	case BLOCK_DOWN:

		data.pos = D3DXVECTOR3(270, 250, 0);	//枠の位置をチップウィンドウの右真ん中に

		break;

		//ゴールのチップのとき
	case BLOCK_GOAL:

		data.pos = D3DXVECTOR3(-5, 500, 0);	//枠の位置をチップウィンドウの左下に

		break;

	case BLOCK_STOP:

		data.pos = D3DXVECTOR3(270, 500, 0);

		break;
	}

	frame->Draw(&data);						//描画

	//-------------------ステージの描画----------------------

	////位置と角度の情報の初期化
	D3DXVECTOR3 blockPos = D3DXVECTOR3(0, 0, 0);			//ステージブロックの位置
	float		blockAngle = 0;								//ステージブロックの角度

	//行列の準備
	D3DXMATRIX matW, matT, matR;		//ワールド行列,移動行列,回転行列,調整用行列


	blocksMatList.clear();
	//リストの数だけループ
	for (itr = stagePartsList.begin(); itr != stagePartsList.end(); itr++)
	{
		//リスト順にステージブロックを描画する
		D3DXMatrixTranslation(&matT, blockPos.x/* + 10*/, blockPos.y, blockPos.z);		//次の位置の行列を作る
		D3DXMatrixRotationY(&matR, D3DXToRadian(blockAngle));						//角度を元に回転行列を作る
		matW = matR *  matT;														//行列の合成

		//リストstagePartsListの要素数とリストblocksMatListが違うとき
		if (stagePartsList.size() != blocksMatList.size())
		{
			//リストblocksMatListにブロックの行列を格納する
			blocksMatList.push_back(matW);
		}

		//行き止まりのときはなにも描画しない
		if (*itr != BLOCK_STOP)
		{
			//描画
			blocksState[*itr].stage.Draw(&matW);

			//今のチップの出口の数だけループ
			for (DWORD i = 0; i < blocksState[*itr].exitList.size(); i++)
			{
				//T字路のとき
				if (*itr == BLOCK_BRANCH)
				{
					//T字路の右側からブロックを設置する
					D3DXVec3TransformCoord(&branchPos, &blocksState[BLOCK_BRANCH].exitList[1].pos, &matR);			//次の位置を回転行列と出口の位置を掛けて求める
					branchPos += blockPos;																			//T字路の位置変数に次の位置を足す
					branchAngle = blocksState[*itr].exitList[1].angle;												//T字路の角度に左出口の角度を入れる
					D3DXVECTOR3 nextPos = D3DXVECTOR3(0, 0, 0);														//次の位置用の変数準備
					D3DXVec3TransformCoord(&nextPos, &blocksState[*itr].exitList[0].pos, &matR);					//次の位置を回転行列と出口の位置を掛けて求める
					blockPos += nextPos;																			//次の位置を入れる
					blockAngle += blocksState[*itr].exitList[0].angle;												//角度を足す
					break;
				}

				//T字路以外のとき
				else
				{
					//出口の位置と角度を今のブロックの位置と角度に足す
					D3DXVECTOR3 nextPos = D3DXVECTOR3(0, 0, 0);												//次の位置用の変数準備
					D3DXVec3TransformCoord(&nextPos, &blocksState[*itr].exitList[i].pos, &matR);			//次の位置を回転行列と出口の位置を掛けて求める
					blockPos += nextPos;																	//次の位置を足す
					blockAngle += blocksState[*itr].exitList[i].angle;										//角度を足す
				}
			}
		}

		//BLOCK_STOPのとき
		else
		{
			//T字路の左側からブロックを設置する
			blockPos = branchPos;											//次の位置を前に格納したT字路の位置を代入
			blockAngle += branchAngle;										//角度を足す
		}
	}
	
	return S_OK;				//成功を返す
}
