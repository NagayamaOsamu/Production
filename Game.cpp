/*=======================================================================================================
�T�@���F�Q�[���S�̗̂������������
�쐬���F6��26��
�X�V���F6��26��
����ҁF�i�R����
=======================================================================================================*/

//---------------�C���N���[�h-----------------------
#include "Game.h"

//�e�V�[���̃C���N���[�h
#include "TitleScene.h"			//�^�C�g���V�[��
#include "StageSelectScene.h"	//�X�e�[�W�I���V�[��
#include "StageEditorScene.h"	//�X�e�[�W�쐬�V�[��
#include "PlayScene.h"			//�v���C�V�[��
#include "ClearScene.h"			//�N���A�V�[��
#include "GameOverScene.h"		//�Q�[���I�[�o�[�V�[��

//�J�����N���X
#include "Camera.h"

//----------------�O���[�o���ϐ�----------------
GAME_SCENE		  g_gameScene;		//���݂̃Q�[���V�[��
LPDIRECT3DDEVICE9 g_pDevice;		//Direct3D�f�o�C�X�I�u�W�F�N�g
Input*			  g_pInput;			//���͏����I�u�W�F�N�g
Camera*           g_pCamera;		//�J�����I�u�W�F�N�g  

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�R���X�g���N�^
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
Game::Game()
{
	//�e�V�[���I�u�W�F�N�g�̐���
	scene[SC_TITLE]		   =	new TitleScene;			//�^�C�g���V�[��
	scene[SC_STAGE_SELECT] =    new StageSelectScene;	//�X�e�[�W�I���V�[��
	scene[SC_STAGE_EDITOR] =    new StageEditorScene;	//�X�e�[�W�쐬�V�[��
	scene[SC_PLAY]		   =	new PlayScene;			//�v���C�V�[��
	scene[SC_CLEAR]		   =	new ClearScene;			//�N���A�V�[��
	scene[SC_GAMEOVER]	   =	new GameOverScene;		//�Q�[���I�[�o�[�V�[��

	//�ŏ��̓^�C�g���V�[��
	g_gameScene = SC_STAGE_SELECT;

	//�J�����N���X�𓮓I�쐬
	g_pCamera = new Camera;
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�f�X�g���N�^
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
Game::~Game()
{
	for (int i = 0; i < SC_MAX; i++)
	{
		delete scene[i];
	}

	//DirectX�J��
	SAFE_DELETE(g_pCamera);		//�J�����N���X
	SAFE_RELEASE(g_pDevice);	//�f�o�C�X�I�u�W�F�N�g
	SAFE_RELEASE(pD3d);			//Direct3D�I�u�W�F�N�g
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// Direct3D������
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::InitD3d(HWND hWnd)
{
	// �uDirect3D�v�I�u�W�F�N�g�̍쐬
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))		//Direct3D���쐬���Ď��s���ĂȂ�������
	{
		MessageBox(0, "Direct3D�̍쐬�Ɏ��s���܂���", "", MB_OK);		//���s���Ă����烁�b�Z�[�W�{�b�N�X�Ŏ��s��\��

		return E_FAIL;		//���s�������Ƃ�Ԃ�
	}


	// �uDIRECT3D�f�o�C�X�v�I�u�W�F�N�g�̍쐬(�\����)
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));								//������
	d3dpp.BackBufferFormat =			   D3DFMT_UNKNOWN;			//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
	d3dpp.BackBufferCount =				   1;						//�p�ӂ���o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect =					   D3DSWAPEFFECT_DISCARD;	//�X���b�v
	d3dpp.Windowed =					   TRUE;					//�f�B�X�v���C�S�̂ɊG��\�����������Ƃ���FALSE
	d3dpp.EnableAutoDepthStencil =		   TRUE;					//Z�o�b�t�@(���s���j���g��(TURE)���g��Ȃ�(FALSE)��
	d3dpp.AutoDepthStencilFormat =		   D3DFMT_D16;				//�o�b�t�@�̃t�H�[�}�b�g

	//�Q�[����ʂ̍쐬
	if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice)))				//���ɂ���Ă�Direct3D�̍쐬�Ɏ��s����\�������邽��if���ɂ���
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);

		//�p�t�H�[�}���X�𗎂Ƃ��čēx�쐬
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice)))			//����ł����s����\��������
		{
			//���s��m�点��
			MessageBox(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
			return E_FAIL;			//���s��Ԃ�
		}
	}
	
	//���C�g��Off�ɂ���
	//if (FAILED(LigtOff()))
	//{
	//	return E_FAIL;				//���s��Ԃ�
	//}
	
	//�J�����O���[�h�̐ݒ���s��
	//if (FAILED(CullingConfig()))
	//{
	//	return E_FAIL;				//���s��Ԃ�
	//}

	//�A���t�@�u�����h�̐ݒ���s��
	if (FAILED(AlphaConfig()))
	{
		return E_FAIL;				//���s��Ԃ�
	}

	//�e�N�X�`���s��̐ݒ���s��
	if (FAILED(TexMatConfig()))
	{
		return E_FAIL;				//���s��Ԃ�
	}

	//���C�g�̏������ݒ�
	if (FAILED(InitLight()))
	{
		return E_FAIL;		//���s��Ԃ�
	}

	
	return S_OK;				//������Ԃ�

}


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�ǂݍ��ݏ���
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::Load()
{	
	//�S�ẴV�[���̓ǂݍ���
	for (int i = 0; i < SC_MAX; i++)
	{
		//�ǂ����Ŏ��s���Ă�����
		if (FAILED(scene[i]->Load()))
		{
			return E_FAIL;	//���s��Ԃ��iMain�֐��Ɂj
		}
	}
	return S_OK;
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�Փ˔���
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::Hit()
{
	//�ǂ����Ŏ��s���Ă�����
	if (FAILED(scene[g_gameScene]->Hit()))
	{
		return E_FAIL;	//���s���Ă�����
	}
	return S_OK;	//������ς���
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�X�V����
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::Update()
{
	//���͏����̍X�V�����i�L�[�̏�Ԃ𒲂ׂ�j
	if (g_pInput)
	{
		//�C���v�b�g�N���X�����݂��Ă���Ƃ������Ă�
		g_pInput->Update();
	}

	//�ǂ����Ŏ��s���Ă�����
	if (FAILED(scene[g_gameScene]->Update()))
	{
		return E_FAIL;	//���s��Ԃ�
	}

	//�J�����N���X�̍X�V�������Ăяo��
	if (FAILED(g_pCamera->Update()))
	{
		return E_FAIL;	//���s��Ԃ�
	}

	return S_OK;		//������Ԃ�
}

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//	�`�揈��
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
HRESULT Game::Render()
{
	//��ʂ��N���A(XRGB�ŉ�ʂ̐F��@ZBUFFER��z�o�b�t�@���N���A�ɂ��Ă���j
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pDevice->BeginScene()))		//DirectX���Ƃ����Ŏ��s����Ƃ�������̂Ő������������肷��
	{

		//�Q�[����ʂ̕`��
		if (FAILED(scene[g_gameScene]->Render()))	//�ǂ����Ŏ��s���Ă�����
		{
			return E_FAIL;	//���s��Ԃ�
		}

		//�`��I��
		g_pDevice->EndScene();
		//BeginScene��EndScene��BeginPaint��EndPaint�̂悤�ȃC���[�W�ł悢
	}
	
	//�t���b�v
	//Direct3D�ł�2���̉�ʁi�t�����g�o�b�t�@�A�o�b�N�o�b�t�@�j��p�ӂ��ăo�b�N�o�b�t�@�̂ق��ɉ摜��`�悷��
	//�S�Ẳ摜��`�悵����A�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@���������邱����t���b�v�Ƃ���
	g_pDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;	//������Ԃ�
}

//�@�\�F���C�g��Off�ɂ���
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Game::LigtOff()
{
	//���C�g��OFF��Ԃɂ���
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))			//��1�������u�����v��2�������u�ǂ����邩�v�ɂȂ��Ă���
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}

	return S_OK;				//������Ԃ�

}

//�@�\�F�J�����O���[�h�̐ݒ�
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Game::CullingConfig()
{
	//�J�����O���[�h(�|���S���̗��ʂ�\�����Ȃ�����)��OFF�ɂ���
	if (FAILED(g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�J�����O���[�h�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}

	return S_OK;				//������Ԃ�
}

//�@�\�F�A���t�@�u�����h�̐ݒ���s��
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Game::AlphaConfig()
{
	//�A���t�@�u�����h��L���ɂ��Ĕ������̕��������̂܂ܔ������ɕ\��������
	//SetRendeerState��3��Ă�Őݒ肷��
	if (FAILED(g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)		||			//1�ڂ̂ŃA���t�@�u�����h�̐ݒ�
			   g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) ||			//2�ڂō�����g���e�N�X�`�����̓����x�����߂�
			   g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))		//3�ڂŔw�i�̐F��2�ڂ̓����x�̋t�̓����x�ɂ���
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�ŕ\��
		MessageBox(0, "�A���t�@�u�����h�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}

	return S_OK;				//������Ԃ�
}

//�@�\�F�e�N�X�`���s��̐ݒ���s��
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Game::TexMatConfig()
{
	//�e�N�X�`���s��(�e�N�X�`���̐؂���Ȃǂ��s����悤�ɂ���)���g����悤�ɂ���
	if (FAILED(g_pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2)))		//��2�����́u�����v�A��3�����́u�ǂ����邩�v
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�e�N�X�`���X�e�[�g�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);

		return E_FAIL;			//���s��Ԃ�
	}

	return S_OK;				//������Ԃ�

}

//�@�\�F���C�g�̏����ݒ�
//�����F�Ȃ�
//�ߒl�F���s��������
HRESULT Game::InitLight()
{
	
	D3DLIGHT9 light;							//���C�g�̐ݒ���܂Ƃ߂��\���̂̏���
	ZeroMemory(&light, sizeof(D3DLIGHT9));		//�\���̂�������

	//���C�g�̐ݒ�
	light.Type = D3DLIGHT_DIRECTIONAL;						//���C�g�̎�ނ��f�B���N�V���i�����C�g(���s���C�g)
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 1.0f);		//���C�g�̌�����

	//���C�g�̐F(����)���O�`�P�͈̔͂�(�P�𒴂��Ă��ǂ�)
	light.Diffuse.r = 1.0f;									//��				
	light.Diffuse.g = 1.0f;									//��
	light.Diffuse.b = 1.0f;									//��

	//�A���r�G���g�̐ݒ�(�ꗥ�Ŗ��邭���郉�C�g�̐ݒ�)
	light.Ambient.r = 0.4f;									//��
	light.Ambient.g = 0.4f;									//��
	light.Ambient.b = 0.4f;									//��

	//���Őݒ肵���l�Ń��C�g���쐬
	if (FAILED(g_pDevice->SetLight(0, &light)))				//������(���C�g�̔ԍ�(���ڂ̃��C�g��),�\���̂̃A�h���X)
	{
		//�ݒ�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���C�g���Z�b�g�ł��܂���ł���", "Game", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//���C�g�̐ݒ��ON�ɂ���
	if (FAILED(g_pDevice->LightEnable(0, TRUE)))
	{
		//���C�g��ON�ł��Ȃ������烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���C�g��L���ɂł��܂���ł���", "Game", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	return S_OK;		//������Ԃ�
}