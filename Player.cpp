/*===========================================================================================
�T�@���F�v���C���[�̏������܂Ƃ߂��N���X�̊֐���`
�쐬���F10��15��
�X�V���F10��15��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Player.h"

#include "Camera.h"		//�J�����N���X
#include "Stage.h"		//�X�e�[�W�N���X

//�萔�錾
#define RAY_START_Y    200	//���C���΂�y���̈ʒu
#define ACCSEL	    0.007f	//�v���C���[�̉����x
#define MAX_SPEED     0.2f	//�v���C���[�̍ō����x
#define MIN_SPEED   0.001f	//�v���C���[�̍Œᑬ�x
#define FRICTION    0.001f	//���C
#define DIR_SPEED	     2	//��]���x



//�R���X�g���N�^
Player::Player()
{
	//�N���X�̏�����
	player   = NULL;							//�v���C���[���f��

	for (int i = 0; i < sizeof(lifeMark) / sizeof(lifeMark[0]); i++)		//�̗͂�\�����鐔�������[�v
	{
		lifeMark[i] = NULL;							//�̗͉摜
	}

	//�����o�ϐ��̏�����
	position   = D3DXVECTOR3(0, 0, 0);		//�v���C���[�̈ʒu
	isStage	   = TRUE;						//�X�e�[�W�ɓ������Ă��邩����(�ŏ��͓������Ă���悤�ɂ���)
	move	   = D3DXVECTOR3(0, 0, 0);		//�ړ���
	dirX	   = 0;							//x���̉�]��
	dirZ	   = 0;							//z���̉�]��
	blockId    = 0;							//�X�e�[�W�̎��
	normal	   = D3DXVECTOR3(0, 0, 0);		//�X�e�[�W�u���b�N�̖@��
	normalVec  = D3DXVECTOR3(0, -1, 0);		//�@���ɏ]���ăv���C���[��]�����x�N�g��
	life	   = 5;							//�����̗�
	timeCount  = 0;							//���Ԃ��J�E���g
	isSprikHit = FALSE;						//��Q���ƏՓ˂������ǂ���
	isDamege   = FALSE;						//�_���[�W���󂯂����ǂ���
	isDown	   = TRUE;						//�X�e�[�W���痎���Ă��邩�ǂ���
	downTime   = 0;							//�X�e�[�W���痎���Ă��鎞��
	D3DXMatrixIdentity(&matRX);				//��]�s��
	D3DXMatrixIdentity(&matRZ);				//��]�s��
}

//�f�X�g���N�^
Player::~Player()
{
	//�N���X�̉��
	SAFE_DELETE(player);					//�v���C���[���f��
	for (int i = 0; i < sizeof(lifeMark) / sizeof(lifeMark[0]); i++)
	{
		SAFE_DELETE(lifeMark[i]);					//�̗͉摜
	}
}

//�@�\�F�ǂݍ��ݏ���
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::Load()
{
	//�N���X�̓��I�쐬
	player = new Fbx;		//�v���C���[

	//�v���C���[�̃��f���̓ǂݍ���
	if (FAILED(player->Load("Asset\\Model\\Player.fbx")))
	{
		//�v���C���[�̓ǂݍ��݂Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�v���C���[�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);
		return E_FAIL;		//���s��Ԃ�
	}

	//�̗͉摜�̓ǂݍ���
	for (int i = 0; i < life; i++)
	{
		lifeMark[i] = new Sprite;
		if (FAILED(lifeMark[i]->Load("Asset\\Sprite\\LifeOn.png")))
		{
			//�v���C���[�̓ǂݍ��݂Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "�̗͉摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);
			return E_FAIL;		//���s��Ԃ�
		}
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::Update()
{

	//�v���C���[�ړ������̊֐����Ă�
	if (FAILED(Move()))
	{
		//�ړ������Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�ړ������Ɏ��s���܂���", "�X�V�G���[", MB_OK);
		return E_FAIL;		//���s��Ԃ�
	}

	//�v���C���[�̉�]�����̊֐����Ă�
	if (FAILED(Rotation()))
	{
		//��]�����Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "��]�����Ɏ��s���܂���", "�X�V�G���[", MB_OK);
		return E_FAIL;		//���s��Ԃ�
	}

	//�J�����̈ړ������̊֐����Ă�
	if (FAILED(MoveCamera()))
	{
		//�J�����̈ړ��Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�J�����̈ړ��Ɏ��s���܂���", "�X�V����", MB_OK);
		return E_FAIL;		//���s��Ԃ�
	}

	//��Q���ƏՓ˂��Ă��_���[�W�t���O��fase�̂Ƃ�
	if (isSprikHit && !isDamege)
	{
		life--;				//���C�t���P���炷
		isDamege = true;	//�_���[�W�t���O��true�ɂ��ă_���[�W��A�����Ď󂯂Ȃ�����
	}
	
	//�_���[�W�t���O��true�̂Ƃ�
	if (isDamege)
	{
		timeCount++;		//���Ԃ��J�E���g

		//90�t���[���܂ŃJ�E���g������
		if (timeCount >= 90)
		{
			//�_���[�W����炦��悤�ɂ���
			isDamege = false;				//�_���[�W�t���O��false��
			timeCount = 0;					//���Ԃ̃J�E���g��������
		}
	}

	//���C�t��0�ɂȂ�����
	if (life <= 0)
	{
		g_gameScene = SC_GAMEOVER;		//�Q�[���I�[�o�[�V�[���Ɉڍs
	}

	//�X�e�[�W���痎���Ă���Ƃ�
	if (!isDown)
	{
		downTime++;		//���Ԃ��J�E���g(�P�ʂ�1�b�Ԃ�60)

		//��莞�Ԍo������
		if (downTime >= 120)
		{
			//�Q�[���I�[�o�[�V�[����
			g_gameScene = SC_GAMEOVER;
		}
	}

	//�X�e�[�W���痎���Ă��Ȃ��Ƃ�
	else
	{
		//���Ԃ�������
		downTime = 0;
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�Փ˔��菈��
//�����F���j�b�g�x�[�X�N���X�Ƃ�����p�������N���X�̃|�C���^
//�ߒl�F���������s��
HRESULT Player::Hit(UnitBase* pTarget)
{
	//pTarget���X�e�[�W��������
	if (typeid(*pTarget) == typeid(Stage))
	{
		//�X�e�[�W�N���X�̕ϐ���������������
		Stage* stage = (Stage*)pTarget;
		
		//�X�e�[�W�N���X����Փ˂����ʒu�A�@���A������󂯎��
		position.y =  stage->GetPlayerPos();		//�ʒu
		normal	   =  stage->GetBlocksNormal();		//�@��
		isSprikHit = stage->GetIsSprikHit();		//��Q���ƏՓ˂������ǂ���
		isDown	   = stage->GetPlayerDown();		//�X�e�[�W���痎���Ă��邩�ǂ���

	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::Render()
{
	//�s��̍쐬
	D3DXMATRIX matW;			//���[���h�s��p(�`�悷��ۂɂ��̍s��������Ƃ��ēn��)

	//�ړ��s��̍쐬
	D3DXMatrixTranslation(&matW, position.x,position.y + 0.9f,position.z);		//�v���C���[�̈ʒu�Ƀv���C���[��`�悳����悤�ɂ���(y���͏�����ɕ`�悵�Ȃ��ƒn�ʂɖ��܂�)

	//�s��̍���
	matW = matRX * matRZ * matW;

	//�v���C���[�̕`��
	if (FAILED(player->Draw(&matW)))
	{
		//�`��Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�v���C���[�̕`��Ɏ��s���܂���", "�`��G���[", MB_OK);
		return E_FAIL;		//���s��Ԃ�
	}

	//�̗͂̕`��
	SpriteData data;		///�X�v���C�g�̏����܂Ƃ߂�\���̂̏���
	float drawShift = 0;		//�`�悷��ۂɃ��C�t�����炷���߂̏���
	for (int i = 0; i < life; i++)
	{
		//�ŏ��̃��[�v�ȊO�̂Ƃ�
		if (i != 0)
		{
			drawShift += 50;		//50���炷
		}

		data.pos = D3DXVECTOR3(6 + drawShift, 5, 0);		//�`��̈ʒu�����߂�
		lifeMark[i]->Draw(&data);							//�`��
	}
	

	return S_OK;			//������Ԃ�
}


//-----------------��������private�֐��̒�`------------------------

//�@�\�F�ړ�����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::Move()
{
	//�ړ��͏��X�ɑ��x���グ�Ă��A�ړ����~�߂�ɂ͖��C�Ŏ~�߂�悤�ɂ���

	//���ׂ�x�N�g���ō⓹�ŏ���ɓ]����悤�ɂ���
	D3DXVec3Normalize(&normalVec, &normalVec);										//�@���Ŏg���x�N�g���̐��K��
	move += (normalVec - (D3DXVec3Dot(&normalVec, &normal)) * normal) * 0.01f;		//���ׂ�x�N�g�����g���č⓹��]����悤�ɂ���

	//�O�ړ������̊֐����Ăяo��
	if (FAILED(frontMove()))
	{
		//���s���Ă����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�O�ړ��Ɏ��s���܂���", "�ړ��G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//��ړ������̊֐����Ăяo��
	if (FAILED(backMove()))
	{
		//���s���Ă����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "��ړ��Ɏ��s���܂���", "�ړ��G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}


	//���ړ������̊֐����Ăяo��
	if (FAILED(leftMove()))
	{
		//���s���Ă����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���ړ��Ɏ��s���܂���", "�ړ��G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�E�ړ������̊֐����Ăяo��
	if (FAILED(rightMove()))
	{
		//���s���Ă����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�E�ړ��Ɏ��s���܂���", "�ړ��G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�O���C�����֐����Ăяo��
	if (FAILED(frontFriction()))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�O���C�����Ɏ��s���܂���", "���C�G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�㖀�C�����֐����Ăяo��
	if (FAILED(backFriction()))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�㖀�C�����Ɏ��s���܂���", "���C�G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}


	//�����C�����֐����Ăяo��
	if (FAILED(leftFriction()))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�����C�����Ɏ��s���܂���", "���C�G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}
	
	//�E���C�����֐����Ăяo��
	if (FAILED(rightFriction()))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�E���C�����Ɏ��s���܂���", "���C�G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	 


	 //�΂߈ړ�����Ƒ��x�������Ȃ�̂ŏ����x�������đ�̓������x�ɂ���
	 if (move.x < 0 && move.z > 0)		move *= 0.97f;				//���O�Ɉړ����Ă���Ƃ�
	 if (move.x < 0 && move.z < 0)		move *= 0.97f;				//����
	 if (move.x > 0 && move.z > 0)		move *= 0.97f;				//�E�O
	 if (move.x > 0 && move.z < 0)		move *= 0.97f;				//�E��

	 //�ړ�
	position.z += move.z;		//�O��ړ�
	position.x += move.x;		//���E�ړ�

	return S_OK;			//������Ԃ�
}

//----------------------------------�ړ������֐�----------------------------------

//�@�\�F�J�����̈ړ�����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::MoveCamera()
{
	//�v���C���[�̏�����납��J������ǐ������邽�߂̏���������
	g_pCamera->SetTarget(position);							//�v���C���[�̈ʒu�𒆐S�ɂȂ�悤�ɃJ��������]������
	D3DXVECTOR3 cameraPos = D3DXVECTOR3(0, 7, -10);			//�J�����̈ʒu���v���C���[�̏������̏�̈ʒu�ɂ���
	g_pCamera->SetPos(position + cameraPos);				//camerapos�̒l��position�����Z���邱�ƂŃv���C���[�̏�����납��v���C���[��ǂ�������悤�ɂ���

	return S_OK;			//������Ԃ�
}

//�@�\�F�O�ړ�����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::frontMove()
{


	//���L�[�őO�ړ�
	if (g_pInput->IsKeyPush(DIK_UP))
	{
		//�O�ړ�
		if (move.z <= MAX_SPEED)			//�ړ��ʂ��ō����x�𒴂��ĂȂ���
		{
			move.z += ACCSEL;		//����������
		}

	}

	return S_OK;				//������Ԃ�
}

//�@�\�F��ړ�����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::backMove()
{
	//���L�[�Ō�Ɉړ�
	if (g_pInput->IsKeyPush(DIK_DOWN))
	{
		//���Ɉړ�
		if (move.z >= -MAX_SPEED)		//�ړ��ʂ��ō����x�𒴂��ĂȂ���
		{
			move.z -= ACCSEL;		//����������
		}

	}

	return S_OK;				//������Ԃ�
}

//�@�\�F���ړ�����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::leftMove()
{
	//���L�[�ō��Ɉړ�
	if (g_pInput->IsKeyPush(DIK_LEFT))
	{
		//���Ɉړ�
		if (move.x >= -MAX_SPEED)		//�ō����x�𒴂��Ă��Ȃ���
		{
			move.x -= ACCSEL;		//����������
		}
	}

	return S_OK;				//������Ԃ�
}

//�@�\�F�E�ړ�����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::rightMove()
{

	//���L�[�ŉE�ړ�
	if (g_pInput->IsKeyPush(DIK_RIGHT))
	{
		//�E�Ɉړ�
		if (move.x <= MAX_SPEED)		//�ō����x�𒴂��Ă��Ȃ���
		{
			move.x += ACCSEL;		//����������
		}
	}

	return S_OK;				//������Ԃ�
}

//--------------------���C�����֐�--------------------------------

//�@�\�F�O���C����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::frontFriction()
{

	//�v���C���[���O�����Ɉړ����Ă�����
	if (move.z > 0)
	{

		//�t�̈ʒu�ɒl�������������Ė��C�𔭐�������
		move.z -= FRICTION;

		//��Q���ƏՓ˂��Ă���Ƃ�
		if (isSprikHit)
		{
			//���Ε����Ɉړ�			
			position.z -= 1;
		}

		//���S�Ɉړ����~�߂�
		if (move.z <= 0.001f)
			move.z = 0;			//���̏����������Ȃ���move�̒l��0�ɂȂ�Ȃ������̂ł��̏�����ǉ�
	}

	return S_OK;			//���s��Ԃ�
}

//�@�\�F�㖀�C����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::backFriction()
{

	//�v���C���[���������Ɉړ����Ă�����
	if (move.z < 0)
	{

		//�t�̈ʒu�ɒl�������������Ė��C�𔭐�������
		move.z += FRICTION;

		//��Q���ƏՓ˂��Ă���Ƃ�
		if (isSprikHit)
		{
			//���Ε����Ɉړ�
			position.z += 1;
		}

		//���S�Ɉړ����~�߂�
		if (move.z >= MIN_SPEED)
			move.z = 0;
	}

	return S_OK;			//���s��Ԃ�
}

//�@�\�F�����C����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::leftFriction()
{

	//�v���C���[���������Ɉړ����Ă�����
	if (move.x < 0)
	{

		//�t�̈ʒu�ɒl�������������Ė��C�𔭐�������
		move.x += FRICTION;

		//��Q���ƏՓ˂��Ă���Ƃ�
		if (isSprikHit)
		{
			//���Ε����Ɉړ�
			position.x -= 1;
		}

		//���S�Ɉړ����~�߂�
		if (move.x >= MIN_SPEED)
			move.x = 0;
	}

	return S_OK;			//���s��Ԃ�
}

//�@�\�F�E���C����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::rightFriction()
{
	//�v���C���[���E�����Ɉړ����Ă�����
	if (move.x > 0)
	{

		//�t�̈ʒu�ɒl�������������Ė��C�𔭐�������
		move.x -= FRICTION;

		//��Q���ƏՓ˂��Ă���Ƃ�
		if (isSprikHit)
		{
			//���Ε����Ɉړ�
			position.x += 1;
		}

		//���S�Ɉړ����~�߂�
		if (move.x <= MIN_SPEED)
			move.x = 0;
	}

	return S_OK;			//���s��Ԃ�
}


//---------------------------��]�����֐�-------------------------------------------

//�@�\�F��]����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Player::Rotation()
{
	//�O�����Ɉړ����Ă���Ƃ�
	if (move.z > 0)
	{
		//�O��]
		dirX += DIR_SPEED;	//��]�ʂ��C���N�������g
		D3DXMatrixRotationX(&matRX, D3DXToRadian(dirX));		//��]�ʕ�x���ɉ�]
	}

	//������Ɉړ����Ă���Ƃ�
	if (move.z < 0)
	{
		//����]
		dirX -= DIR_SPEED;	//��]�ʂ��f�N�������g
		D3DXMatrixRotationX(&matRX, D3DXToRadian(dirX));		//��]�ʕ�x���ɉ�]
	}

	//�������Ɉړ����Ă���Ƃ�
	if (move.x < 0)
	{
		//���ɉ�]
		dirZ += DIR_SPEED;		//�f�N�������g
		D3DXMatrixRotationZ(&matRZ, D3DXToRadian(dirZ));		//�ړ��ʕ�z���ɉ�]
	}

	//�E�����Ɉړ����Ă���Ƃ�
	if (move.x > 0)
	{
		//�E�ɉ�]
		dirZ -= DIR_SPEED;		//�C���N�������g
		D3DXMatrixRotationZ(&matRZ, D3DXToRadian(dirZ));		//�ړ��ʕ�z���ɉ�]
	}

	//�O��Ɉړ����Ă��Ȃ��Ƃ�
	if (move.z == 0)			dirX = 0;		//��]�ʂ�������
	
	//���E�Ɉړ����Ă��Ȃ��Ƃ�
	if (move.x == 0)			dirZ = 0;		//��]�ʂ�������

	return S_OK;			//������Ԃ�
}



