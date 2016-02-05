/*===========================================================================================
�T�@���F�^�C�g����ʂł̏������܂Ƃ߂��N���X�̊֐���`
�쐬���F12��21��
�X�V���F12��21��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Title.h"

//�R���X�g���N�^
Title::Title()
{
	//�N���X�̏�����
	title	   = NULL;		//�^�C�g���摜
	wait	   = NULL;		//�ҋ@���̉摜
	modeSelect = NULL;		//���[�h�Z���N�g�Ɏg���摜
	arrow	   = NULL;		//���

	//�����o�ϐ��̏�����
	state = WAIT_STATE;		//�ŏ��͑ҋ@���
	mode = SELECT_MODE;		//�ŏ��̓X�e�[�W�I�����[�h
}

//�f�X�g���N�^
Title::~Title()
{
	//�N���X�̉��
	SAFE_DELETE(title);			//�^�C�g���摜
	SAFE_DELETE(wait);			//�ҋ@���̉摜
	SAFE_DELETE(modeSelect);	//���[�h�Z���N�g�Ɏg���摜
	SAFE_DELETE(arrow);			//���摜
}


//�@�\�F�ǂݍ��ݏ���
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Title::Load()
{
	//�N���X�̓��I�쐬
	title	   = new Sprite;		//�^�C�g���摜
	wait	   = new Sprite;		//�ҋ@���̉摜
	modeSelect = new Sprite;		//���[�h�Z���N�g�Ɏg���摜
	arrow	   = new Sprite;		//���

	//�^�C�g���摜�̓ǂݍ���
	if (FAILED(title->Load("Asset\\Sprite\\title.bmp")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�^�C�g���摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}

	//�ҋ@���̉摜�̓ǂݍ���
	if (FAILED(wait->Load("Asset\\Sprite\\Wait.png")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���[�ŃZ���N�g�摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}

	//���[�h�Z���N�g�Ɏg���摜�̓ǂݍ���
	if (FAILED(modeSelect->Load("Asset\\Sprite\\ModeSelect.png")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���[�ŃZ���N�g�摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}

	if (FAILED(arrow->Load("Asset\\Sprite\\Arrow.png")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}

	return S_OK;				//������Ԃ�
}

//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Title::Update()
{
	//��ʂ̏�Ԗ��ɏ���
	switch (state)
	{
		//�ҋ@��Ԃ̂Ƃ�
	case WAIT_STATE:

		//�X�y�[�X�L�[�������ꂽ��
		if (g_pInput->IsKeyTap(DIK_SPACE))
		{
			state = MODESELECT_STATE;		//���[�h�Z���N�g��Ԃɂ���
		}

		break;

		//���[�h�Z���N�g��Ԃ̂Ƃ�
	case MODESELECT_STATE:

		//�o�b�N�X�y�[�X�L�[�������ꂽ��
		if (g_pInput->IsKeyTap(DIK_BACK))
		{
			state = WAIT_STATE;		//�ҋ@��Ԃɖ߂�
		}

		//�X�e�[�W�I�����[�h�̂Ƃ�
		if (mode == SELECT_MODE)
		{
			//�\����L�[�Ə\�����L�[�̂ǂ��炩�������ꂽ�Ƃ�
			if (g_pInput->IsKeyTap(DIK_UP) || g_pInput->IsKeyTap(DIK_DOWN))
			{
				mode = CREATE_MODE;		//�X�e�[�W�쐬���[�h�ɂ���
			}

			//�G���^�[�L�[�������ꂽ��
			else if (g_pInput->IsKeyTap(DIK_RETURN))
			{
				g_gameScene = SC_STAGE_SELECT;		//�X�e�[�W�I���V�[���ɂ���
			}
		}

		//�X�e�[�W�쐬���[�h�̂Ƃ�
		else
		{
			//�\����L�[�Ə\�����L�[�̂ǂ��炩�������ꂽ�Ƃ�
			if (g_pInput->IsKeyTap(DIK_UP) || g_pInput->IsKeyTap(DIK_DOWN))
			{
				mode = SELECT_MODE;		//�X�e�[�W�I�����[�h�ɂ���
			}

			//�G���^�[�L�[�������ꂽ��
			else if (g_pInput->IsKeyTap(DIK_RETURN))
			{
				g_gameScene = SC_STAGE_EDITOR;		//�X�e�[�W�쐬�V�[���ɂ���
			}
		}

		break;
	}

	return S_OK;				//������Ԃ�
}

//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Title::Render()
{
	//�^�C�g���摜�̕`��
	SpriteData data;				//�X�v���C�g�̏����܂Ƃ߂��\���̂̏���
	title->Draw(&data);				//�^�C�g���摜�͏���ς����ɕ`��

	//��ʂ̏�Ԗ��ɏ���
	switch (state)
	{
		//�ҋ@��Ԃ̂Ƃ�
	case WAIT_STATE:

		//�ҋ@���̉摜�̓ǂݍ���
		data.pos.y = WINDOW_HEIGHT / 2;		//��ʂ̔�����艺�ɂ���
		wait->Draw(&data);					//�`��

		break;

		//���[�h�I����Ԃ̂Ƃ�
	case MODESELECT_STATE:

		//���[�h�Z���N�g�摜�̕`��
		data.pos.y = WINDOW_HEIGHT / 2;		//��ʂ̔�����艺�ɂ���
		modeSelect->Draw(&data);			//�`��

		//�I��ł��郂�[�h���ƂɈʒu��ς��Ė���`��
		if (mode == SELECT_MODE)		//�X�e�[�W�I�����[�h�̂Ƃ�
		{
			//�ʒu��ύX
			data.pos.x = 250;
			data.pos.y = WINDOW_HEIGHT / 2 + 50;
			arrow->Draw(&data);		//�`��
		}

		//�X�e�[�W�쐬���[�h�̂Ƃ�
		else
		{
			//�ʒu��ύX
			data.pos.x = 250;
			data.pos.y = WINDOW_HEIGHT / 2 + 220;
			arrow->Draw(&data);		//�`��
		}
	}
	return S_OK;				//������Ԃ�
}