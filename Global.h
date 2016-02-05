/*===========================================================================================
�T�@���F�v���W�F�N�g�S�̂ŕK�v�Ȃ��̂��܂Ƃ߂��t�@�C��
�쐬���F6��26��
�X�V���F6��26��
����ҁF�i�R����
=============================================================================================*/
#pragma once

//---------------�C���N���[�h-----------------------
#include <windows.h>
#include <d3dx9.h>
#include "Input.h"


//----------���C�u�����t�@�C���̃��[�h--------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//-------------------�萔�錾------------------
//�E�B���h�E�T�C�Y
#define WINDOW_WIDTH	1280	//�E�B���h�E�̕�
#define WINDOW_HEIGHT	 720    //�E�B���h�E����
#define ENEMY_LIMIT		  5		//�G�̍ő吔
#define CHAR_SIZE		 64		//���@�ƓG�̃T�C�Y

//-------------------�}�N��----------------------
#define SAFE_DELETE(p) {delete(p);	(p) = NULL;}						//���I�ɍ�������m�����S�Ƀf���[�g����
#define SAFE_DELETE_ARRAY(p) {delete[] (p);  (p) = NULL;}				//�z��p
#define SAFE_RELEASE(p) {if(p != NULL){(p)->Release(); (p) = NULL;}}	//LP�^�p

//-------------------�O���[�o���ϐ�------------------
//�V�[��
enum GAME_SCENE
{
	SC_TITLE,			//�^�C�g���V�[��
	SC_STAGE_SELECT,	//�X�e�[�W�I���V�[��
	SC_STAGE_EDITOR,	//�X�e�[�W�쐬�V�[��
	SC_PLAY,			//�v���C�V�[��
	SC_CLEAR,			//�N���A�V�[��
	SC_GAMEOVER,		//�Q�[���I�[�o�[�V�[��
	SC_MAX
};
extern GAME_SCENE	g_gameScene;		//���Ԃ�Game.cpp�Ő錾

//Direct3D�f�o�C�X�I�u�W�F�N�g
extern LPDIRECT3DDEVICE9 g_pDevice;

//���͏����I�u�W�F�N�g
extern Input* g_pInput;	//�N���X�����Ƃ��͊�{�I�Ƀ|�C���^�ɂ��������ǂ�

//�J�����I�u�W�F�N�g
class Camera;				//�J�����N���X��Global.h���C���N���[�h���Ă���̂ł��̃w�b�_�[��Camera�N���X���C���N���[�h�ł��Ȃ��̂ŃN���X�̐錾������
extern Camera* g_pCamera;	//���Ő錾���Ă����΃|�C���^�͎g����̂Ń|�C���^�Ő錾