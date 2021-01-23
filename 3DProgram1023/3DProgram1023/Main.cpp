#include <Windows.h>
#include "DirectGraphics.h"
#include "DirectInput.h"

// pragma comment�ɂ��lib�t�@�C���̐ݒ�
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/*
	�E�B���h�E�v���V�[�W��
		�߂�l�F
			LRESULT
				���s����

		�����F
			HWND
				���b�Z�[�W�̑ΏۂƂȂ��Ă���E�B���h�E�̃n���h��

				HWND
					�E�B���h�E�n���h����ۑ�����^

			UINT
				���b�Z�[�W���
					�N���b�N���ꂽ���̏�񂪕ۑ�����Ă���

			WPARAM
				���b�Z�[�W�t����񂻂̂P

			LPARAM
				���b�Z�[�W�t����񂻂̂Q

			�t�����ɂ̓��b�Z�[�W���̕⑫��񂪕ۑ�����Ă���
				��F�L�[�̏ڍ׏��(�啶���A�������Ȃ�)
*/
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		// ���b�Z�[�W�������Ή����Ȃ����Ɏ��s����֐�
		// �����ɂ̓E�B���h�E�v���V�[�W���œn����Ă�����������̂܂ܓn��
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
}

/*
	WinMain
		Windows�A�v���ɂ����ẴG���g���|�C���g

		�߂�l�F
			�A�v���̌���

		�����F
			HINSTANCE hInstance
				�A�v���̃C���X�^���X�n���h��
				H => Handle�̗�

			HINSTANCE hPrevInstance
				�Â��d�l�̎c��Ȃ̂Ŏg��Ȃ�
				���Ƃ��Ẳ��l�͂Ȃ�

			LPSTR lpCmpLine
				�R�}���h���C���̃p�����[�^�̃��X�g

			INT nCmdShow
				�R�}���h���C�������̐�
*/
int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// �E�B���h�E�̐����̓��C�����[�v���n�܂�O�ɍs��
	// �E�B���h�E��������������
	// ���̕����͊o���Ă����K�v����
	// ����ȊO�͌Œ�l�Ŗ��Ȃ�
	WNDCLASS window_class =
	{
		CS_HREDRAW | CS_VREDRAW,			// �N���X�̃X�^�C��(CS_HREDRAW �� CS_VREDRAW�͉��Əc�̕ύX���ݒ�)
		WindowProcedure,					// ���E�B���h�E�v���V�[�W��
		0,									// �⏕������(0�Œ�Ŗ��Ȃ�)
		0,									// �⏕������(0�Œ�Ŗ��Ȃ�)
		hInstance,							// �����̃v���O�����̃C���X�^���X�n���h��
		LoadIcon(nullptr, IDI_APPLICATION), // �A�C�R���摜
		LoadCursor(nullptr, IDC_ARROW),		// �J�[�\���摜
		nullptr,							// �w�i�u���V(�w�i�F)
		nullptr,							// ���j���[��
		// TEXT() char* => wchar_t* �ɕϊ����Ă����
		TEXT("WindowClass"),				// ���N���X��
	};

	// �����������E�B���h�E����o�^����
	/*
		RegisterClass
			�E�B���h�E����o�^����֐�
		
		�߂�l
			��0 => �o�^����
			0 => �o�^���s
	*/
	if (RegisterClass(&window_class) == 0)
	{
		// ���s
		return 0;
	}

	int width = 640;
	int height = 480;

	// �E�B���h�E�쐬
	/*
		���s����
			���� => �E�B���h�E�̃n���h��
			���s => nullptr
	*/
	HWND window_handle = CreateWindow(
		// ���o�^���Ă���E�B���h�E�N���X�\���̖̂��O
		/*
			RegisterWindow�֐��œo�^�����\���̂ɐݒ肵���N���X�����w�肷��
		*/
		TEXT("WindowClass"),
		// ���E�B���h�E��(�^�C�g���o�[�ɕ\������镶����)
		TEXT("�E�B���h�E�����T���v��"),
		// �E�B���h�E�X�^�C��
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		// ���\���ʒu
		/*
			�E�B���h�E��\������ʒu���w�肷��
			(CW_USEDEFAULT => OS�C��)
		*/
		CW_USEDEFAULT,	// x��
		CW_USEDEFAULT,	// y��
		// ���T�C�Y => �E�B���h�E�̃T�C�Y
		width,
		height,
		// �e�n���h��
		NULL,
		// ���j���[�n���h��
		NULL,
		// ���C���X�^���X�n���h��
		// WinMain�̑��������g�p
		hInstance,
		// WM_CREATE���b�Z�[�W��lpparam�ɓn�������l
		NULL);

	RECT window_rect;

	if (GetWindowRect(window_handle, &window_rect) == false)
	{
		// ���s
		return 0;
	}

	RECT client_rect;

	if (GetClientRect(window_handle, &client_rect) == false)
	{
		// ���s
		return 0;
	}

	// �t���[���T�C�Y�̎Z�o
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	// ���T�C�Y�p�T�C�Y�̎Z�o
	int resize_width = frame_size_x + width;
	int resize_height = frame_size_y + height;

	// �E�B���h�E�T�C�Y�̍X�V
	SetWindowPos(
		window_handle,
		nullptr,
		// �\���ʒu
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// �����T�C�Y�̉���
		resize_width,
		// �����T�C�Y�̏c��
		resize_height,
		// �I�v�V�����̎w��
		SWP_NOMOVE
	);

	// �E�B���h�E�\��
	ShowWindow(window_handle, SW_SHOW);

	if (InitDirectGraphics(window_handle) == false)
	{
		return 0;
	}

	if (InitDirectInput() == false)
	{
		return 0;
	}

	if (LoadTexture(TextureID::TexIDBackground) == false)
	{
		return 0;
	}

	LoadXFile(TEXT("Witchwait.X"));

	float pos_x = 0.0f;
	float pos_y = 0.0f;
	float pos_z = 0.0f;
	float angle = 0.0f;

	// ���C�����[�v
	while (true)
	{
		if (IsKeyPushed(DIK_ESCAPE))
		{
			return 0;
		}
		// �t���[���Ǘ�
		/*
			�t���[���Ǘ��̓��[�v�����̎����ɂȂ�悤�ɊǗ�����
			�Ⴆ��60FPS�ŃQ�[�����i�s����Ȃ�
			1��̃��[�v�͖�0.016�b�Ŏ��s����

			��DirectX��60FPS�Ńt���[���Ǘ��������ōs���̂�
			�@�����DirectX�ɊǗ��͔C����
		*/

		// ���b�Z�[�W�Ή�
		/*
			Windows(OS)���瑗���Ă��郁�b�Z�[�W��
			�Ή����s���K�v������

			���b�Z�[�W�̗�
				�E�B���h�E���N���b�N������
				�E�B���h�E���ړ�������
				�E�B���h�E�́~�{�^�����N���b�N����
		*/
		MSG message;
		/*
			PeekMessage
				���b�Z�[�W�̗L���ɂ�����炸���̏��������s����

				�߂�l�F
					true => ���b�Z�[�W�L��
					false => ���b�Z�[�W����

				�������F
					��M�������b�Z�[�W���

				���`��܈����F
					�Œ�l�Ŗ��Ȃ�
		*/
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// ��M�����f�[�^��|�󂷂�
				TranslateMessage(&message);

				// �E�B���h�E�v���V�[�W���ɑ��M����
				DispatchMessage(&message);
			}
		}
		else
		{
			UpdateDirectInput();

			if (IsKeyHeld(DIK_LEFTARROW))
			{
				angle -= 3.0f;
			}
			else if (IsKeyHeld(DIK_RIGHTARROW))
			{
				angle += 3.0f;
			}
			if (IsKeyHeld(DIK_UPARROW))
			{
				float vec_z = cosf(D3DXToRadian(angle));
				float vec_x = sinf(D3DXToRadian(angle));
				float speed = 2.0f;

				pos_x += vec_x * speed;
				pos_z += vec_z * speed;
			}
			// �Q�[���Ɋ֘A���鏈��
			//	�Q�[�������ƕ`�揈�������s����

			//�J�����̐ݒ�
			//SetUpView();

			UpdateTPSCamera(pos_x, pos_y, pos_z, angle);

			StartDrawing();

			//�v���W�F�N�V�����̐ݒ�
			SetUpProjection();

			//3D�I�u�W�F�N�g�̕`��
			Draw3DPorigon();

			//XFile�̕`��
			DrawXFile(pos_x, pos_y, pos_z, angle);

			//2D�I�u�W�F�N�g�̕`��
			//DrawPorigon();

			//DrawPorigonWithTriangleList();

			//DrawPorigonWithTriangleStrip();

			//DrawPorigonWithTriangleFan(TextureID::TexIDBackground);

			FinishDrawing();
		}
	}

	// ���
	ReleaseTexture();
	ReleaseDirectGraphics();
	ReleaseDirectInput();

	return 0;
}
