/*===========================================================================================
概　略：線を引くクラスのヘッダー作成
作成日：8月31日
更新日：8月31日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Global.h"

//多重定義防止処理
#pragma once

class Line
{

public:

	HRESULT Draw(D3DXVECTOR3 start, D3DXVECTOR3 end);				//直線を引く
	HRESULT Draw(D3DXVECTOR3 start, D3DXVECTOR3 end, DWORD color);	//直線を引く(オーバーロード関数、色情報を追加している)
	
};

