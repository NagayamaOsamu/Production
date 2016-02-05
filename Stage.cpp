/*===========================================================================================
概　略：ステージの処理をまとめたクラスの関数定義
作成日：10月15日
更新日：10月15日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Stage.h"
#include "Camera.h"		//カメラクラス
#include "Player.h"		//プレイヤークラス
#include "StageData.h"	//ステージデータクラス

//グローバル変数
//パーツの並び（これをエディタで作れればいい）
DWORD stagePartsList[] = { BLOCK_START, BLOCK_UP, BLOCK_BRANCH, BLOCK_DOWN, BLOCK_STOP, BLOCK_STRAIGHT, BLOCK_BRANCH, BLOCK_STRAIGHT,BLOCK_UP,BLOCK_DOWN, BLOCK_STOP, BLOCK_UP,BLOCK_DOWN,BLOCK_STRAIGHT, BLOCK_GOAL };

//コンストラクタ
Stage::Stage()
{
	//各パーツの出口の位置を設定しておく（この辺は外部ファイルから読み込めるとカッコイイ）
	ExitState exit;

	//スタートの設定
	exit.pos   = D3DXVECTOR3(0, 0, 24);		//位置
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
	exit.angle = 270;						//角度
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

	

	//初期化処理
	playerPos		= D3DXVECTOR3(0, 0, 0);		//プレイヤーの位置
	stageBlock	    = BLOCK_MAX;				//ステージブロックの種類は最初は何も指定しないで定数の最大値を入れる
	isSprikHit		= false;					//障害物とプレイヤーが衝突したかどうか
	sprik			= NULL;						//障害物モデル
	isPlayerDown	= TRUE;						//ステージにいる間はTRUE、落ちている間はFALSE
	flg = 0;
}

//デストラクタ
Stage::~Stage()
{
	if (flg == 0x01)
		Clear(&startNode);		//ノードの削除(スタートノードから始める)

	SAFE_DELETE(sprik);		//障害物モデルの解放
}

//機能：ノードの削除
//引数：ノードの種類
//戻値：なし
void Stage::Clear(Node* pNode)
{

	if (pNode->type == 99)
	{
		return;
	}

	//そのブロックの出口分ループ
	for (DWORD i = 0; i < blocksState[pNode->type].exitList.size(); i++)
	{
		Clear(pNode->childNode[i]);			//最後のステージブロックまで再帰処理で読み込む
		SAFE_DELETE(pNode->childNode[i]);	//解放
	}
}

//機能：読み込み処理
//引数：なし
//戻値：成功か失敗か
HRESULT Stage::Load()
{
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

	//障害物モデルの読み込み

	sprik = new Fbx;

	if (FAILED(sprik->Load("Asset\\Stage\\Sprik.fbx")))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "障害物の読み込みに失敗しました", "読み込みエラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	return S_OK;			//成功を返す
}

//機能：ステージの作成
//引数：設定させるノード
//戻値：なし
void Stage::Create(D3DXVECTOR3 pos, float angle, Node* pNode)
{

	//パーツの種類（冒頭の配列から）
	if (g_gameScene != SC_PLAY)
		pNode->type = stagePartsList[nodeID];					//グローバル変数に入れてあるブロックを取得する
	else
	{
		//pNode->childNode.pop_back();
		pNode->type = StageData::GetInst()->data[nodeID];		//グローバル変数に入れてあるブロックを取得する
	}

	if (pNode->type == 99)
		return;

	nodeID++;												//次のブロックに移動

	pNode->pos = pos;										//ブロックの位置を記憶する

	//ブロックごとの行列の作成
	D3DXMATRIX matR, matT;									//左から回転・移動・ワールド行列
	D3DXMatrixRotationY(&matR, D3DXToRadian(angle));		//回転行列の作成
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);		//移動行列の作成
	pNode->matrix = matR * matT;							//行列の合成

	//ブロックがスタートブロックでないとき
	if ((pNode->type != BLOCK_START) && (pNode->type != BLOCK_GOAL) )
	{
		////障害物を適当に配置
		int sprikNum = rand() % 5 + 1;							//障害物を何個配置するか乱数で決める(最大５つ)

		//変数sprikNumの値だけループ
		for (int i = 0; i < sprikNum; i++)
		{
			//障害物の各座標を格納する変数
			int sprikPosX = 0;				//x座標	
			int sprikPosY = 0;				//y座標
			int sprikPosZ = 0;				//z座標

			//ブロックが回転していないとき
			if (angle == 0 || angle == 360)
			{
				sprikPosX = (int)pos.x + (rand() % 10 - 5);		//x位置をノードの幅に合わせてランダムな位置にする
				sprikPosY = (int)pos.y;							//y位置はノードの位置に
				sprikPosZ = (rand() % 12) + (int)pos.z;			//z位置はノードの奥行に合わせてランダムな位置にする
			}

			//ブロックが回転してるとき
			else if (angle == 90 || angle == 270)
			{
				sprikPosX = (rand() % 12) + (int)pos.x;				//x位置をノードの奥行きに合わせてランダムな位置にする
				sprikPosY = (int)pos.y;							//y位置はノードの位置に
				sprikPosZ = (int)pos.z + (rand() % 10 - 5);		//z位置はノードの幅に合わせてランダムな位置にする
			}

			//障害物とノードでレイキャスト
			RayCastData data;																	//構造体の準備
			data.start = D3DXVECTOR3((float)sprikPosX, (float)sprikPosY, (float)sprikPosZ);		//レイを飛ばす位置を障害物の位置にする
			data.start.y = 100;																	//y軸はかなり高い位置から
			data.dir = D3DXVECTOR3(0, -1, 0);													//レイを下に飛ばす
			data.matrix = pNode->matrix;														//今のノードの行列を渡す
			blocksState[pNode->type].stage.RayCast(&data);										//レイキャスト判定を行う

			//衝突したとき
			if (data.hit)
			{
				//y軸をノードに合わせる
				data.pos.y += 2;			//少しだけ上にする
				sprikPosY = (int)data.pos.y;		//その位置に移動
				
			}

			//障害物の位置を配列に格納
			pNode->sprikPos.push_back(D3DXVECTOR3((float)sprikPosX, (float)sprikPosY, (float)sprikPosZ));
		}
	}

	//そのブロックの出口分ループ
	for (DWORD i = 0; i < blocksState[pNode->type].exitList.size(); i++)
	{

		Node* child = new Node;					//新たなノードを作り
		pNode->childNode.push_back(child);		//子供にする

		//次のノードの位置(パーツの出口位置に回転行列をかける）
		D3DXVECTOR3 nextPos;
		D3DXVec3TransformCoord(&nextPos, &blocksState[pNode->type].exitList[i].pos, &matR);

		//次のノードの角度(パーツ情報から取得）
		float nextAngle = blocksState[pNode->type].exitList[i].angle;

		//作ったノードの設定
		Create(pos + nextPos, angle + nextAngle, child);		//再帰処理で子供のノードの設定をする
	}

}

//機能：更新処理
//引数：なし
//戻値：成功か失敗か
HRESULT Stage::Update()
{


	return S_OK;			//成功を返す
}

//機能：衝突判定処理
//引数：ユニットベースクラスとユニットベースクラスを継承しているクラス
//戻値：成功か失敗か
HRESULT Stage::Hit(UnitBase* pTarget)
{

	//プレイヤークラスとの当たり判定
	if (typeid(*pTarget) == typeid(Player))
	{
		Player* player = (Player*)pTarget;	//プレイヤークラスの生成

		playerPos = player->GetPos();		//プレイヤーの位置を取得する

		//最初のノードからステージブロックとプレイヤーの衝突判定を行う
		if (FAILED(Hit(&startNode)))
		{
			//失敗したらメッセージボックスで知らせる
			MessageBox(0, "ステージとの衝突に失敗しました", "ヒットエラー", MB_OK);

			return E_FAIL;		//失敗を返す
		}
	}


	return S_OK;			//成功を返す
}


//機能：ステージブロックとプレイヤーの衝突判定
//引数：ステージブロックのノード
//戻値：成功か失敗か
HRESULT Stage::Hit(Node* pNode)
{
	//プレイヤーとステージブロックとの距離を調べる
	D3DXVECTOR3 toPlayer = pNode->pos - playerPos;					//プレイヤーとステージブロックの差ベクトルを求め
	float dist = D3DXVec3Length(&toPlayer);							//その長さ(距離)を受け取る

	//距離が一定以下のとき
	if (dist <= 24)
	{
		//レイキャストで衝突判定を行う
		RayCastData rayData;										//レイキャスト構造体の準備
		rayData.start = playerPos;									//プレイヤーの位置をレイの発射位置にする
		rayData.start.y = 200;										//プレイヤーより少し上からレイを発射する
		rayData.dir = D3DXVECTOR3(0, -1, 0);						//プレイヤーの位置からy軸に-1の位置にレイを飛ばす
		rayData.matrix = pNode->matrix;								//今のステージブロックの行列を渡す
		blocksState[pNode->type].stage.RayCast(&rayData);			//ステージのモデルとレイキャストする


		//ステージと衝突してたら
		if (rayData.hit)
		{
			//ステージブロックとプレイヤーの衝突位置と法線を渡す
			playerPos = rayData.pos;								//衝突位置
			blocksNormal = rayData.normal;							//法線
			isPlayerDown = TRUE;									//ステージにいるのでTRUEに

			//今のノードにある障害物の数だけループ
			for (DWORD i = 0; i < pNode->sprikPos.size(); i++)
			{
				//プレイヤーと障害物が衝突しているか調べる
				D3DXVECTOR3 toSprik = pNode->sprikPos[i] - playerPos;		//障害物とプレイヤーの位置の差のベクトルを求め
				float sprikDist = D3DXVec3Length(&toSprik);					//その長さ(距離)を受け取る

				isSprikHit = (sprikDist <= 2);								//距離が一定以下のときにtrueにする
				if (isSprikHit)												//衝突していたらこのループを終わらす
					break;
			}

			//ゴールのとき
			if (pNode->type == BLOCK_GOAL && dist <= 5)
			{
				g_gameScene = SC_CLEAR;								//クリアシーンのとき
			}


			return S_OK;
		}

	}

	//衝突していないとき
	else
	{
		//プレイヤーを下に落とす
		//playerPos.y -= 0.05f;
		//blocksNormal = D3DXVECTOR3(0, 0, 0);
		//isPlayerDown = FALSE;					//ステージと衝突していないのでそのFALSEに
	}

	//今のノードの子供の数だけループ
	for (DWORD i = 0; i < pNode->childNode.size(); i++)
	{
		//次のノードで衝突判定を行う(再帰処理)
		Hit(pNode->childNode[i]);
	}

	return S_OK;				//失敗を返す
}

//機能：描画処理
//引数：なし
//戻値：成功か失敗か
HRESULT Stage::Render()
{
	if (flg == 0x00)
	{
		flg |= 1;
		//ステージ作成
		nodeID = 0;				//ノードの番号(最初は0)
		Create(D3DXVECTOR3(0, 0, 0), 0, &startNode);		//ステージを作成
	}

	//ノードごとに描画する(その処理を書いた関数の呼び出し)
	if (FAILED(BlocksRender(D3DXVECTOR3(0, 0, 0), 0, &startNode)))		//最初のノードから描画
	{
		return E_FAIL;		//失敗を返す
	}

	return S_OK;			//成功を返す
}

//機能：ノードの描画
//引数：pos　　表示する位置
//引数：angle　表示する角度
//引数：pNode　表示するノードの種類
//戻値：なし
HRESULT Stage::BlocksRender(D3DXVECTOR3 pos, float angle, Node* pNode)
{
	//行き止まりか99なら表示なし
	if (pNode->type == BLOCK_STOP || pNode->type == 99)
		return S_OK;


	D3DXMATRIX matR, matT,matW;								//左から回転・移動・ワールド行列
	D3DXMatrixRotationY(&matR, D3DXToRadian(angle));		//回転行列の作成
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);		//移動行列の作成
	matW = matR * matT;

	//引数からノードの出口に合わせた移動・回転行列を作って描画
	blocksState[pNode->type].stage.Draw(&matW);				//ノードの描画

	for (DWORD i = 0; i < pNode->sprikPos.size(); i++)
	{
		D3DXMatrixTranslation(&matW, pNode->sprikPos[i].x, pNode->sprikPos[i].y, pNode->sprikPos[i].z);
		sprik->Draw(&matW);
	}

	//そのノードの出口分ループ
	for (DWORD i = 0; i < blocksState[pNode->type].exitList.size(); i++)
	{
		//次のノードの位置(パーツの出口位置に回転行列をかける）
		D3DXVECTOR3 nextPos;
		D3DXVec3TransformCoord(&nextPos, &blocksState[pNode->type].exitList[i].pos, &matR);

		//次のノードの角度(パーツ情報から取得）
		float nextAngle = blocksState[pNode->type].exitList[i].angle;

		//次のノードを描画
		BlocksRender(pos + nextPos, angle + nextAngle, pNode->childNode[i]);		//今のノードの位置・角度から次のノードの出口までの位置・角度を足したものを引数にして再帰処理
	}



	return S_OK;			//成功を返す
}