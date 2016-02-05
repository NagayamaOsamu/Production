/*===========================================================================================
�T�@���F�Q�[���I�[�o�[��ʂł̏������܂Ƃ߂��N���X�̊֐���`
�쐬���F12��21��
�X�V���F12��21��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "GameOver.h"

//�R���X�g���N�^
GameOver::GameOver()
{
	//�N���X�̏�����
	gameOver = NULL;
}

//�f�X�g���N�^
GameOver::~GameOver()
{
	//�N���X�̉��
	SAFE_DELETE(gameOver);
}

//�@�\�F�ǂݍ��ݏ���
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT GameOver::Load()
{
	//�N���X�𓮓I�ɍ쐬
	gameOver = new Sprite;

	//�Q�[���I�[�o�[�摜�̓ǂݍ���
	if (FAILED(gameOver->Load("Asset\\Sprite\\GAMEOVER.bmp")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�Q�[���I�[�o�[�摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT GameOver::Update()
{
	//�X�y�[�X�L�[�������ꂽ��^�C�g���֖߂�
	/*if (g_pInput->IsKeyTap(DIK_SPACE))
		g_gameScene = SC_TITLE;*/

	return S_OK;			//������Ԃ�
}

//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT GameOver::Render()
{
	//�Q�[���I�[�o�[�摜��`��
	SpriteData data;			//�摜�̏����܂Ƃ߂��\���̂̏���
	//����͏���ύX�����ɂ��̂܂ܕ`��
	gameOver->Draw(&data);

	return S_OK;			//������Ԃ�
}