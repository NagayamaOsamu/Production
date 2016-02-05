/*===========================================================================================
�T�@���F�X�e�[�W�G�f�B�^�[�N���X�̊֐���`
�쐬���F1��12��
�X�V���F1��12��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "StageEditor.h"
#include "Camera.h"			//�J�����N���X

//�R���X�g���N�^
StageEditor::StageEditor()
{
	//�e�p�[�c�̏o���̈ʒu��ݒ肵�Ă����i���̕ӂ͊O���t�@�C������ǂݍ��߂�ƃJ�b�R�C�C�j
	ExitState exit;

	//�X�^�[�g�̐ݒ�
	exit.pos = D3DXVECTOR3(0, 0, 24);		//�ʒu
	exit.angle = 0;							//�p�x
	blocksState[BLOCK_START].exitList.push_back(exit);		//����z��Ɋi�[

	//�����̐ݒ�
	exit.pos = D3DXVECTOR3(0, 0, 24);		//�ʒu
	exit.angle = 0;							//�p�x
	blocksState[BLOCK_STRAIGHT].exitList.push_back(exit);	//����z��Ɋi�[

	//T���H(�o����2����̂�2���)

	exit.pos = D3DXVECTOR3(24, 0, 6);		//�ʒu
	exit.angle = 90;						//�p�x
	blocksState[BLOCK_BRANCH].exitList.push_back(exit);	//����z��Ɋi�[

	exit.pos = D3DXVECTOR3(-24, 0, 6);		//�ʒu
	exit.angle = 180;						//�p�x
	blocksState[BLOCK_BRANCH].exitList.push_back(exit);	//����z��Ɋi�[

	//����
	exit.pos = D3DXVECTOR3(0, 8, 22);		//�ʒu
	exit.angle = 0;							//�p�x
	blocksState[BLOCK_UP].exitList.push_back(exit);	//����z��Ɋi�[

	//�����
	exit.pos = D3DXVECTOR3(0, -8, 24);		//�ʒu
	exit.angle = 0;							//�p�x
	blocksState[BLOCK_DOWN].exitList.push_back(exit);	//����z��Ɋi�[

	//�s���~�܂�
	//�ݒ肷�邱�ƂȂ�



	//�N���X�̏�����
	chipWindow = NULL;							//�`�b�v�E�B���h�E
	frame = NULL;							//�g

	//�����o�ϐ��̏�����
	stagePartsList.push_back(BLOCK_START);		//�ŏ��̃u���b�N�̓X�^�[�g
	cameraPos		= D3DXVECTOR3(-10, 100,-1);	//�J�����̈ʒu
	cameraTarget	= D3DXVECTOR3(-10,0,0);		//�J�����̊p�x
	selectChip		= BLOCK_STRAIGHT;			//�ŏ��͒����̃`�b�v
	mouseTapPos.x	= 0;						//�}�E�X�N���b�N����x�ʒu
	mouseTapPos.y	= 0;						//�}�E�X�N���b�N����y�ʒu
	mouseDownPos.x	= 0;						//�}�E�X��������Ă�Ԃ�x�ʒu
	mouseDownPos.y  = 0;						//�}�E�X��������Ă�Ԃ�y�ʒu
	branchPos		= D3DXVECTOR3(0, 0, 0);		//T���H�̈ʒu
	branchAngle		= 0;						//T���H�̊p�x
	isChipWin		= FALSE;					//�}�E�X�J�[�\�����`�b�v�E�B���h�E�ɂ��邩����
}

//�f�X�g���N�^
StageEditor::~StageEditor()
{
	//�N���X�̉��
	SAFE_DELETE(chipWindow);		//�`�b�v�E�B���h�E
	SAFE_DELETE(frame);				//�g

}

//�@�\�F�ǂݍ��ݏ���
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT StageEditor::Load()
{
	//�N���X�̓��I�쐬
	chipWindow = new Sprite;		//�`�b�v�E�B���h�E
	frame	   = new Sprite;		//�g

	//----------------------�X�e�[�W�u���b�N�̓ǂݍ���--------------------------------

	//�X�e�[�W�u���b�N�̓ǂݍ���
	if (FAILED(blocksState[BLOCK_START].stage.Load("Asset\\Stage\\Start.fbx")))		//�X�^�[�g�u���b�N
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�X�^�[�g�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	if (FAILED(blocksState[BLOCK_STRAIGHT].stage.Load("Asset\\Stage\\Staight.fbx")))		//�����u���b�N
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�����̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	if (FAILED(blocksState[BLOCK_BRANCH].stage.Load("Asset\\Stage\\Branch.fbx")))		//T���H�u���b�N
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "T���H�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	if (FAILED(blocksState[BLOCK_UP].stage.Load("Asset\\Stage\\Up.fbx")))		//����u���b�N
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "����̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	if (FAILED(blocksState[BLOCK_DOWN].stage.Load("Asset\\Stage\\Down.fbx")))		//�����u���b�N
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�����̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}


	if (FAILED(blocksState[BLOCK_GOAL].stage.Load("Asset\\Stage\\Goal.fbx")))		//�S�[��		
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�S�[���̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	if (FAILED(blocksState[BLOCK_STOP].stage.Load("Asset\\Stage\\Stop.fbx")))		//�s���~�܂�
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�s���~�܂�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//----------------------�X�v���C�g�̓ǂݍ���----------------------------

	//�`�b�v�E�B���h�E�摜�̓ǂݍ���
	if (FAILED(chipWindow->Load("Asset\\Sprite\\ChipWindow.png")))
	{
		//���s�����烁�b�Z�[�W�E�B���h�E�Œm�点��
		MessageBox(0, "�`�b�v�E�B���h�E�摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}

	//�g�̉摜�̓ǂݍ���
	if (FAILED(frame->Load("Asset\\Sprite\\Frame.png")))
	{
		//���s�����烁�b�Z�[�W�E�B���h�E�Œm�点��
		MessageBox(0, "�g�̉摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}


	return S_OK;				//������Ԃ�
}

//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT StageEditor::Update()
{
	//�J�����̈ʒu�Ɗp�x��ύX
	g_pCamera->SetPos(cameraPos);				//�ʒu
	g_pCamera->SetTarget(cameraTarget);

	//�}�E�X�ō��N���b�N���ꂽ�Ƃ�
	if (g_pInput->IsMouseTap(0))
	{
		//�N���b�N���ꂽ�ʒu�̎擾
		mouseTapPos = g_pInput->GetMousePos();
		
		//�}�E�X�̈ʒu���`�b�v�E�B���h�E�̒����̈ʒu�ɂ���Ƃ�
		if ( (  mouseTapPos.x >= 0  &&  mouseTapPos.x <= 250  ) && ( mouseTapPos.y >= 0 && mouseTapPos.y <= 250 ) )
		{
			selectChip = BLOCK_STRAIGHT;		//�����`�b�v�̑I��
			isChipWin = TRUE;					//�}�E�X�J�[�\���̈ʒu�t���O��TRUE��
			
		}

		//T���H�̂Ƃ�
		else if ( ( mouseTapPos.x > 250 && mouseTapPos.x <= 500 ) && (mouseTapPos.y >= 0 && mouseTapPos.y <= 250 ) )
		{
			selectChip = BLOCK_BRANCH;			//T���H�`�b�v�̑I��
			isChipWin = TRUE;					//�}�E�X�J�[�\���̈ʒu�t���O��TRUE��
		}

		//����̂Ƃ�
		else if ( ( mouseTapPos.x >= 0 && mouseTapPos.x <= 250 ) && ( mouseTapPos.y > 250 && mouseTapPos.y <= 500 ) )
		{
			selectChip = BLOCK_UP;				//����`�b�v�̑I��
			isChipWin = TRUE;					//�}�E�X�J�[�\���̈ʒu�t���O��TRUE��
		}

		//�����̂Ƃ�
		else if ( ( mouseTapPos.x > 250 && mouseTapPos.x <= 500) && (mouseTapPos.y > 250 && mouseTapPos.y <= 500 ) )
		{
			selectChip = BLOCK_DOWN;			//�����`�b�v�̑I��
			isChipWin = TRUE;					//�}�E�X�J�[�\���̈ʒu�t���O��TRUE��
		}

		//�S�[���̂Ƃ�
		else if ( ( mouseTapPos.x >= 0 && mouseTapPos.x <= 250 ) && ( mouseTapPos.y > 500 && mouseTapPos.y <= 750 ) )
		{
			selectChip = BLOCK_GOAL;			//�S�[���`�b�v�̑I��
			isChipWin = TRUE;					//�}�E�X�J�[�\���̈ʒu�t���O��TRUE��
		}

		//�X�g�b�v�̂Ƃ�
		else if ( ( mouseTapPos.x > 250 && mouseTapPos.x <= 500 ) && ( mouseTapPos.y > 500 && mouseTapPos.y <= 750 ) )
		{
			selectChip = BLOCK_STOP;		//�X�g�b�v�̑I��
			isChipWin  = TRUE;				//�}�E�X�J�[�\���̈ʒu�t���O��TRUE��
		}

		//�`�b�v�E�B���h�E�łȂ��Ƃ�
		else
		{
			isChipWin = FALSE;			//�}�E�X�J�[�\���̈ʒu�t���O��FALSE��
		}


		//�}�E�X�J�[�\���̈ʒu�t���O��TRUE�̂Ƃ�
		if (isChipWin)
		{
			stagePartsList.push_back(selectChip);		//�I�΂�Ă���`�b�v�����X�g�ɒǉ�
		}
	}

	//�`�b�v�E�B���h�E�O�ō��N���b�N���ꑱ���Ă����
	else if ( ( !isChipWin ) && ( g_pInput->IsMousePush(0) ) )
	{
		//�}�E�X�̈ʒu���擾
		mouseDownPos = g_pInput->GetMousePos();

		//�}�E�X�^�b�v���ꂽ�ʒu����ǂꂾ���h���b�N���ꂽ�������߂�
		float mouseMoveX = (float)(mouseDownPos.x - mouseTapPos.x) / 100;		//x����
		float mouseMoveZ = (float)(mouseDownPos.y - mouseTapPos.y) / 100;		//y����

		//------------�J�����̈ړ����}�E�X���h���b�O���ꂽ�������ړ�������悤�ɂ���--------------

		//�^�b�v�����ʒu���獶�Ƀ}�E�X������Ƃ�
		if (mouseTapPos.x > mouseDownPos.x)
		{
			//�J���������������ړ�
			cameraPos.x += mouseMoveX;
			cameraTarget.x += mouseMoveX;
		}

		//�E�̂Ƃ�
		else if (mouseTapPos.x < mouseDownPos.x)
		{
			//�J�������E�����Ɉړ�
			cameraPos.x += mouseMoveX;
			cameraTarget.x += mouseMoveX;
		}

		//��̂Ƃ�
		 if (mouseTapPos.y < mouseDownPos.y)
		{
			//�J������������Ɉړ�
			 cameraPos.z    += mouseMoveZ;
			 cameraTarget.z += mouseMoveZ;
		}

		//���̂Ƃ�
		else if (mouseTapPos.y > mouseDownPos.y)
		{
			//�J�������������Ɉړ�
			cameraPos.z += mouseMoveZ;
			cameraTarget.z += mouseMoveZ;
		}
	}

	//�}�E�X�̍��N���b�N�����ꂽ��
	else if (g_pInput->IsMouseRelease(0))
	{
		mouseDownPos = mouseTapPos;
	}

	//�E�N���b�N���ꂽ�Ƃ�
	if (g_pInput->IsMouseTap(1))
	{
		//�s��p�̃C�e���[�^�ɏ����l����
		matItr = blocksMatList.begin();

		//�X�e�[�W�̃p�[�c�����������[�v
		for (itr = stagePartsList.begin(); itr != stagePartsList.end();itr++)
		{
			//�N���b�N�����ʒu�ƃX�e�[�W�u���b�N�̏Փ˔���
			BOOL isHit;
			isHit = blocksState[*itr].stage.HitMouseRay(*matItr);

			//�X�^�[�g�ʒu�ȊO�ƏՓ˂����Ƃ�
			if (isHit && (*itr != BLOCK_START))
			{
				//�X�e�[�W�u���b�N�ƍs����폜
				stagePartsList.erase(itr);		//�X�e�[�W�u���b�N
				blocksMatList.erase(matItr);	//�s��

				break;
			}

			//�s��p���X�g���Ō�ȊO�̂Ƃ�
			if (matItr != blocksMatList.end())
				matItr++;						//���̃C�e���[�^��
		}
	}

	//S�L�[�������ꂽ��
	if (g_pInput->IsKeyTap(DIK_S))
	{
		//stagePartsList��ۑ�
		char szFile[MAX_PATH] = "stage.stage";		//�t�@�C����������ϐ��i�����̖��O������Ă����j

		//�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
		OPENFILENAME ofn;		//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
		ZeroMemory(&ofn, sizeof(ofn));									//�\���̂̏�����
		ofn.lStructSize = sizeof(OPENFILENAME);							//�\���̂̃T�C�Y
		ofn.lpstrFilter = TEXT("�X�e�[�W�f�[�^(*.stage)\0*.stage\0")	//_____	�t�@�C���̎��
						  TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0\0");			//_____|_
		ofn.lpstrFile	= szFile;											//�t�@�C����
		ofn.nMaxFile	= MAX_PATH;										//�p�X�̍ő啶����
		ofn.Flags		=  OFN_OVERWRITEPROMPT;								//�t���O�i�����t�@�C�������݂�����㏑���ۑ�����悤�ɂ���j
		ofn.lpstrDefExt = "stage";										//�f�t�H���g�g���q

		//�u�t�@�C����ۑ��v�_�C�A���O��\��
		BOOL selFile;		//�_�C�A���O�̂ǂ��̃{�^���������ꂽ�����f����ϐ�

		selFile = GetSaveFileName(&ofn);		//�_�C�A���O�\��

		//�L�����Z�������璆�f
		if (selFile == FALSE)	
			return S_OK;

		//�t�@�C���쐬
		HANDLE hFile;				//�t�@�C���̃n���h��
		hFile = CreateFile(
			szFile,					//�t�@�C����
			GENERIC_WRITE,			//�A�N�Z�X���[�h�i�������ݗp)
			0,						//���L�i�Ȃ��j
			NULL,					//�Z�L�����e�B�����i�p�����Ȃ��j
			CREATE_ALWAYS,			//�쐬���@�i�ݒ�Ȃ��j
			FILE_ATTRIBUTE_NORMAL,	//�����ƃt���O�i�ݒ�Ȃ��j
			NULL					//�g�������i�Ȃ��j
			);

		//stagePartsList�̃T�C�Y�𕶎���Ƃ��Ċi�[����
		char buf[256];										//�����������z��
		wsprintf(buf, "%d,", stagePartsList.size());	//������ɕϊ�

		//���ƍ������t�@�C���ɏ�������
		DWORD dwBytes = 0;	//�������݈ʒu
		WriteFile(
			hFile,				//�t�@�C���n���h��
			buf,				//�ۑ�����f�[�^
			(DWORD)strlen(buf),	//�������ރT�C�Y
			&dwBytes,			//�������񂾃T�C�Y������ϐ�
			NULL
			);

		//stagePartsList�̐��������[�v
		for (itr = stagePartsList.begin(); itr != stagePartsList.end();itr++)
		{
			//�e�X�e�[�W�u���b�N���u,�v�ŋ�؂��ď�������
			wsprintf(buf, "%d,", *itr);
			WriteFile(hFile, buf, (DWORD)strlen(buf), &dwBytes, NULL);
		}

		//�t�@�C�������
		CloseHandle(hFile);
	}

	//�I�΂�Ă���`�b�v���Ƃ̏���
	switch (selectChip)
	{
		//�����̃`�b�v�̂Ƃ�
	case BLOCK_STRAIGHT:

		//�\���E�L�[�����L�[�������ꂽ�Ƃ�
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_BRANCH;		//T���H�̃`�b�v��I�����Ă����Ԃɂ���
		}

		//�\�����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_UP;		//����`�b�v��I�����Ă����Ԃɂ���
		}

		//�\����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_GOAL;		//�S�[���̃`�b�v��I�����Ă����Ԃɂ���
		}

		break;

		//T���H�̃`�b�v�̂Ƃ�
	case BLOCK_BRANCH:

		//�\���E�L�[�����L�[�������ꂽ�Ƃ�
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_STRAIGHT;		//�����̃`�b�v��I�����Ă����Ԃɂ���
		}

		//�\�����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_DOWN;			//�����`�b�v��I�����Ă����Ԃɂ���
		}

		break;

		//����̃`�b�v�̂Ƃ�
	case BLOCK_UP:

		//�\���E�L�[�����L�[�������ꂽ�Ƃ�
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_DOWN;			//�����̃`�b�v��I�����Ă����Ԃɂ���
		}

		//�\�����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_GOAL;			//����`�b�v��I�����Ă����Ԃɂ���
		}

		//�\����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_STRAIGHT;		//�����̃`�b�v��I�����Ă����Ԃɂ���
		}

		break;

		//�����̃`�b�v�̂Ƃ�
	case BLOCK_DOWN:

		//�\���E�L�[�����L�[�������ꂽ�Ƃ�
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_UP;				//����̃`�b�v��I�����Ă����Ԃɂ���
		}

		//�\�����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_STOP;			//�s���~�܂��I�����Ă����Ԃɂ���
		}

		//�\����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_BRANCH;			//T���H�̃`�b�v��I�����Ă����Ԃɂ���
		}

		break;

		//�S�[���̃`�b�v�̂Ƃ�
	case BLOCK_GOAL:

		//�\���E�L�[�����L�[�������ꂽ�Ƃ�
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_STOP;			//�s���~�܂��I�����Ă����Ԃɂ���
		}

		//�\�����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_STRAIGHT;			//�����̃`�b�v��I�����Ă����Ԃɂ���
		}

		//�\����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_UP;				//����̃`�b�v��I�����Ă����Ԃɂ���
		}


		break;

		//�s���~�܂�̂Ƃ�
	case BLOCK_STOP:

		//�\���E�L�[�����L�[�������ꂽ�Ƃ�
		if (g_pInput->IsKeyTap(DIK_RIGHT) || g_pInput->IsKeyTap(DIK_LEFT))
		{
			selectChip = BLOCK_GOAL;			//�����̃`�b�v��I�����Ă����Ԃɂ���
		}

		//�\�����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_DOWN))
		{
			selectChip = BLOCK_BRANCH;			//�����̃`�b�v��I�����Ă����Ԃɂ���
		}

		//�\����L�[�������ꂽ�Ƃ�
		else if (g_pInput->IsKeyTap(DIK_UP))
		{
			selectChip = BLOCK_BRANCH;				//����̃`�b�v��I�����Ă����Ԃɂ���
		}


		break;

	}

	//Enter�L�[�������ꂽ��
	if (g_pInput->IsKeyTap(DIK_RETURN))
	{
		//�I�΂�Ă���`�b�v�����X�g�ɒǉ�
		stagePartsList.push_back(selectChip);


		//�J�������ړ�
		cameraPos.z += 10;
		cameraTarget.z += 10;
	}

	return S_OK;				//������Ԃ�
}

//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT StageEditor::Render()
{
	//--------------------�X�v���C�g�̕`��-------------------

	//�X�v���C�g�̏����܂Ƃ߂��\���̂̏���
	SpriteData data;

	//�`�b�v�E�B���h�E�̕`��
	chipWindow->Draw(&data);		//���͕ς����ɕ`��

	//�I��ł���`�b�v���Ƃɘg�̕`��
	switch (selectChip)
	{
		//�����̃`�b�v�̂Ƃ�
	case BLOCK_STRAIGHT:
		
		data.pos = D3DXVECTOR3(-5, -4, 0);	//�g�̈ʒu���`�b�v�E�B���h�E�̍����

		break;

		//T���H�̃`�b�v�̂Ƃ�
	case BLOCK_BRANCH:

		data.pos = D3DXVECTOR3(270, -4, 0);	//�g�̈ʒu���`�b�v�E�B���h�E�̉E���

		break;

		//����̃`�b�v�̂Ƃ�
	case BLOCK_UP:

		data.pos = D3DXVECTOR3(-5, 250, 0);	//�g�̈ʒu���`�b�v�E�B���h�E�̍��^�񒆂�

		break;

		//�����̃`�b�v�̂Ƃ�
	case BLOCK_DOWN:

		data.pos = D3DXVECTOR3(270, 250, 0);	//�g�̈ʒu���`�b�v�E�B���h�E�̉E�^�񒆂�

		break;

		//�S�[���̃`�b�v�̂Ƃ�
	case BLOCK_GOAL:

		data.pos = D3DXVECTOR3(-5, 500, 0);	//�g�̈ʒu���`�b�v�E�B���h�E�̍�����

		break;

	case BLOCK_STOP:

		data.pos = D3DXVECTOR3(270, 500, 0);

		break;
	}

	frame->Draw(&data);						//�`��

	//-------------------�X�e�[�W�̕`��----------------------

	////�ʒu�Ɗp�x�̏��̏�����
	D3DXVECTOR3 blockPos = D3DXVECTOR3(0, 0, 0);			//�X�e�[�W�u���b�N�̈ʒu
	float		blockAngle = 0;								//�X�e�[�W�u���b�N�̊p�x

	//�s��̏���
	D3DXMATRIX matW, matT, matR;		//���[���h�s��,�ړ��s��,��]�s��,�����p�s��


	blocksMatList.clear();
	//���X�g�̐��������[�v
	for (itr = stagePartsList.begin(); itr != stagePartsList.end(); itr++)
	{
		//���X�g���ɃX�e�[�W�u���b�N��`�悷��
		D3DXMatrixTranslation(&matT, blockPos.x/* + 10*/, blockPos.y, blockPos.z);		//���̈ʒu�̍s������
		D3DXMatrixRotationY(&matR, D3DXToRadian(blockAngle));						//�p�x�����ɉ�]�s������
		matW = matR *  matT;														//�s��̍���

		//���X�gstagePartsList�̗v�f���ƃ��X�gblocksMatList���Ⴄ�Ƃ�
		if (stagePartsList.size() != blocksMatList.size())
		{
			//���X�gblocksMatList�Ƀu���b�N�̍s����i�[����
			blocksMatList.push_back(matW);
		}

		//�s���~�܂�̂Ƃ��͂Ȃɂ��`�悵�Ȃ�
		if (*itr != BLOCK_STOP)
		{
			//�`��
			blocksState[*itr].stage.Draw(&matW);

			//���̃`�b�v�̏o���̐��������[�v
			for (DWORD i = 0; i < blocksState[*itr].exitList.size(); i++)
			{
				//T���H�̂Ƃ�
				if (*itr == BLOCK_BRANCH)
				{
					//T���H�̉E������u���b�N��ݒu����
					D3DXVec3TransformCoord(&branchPos, &blocksState[BLOCK_BRANCH].exitList[1].pos, &matR);			//���̈ʒu����]�s��Əo���̈ʒu���|���ċ��߂�
					branchPos += blockPos;																			//T���H�̈ʒu�ϐ��Ɏ��̈ʒu�𑫂�
					branchAngle = blocksState[*itr].exitList[1].angle;												//T���H�̊p�x�ɍ��o���̊p�x������
					D3DXVECTOR3 nextPos = D3DXVECTOR3(0, 0, 0);														//���̈ʒu�p�̕ϐ�����
					D3DXVec3TransformCoord(&nextPos, &blocksState[*itr].exitList[0].pos, &matR);					//���̈ʒu����]�s��Əo���̈ʒu���|���ċ��߂�
					blockPos += nextPos;																			//���̈ʒu������
					blockAngle += blocksState[*itr].exitList[0].angle;												//�p�x�𑫂�
					break;
				}

				//T���H�ȊO�̂Ƃ�
				else
				{
					//�o���̈ʒu�Ɗp�x�����̃u���b�N�̈ʒu�Ɗp�x�ɑ���
					D3DXVECTOR3 nextPos = D3DXVECTOR3(0, 0, 0);												//���̈ʒu�p�̕ϐ�����
					D3DXVec3TransformCoord(&nextPos, &blocksState[*itr].exitList[i].pos, &matR);			//���̈ʒu����]�s��Əo���̈ʒu���|���ċ��߂�
					blockPos += nextPos;																	//���̈ʒu�𑫂�
					blockAngle += blocksState[*itr].exitList[i].angle;										//�p�x�𑫂�
				}
			}
		}

		//BLOCK_STOP�̂Ƃ�
		else
		{
			//T���H�̍�������u���b�N��ݒu����
			blockPos = branchPos;											//���̈ʒu��O�Ɋi�[����T���H�̈ʒu����
			blockAngle += branchAngle;										//�p�x�𑫂�
		}
	}
	
	return S_OK;				//������Ԃ�
}
