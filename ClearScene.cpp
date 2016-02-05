//インクルード
#include "ClearScene.h"
#include "Clear.h"			//クリアクラス

//コンストラクタ
ClearScene::ClearScene()
{
	//ユニット追加
	unit.push_back(new Clear);
}


