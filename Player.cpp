/*===========================================================================================
概　略：プレイヤーの処理をまとめたクラスの関数定義
作成日：10月15日
更新日：10月15日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Player.h"

#include "Camera.h"		//カメラクラス
#include "Stage.h"		//ステージクラス

//定数宣言
#define RAY_START_Y    200	//レイを飛ばすy軸の位置
#define ACCSEL	    0.007f	//プレイヤーの加速度
#define MAX_SPEED     0.2f	//プレイヤーの最高速度
#define MIN_SPEED   0.001f	//プレイヤーの最低速度
#define FRICTION    0.001f	//摩擦
#define DIR_SPEED	     2	//回転速度



//コンストラクタ
Player::Player()
{
	//クラスの初期化
	player   = NULL;							//プレイヤーモデル

	for (int i = 0; i < sizeof(lifeMark) / sizeof(lifeMark[0]); i++)		//体力を表示する数だけループ
	{
		lifeMark[i] = NULL;							//体力画像
	}

	//メンバ変数の初期化
	position   = D3DXVECTOR3(0, 0, 0);		//プレイヤーの位置
	isStage	   = TRUE;						//ステージに当たっているか判定(最初は当たっているようにする)
	move	   = D3DXVECTOR3(0, 0, 0);		//移動量
	dirX	   = 0;							//x軸の回転量
	dirZ	   = 0;							//z軸の回転量
	blockId    = 0;							//ステージの種類
	normal	   = D3DXVECTOR3(0, 0, 0);		//ステージブロックの法線
	normalVec  = D3DXVECTOR3(0, -1, 0);		//法線に従ってプレイヤーを転がすベクトル
	life	   = 5;							//初期体力
	timeCount  = 0;							//時間をカウント
	isSprikHit = FALSE;						//障害物と衝突したかどうか
	isDamege   = FALSE;						//ダメージを受けたかどうか
	isDown	   = TRUE;						//ステージから落ちているかどうか
	downTime   = 0;							//ステージから落ちている時間
	D3DXMatrixIdentity(&matRX);				//回転行列
	D3DXMatrixIdentity(&matRZ);				//回転行列
}

//デストラクタ
Player::~Player()
{
	//クラスの解放
	SAFE_DELETE(player);					//プレイヤーモデル
	for (int i = 0; i < sizeof(lifeMark) / sizeof(lifeMark[0]); i++)
	{
		SAFE_DELETE(lifeMark[i]);					//体力画像
	}
}

//機能：読み込み処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::Load()
{
	//クラスの動的作成
	player = new Fbx;		//プレイヤー

	//プレイヤーのモデルの読み込み
	if (FAILED(player->Load("Asset\\Model\\Player.fbx")))
	{
		//プレイヤーの読み込みに失敗したらメッセージボックスで知らせる
		MessageBox(0, "プレイヤーの読み込みに失敗しました", "読み込みエラー", MB_OK);
		return E_FAIL;		//失敗を返す
	}

	//体力画像の読み込み
	for (int i = 0; i < life; i++)
	{
		lifeMark[i] = new Sprite;
		if (FAILED(lifeMark[i]->Load("Asset\\Sprite\\LifeOn.png")))
		{
			//プレイヤーの読み込みに失敗したらメッセージボックスで知らせる
			MessageBox(0, "体力画像の読み込みに失敗しました", "読み込みエラー", MB_OK);
			return E_FAIL;		//失敗を返す
		}
	}

	return S_OK;			//成功を返す
}

//機能：更新処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::Update()
{

	//プレイヤー移動処理の関数を呼ぶ
	if (FAILED(Move()))
	{
		//移動処理に失敗したらメッセージボックスで知らせる
		MessageBox(0, "移動処理に失敗しました", "更新エラー", MB_OK);
		return E_FAIL;		//失敗を返す
	}

	//プレイヤーの回転処理の関数を呼ぶ
	if (FAILED(Rotation()))
	{
		//回転処理に失敗したらメッセージボックスで知らせる
		MessageBox(0, "回転処理に失敗しました", "更新エラー", MB_OK);
		return E_FAIL;		//失敗を返す
	}

	//カメラの移動処理の関数を呼ぶ
	if (FAILED(MoveCamera()))
	{
		//カメラの移動に失敗したらメッセージボックスで知らせる
		MessageBox(0, "カメラの移動に失敗しました", "更新処理", MB_OK);
		return E_FAIL;		//失敗を返す
	}

	//障害物と衝突してかつダメージフラグがfaseのとき
	if (isSprikHit && !isDamege)
	{
		life--;				//ライフを１減らす
		isDamege = true;	//ダメージフラグをtrueにしてダメージを連続して受けなくする
	}
	
	//ダメージフラグがtrueのとき
	if (isDamege)
	{
		timeCount++;		//時間をカウント

		//90フレームまでカウントしたら
		if (timeCount >= 90)
		{
			//ダメージを喰らえるようにする
			isDamege = false;				//ダメージフラグをfalseに
			timeCount = 0;					//時間のカウントを初期化
		}
	}

	//ライフが0になったら
	if (life <= 0)
	{
		g_gameScene = SC_GAMEOVER;		//ゲームオーバーシーンに移行
	}

	//ステージから落ちているとき
	if (!isDown)
	{
		downTime++;		//時間をカウント(単位は1秒間に60)

		//一定時間経ったら
		if (downTime >= 120)
		{
			//ゲームオーバーシーンに
			g_gameScene = SC_GAMEOVER;
		}
	}

	//ステージから落ちていないとき
	else
	{
		//時間を初期化
		downTime = 0;
	}

	return S_OK;			//成功を返す
}

//機能：衝突判定処理
//引数：ユニットベースクラスとそれを継承したクラスのポインタ
//戻値：成功か失敗か
HRESULT Player::Hit(UnitBase* pTarget)
{
	//pTargetがステージだったら
	if (typeid(*pTarget) == typeid(Stage))
	{
		//ステージクラスの変数を作り引数を入れる
		Stage* stage = (Stage*)pTarget;
		
		//ステージクラスから衝突した位置、法線、判定を受け取る
		position.y =  stage->GetPlayerPos();		//位置
		normal	   =  stage->GetBlocksNormal();		//法線
		isSprikHit = stage->GetIsSprikHit();		//障害物と衝突したかどうか
		isDown	   = stage->GetPlayerDown();		//ステージから落ちているかどうか

	}

	return S_OK;			//成功を返す
}

//機能：描画処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::Render()
{
	//行列の作成
	D3DXMATRIX matW;			//ワールド行列用(描画する際にこの行列を引数として渡す)

	//移動行列の作成
	D3DXMatrixTranslation(&matW, position.x,position.y + 0.9f,position.z);		//プレイヤーの位置にプレイヤーを描画させるようにする(y軸は少し上に描画しないと地面に埋まる)

	//行列の合成
	matW = matRX * matRZ * matW;

	//プレイヤーの描画
	if (FAILED(player->Draw(&matW)))
	{
		//描画に失敗したらメッセージボックスで知らせる
		MessageBox(0, "プレイヤーの描画に失敗しました", "描画エラー", MB_OK);
		return E_FAIL;		//失敗を返す
	}

	//体力の描画
	SpriteData data;		///スプライトの情報をまとめる構造体の準備
	float drawShift = 0;		//描画する際にライフをずらすための処理
	for (int i = 0; i < life; i++)
	{
		//最初のループ以外のとき
		if (i != 0)
		{
			drawShift += 50;		//50ずらす
		}

		data.pos = D3DXVECTOR3(6 + drawShift, 5, 0);		//描画の位置を決める
		lifeMark[i]->Draw(&data);							//描画
	}
	

	return S_OK;			//成功を返す
}


//-----------------ここからprivate関数の定義------------------------

//機能：移動処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::Move()
{
	//移動は徐々に速度を上げてく、移動を止めるには摩擦で止めるようにする

	//すべりベクトルで坂道で勝手に転がるようにする
	D3DXVec3Normalize(&normalVec, &normalVec);										//法線で使うベクトルの正規化
	move += (normalVec - (D3DXVec3Dot(&normalVec, &normal)) * normal) * 0.01f;		//すべりベクトルを使って坂道を転がるようにする

	//前移動処理の関数を呼び出す
	if (FAILED(frontMove()))
	{
		//失敗していたらメッセージボックスで知らせる
		MessageBox(0, "前移動に失敗しました", "移動エラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//後移動処理の関数を呼び出す
	if (FAILED(backMove()))
	{
		//失敗していたらメッセージボックスで知らせる
		MessageBox(0, "後移動に失敗しました", "移動エラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}


	//左移動処理の関数を呼び出す
	if (FAILED(leftMove()))
	{
		//失敗していたらメッセージボックスで知らせる
		MessageBox(0, "左移動に失敗しました", "移動エラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//右移動処理の関数を呼び出す
	if (FAILED(rightMove()))
	{
		//失敗していたらメッセージボックスで知らせる
		MessageBox(0, "右移動に失敗しました", "移動エラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//前摩擦処理関数を呼び出す
	if (FAILED(frontFriction()))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "前摩擦処理に失敗しました", "摩擦エラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//後摩擦処理関数を呼び出す
	if (FAILED(backFriction()))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "後摩擦処理に失敗しました", "摩擦エラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}


	//左摩擦処理関数を呼び出す
	if (FAILED(leftFriction()))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "左摩擦処理に失敗しました", "摩擦エラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}
	
	//右摩擦処理関数を呼び出す
	if (FAILED(rightFriction()))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "右摩擦処理に失敗しました", "摩擦エラー", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	 


	 //斜め移動すると速度が速くなるので少し遅くさせて大体同じ速度にする
	 if (move.x < 0 && move.z > 0)		move *= 0.97f;				//左前に移動しているとき
	 if (move.x < 0 && move.z < 0)		move *= 0.97f;				//左後
	 if (move.x > 0 && move.z > 0)		move *= 0.97f;				//右前
	 if (move.x > 0 && move.z < 0)		move *= 0.97f;				//右後

	 //移動
	position.z += move.z;		//前後移動
	position.x += move.x;		//左右移動

	return S_OK;			//成功を返す
}

//----------------------------------移動処理関数----------------------------------

//機能：カメラの移動処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::MoveCamera()
{
	//プレイヤーの少し後ろからカメラを追随させるための処理を書く
	g_pCamera->SetTarget(position);							//プレイヤーの位置を中心になるようにカメラを回転させる
	D3DXVECTOR3 cameraPos = D3DXVECTOR3(0, 7, -10);			//カメラの位置をプレイヤーの少し後ろの上の位置にする
	g_pCamera->SetPos(position + cameraPos);				//cameraposの値とpositionを加算することでプレイヤーの少し後ろからプレイヤーを追い続けるようにする

	return S_OK;			//成功を返す
}

//機能：前移動処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::frontMove()
{


	//↑キーで前移動
	if (g_pInput->IsKeyPush(DIK_UP))
	{
		//前移動
		if (move.z <= MAX_SPEED)			//移動量が最高速度を超えてない時
		{
			move.z += ACCSEL;		//加速させる
		}

	}

	return S_OK;				//成功を返す
}

//機能：後移動処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::backMove()
{
	//↓キーで後に移動
	if (g_pInput->IsKeyPush(DIK_DOWN))
	{
		//後ろに移動
		if (move.z >= -MAX_SPEED)		//移動量が最高速度を超えてない時
		{
			move.z -= ACCSEL;		//加速させる
		}

	}

	return S_OK;				//成功を返す
}

//機能：左移動処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::leftMove()
{
	//←キーで左に移動
	if (g_pInput->IsKeyPush(DIK_LEFT))
	{
		//左に移動
		if (move.x >= -MAX_SPEED)		//最高速度を超えていない時
		{
			move.x -= ACCSEL;		//加速させる
		}
	}

	return S_OK;				//成功を返す
}

//機能：右移動処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::rightMove()
{

	//→キーで右移動
	if (g_pInput->IsKeyPush(DIK_RIGHT))
	{
		//右に移動
		if (move.x <= MAX_SPEED)		//最高速度を超えていない時
		{
			move.x += ACCSEL;		//加速させる
		}
	}

	return S_OK;				//成功を返す
}

//--------------------摩擦処理関数--------------------------------

//機能：前摩擦処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::frontFriction()
{

	//プレイヤーが前方向に移動していたら
	if (move.z > 0)
	{

		//逆の位置に値を代入させ続けて摩擦を発生させる
		move.z -= FRICTION;

		//障害物と衝突しているとき
		if (isSprikHit)
		{
			//反対方向に移動			
			position.z -= 1;
		}

		//完全に移動を止める
		if (move.z <= 0.001f)
			move.z = 0;			//この処理を書かないとmoveの値が0にならなかったのでこの処理を追加
	}

	return S_OK;			//失敗を返す
}

//機能：後摩擦処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::backFriction()
{

	//プレイヤーが後ろ方向に移動していたら
	if (move.z < 0)
	{

		//逆の位置に値を代入させ続けて摩擦を発生させる
		move.z += FRICTION;

		//障害物と衝突しているとき
		if (isSprikHit)
		{
			//反対方向に移動
			position.z += 1;
		}

		//完全に移動を止める
		if (move.z >= MIN_SPEED)
			move.z = 0;
	}

	return S_OK;			//失敗を返す
}

//機能：左摩擦処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::leftFriction()
{

	//プレイヤーが左方向に移動していたら
	if (move.x < 0)
	{

		//逆の位置に値を代入させ続けて摩擦を発生させる
		move.x += FRICTION;

		//障害物と衝突しているとき
		if (isSprikHit)
		{
			//反対方向に移動
			position.x -= 1;
		}

		//完全に移動を止める
		if (move.x >= MIN_SPEED)
			move.x = 0;
	}

	return S_OK;			//失敗を返す
}

//機能：右摩擦処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::rightFriction()
{
	//プレイヤーが右方向に移動していたら
	if (move.x > 0)
	{

		//逆の位置に値を代入させ続けて摩擦を発生させる
		move.x -= FRICTION;

		//障害物と衝突しているとき
		if (isSprikHit)
		{
			//反対方向に移動
			position.x += 1;
		}

		//完全に移動を止める
		if (move.x <= MIN_SPEED)
			move.x = 0;
	}

	return S_OK;			//失敗を返す
}


//---------------------------回転処理関数-------------------------------------------

//機能：回転処理
//引数：なし
//戻値：成功か失敗か
HRESULT Player::Rotation()
{
	//前方向に移動しているとき
	if (move.z > 0)
	{
		//前回転
		dirX += DIR_SPEED;	//回転量をインクリメント
		D3DXMatrixRotationX(&matRX, D3DXToRadian(dirX));		//回転量分x軸に回転
	}

	//後方向に移動しているとき
	if (move.z < 0)
	{
		//後ろ回転
		dirX -= DIR_SPEED;	//回転量をデクリメント
		D3DXMatrixRotationX(&matRX, D3DXToRadian(dirX));		//回転量分x軸に回転
	}

	//左方向に移動しているとき
	if (move.x < 0)
	{
		//左に回転
		dirZ += DIR_SPEED;		//デクリメント
		D3DXMatrixRotationZ(&matRZ, D3DXToRadian(dirZ));		//移動量分z軸に回転
	}

	//右方向に移動しているとき
	if (move.x > 0)
	{
		//右に回転
		dirZ -= DIR_SPEED;		//インクリメント
		D3DXMatrixRotationZ(&matRZ, D3DXToRadian(dirZ));		//移動量分z軸に回転
	}

	//前後に移動していないとき
	if (move.z == 0)			dirX = 0;		//回転量を初期化
	
	//左右に移動していないとき
	if (move.x == 0)			dirZ = 0;		//回転量を初期化

	return S_OK;			//成功を返す
}



