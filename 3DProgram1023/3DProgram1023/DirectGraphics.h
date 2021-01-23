#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

// DirectGraphics�ŕK�v�Ȋ֐�����`����Ă���w�b�_�t�@�C��
#include <d3d9.h>
#include <d3dx9.h>

struct CustomVertex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;
};

struct CustomVertexTex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;

	float Tu;
	float Tv;
};
/*
	DrawPrimitiveUp�Ȃǂ�DirectX��CustyomVertex���g�p�����֐��ɂ���
		���ۂ�3D�`����s���Ă���
		3D�`��Ȃ̂Ŗ{���̓��[���h���W�ϊ��A�r���[���W�ϊ��A�v���W�F�N�V�������W�ϊ��̂��߂�
		�ݒ���s���K�v������
		
		�������A2D�`����s�������ꍇ�͂킴�킴��L�̕ϊ��ݒ������̂��ʓ|
		�����ŁARHW�����g�p���Ďw�肳�ꂽCostomVertex�̏�����C�ɃX�N���[�����W�ɕϊ����Ă���
*/
struct CustomVertex3D
{
	//���W���
	float X;
	float Y;
	float Z;

	//�J���[
	DWORD Color;
};

//FVF�ݒ�(2D�p�e�N�X�`���Ȃ�)
#define FVF_2D_PORIGON (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
//FVF�ݒ�(2D�p�e�N�X�`������)
#define FVF_2D_PORIGON_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//FVF�ݒ�(3D�p)
#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

/*
	
*/
#define ABC (1.0f)

// �e�N�X�`���̎�ޕ���enum���쐬
enum TextureID
{
	TexIDBackground,
	TexIDMax,
};

/**
* @brief DirectGraphics�������֐�
* @details �`����g�p���邽�߂̏��������s��
* @retval true ����������
* @retval false ���������s
* @param[in] window_handle �`��Ɏg�p����window�n���h��
*/
bool InitDirectGraphics(HWND window_handle);

void ReleaseDirectGraphics();

void StartDrawing();

void FinishDrawing();

void DrawPorigon();

void DrawPorigonWithTriangleList();
void DrawPorigonWithTriangleStrip();
void DrawPorigonWithTriangleFan(TextureID tex_id);

//View���W�ϊ��̐ݒ�
void SetUpView();
//�v���W�F�N�V�������W�ϊ��̐ݒ�
void SetUpProjection();
//3D�|���S���`��
void Draw3DPorigon();

void UpdateTPSCamera();

void DrawXFile(float pos_x, float pos_y, float pos_z, float angle);

void UpdateTPSCamera(float target_pos_x, float target_pos_y, float target_pos_z, float angle);

bool LoadTexture(TextureID tex_id);
void ReleaseTexture();
void ReleaseXFile();

bool LoadXFile(LPCWSTR file_name);

#endif
