/*===========================================================================================
�T�@���F�X�e�[�W�̏������܂Ƃ߂��N���X�̊֐���`
�쐬���F10��15��
�X�V���F10��15��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Stage.h"
#include "Camera.h"		//�J�����N���X
#include "Player.h"		//�v���C���[�N���X
#include "StageData.h"	//�X�e�[�W�f�[�^�N���X

//�O���[�o���ϐ�
//�p�[�c�̕��сi������G�f�B�^�ō���΂����j
DWORD stagePartsList[] = { BLOCK_START, BLOCK_UP, BLOCK_BRANCH, BLOCK_DOWN, BLOCK_STOP, BLOCK_STRAIGHT, BLOCK_BRANCH, BLOCK_STRAIGHT,BLOCK_UP,BLOCK_DOWN, BLOCK_STOP, BLOCK_UP,BLOCK_DOWN,BLOCK_STRAIGHT, BLOCK_GOAL };

//�R���X�g���N�^
Stage::Stage()
{
	//�e�p�[�c�̏o���̈ʒu��ݒ肵�Ă����i���̕ӂ͊O���t�@�C������ǂݍ��߂�ƃJ�b�R�C�C�j
	ExitState exit;

	//�X�^�[�g�̐ݒ�
	exit.pos   = D3DXVECTOR3(0, 0, 24);		//�ʒu
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
	exit.angle = 270;						//�p�x
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

	

	//����������
	playerPos		= D3DXVECTOR3(0, 0, 0);		//�v���C���[�̈ʒu
	stageBlock	    = BLOCK_MAX;				//�X�e�[�W�u���b�N�̎�ނ͍ŏ��͉����w�肵�Ȃ��Œ萔�̍ő�l������
	isSprikHit		= false;					//��Q���ƃv���C���[���Փ˂������ǂ���
	sprik			= NULL;						//��Q�����f��
	isPlayerDown	= TRUE;						//�X�e�[�W�ɂ���Ԃ�TRUE�A�����Ă���Ԃ�FALSE
	flg = 0;
}

//�f�X�g���N�^
Stage::~Stage()
{
	if (flg == 0x01)
		Clear(&startNode);		//�m�[�h�̍폜(�X�^�[�g�m�[�h����n�߂�)

	SAFE_DELETE(sprik);		//��Q�����f���̉��
}

//�@�\�F�m�[�h�̍폜
//�����F�m�[�h�̎��
//�ߒl�F�Ȃ�
void Stage::Clear(Node* pNode)
{

	if (pNode->type == 99)
	{
		return;
	}

	//���̃u���b�N�̏o�������[�v
	for (DWORD i = 0; i < blocksState[pNode->type].exitList.size(); i++)
	{
		Clear(pNode->childNode[i]);			//�Ō�̃X�e�[�W�u���b�N�܂ōċA�����œǂݍ���
		SAFE_DELETE(pNode->childNode[i]);	//���
	}
}

//�@�\�F�ǂݍ��ݏ���
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Stage::Load()
{
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

	//��Q�����f���̓ǂݍ���

	sprik = new Fbx;

	if (FAILED(sprik->Load("Asset\\Stage\\Sprik.fbx")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "��Q���̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�X�e�[�W�̍쐬
//�����F�ݒ肳����m�[�h
//�ߒl�F�Ȃ�
void Stage::Create(D3DXVECTOR3 pos, float angle, Node* pNode)
{

	//�p�[�c�̎�ށi�`���̔z�񂩂�j
	if (g_gameScene != SC_PLAY)
		pNode->type = stagePartsList[nodeID];					//�O���[�o���ϐ��ɓ���Ă���u���b�N���擾����
	else
	{
		//pNode->childNode.pop_back();
		pNode->type = StageData::GetInst()->data[nodeID];		//�O���[�o���ϐ��ɓ���Ă���u���b�N���擾����
	}

	if (pNode->type == 99)
		return;

	nodeID++;												//���̃u���b�N�Ɉړ�

	pNode->pos = pos;										//�u���b�N�̈ʒu���L������

	//�u���b�N���Ƃ̍s��̍쐬
	D3DXMATRIX matR, matT;									//�������]�E�ړ��E���[���h�s��
	D3DXMatrixRotationY(&matR, D3DXToRadian(angle));		//��]�s��̍쐬
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);		//�ړ��s��̍쐬
	pNode->matrix = matR * matT;							//�s��̍���

	//�u���b�N���X�^�[�g�u���b�N�łȂ��Ƃ�
	if ((pNode->type != BLOCK_START) && (pNode->type != BLOCK_GOAL) )
	{
		////��Q����K���ɔz�u
		int sprikNum = rand() % 5 + 1;							//��Q�������z�u���邩�����Ō��߂�(�ő�T��)

		//�ϐ�sprikNum�̒l�������[�v
		for (int i = 0; i < sprikNum; i++)
		{
			//��Q���̊e���W���i�[����ϐ�
			int sprikPosX = 0;				//x���W	
			int sprikPosY = 0;				//y���W
			int sprikPosZ = 0;				//z���W

			//�u���b�N����]���Ă��Ȃ��Ƃ�
			if (angle == 0 || angle == 360)
			{
				sprikPosX = (int)pos.x + (rand() % 10 - 5);		//x�ʒu���m�[�h�̕��ɍ��킹�ă����_���Ȉʒu�ɂ���
				sprikPosY = (int)pos.y;							//y�ʒu�̓m�[�h�̈ʒu��
				sprikPosZ = (rand() % 12) + (int)pos.z;			//z�ʒu�̓m�[�h�̉��s�ɍ��킹�ă����_���Ȉʒu�ɂ���
			}

			//�u���b�N����]���Ă�Ƃ�
			else if (angle == 90 || angle == 270)
			{
				sprikPosX = (rand() % 12) + (int)pos.x;				//x�ʒu���m�[�h�̉��s���ɍ��킹�ă����_���Ȉʒu�ɂ���
				sprikPosY = (int)pos.y;							//y�ʒu�̓m�[�h�̈ʒu��
				sprikPosZ = (int)pos.z + (rand() % 10 - 5);		//z�ʒu�̓m�[�h�̕��ɍ��킹�ă����_���Ȉʒu�ɂ���
			}

			//��Q���ƃm�[�h�Ń��C�L���X�g
			RayCastData data;																	//�\���̂̏���
			data.start = D3DXVECTOR3((float)sprikPosX, (float)sprikPosY, (float)sprikPosZ);		//���C���΂��ʒu����Q���̈ʒu�ɂ���
			data.start.y = 100;																	//y���͂��Ȃ荂���ʒu����
			data.dir = D3DXVECTOR3(0, -1, 0);													//���C�����ɔ�΂�
			data.matrix = pNode->matrix;														//���̃m�[�h�̍s���n��
			blocksState[pNode->type].stage.RayCast(&data);										//���C�L���X�g������s��

			//�Փ˂����Ƃ�
			if (data.hit)
			{
				//y�����m�[�h�ɍ��킹��
				data.pos.y += 2;			//����������ɂ���
				sprikPosY = (int)data.pos.y;		//���̈ʒu�Ɉړ�
				
			}

			//��Q���̈ʒu��z��Ɋi�[
			pNode->sprikPos.push_back(D3DXVECTOR3((float)sprikPosX, (float)sprikPosY, (float)sprikPosZ));
		}
	}

	//���̃u���b�N�̏o�������[�v
	for (DWORD i = 0; i < blocksState[pNode->type].exitList.size(); i++)
	{

		Node* child = new Node;					//�V���ȃm�[�h�����
		pNode->childNode.push_back(child);		//�q���ɂ���

		//���̃m�[�h�̈ʒu(�p�[�c�̏o���ʒu�ɉ�]�s���������j
		D3DXVECTOR3 nextPos;
		D3DXVec3TransformCoord(&nextPos, &blocksState[pNode->type].exitList[i].pos, &matR);

		//���̃m�[�h�̊p�x(�p�[�c��񂩂�擾�j
		float nextAngle = blocksState[pNode->type].exitList[i].angle;

		//������m�[�h�̐ݒ�
		Create(pos + nextPos, angle + nextAngle, child);		//�ċA�����Ŏq���̃m�[�h�̐ݒ������
	}

}

//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Stage::Update()
{


	return S_OK;			//������Ԃ�
}

//�@�\�F�Փ˔��菈��
//�����F���j�b�g�x�[�X�N���X�ƃ��j�b�g�x�[�X�N���X���p�����Ă���N���X
//�ߒl�F���������s��
HRESULT Stage::Hit(UnitBase* pTarget)
{

	//�v���C���[�N���X�Ƃ̓����蔻��
	if (typeid(*pTarget) == typeid(Player))
	{
		Player* player = (Player*)pTarget;	//�v���C���[�N���X�̐���

		playerPos = player->GetPos();		//�v���C���[�̈ʒu���擾����

		//�ŏ��̃m�[�h����X�e�[�W�u���b�N�ƃv���C���[�̏Փ˔�����s��
		if (FAILED(Hit(&startNode)))
		{
			//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "�X�e�[�W�Ƃ̏Փ˂Ɏ��s���܂���", "�q�b�g�G���[", MB_OK);

			return E_FAIL;		//���s��Ԃ�
		}
	}


	return S_OK;			//������Ԃ�
}


//�@�\�F�X�e�[�W�u���b�N�ƃv���C���[�̏Փ˔���
//�����F�X�e�[�W�u���b�N�̃m�[�h
//�ߒl�F���������s��
HRESULT Stage::Hit(Node* pNode)
{
	//�v���C���[�ƃX�e�[�W�u���b�N�Ƃ̋����𒲂ׂ�
	D3DXVECTOR3 toPlayer = pNode->pos - playerPos;					//�v���C���[�ƃX�e�[�W�u���b�N�̍��x�N�g��������
	float dist = D3DXVec3Length(&toPlayer);							//���̒���(����)���󂯎��

	//���������ȉ��̂Ƃ�
	if (dist <= 24)
	{
		//���C�L���X�g�ŏՓ˔�����s��
		RayCastData rayData;										//���C�L���X�g�\���̂̏���
		rayData.start = playerPos;									//�v���C���[�̈ʒu�����C�̔��ˈʒu�ɂ���
		rayData.start.y = 200;										//�v���C���[��菭���ォ�烌�C�𔭎˂���
		rayData.dir = D3DXVECTOR3(0, -1, 0);						//�v���C���[�̈ʒu����y����-1�̈ʒu�Ƀ��C���΂�
		rayData.matrix = pNode->matrix;								//���̃X�e�[�W�u���b�N�̍s���n��
		blocksState[pNode->type].stage.RayCast(&rayData);			//�X�e�[�W�̃��f���ƃ��C�L���X�g����


		//�X�e�[�W�ƏՓ˂��Ă���
		if (rayData.hit)
		{
			//�X�e�[�W�u���b�N�ƃv���C���[�̏Փˈʒu�Ɩ@����n��
			playerPos = rayData.pos;								//�Փˈʒu
			blocksNormal = rayData.normal;							//�@��
			isPlayerDown = TRUE;									//�X�e�[�W�ɂ���̂�TRUE��

			//���̃m�[�h�ɂ����Q���̐��������[�v
			for (DWORD i = 0; i < pNode->sprikPos.size(); i++)
			{
				//�v���C���[�Ə�Q�����Փ˂��Ă��邩���ׂ�
				D3DXVECTOR3 toSprik = pNode->sprikPos[i] - playerPos;		//��Q���ƃv���C���[�̈ʒu�̍��̃x�N�g��������
				float sprikDist = D3DXVec3Length(&toSprik);					//���̒���(����)���󂯎��

				isSprikHit = (sprikDist <= 2);								//���������ȉ��̂Ƃ���true�ɂ���
				if (isSprikHit)												//�Փ˂��Ă����炱�̃��[�v���I��炷
					break;
			}

			//�S�[���̂Ƃ�
			if (pNode->type == BLOCK_GOAL && dist <= 5)
			{
				g_gameScene = SC_CLEAR;								//�N���A�V�[���̂Ƃ�
			}


			return S_OK;
		}

	}

	//�Փ˂��Ă��Ȃ��Ƃ�
	else
	{
		//�v���C���[�����ɗ��Ƃ�
		//playerPos.y -= 0.05f;
		//blocksNormal = D3DXVECTOR3(0, 0, 0);
		//isPlayerDown = FALSE;					//�X�e�[�W�ƏՓ˂��Ă��Ȃ��̂ł���FALSE��
	}

	//���̃m�[�h�̎q���̐��������[�v
	for (DWORD i = 0; i < pNode->childNode.size(); i++)
	{
		//���̃m�[�h�ŏՓ˔�����s��(�ċA����)
		Hit(pNode->childNode[i]);
	}

	return S_OK;				//���s��Ԃ�
}

//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Stage::Render()
{
	if (flg == 0x00)
	{
		flg |= 1;
		//�X�e�[�W�쐬
		nodeID = 0;				//�m�[�h�̔ԍ�(�ŏ���0)
		Create(D3DXVECTOR3(0, 0, 0), 0, &startNode);		//�X�e�[�W���쐬
	}

	//�m�[�h���Ƃɕ`�悷��(���̏������������֐��̌Ăяo��)
	if (FAILED(BlocksRender(D3DXVECTOR3(0, 0, 0), 0, &startNode)))		//�ŏ��̃m�[�h����`��
	{
		return E_FAIL;		//���s��Ԃ�
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�m�[�h�̕`��
//�����Fpos�@�@�\������ʒu
//�����Fangle�@�\������p�x
//�����FpNode�@�\������m�[�h�̎��
//�ߒl�F�Ȃ�
HRESULT Stage::BlocksRender(D3DXVECTOR3 pos, float angle, Node* pNode)
{
	//�s���~�܂肩99�Ȃ�\���Ȃ�
	if (pNode->type == BLOCK_STOP || pNode->type == 99)
		return S_OK;


	D3DXMATRIX matR, matT,matW;								//�������]�E�ړ��E���[���h�s��
	D3DXMatrixRotationY(&matR, D3DXToRadian(angle));		//��]�s��̍쐬
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);		//�ړ��s��̍쐬
	matW = matR * matT;

	//��������m�[�h�̏o���ɍ��킹���ړ��E��]�s�������ĕ`��
	blocksState[pNode->type].stage.Draw(&matW);				//�m�[�h�̕`��

	for (DWORD i = 0; i < pNode->sprikPos.size(); i++)
	{
		D3DXMatrixTranslation(&matW, pNode->sprikPos[i].x, pNode->sprikPos[i].y, pNode->sprikPos[i].z);
		sprik->Draw(&matW);
	}

	//���̃m�[�h�̏o�������[�v
	for (DWORD i = 0; i < blocksState[pNode->type].exitList.size(); i++)
	{
		//���̃m�[�h�̈ʒu(�p�[�c�̏o���ʒu�ɉ�]�s���������j
		D3DXVECTOR3 nextPos;
		D3DXVec3TransformCoord(&nextPos, &blocksState[pNode->type].exitList[i].pos, &matR);

		//���̃m�[�h�̊p�x(�p�[�c��񂩂�擾�j
		float nextAngle = blocksState[pNode->type].exitList[i].angle;

		//���̃m�[�h��`��
		BlocksRender(pos + nextPos, angle + nextAngle, pNode->childNode[i]);		//���̃m�[�h�̈ʒu�E�p�x���玟�̃m�[�h�̏o���܂ł̈ʒu�E�p�x�𑫂������̂������ɂ��čċA����
	}



	return S_OK;			//������Ԃ�
}