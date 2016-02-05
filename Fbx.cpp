/*===========================================================================================
�T�@���FMaya�ō쐬�������f��(Fbx�`��)��`�悷��N���X�̊֐���`
�쐬���F9��10��
�X�V���F10��5��
����ҁF�i�R����
=============================================================================================*/

//----------------------�C���N���[�h------------------------
#include "Fbx.h"
#include "Camera.h"		//�J�����N���X

//�R���X�g���N�^
Fbx::Fbx()
{
	//������
	//���_�o�b�t�@�Ŏg���ϐ�
	vertexCount = 0;		//���_��
	pVertexBuffer = NULL;	//���_�o�b�t�@������ϐ�

	//�C���f�b�N�X�o�b�t�@�Ŏg���ϐ�
	polygonCount = 0;		//�|���S����
	indexCount = 0;			//�C���f�b�N�X��
	pIndexBuffer = NULL;	//�C���f�b�N�X�o�b�t�@������ϐ�


	//�}�e���A���쐬�Ɏg���ϐ�
	materialCount = 0;				//�}�e���A���̌�
	pMaterial = NULL;				//�}�e���A���̔z������̂ɕK�v�ȃ|�C���^
	polygonCountOfMaterial = NULL;	//�}�e���A�����̃|���S����������|�C���^

	//�e�N�X�`���Ɏg���ϐ�
	pTexture = NULL;				//�e�N�X�`��������|�C���^

	//���C�L���X�g�����Ɏg���ϐ�
	D3DXMatrixIdentity(&lastWorldMatrix);
}

//�f�X�g���N�^
Fbx::~Fbx()
{
	//�C���f�b�N�X�o�b�t�@�̍폜
	for (int i = 0; i < materialCount; i++)
	{
		SAFE_RELEASE(pIndexBuffer[i]);			//���폜
	}
	SAFE_DELETE_ARRAY(pIndexBuffer);			//���̌�ɔz����폜

	
	SAFE_RELEASE(pVertexBuffer);				//���_�o�b�t�@�̍폜	

	
	SAFE_DELETE_ARRAY(pMaterial);				//�}�e���A��������ϐ��̍폜

	
	SAFE_DELETE_ARRAY(polygonCountOfMaterial);	//�}�e���A�����̃|���S����������|�C���^�̍폜

	
	SAFE_DELETE_ARRAY(pTexture);				//�e�N�X�`��������|�C���^�̍폜

	
	manager->Destroy();							//�}�l�[�W�������(����ŃC���|�[�^�ƃV�[������������)
}

//�@�\�F�ǂݍ��ݏ���
//�����F�t�@�C����
//�ߒl�F���������s��
HRESULT Fbx::Load(char* fileName)
{
	//�}�l�[�W���̍쐬
	manager = FbxManager::Create();

	//�쐬�����}�l�[����ɃC���|�[�^�A�V�[�����쐬
	importer = FbxImporter::Create(manager, "");
	scene = FbxScene::Create(manager, "");

	//�C���|�[�^���g���ăt�@�C�����J��
	importer->Initialize(fileName);

	//�J�����t�@�C�����V�[���ɓn��
	importer->Import(scene);

	//�C���|�[�^�̉��
	importer->Destroy();

	//���݂̃J�����g�f�B���N�g���̃t�H���_�����o���Ă�������
	char  defaultCurrentDir[MAX_PATH];							//�f�t�H���g�̃t�H���_��������ϐ�
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);			//���݂̃J�����g�f�B���N�g����GetCurrentDirectory�֐��Œ��וϐ��ɑ��

	//���[�h�����t�@�C���̃p�X����t�H���_�����擾������
	char    dir[MAX_PATH];				//�t�H���_��������ϐ�

	//_splitpath_s�Ńt�H���_���̂ݎ擾����
	_splitpath_s(fileName, NULL, 0, dir, MAX_PATH, NULL, 0, NULL, 0);

	//���̏����ȍ~�́��Ŏ擾�����t�H���_���̃t�H���_�������g������悤�ɂ���
	SetCurrentDirectory(dir);

	//���[�g�m�[�h�����o��
	FbxNode* rootNode = scene->GetRootNode();

	//���[�g�m�[�h�Ɏq�������l���邩���א���ϐ��ɑ��
	int childCount = rootNode->GetChildCount();

	//�q���̐l�����������[�v
	for (int i = 0; childCount > i; i++)
	{
		//�m�[�h�̏������b�V���m�[�h�����ׂ邽�ߊ֐��Ăяo��
		CheckNode(rootNode->GetChild(i));
	}

	//�f�t�H���g�̃f�B���N�g���ɖ߂�
	SetCurrentDirectory(defaultCurrentDir);

	return S_OK;			//������Ԃ�
}

//�@�\�F���b�V���̏�񂪓������m�[�h��T��
//�����F�m�[�h���̃|�C���^
//�ߒl�F���������s��
HRESULT  Fbx::CheckNode(FbxNode* pNode)
{
	//�����Ŏ󂯎�����m�[�h�̑������擾���ĕϐ��ɑ��
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();

	//�擾�����m�[�h�����b�V���m�[�h��������
	if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		materialCount = pNode->GetMaterialCount();			//�}�e���A���̐�������
		pMaterial = new D3DMATERIAL9[materialCount];		//�}�e���A������
		pTexture = new LPDIRECT3DTEXTURE9[materialCount];	//�e�N�X�`��������|�C���^���}�e���A���̐������z��ɂ���

		//�}�e���A���̐��������[�v
		for (int i = 0; i < materialCount; i++)
		{
			//�}�e���A���̏��̎擾
			FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)pNode->GetMaterial(i);

			//�|���S���̐F�Ɗ���(�A���r�G���g)�̏����擾
			FbxDouble3  diffuse = pLambert->Diffuse;			//�f�B�t���[�Y(�|���S���̐F)
			FbxDouble3  ambient = pLambert->Ambient;			//�A���r�G���g

			//�}�e���A��������ϐ��̏�����
			ZeroMemory(&pMaterial[i], sizeof(D3DMATERIAL9));

			//�f�B�t���[�Y�̏�����
			pMaterial[i].Diffuse.r = (float)diffuse[0];		//��
			pMaterial[i].Diffuse.g = (float)diffuse[1];		//��
			pMaterial[i].Diffuse.b = (float)diffuse[2];		//��
			pMaterial[i].Diffuse.a = 1.0f;					//�����x

			//�����̏�����
			pMaterial[i].Ambient.r = (float)ambient[0];		//��
			pMaterial[i].Ambient.g = (float)ambient[1];		//��
			pMaterial[i].Ambient.b = (float)ambient[2];		//��
			pMaterial[i].Ambient.a = 1.0f;					//�����x

			//�e�N�X�`���̏����擾
			FbxProperty  lProperty = pNode->GetMaterial(i)->FindProperty(FbxSurfaceMaterial::sDiffuse);		//�ݒ肳�ꂽ�e�N�X�`���̏����擾
			FbxFileTexture* textureFile = lProperty.GetSrcObject<FbxFileTexture>(0);						//���Ŏg��ꂽ�t�@�C���̏����擾(�e�N�X�`���������ꍇ��NULL���Ԃ��Ă���)

			//���̃}�e���A���Ƀe�N�X�`�����\���ĂȂ�������
			if (textureFile == NULL)
			{
				pTexture[i] = NULL;		//�e�N�X�`����\��Ȃ�
			}

			//�e�N�X�`�����\���Ă�����
			else
			{
				//�e�N�X�`���̉摜�t�@�C�����̎擾
				const char* textureFileName = textureFile->GetFileName();		//GetFileName�Ŗ��O���擾�ł���(�A��Maya�őI��������΃p�X�ɂȂ�)

				//�擾�����t�@�C�����͐�΃p�X�Ȃ̂����K�����̃p�X�Ƀt�@�C�������݂��Ă�Ƃ͌���Ȃ�
				//�擾�������ŊԈႢ���K�������@�t�@�C�����Ɗg���q�������o��
				char    name[_MAX_FNAME];		//�t�@�C����������ϐ�
				char    ext[_MAX_EXT];			//�g���q������ϐ�

				//��΃p�X����t�@�C�����Ɗg���q�݂̂��擾����
				_splitpath_s(textureFileName, NULL, 0, NULL, 0, name, _MAX_FNAME, ext, _MAX_EXT);	
				//_sprintpath_s�Ő�΃p�X���h���C�u����t�H���_���A�t�@�C�����A�g���q�ɕ����邱�Ƃ��ł���
				//������(�p�X,�h���C�u��������ϐ�,�h���C�u���̍ۂ���������,�t�H���_��������ϐ�,�t�H���_���̍ő啶����,�t�@�C����������ϐ�,�t�@�C�����̍ő啶����,�g���q,�g���q�̍ő啶����)

				wsprintf(name, "%s%s", name, ext);		//�t�@�C�����Ƀt�@�C�����Ɗg���q���q�����̂�������

				//�e�N�X�`���̍쐬
				if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, name, 0, 0, 0, 0, D3DFMT_UNKNOWN,
					D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &pTexture[i])))
				{
					//�e�N�X�`���̍쐬�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
					MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", name, MB_OK);

					return E_FAIL;		//���s��m�点��
				}

			}

		}

		//���b�V���̒��_�������o���֐����Ă�
		if (FAILED(CheckMesh(pNode->GetMesh())))
		{
			return E_FAIL;		//���s��Ԃ�
		}

	}

	//���b�V���m�[�h�ȊO��������
	else
	{
		int childCount = pNode->GetChildCount();		//���[�g�m�[�h�̎q���̎q���̐��𒲂ׂđ��

		//�q���̐��������[�v
		for (int i = 0; childCount > i; i++)
		{
			//�ċA�Ăяo�����ăm�[�h�̏��𒲂ׂ�
			if (FAILED(CheckNode(pNode->GetChild(i))))
			{
				return E_FAIL;		//���s��Ԃ�
			}
		}
	}

	return S_OK;					//������Ԃ�
}

//�@�\�F���b�V���m�[�h�̒��_�������o��
//�����F���b�V���̃|�C���^
//�ߒl�F���������s��
HRESULT  Fbx::CheckMesh(FbxMesh* pMesh)
{
	//���_�o�b�t�@���쐬����֐��̌Ăяo��
	if (FAILED(CreateVertexBuf(pMesh)))
	{
		return E_FAIL;		//���s��Ԃ�
	}

	//�C���N�f�b�N�X�o�b�t�@���쐬����֐��̌Ăяo��
	if (FAILED(CreateindexBuf(pMesh)))
	{
		return E_FAIL;		//���s��Ԃ�
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F���_�o�b�t�@�̍쐬
//�����F���b�V���̃|�C���^
//�ߒl�F���������s��
HRESULT Fbx::CreateVertexBuf(FbxMesh* pMesh)
{
	//���_����ǂݍ��ނ��߂ɐ擪�̃A�h���X���m��
	FbxVector4* pVertexPos = pMesh->GetControlPoints();

	//�|���S����
	polygonCount = pMesh->GetPolygonCount();		

	//���_������
	vertexCount = pMesh->GetControlPointsCount();

	//���_�̐�����VerTex�\���̂̔z������(�����ɒ��_�o�b�t�@�̏����ꎞ�I�ɓ����)
	Vertex* pVertexList = new Vertex[vertexCount];
	/*����vertex�͓񎟌��z��Ƃ��Ĉ������Ƃ��ł��A
	pVertexPos[0][0]�ɂ͍ŏ��̒��_��X���W�ApVertexPos[0][1]�ɂ͍ŏ��̒��_��Y���W�A
	pVertexPos[0][2]�ɂ͍ŏ��̒��_��Z���W�A������pVertexPos[1][0]�ɂ�2�Ԗڂ̒��_��X���W������B*/

	//���_�̐��������[�v
	for (int i = 0; vertexCount > i; i++)
	{
		//���_��x,y,z���W��������
		pVertexList[i].pos.x = (float)pVertexPos[i][0];		//���_��x���W
		pVertexList[i].pos.y = (float)pVertexPos[i][1];		//���_��y���W
		pVertexList[i].pos.z = (float)pVertexPos[i][2];		//���_��z���W
	}

	//uv�̐��𐔂��鏈��
	FbxStringList uvSetName;			
	pMesh->GetUVSetNames(uvSetName);
	int uvCount = uvSetName.GetCount();		//������uv�̐�������
	bool unMap = false;
	FbxVector2  uv;

	//�|���S���̐��������[�v
	for (int i = 0; i < polygonCount; i++)
	{
		//�ŏ��̒��_�̔ԍ��𒲂ׂĕϐ��ɑ��
		int   startIndex = pMesh->GetPolygonVertexIndex(i);

		//�O�p�`�̒��_�̐��������[�v
		for (int j = 0; j < 3; j++)
		{
			//���_�ԍ��̈ꗗ��ϐ��ɑ��
			int index = pMesh->GetPolygonVertices()[startIndex + j];

			//�@���̏����擾
			FbxVector4  Normal;								//�@���̏�������ϐ�
			pMesh->GetPolygonVertexNormal(i, j, Normal);	//�@���̏����3�����Ɏ擾�ł���֐�(��1�����̓|���S���̔ԍ��A���_�̔ԍ�)

			//���_���ɖ@������ǉ�
			pVertexList[index].normal = D3DXVECTOR3((float)Normal[0], (float)Normal[1], (float)Normal[2]);

			//uv�̐��������[�v
			for (int k = 0; k < uvCount; k++)
			{
				//uv���̎擾
				FbxString name = uvSetName.GetStringAt(k);
				pMesh->GetPolygonVertexUV(i, j, name, uv, unMap);
				pVertexList[index].uv = D3DXVECTOR2((float)uv.mData[0], (float)(1.0 - uv.mData[1]));		//uv����ϐ��ɑ��(y���͔��]����Ă���̂�-1�Ō��ɖ߂��Ă���)
			}
		}
	}

	//��̒��_�o�b�t�@�̍쐬
	//���_�o�b�t�@����鎞��CreateVertexBuffer(�f�[�^�̃T�C�Y(1���_�̃T�C�Y�~���_��),0,���_�����̏��������Ă��邩,,��������_�o�b�t�@�̃A�h���X,0)
	if (FAILED(g_pDevice->CreateVertexBuffer(sizeof(Vertex)* vertexCount, 0, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, D3DPOOL_MANAGED, &pVertexBuffer, 0)))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "Fbx", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//���_���̍\���̂̃|�C���^�쐬
	Vertex *pVertexCopy;

	//���_�o�b�t�@�̃��b�N(����������������ł��Ȃ�����)
	if (FAILED(pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0)))
	{
		//���b�N�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", "Fbx", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//pVertexCopy��pVertexList�̓��e���R�s�[
	memcpy(pVertexCopy, pVertexList, sizeof(Vertex)*vertexCount);

	//���_�o�b�t�@�̃A�����b�N(���̐l������\�ɂȂ�)
	if (FAILED(pVertexBuffer->Unlock()))
	{
		//�A�����b�N�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���_�o�b�t�@�̃A�����b�N�Ɏ��s���܂���", "Fbx", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//pVertexList�̉��
	SAFE_DELETE_ARRAY(pVertexList);

	return S_OK;			//������Ԃ�
}

//�@�\�F�C���f�b�N�X�o�b�t�@�̍쐬
//�����F���b�V���̃|�C���^
//�ߒl�F���������s��
HRESULT Fbx::CreateindexBuf(FbxMesh* pMesh)
{
	//�|���S�����ƃC���f�b�N�X���̎擾
	polygonCount = pMesh->GetPolygonCount();		//�|���S����
	indexCount = pMesh->GetPolygonVertexCount();	//�C���f�b�N�X��

	//�C���f�b�N�X�o�b�t�@���}�e���A���̐������������悤�ɔz��
	pIndexBuffer = new IDirect3DIndexBuffer9*[materialCount];

	//�}�e���A���̐������z���
	polygonCountOfMaterial = new int[materialCount];

	for (int i = 0; i < materialCount; i++)
	{
		//�ꎞ�I�ɃC���f�b�N�X��������ϐ�(�z��)
		int* pIndexList = new int[indexCount];

		//pIndexList�̗v�f����
		int count = 0;			//�ŏ���0

		//�|���S���̐��������[�v
		for (int polygon = 0; polygon < polygonCount; polygon++)
		{
			//�����Ă���|���S�������Ԗڂ̃}�e���A�����𒲂ׂĕϐ��ɑ��
			int materialID = pMesh->GetLayer(0)->GetMaterials()->GetIndexArray().GetAt(polygon);

			//�}�e���A���̔ԍ��Ɨv�f����������
			if (materialID == i)
			{
				//�X��Vertex��3�����̊ԃ��[�v(Vertex�ɒ��_��񂪓����Ă��č���̓|���S�����S�ĎO�p�`�Ȃ̂�0,1,2�̗v�f�ŏ\��)
				for (int vertex = 0; vertex < 3; vertex++)
				{
					//�z��ɃC���f�b�N�X��������
					pIndexList[count++] = pMesh->GetPolygonVertex(polygon, vertex);
				}
			}
		}

		//���_�̐�����R�����邱�ƂŃ|���S���������߂���
		polygonCountOfMaterial[i] = count / 3;

		//��̃C���f�b�N�X�o�b�t�@�쐬
		if (FAILED(g_pDevice->CreateIndexBuffer(sizeof(int)* indexCount, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &pIndexBuffer[i], 0)))
		{
			//�쐬�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "Fbx", MB_OK);

			return E_FAIL;		//���s��Ԃ�
		}

		//�C���f�b�N�X��������ϐ�
		DWORD *pIndexCopy;

		//�C���f�b�N�X�o�b�t�@�̃��b�N
		if (FAILED(pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0)))
		{
			//���b�N�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "�C���f�b�N�X�o�b�t�@�̃��b�N�Ɏ��s���܂���", "Fbx", MB_OK);

			return E_FAIL;		//���s��Ԃ�
		}

		//�C���f�b�N�X���̃R�s�[
		memcpy(pIndexCopy, pIndexList, sizeof(int)* indexCount);

		//�C���f�b�N�X�o�b�t�@�̃A�����b�N
		if (FAILED(pIndexBuffer[i]->Unlock()))
		{
			//�A�����b�N�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "�C���f�b�N�X�o�b�t�@�̃A�����b�N�Ɏ��s���܂���", "Fbx", MB_OK);

			return E_FAIL;		//���s��Ԃ�
		}

		//pIndexList�̉��
		SAFE_DELETE_ARRAY(pIndexList);
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�`�揈��
//�����F���[���h�s��
//�ߒl�F���������s��
HRESULT Fbx::Draw(D3DXMATRIX* matWorld)
{
	//���[���h�s��̃Z�b�g
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���[���h�s��̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}


	//���_�X�g���[���̐ݒ�
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)))
	{
		//�ݒ�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���_�X�g���[���̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�g�����_�o�b�t�@�̎w��
	if (FAILED(g_pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(Vertex))))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���_�o�b�t�@�̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�g���C���f�b�N�X�o�b�t�@�̎w��
	for (int i = 0; i < materialCount; i++)
	{
		if (FAILED(g_pDevice->SetIndices(pIndexBuffer[i])))
		{
			//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "�C���f�b�N�X�o�b�t�@�̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);

			return E_FAIL;		//���s��Ԃ�
		}

		//�}�e���A���̐ݒ�
		if (FAILED(g_pDevice->SetMaterial(&pMaterial[i])))
		{
			//�ݒ�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "�}�e���A���̐ݒ�Ɏ��s���܂���", "Fbx", MB_OK);

			return E_FAIL;		//���s��Ԃ�
		}

		//�e�N�X�`���̓\��t��
		if (FAILED(g_pDevice->SetTexture(0, pTexture[i])))
		{
			//�e�N�X�`���̓\��t���Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "�e�N�X�`���̃Z�b�g�Ɏ��s���܂���", "Fbx", MB_OK);

			return E_FAIL;		//���s��Ԃ�
		}

		//���_�E�C���f�b�N�X�o�b�t�@���g���ă��b�V���̕`��
		if (FAILED(g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, indexCount, 0, polygonCountOfMaterial[i])))
		{
			//�`��Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "���b�V���̕`��Ɏ��s���܂���", "Fbx", MB_OK);

			return E_FAIL;	//���s��Ԃ�
		}
	}

	lastWorldMatrix = *matWorld;		//�Ō�Ɏg�������[���h�s�����

	return S_OK;			//������Ԃ�
}

//�@�\�F���C�L���X�g����
//�����F���C�L���X�g�ɕK�v�ȃf�[�^���܂Ƃ߂��\����
//�ߒl�F�^���U
BOOL Fbx::RayCast(RayCastData *data)
{
	//�\���̂̕ϐ��̏�����
	data->dist = 99999.0f;		//�Փ˓_�܂ł̋���
	data->hit = FALSE;			//�����������������ĂȂ����̔���

	//���_�o�b�t�@�ɃA�N�Z�X�ł���悤��
	Vertex *pVertexCopy;									//���_�����܂Ƃ߂��\���̂̍쐬
	pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0);		//���_�o�b�t�@�����b�N���đ��̊֐��ŏ����ł��Ȃ�����


	//�}�e���A�������[�v
	for (int i = 0; i < materialCount; i++)
	{
		//�C���f�b�N�X�o�b�t�@�ɃA�N�Z�X�ł���悤��
		DWORD *pIndexCopy;
		pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0);

		//���̃}�e���A���̃|���S����
		for (int j = 0; j < polygonCountOfMaterial[i]; j++)
		{
			//3���_��
			D3DXVECTOR3 ver[3];
			ver[0] = pVertexCopy[pIndexCopy[j * 3 + 0]].pos;					//�v�f0��
			ver[1] = pVertexCopy[pIndexCopy[j * 3 + 1]].pos;					//�v�f1��
			ver[2] = pVertexCopy[pIndexCopy[j * 3 + 2]].pos;					//�v�f2��

			//�`�悷��ۂɏ�񂪕ύX����Ă����炻��ɍ��킹�Ă��̍s����|����
			D3DXVec3TransformCoord(&ver[0], &ver[0], &data->matrix);			//�v�f0��
			D3DXVec3TransformCoord(&ver[1], &ver[1], &data->matrix);			//�v�f1��
			D3DXVec3TransformCoord(&ver[2], &ver[2], &data->matrix);			//�v�f2��


			//�Ԃ����Ă邩�`�F�b�N
			float dist, u, v;								//D3DXIntersectTri�֐��ɕK�v�ȕϐ��p��
			D3DXVec3Normalize(&data->dir, &data->dir);		//���C�̕����𐳋K��
			BOOL hit = D3DXIntersectTri(&ver[0], &ver[1], &ver[2], &data->start, &data->dir, &u, &v, &dist);		//�}�e���A���ɏՓ˂��Ă�����true���Ԃ�

			//���܂ň�ԋ߂��H
			if (hit && dist < data->dist)
			{
				data->hit = TRUE;		//�Փ˂���
				data->dist = dist;		//�Փ˂�������������

				//�O�ς����߂�Ƃ��Ɏg���ϐ�����
				D3DXVECTOR3 v1 = ver[2] - ver[0];			//ver[2] - ver[0]�̃x�N�g������
				D3DXVECTOR3 v2 = ver[2] - ver[1];			//ver[1] - ver[2]�̃x�N�g������

				//�ϐ�v1��v2�̊O�ς����߂ă��C�L���X�g�\���̂�normal�ɑ��
				D3DXVec3Cross(&data->normal, &v1, &v2);

				//�O�ς����߂��l���傫���Ȃ�̂ł���Ō덷���N���邩������Ȃ��̂Ő��K��
				D3DXVec3Normalize(&data->normal, &data->normal);

				data->pos = ver[0] + u * (ver[1] - ver[0]) + v * (ver[2] - ver[0]);		//�ʒu�����󂯎��
			}
		}

		//�C���f�b�N�X�o�b�t�@�g�p�I��
		pIndexBuffer[i]->Unlock();
	}

	//���_�o�b�t�@�g�p�I��
	pVertexBuffer->Unlock();

	
	return data->hit;			//�Փ˂�������Ԃ�
}

BOOL Fbx::HitRay(D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist, D3DXMATRIX matW)
{
	return HitRay(pRayPos, pRayDir, pDist, NULL,matW);
}

BOOL Fbx::HitRay(D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist, D3DXVECTOR3* normal, D3DXMATRIX matW)
{
	*pDist = 9999999;

	//���_�o�b�t�@�ɃA�N�Z�X�ł���悤��
	Vertex *pVertexCopy;									//���_�����܂Ƃ߂��\���̂̍쐬
	pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0);		//���_�o�b�t�@�����b�N���đ��̊֐��ŏ����ł��Ȃ�����

	//�}�e���A�������[�v
	for (int i = 0; i < materialCount; i++)
	{
		//�C���f�b�N�X�o�b�t�@�ɃA�N�Z�X�ł���悤��
		DWORD *pIndexCopy;
		pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0);

		//���̃}�e���A���̃|���S����
		for (int j = 0; j < polygonCountOfMaterial[i]; j++)
		{
			//3���_��
			D3DXVECTOR3 ver[3];
			ver[0] = pVertexCopy[pIndexCopy[j * 3 + 0]].pos;					//�v�f0��
			ver[1] = pVertexCopy[pIndexCopy[j * 3 + 1]].pos;					//�v�f1��
			ver[2] = pVertexCopy[pIndexCopy[j * 3 + 2]].pos;					//�v�f2��

			//x���𔽓]������
			ver[0].x *= -1.0f;
			ver[1].x *= -1.0f;
			ver[2].x *= -1.0f;

			//�`��ʒu�ɍ��킹��
			D3DXVec3TransformCoord(&ver[0], &ver[0], &matW);			//�v�f0��
			D3DXVec3TransformCoord(&ver[1], &ver[1], &matW);			//�v�f1��
			D3DXVec3TransformCoord(&ver[2], &ver[2], &matW);			//�v�f2��



			//�����̐��K��
			D3DXVec3Normalize(pRayDir, pRayDir);

			//�}�E�X�̃��C�ƃ��f���Ƃ̏Փ˔���
			float dist;
			BOOL hit = D3DXIntersectTri(&ver[0], &ver[1], &ver[2], pRayPos, pRayDir, NULL, NULL, &dist);

			//�Փ˂�����
			if (hit)
			{
				//�����������̋��������Z���Ƃ�(pDist�ɂ͏����l�Ƃ���999999�������Ă���̂ŏՓ˂��Ă���TRUE�ɂȂ�͂�)
				if (dist < *pDist)
				{
					//��������
					*pDist = dist;
				}

			}

		}


		//�C���f�b�N�X�o�b�t�@�g�p�I��
		pIndexBuffer[i]->Unlock();

	}

	//���_�o�b�t�@�g�p�I��
	pVertexBuffer->Unlock();

	//pDist�ɏ����l�ȊO�̒l�������Ă�����TRUE�ɂ���
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
// �q�K�w�I�u�W�F�N�g�ɑ΂��Ă��`�揈�������s
//for (DWORD i = 0; i < pMesh->GetNumChild(); i++)
//{
//	HitRay(pMesh->GetChild()[i], pRayPos, pRayDir, pDist, normal);
//}
//	return FALSE;
//}


//�@�\�F�}�E�X���W��Fbx�Ƃ̏Փ˔���
//�����F���[���h���W
//�ߒl�F�Փ˂���(TRUE)���Փ˂��ĂȂ�(FALSE)�̔���
BOOL Fbx::HitMouseRay(D3DXMATRIX matW)
{
	//�r���[�|�[�g�s��(�����_�����O���ʂ��E�B���h�E�T�C�Y�ɍ��킹�邽�߂̍s��)���쐬
	D3DXMATRIX VP;				//�s�񏀔�
	D3DXMatrixIdentity(&VP);	//������

	//�r���[�|�[�g�s��ɒl����
	VP._11 = (float)WINDOW_WIDTH / 2.0f;
	VP._22 = (float)-WINDOW_HEIGHT / 2.0f;
	VP._41 = (float)WINDOW_WIDTH / 2.0f;
	VP._42 = (float)WINDOW_HEIGHT / 2.0f;

	//�r���[�s��̋t�s��
	D3DXMATRIX InvView;												//�s�񏀔�
	D3DXMatrixInverse(&InvView, NULL, g_pCamera->GetView());		//�J�����N���X����r���[�s����󂯎���ċt�s���

	//�ˉe�s��̋t�s��
	D3DXMATRIX InvPrj;												//�s�񏀔�
	D3DXMatrixInverse(&InvPrj, NULL, g_pCamera->GetProj());			//�J�����N���X����ˉe�s����󂯎���ċt�s���

	//�r���[�|�[�g�s��
	D3DXMATRIX InvViewport;											//�s�񏀔�
	D3DXMatrixInverse(&InvViewport, NULL, &VP);						//���̊֐��̍ŏ��ő�������l�����Ƃɋt�s���

	// �e�t�s�������
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;

	//���f���̃��[���h�s��̋t�s��
	D3DXMATRIX InvWorld;											//�s�񏀔�
	D3DXMatrixInverse(&InvWorld, NULL, &matW);						//�����̃��[���h�s����t�s���

	//�}�E�X�̈ʒu�̎�O���牜�Ƀ��C���΂�

	//��O�̓_�i�}�E�X�ʒu�ły��0�j
	D3DXVECTOR3	mouseFront((float)g_pInput->GetMousePos().x, (float)g_pInput->GetMousePos().y, 0);
	D3DXVec3TransformCoord(&mouseFront, &mouseFront, &tmp);
	//D3DXVec3TransformCoord(&mouseFront, &mouseFront, &InvWorld);

	//���̓_�i�}�E�X�ʒu�ły��1�j
	D3DXVECTOR3	mouseBack((float)g_pInput->GetMousePos().x, (float)g_pInput->GetMousePos().y , 1);
	D3DXVec3TransformCoord(&mouseBack, &mouseBack, &tmp);
	//D3DXVec3TransformCoord(&mouseBack, &mouseBack, &InvWorld);


	//���C�̕����i��O�̓_���牜�̓_�Ɍ������x�N�g���j
	D3DXVECTOR3 rayDir = mouseBack - mouseFront;

	//���C���΂�
	float dist;
	BOOL isHit = HitRay(&mouseFront, &rayDir, &dist, matW);

	return isHit;		//�Փ˔����Ԃ�
}