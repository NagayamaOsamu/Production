/*===========================================================================================
概　略：Mayaで作成したモデル(Fbx形式)を描画するクラスの関数定義
作成日：9月10日
更新日：10月5日
制作者：永山治夢
=============================================================================================*/

//----------------------インクルード------------------------
#include "Fbx.h"
#include "Camera.h"		//カメラクラス

//コンストラクタ
Fbx::Fbx()
{
	//初期化
	//頂点バッファで使う変数
	vertexCount = 0;		//頂点数
	pVertexBuffer = NULL;	//頂点バッファを入れる変数

	//インデックスバッファで使う変数
	polygonCount = 0;		//ポリゴン数
	indexCount = 0;			//インデックス数
	pIndexBuffer = NULL;	//インデックスバッファを入れる変数


	//マテリアル作成に使う変数
	materialCount = 0;				//マテリアルの個数
	pMaterial = NULL;				//マテリアルの配列を作るのに必要なポインタ
	polygonCountOfMaterial = NULL;	//マテリアル毎のポリゴン数を入れるポインタ

	//テクスチャに使う変数
	pTexture = NULL;				//テクスチャを入れるポインタ

	//レイキャスト処理に使う変数
	D3DXMatrixIdentity(&lastWorldMatrix);
}

//デストラクタ
Fbx::~Fbx()
{
	//インデックスバッファの削除
	for (int i = 0; i < materialCount; i++)
	{
		SAFE_RELEASE(pIndexBuffer[i]);			//一つ一つ削除
	}
	SAFE_DELETE_ARRAY(pIndexBuffer);			//その後に配列を削除

	
	SAFE_RELEASE(pVertexBuffer);				//頂点バッファの削除	

	
	SAFE_DELETE_ARRAY(pMaterial);				//マテリアルを入れる変数の削除

	
	SAFE_DELETE_ARRAY(polygonCountOfMaterial);	//マテリアル毎のポリゴン数を入れるポインタの削除

	
	SAFE_DELETE_ARRAY(pTexture);				//テクスチャを入れるポインタの削除

	
	manager->Destroy();							//マネージャを解放(これでインポータとシーンも解放される)
}

//機能：読み込み処理
//引数：ファイル名
//戻値：成功か失敗か
HRESULT Fbx::Load(char* fileName)
{
	//マネージャの作成
	manager = FbxManager::Create();

	//作成したマネーを基にインポータ、シーンを作成
	importer = FbxImporter::Create(manager, "");
	scene = FbxScene::Create(manager, "");

	//インポータを使ってファイルを開く
	importer->Initialize(fileName);

	//開いたファイルをシーンに渡す
	importer->Import(scene);

	//インポータの解放
	importer->Destroy();

	//現在のカレントディレクトリのフォルダ名を覚えておく処理
	char  defaultCurrentDir[MAX_PATH];							//デフォルトのフォルダ名を入れる変数
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);			//現在のカレントディレクトリをGetCurrentDirectory関数で調べ変数に代入

	//ロードしたファイルのパスからフォルダ名を取得させる
	char    dir[MAX_PATH];				//フォルダ名を入れる変数

	//_splitpath_sでフォルダ名のみ取得する
	_splitpath_s(fileName, NULL, 0, dir, MAX_PATH, NULL, 0, NULL, 0);

	//この処理以降は↑で取得したフォルダ名のフォルダだけを使うするようにする
	SetCurrentDirectory(dir);

	//ルートノードを取り出す
	FbxNode* rootNode = scene->GetRootNode();

	//ルートノードに子供が何人いるか調べ数を変数に代入
	int childCount = rootNode->GetChildCount();

	//子供の人数分だけループ
	for (int i = 0; childCount > i; i++)
	{
		//ノードの情報をメッシュノードか調べるため関数呼び出し
		CheckNode(rootNode->GetChild(i));
	}

	//デフォルトのディレクトリに戻す
	SetCurrentDirectory(defaultCurrentDir);

	return S_OK;			//成功を返す
}

//機能：メッシュの情報が入ったノードを探す
//引数：ノード情報のポインタ
//戻値：成功か失敗か
HRESULT  Fbx::CheckNode(FbxNode* pNode)
{
	//引数で受け取ったノードの属性を取得して変数に代入
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();

	//取得したノードがメッシュノードだったら
	if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		materialCount = pNode->GetMaterialCount();			//マテリアルの数を入れる
		pMaterial = new D3DMATERIAL9[materialCount];		//マテリアルを代入
		pTexture = new LPDIRECT3DTEXTURE9[materialCount];	//テクスチャを入れるポインタをマテリアルの数だけ配列にする

		//マテリアルの数だけループ
		for (int i = 0; i < materialCount; i++)
		{
			//マテリアルの情報の取得
			FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)pNode->GetMaterial(i);

			//ポリゴンの色と環境光(アンビエント)の情報を取得
			FbxDouble3  diffuse = pLambert->Diffuse;			//ディフューズ(ポリゴンの色)
			FbxDouble3  ambient = pLambert->Ambient;			//アンビエント

			//マテリアルを入れる変数の初期化
			ZeroMemory(&pMaterial[i], sizeof(D3DMATERIAL9));

			//ディフューズの情報を代入
			pMaterial[i].Diffuse.r = (float)diffuse[0];		//赤
			pMaterial[i].Diffuse.g = (float)diffuse[1];		//緑
			pMaterial[i].Diffuse.b = (float)diffuse[2];		//青
			pMaterial[i].Diffuse.a = 1.0f;					//透明度

			//環境光の情報を代入
			pMaterial[i].Ambient.r = (float)ambient[0];		//赤
			pMaterial[i].Ambient.g = (float)ambient[1];		//緑
			pMaterial[i].Ambient.b = (float)ambient[2];		//青
			pMaterial[i].Ambient.a = 1.0f;					//透明度

			//テクスチャの情報を取得
			FbxProperty  lProperty = pNode->GetMaterial(i)->FindProperty(FbxSurfaceMaterial::sDiffuse);		//設定されたテクスチャの情報を取得
			FbxFileTexture* textureFile = lProperty.GetSrcObject<FbxFileTexture>(0);						//↑で使われたファイルの情報を取得(テクスチャが無い場合はNULLが返ってくる)

			//このマテリアルにテクスチャが貼られてなかったら
			if (textureFile == NULL)
			{
				pTexture[i] = NULL;		//テクスチャを貼らない
			}

			//テクスチャが貼られていたら
			else
			{
				//テクスチャの画像ファイル名の取得
				const char* textureFileName = textureFile->GetFileName();		//GetFileNameで名前を取得できる(但しMayaで選択した絶対パスになる)

				//取得したファイル名は絶対パスなのだが必ずそのパスにファイルが存在してるとは限らない
				//取得した情報で間違いが必ず無い　ファイル名と拡張子だけ取り出す
				char    name[_MAX_FNAME];		//ファイル名を入れる変数
				char    ext[_MAX_EXT];			//拡張子を入れる変数

				//絶対パスからファイル名と拡張子のみを取得する
				_splitpath_s(textureFileName, NULL, 0, NULL, 0, name, _MAX_FNAME, ext, _MAX_EXT);	
				//_sprintpath_sで絶対パスをドライブ名やフォルダ名、ファイル名、拡張子に分けることができる
				//引数は(パス,ドライブ名を入れる変数,ドライブ名の際だし文字数,フォルダ名を入れる変数,フォルダ名の最大文字数,ファイル名を入れる変数,ファイル名の最大文字数,拡張子,拡張子の最大文字数)

				wsprintf(name, "%s%s", name, ext);		//ファイル名にファイル名と拡張子を繋げたのを代入する

				//テクスチャの作成
				if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, name, 0, 0, 0, 0, D3DFMT_UNKNOWN,
					D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &pTexture[i])))
				{
					//テクスチャの作成に失敗したらメッセージボックスで知らせる
					MessageBox(0, "テクスチャの作成に失敗しました", name, MB_OK);

					return E_FAIL;		//失敗を知らせる
				}

			}

		}

		//メッシュの頂点情報を取り出す関数を呼ぶ
		if (FAILED(CheckMesh(pNode->GetMesh())))
		{
			return E_FAIL;		//失敗を返す
		}

	}

	//メッシュノード以外だったら
	else
	{
		int childCount = pNode->GetChildCount();		//ルートノードの子供の子供の数を調べて代入

		//子供の数だけループ
		for (int i = 0; childCount > i; i++)
		{
			//再帰呼び出ししてノードの情報を調べる
			if (FAILED(CheckNode(pNode->GetChild(i))))
			{
				return E_FAIL;		//失敗を返す
			}
		}
	}

	return S_OK;					//成功を返す
}

//機能：メッシュノードの頂点情報を取り出す
//引数：メッシュのポインタ
//戻値：成功か失敗か
HRESULT  Fbx::CheckMesh(FbxMesh* pMesh)
{
	//頂点バッファを作成する関数の呼び出し
	if (FAILED(CreateVertexBuf(pMesh)))
	{
		return E_FAIL;		//失敗を返す
	}

	//インクデックスバッファを作成する関数の呼び出し
	if (FAILED(CreateindexBuf(pMesh)))
	{
		return E_FAIL;		//失敗を返す
	}

	return S_OK;			//成功を返す
}

//機能：頂点バッファの作成
//引数：メッシュのポインタ
//戻値：成功か失敗か
HRESULT Fbx::CreateVertexBuf(FbxMesh* pMesh)
{
	//頂点情報を読み込むために先頭のアドレスを確保
	FbxVector4* pVertexPos = pMesh->GetControlPoints();

	//ポリゴン数
	polygonCount = pMesh->GetPolygonCount();		

	//頂点数を代入
	vertexCount = pMesh->GetControlPointsCount();

	//頂点の数だけVerTex構造体の配列を作る(ここに頂点バッファの情報を一時的に入れる)
	Vertex* pVertexList = new Vertex[vertexCount];
	/*このvertexは二次元配列として扱うことができ、
	pVertexPos[0][0]には最初の頂点のX座標、pVertexPos[0][1]には最初の頂点のY座標、
	pVertexPos[0][2]には最初の頂点のZ座標、そしてpVertexPos[1][0]には2番目の頂点のX座標が入る。*/

	//頂点の数だけループ
	for (int i = 0; vertexCount > i; i++)
	{
		//頂点のx,y,z座標を代入する
		pVertexList[i].pos.x = (float)pVertexPos[i][0];		//頂点のx座標
		pVertexList[i].pos.y = (float)pVertexPos[i][1];		//頂点のy座標
		pVertexList[i].pos.z = (float)pVertexPos[i][2];		//頂点のz座標
	}

	//uvの数を数える処理
	FbxStringList uvSetName;			
	pMesh->GetUVSetNames(uvSetName);
	int uvCount = uvSetName.GetCount();		//ここでuvの数を入れる
	bool unMap = false;
	FbxVector2  uv;

	//ポリゴンの数だけループ
	for (int i = 0; i < polygonCount; i++)
	{
		//最初の頂点の番号を調べて変数に代入
		int   startIndex = pMesh->GetPolygonVertexIndex(i);

		//三角形の頂点の数だけループ
		for (int j = 0; j < 3; j++)
		{
			//頂点番号の一覧を変数に代入
			int index = pMesh->GetPolygonVertices()[startIndex + j];

			//法線の情報を取得
			FbxVector4  Normal;								//法線の情報を入れる変数
			pMesh->GetPolygonVertexNormal(i, j, Normal);	//法線の情報を第3引数に取得できる関数(第1引数はポリゴンの番号、頂点の番号)

			//頂点情報に法線情報を追加
			pVertexList[index].normal = D3DXVECTOR3((float)Normal[0], (float)Normal[1], (float)Normal[2]);

			//uvの数だけループ
			for (int k = 0; k < uvCount; k++)
			{
				//uv情報の取得
				FbxString name = uvSetName.GetStringAt(k);
				pMesh->GetPolygonVertexUV(i, j, name, uv, unMap);
				pVertexList[index].uv = D3DXVECTOR2((float)uv.mData[0], (float)(1.0 - uv.mData[1]));		//uv情報を変数に代入(y軸は反転されているので-1で元に戻している)
			}
		}
	}

	//空の頂点バッファの作成
	//頂点バッファを作る時はCreateVertexBuffer(データのサイズ(1頂点のサイズ×頂点数),0,頂点が何の情報を持っているか,,作った頂点バッファのアドレス,0)
	if (FAILED(g_pDevice->CreateVertexBuffer(sizeof(Vertex)* vertexCount, 0, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, D3DPOOL_MANAGED, &pVertexBuffer, 0)))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "頂点バッファの作成に失敗しました", "Fbx", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//頂点情報の構造体のポインタ作成
	Vertex *pVertexCopy;

	//頂点バッファのロック(自分だけしか操作できなくする)
	if (FAILED(pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0)))
	{
		//ロックに失敗したらメッセージボックスで知らせる
		MessageBox(0, "頂点バッファのロックに失敗しました", "Fbx", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//pVertexCopyにpVertexListの内容をコピー
	memcpy(pVertexCopy, pVertexList, sizeof(Vertex)*vertexCount);

	//頂点バッファのアンロック(他の人が操作可能になる)
	if (FAILED(pVertexBuffer->Unlock()))
	{
		//アンロックに失敗したらメッセージボックスで知らせる
		MessageBox(0, "頂点バッファのアンロックに失敗しました", "Fbx", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//pVertexListの解放
	SAFE_DELETE_ARRAY(pVertexList);

	return S_OK;			//成功を返す
}

//機能：インデックスバッファの作成
//引数：メッシュのポインタ
//戻値：成功か失敗か
HRESULT Fbx::CreateindexBuf(FbxMesh* pMesh)
{
	//ポリゴン数とインデックス数の取得
	polygonCount = pMesh->GetPolygonCount();		//ポリゴン数
	indexCount = pMesh->GetPolygonVertexCount();	//インデックス数

	//インデックスバッファをマテリアルの数だけ入れられるように配列化
	pIndexBuffer = new IDirect3DIndexBuffer9*[materialCount];

	//マテリアルの数だけ配列に
	polygonCountOfMaterial = new int[materialCount];

	for (int i = 0; i < materialCount; i++)
	{
		//一時的にインデックス情報を入れる変数(配列)
		int* pIndexList = new int[indexCount];

		//pIndexListの要素準備
		int count = 0;			//最初は0

		//ポリゴンの数だけループ
		for (int polygon = 0; polygon < polygonCount; polygon++)
		{
			//今見ているポリゴンが何番目のマテリアルかを調べて変数に代入
			int materialID = pMesh->GetLayer(0)->GetMaterials()->GetIndexArray().GetAt(polygon);

			//マテリアルの番号と要素数が同じ時
			if (materialID == i)
			{
				//更にVertexが3未満の間ループ(Vertexに頂点情報が入っていて今回はポリゴンが全て三角形なので0,1,2の要素で十分)
				for (int vertex = 0; vertex < 3; vertex++)
				{
					//配列にインデックス情報を入れる
					pIndexList[count++] = pMesh->GetPolygonVertex(polygon, vertex);
				}
			}
		}

		//頂点の数から３を割ることでポリゴン数を求められる
		polygonCountOfMaterial[i] = count / 3;

		//空のインデックスバッファ作成
		if (FAILED(g_pDevice->CreateIndexBuffer(sizeof(int)* indexCount, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &pIndexBuffer[i], 0)))
		{
			//作成に失敗したらメッセージボックスで知らせる
			MessageBox(0, "インデックスバッファの作成に失敗しました", "Fbx", MB_OK);

			return E_FAIL;		//失敗を返す
		}

		//インデックス情報を入れる変数
		DWORD *pIndexCopy;

		//インデックスバッファのロック
		if (FAILED(pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0)))
		{
			//ロックに失敗したらメッセージボックスで知らせる
			MessageBox(0, "インデックスバッファのロックに失敗しました", "Fbx", MB_OK);

			return E_FAIL;		//失敗を返す
		}

		//インデックス情報のコピー
		memcpy(pIndexCopy, pIndexList, sizeof(int)* indexCount);

		//インデックスバッファのアンロック
		if (FAILED(pIndexBuffer[i]->Unlock()))
		{
			//アンロックに失敗したらメッセージボックスで知らせる
			MessageBox(0, "インデックスバッファのアンロックに失敗しました", "Fbx", MB_OK);

			return E_FAIL;		//失敗を返す
		}

		//pIndexListの解放
		SAFE_DELETE_ARRAY(pIndexList);
	}

	return S_OK;			//成功を返す
}

//機能：描画処理
//引数：ワールド行列
//戻値：成功か失敗か
HRESULT Fbx::Draw(D3DXMATRIX* matWorld)
{
	//ワールド行列のセット
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "ワールド行列の設定に失敗しました", "Fbx", MB_OK);

		return E_FAIL;		//失敗を返す
	}


	//頂点ストリームの設定
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)))
	{
		//設定に失敗したらメッセージボックスで知らせる
		MessageBox(0, "頂点ストリームの設定に失敗しました", "Fbx", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//使う頂点バッファの指定
	if (FAILED(g_pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(Vertex))))
	{
		//失敗したらメッセージボックスで知らせる
		MessageBox(0, "頂点バッファの設定に失敗しました", "Fbx", MB_OK);

		return E_FAIL;		//失敗を返す
	}

	//使うインデックスバッファの指定
	for (int i = 0; i < materialCount; i++)
	{
		if (FAILED(g_pDevice->SetIndices(pIndexBuffer[i])))
		{
			//失敗したらメッセージボックスで知らせる
			MessageBox(0, "インデックスバッファの設定に失敗しました", "Fbx", MB_OK);

			return E_FAIL;		//失敗を返す
		}

		//マテリアルの設定
		if (FAILED(g_pDevice->SetMaterial(&pMaterial[i])))
		{
			//設定に失敗したらメッセージボックスで知らせる
			MessageBox(0, "マテリアルの設定に失敗しました", "Fbx", MB_OK);

			return E_FAIL;		//失敗を返す
		}

		//テクスチャの貼り付け
		if (FAILED(g_pDevice->SetTexture(0, pTexture[i])))
		{
			//テクスチャの貼り付けに失敗したらメッセージボックスで知らせる
			MessageBox(0, "テクスチャのセットに失敗しました", "Fbx", MB_OK);

			return E_FAIL;		//失敗を返す
		}

		//頂点・インデックスバッファを使ってメッシュの描画
		if (FAILED(g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, indexCount, 0, polygonCountOfMaterial[i])))
		{
			//描画に失敗したらメッセージボックスで知らせる
			MessageBox(0, "メッシュの描画に失敗しました", "Fbx", MB_OK);

			return E_FAIL;	//失敗を返す
		}
	}

	lastWorldMatrix = *matWorld;		//最後に使ったワールド行列を代入

	return S_OK;			//成功を返す
}

//機能：レイキャスト処理
//引数：レイキャストに必要なデータをまとめた構造体
//戻値：真か偽
BOOL Fbx::RayCast(RayCastData *data)
{
	//構造体の変数の初期化
	data->dist = 99999.0f;		//衝突点までの距離
	data->hit = FALSE;			//当たったか当たってないかの判定

	//頂点バッファにアクセスできるように
	Vertex *pVertexCopy;									//頂点情報をまとめた構造体の作成
	pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0);		//頂点バッファをロックして他の関数で処理できなくする


	//マテリアル毎ループ
	for (int i = 0; i < materialCount; i++)
	{
		//インデックスバッファにアクセスできるように
		DWORD *pIndexCopy;
		pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0);

		//そのマテリアルのポリゴン毎
		for (int j = 0; j < polygonCountOfMaterial[i]; j++)
		{
			//3頂点分
			D3DXVECTOR3 ver[3];
			ver[0] = pVertexCopy[pIndexCopy[j * 3 + 0]].pos;					//要素0目
			ver[1] = pVertexCopy[pIndexCopy[j * 3 + 1]].pos;					//要素1目
			ver[2] = pVertexCopy[pIndexCopy[j * 3 + 2]].pos;					//要素2目

			//描画する際に情報が変更されていたらそれに合わせてその行列を掛ける
			D3DXVec3TransformCoord(&ver[0], &ver[0], &data->matrix);			//要素0目
			D3DXVec3TransformCoord(&ver[1], &ver[1], &data->matrix);			//要素1目
			D3DXVec3TransformCoord(&ver[2], &ver[2], &data->matrix);			//要素2目


			//ぶつかってるかチェック
			float dist, u, v;								//D3DXIntersectTri関数に必要な変数用意
			D3DXVec3Normalize(&data->dir, &data->dir);		//レイの方向を正規化
			BOOL hit = D3DXIntersectTri(&ver[0], &ver[1], &ver[2], &data->start, &data->dir, &u, &v, &dist);		//マテリアルに衝突していたらtrueが返る

			//今まで一番近い？
			if (hit && dist < data->dist)
			{
				data->hit = TRUE;		//衝突した
				data->dist = dist;		//衝突した距離を入れる

				//外積を求めるときに使う変数準備
				D3DXVECTOR3 v1 = ver[2] - ver[0];			//ver[2] - ver[0]のベクトルを代入
				D3DXVECTOR3 v2 = ver[2] - ver[1];			//ver[1] - ver[2]のベクトルを代入

				//変数v1とv2の外積を求めてレイキャスト構造体のnormalに代入
				D3DXVec3Cross(&data->normal, &v1, &v2);

				//外積を求めた値が大きくなるのでそれで誤差が起きるかもしれないので正規化
				D3DXVec3Normalize(&data->normal, &data->normal);

				data->pos = ver[0] + u * (ver[1] - ver[0]) + v * (ver[2] - ver[0]);		//位置情報を受け取る
			}
		}

		//インデックスバッファ使用終了
		pIndexBuffer[i]->Unlock();
	}

	//頂点バッファ使用終了
	pVertexBuffer->Unlock();

	
	return data->hit;			//衝突したかを返す
}

BOOL Fbx::HitRay(D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist, D3DXMATRIX matW)
{
	return HitRay(pRayPos, pRayDir, pDist, NULL,matW);
}

BOOL Fbx::HitRay(D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist, D3DXVECTOR3* normal, D3DXMATRIX matW)
{
	*pDist = 9999999;

	//頂点バッファにアクセスできるように
	Vertex *pVertexCopy;									//頂点情報をまとめた構造体の作成
	pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0);		//頂点バッファをロックして他の関数で処理できなくする

	//マテリアル毎ループ
	for (int i = 0; i < materialCount; i++)
	{
		//インデックスバッファにアクセスできるように
		DWORD *pIndexCopy;
		pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0);

		//そのマテリアルのポリゴン毎
		for (int j = 0; j < polygonCountOfMaterial[i]; j++)
		{
			//3頂点分
			D3DXVECTOR3 ver[3];
			ver[0] = pVertexCopy[pIndexCopy[j * 3 + 0]].pos;					//要素0目
			ver[1] = pVertexCopy[pIndexCopy[j * 3 + 1]].pos;					//要素1目
			ver[2] = pVertexCopy[pIndexCopy[j * 3 + 2]].pos;					//要素2目

			//x軸を反転させる
			ver[0].x *= -1.0f;
			ver[1].x *= -1.0f;
			ver[2].x *= -1.0f;

			//描画位置に合わせる
			D3DXVec3TransformCoord(&ver[0], &ver[0], &matW);			//要素0目
			D3DXVec3TransformCoord(&ver[1], &ver[1], &matW);			//要素1目
			D3DXVec3TransformCoord(&ver[2], &ver[2], &matW);			//要素2目



			//方向の正規化
			D3DXVec3Normalize(pRayDir, pRayDir);

			//マウスのレイとモデルとの衝突判定
			float dist;
			BOOL hit = D3DXIntersectTri(&ver[0], &ver[1], &ver[2], pRayPos, pRayDir, NULL, NULL, &dist);

			//衝突したら
			if (hit)
			{
				//距離が引数の距離よりも短いとき(pDistには初期値として999999が入っているので衝突してたらTRUEになるはず)
				if (dist < *pDist)
				{
					//距離を代入
					*pDist = dist;
				}

			}

		}


		//インデックスバッファ使用終了
		pIndexBuffer[i]->Unlock();

	}

	//頂点バッファ使用終了
	pVertexBuffer->Unlock();

	//pDistに初期値以外の値が入っていたらTRUEにする
	return (*pDist < 99999);
}

//BOOL Fbx::HitRay( D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist, D3DXVECTOR3* normal)
//{
//	D3DXVECTOR3 v1, v2, v3;
//	v1 = pMesh->m_pVertices[ pMesh->m_pIndex[count+0] ].vPos;
//	v2 = pMesh->m_pVertices[pMesh->m_pIndex[count + 1]].vPos;
//	v3 = pMesh->m_pVertices[pMesh->m_pIndex[count + 2]].vPos;
//
//	v1.x *= -1.0f;
//	v2.x *= -1.0f;
//	v3.x *= -1.0f;
//
//
//	FLOAT dist;
//	BOOL hit = D3DXIntersectTri(&v1, &v2, &v3, pRayPos, pRayDir, NULL, NULL, &dist);
//
//	if (hit == TRUE)
//	{
//		if (dist < *pDist)
//		{
//			*pDist = dist;
//
//			if (normal != NULL)
//			{
//				D3DXVECTOR3 a = v2 - v1;
//				D3DXVECTOR3 b = v3 - v1;
//				D3DXVec3Cross(normal, &a, &b);
//				D3DXVec3Normalize(normal, normal);
//			}
//		}
//	}
//
// 子階層オブジェクトに対しても描画処理を実行
//for (DWORD i = 0; i < pMesh->GetNumChild(); i++)
//{
//	HitRay(pMesh->GetChild()[i], pRayPos, pRayDir, pDist, normal);
//}
//	return FALSE;
//}


//機能：マウス座標とFbxとの衝突判定
//引数：ワールド座標
//戻値：衝突した(TRUE)か衝突してない(FALSE)の判定
BOOL Fbx::HitMouseRay(D3DXMATRIX matW)
{
	//ビューポート行列(レンダリング結果をウィンドウサイズに合わせるための行列)を作成
	D3DXMATRIX VP;				//行列準備
	D3DXMatrixIdentity(&VP);	//初期化

	//ビューポート行列に値を代入
	VP._11 = (float)WINDOW_WIDTH / 2.0f;
	VP._22 = (float)-WINDOW_HEIGHT / 2.0f;
	VP._41 = (float)WINDOW_WIDTH / 2.0f;
	VP._42 = (float)WINDOW_HEIGHT / 2.0f;

	//ビュー行列の逆行列
	D3DXMATRIX InvView;												//行列準備
	D3DXMatrixInverse(&InvView, NULL, g_pCamera->GetView());		//カメラクラスからビュー行列を受け取って逆行列に

	//射影行列の逆行列
	D3DXMATRIX InvPrj;												//行列準備
	D3DXMatrixInverse(&InvPrj, NULL, g_pCamera->GetProj());			//カメラクラスから射影行列を受け取って逆行列に

	//ビューポート行列
	D3DXMATRIX InvViewport;											//行列準備
	D3DXMatrixInverse(&InvViewport, NULL, &VP);						//この関数の最初で代入した値をもとに逆行列に

	// 各逆行列を合成
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;

	//モデルのワールド行列の逆行列
	D3DXMATRIX InvWorld;											//行列準備
	D3DXMatrixInverse(&InvWorld, NULL, &matW);						//引数のワールド行列を逆行列に

	//マウスの位置の手前から奥にレイを飛ばす

	//手前の点（マウス位置でＺが0）
	D3DXVECTOR3	mouseFront((float)g_pInput->GetMousePos().x, (float)g_pInput->GetMousePos().y, 0);
	D3DXVec3TransformCoord(&mouseFront, &mouseFront, &tmp);
	//D3DXVec3TransformCoord(&mouseFront, &mouseFront, &InvWorld);

	//奥の点（マウス位置でＺが1）
	D3DXVECTOR3	mouseBack((float)g_pInput->GetMousePos().x, (float)g_pInput->GetMousePos().y , 1);
	D3DXVec3TransformCoord(&mouseBack, &mouseBack, &tmp);
	//D3DXVec3TransformCoord(&mouseBack, &mouseBack, &InvWorld);


	//レイの方向（手前の点から奥の点に向かうベクトル）
	D3DXVECTOR3 rayDir = mouseBack - mouseFront;

	//レイを飛ばす
	float dist;
	BOOL isHit = HitRay(&mouseFront, &rayDir, &dist, matW);

	return isHit;		//衝突判定を返す
}