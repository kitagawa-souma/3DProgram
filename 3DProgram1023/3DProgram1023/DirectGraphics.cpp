#include "DirectGraphics.h"

//XFile�f�[�^�ۑ��p�̍\����
/*
	�Ȃ��A�w�b�_�t�@�C���ł͂Ȃ��A�\�[�X�t�@�C���ɍ\���̂�錾�������H
*/
struct XFile
{
	DWORD MaterialNum;				//!< ���b�V����
	LPD3DXMESH Meshes;				//!< ���b�V��
	LPD3DXBUFFER Materials;			//!< ���b�V���}�e���A��
	LPDIRECT3DTEXTURE9* Textures;	//!< �e�N�X�`���f�[�^
};

LPDIRECT3D9 g_Interface = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;
// �e�N�X�`���f�[�^
LPDIRECT3DTEXTURE9 g_Textures[TextureID::TexIDMax];

XFile g_XFile;

/*
	TexIDUnityChanIdle,	// UnityChanIdle.png
	TexIDUnityChanRun,	// UnityChanRun.png
	TexIDUnityChanJump,	// UnityChanJump.png
*/
LPCWSTR g_TextureNameList[] =
{
	TEXT("Image_3692d50.jpg"),
};


bool InitDirectGraphics(HWND window_handle)
{
	/*
		Direct3DCreate9
			IDirect3D9�����֐�

			�����F
				DirectX�̃o�[�W����(D3D_SDK_VERSION�Œ�)

			�߂�l�F
				�����F
					IDirect3D9*
				���s�F
					nullptr
	*/
	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr)
	{
		// ���������s
		return false;
	}

	// Device�̏���ݒ肷�邽�߂̍\����
	D3DPRESENT_PARAMETERS parameters;

	// �w�肳�ꂽ�f�[�^���T�C�Y������0�ŏ���������
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	// �o�b�N�o�b�t�@�̐�
	parameters.BackBufferCount = 1;

	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	// D3DFMT_UNKNOWN => �m��܂���
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;

	// �E�B���h�E���[�h�w��
	/*
		true => �E�B���h�E���[�h

		false => �t���X�N���[�����[�h
					���t���X�N���[���͕ʓr�����w�肷��K�v����
	*/
	parameters.Windowed = true;

	/*
		�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̐؂�ւ����@

			D3DSWAPEFFECT_DISCARD => �����ݒ�
	*/
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	/*
		Z�o�b�t�@�@���g���ꍇPresentParameters��
		�g�����Ƃ�ʒm����K�v����
	*/
	//Z�o�b�t�@ON
	//EnableAutoDepthStencil => �f�v�X�A�X�e���V���o�b�t�@�̎g�p�t���O
	parameters.EnableAutoDepthStencil = TRUE;
	//AutoDepthStencilFormat => �f�v�X�A�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	//D3DFMT_D16 => �f�v�X�o�b�t�@��1�f�[�^��16�r�b�g�ō쐬����
	parameters.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = g_Interface->CreateDevice(
		// �f�B�X�v���C�A�_�v�^�̎��
		D3DADAPTER_DEFAULT,
		// �f�o�C�X�̎��
		D3DDEVTYPE_HAL,
		// �f�o�C�X���g�p����E�B���h�E�̃n���h��
		window_handle,
		// �f�o�C�X����̕��@
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		// PRESENT_PARAMETERS
		&parameters,
		// Device�̕ۑ��ϐ��w��
		&g_Device
	);

	/*
		FAILED => HRESULT�̌��ʂ𔻒肵�Ă����}�N��
					true�Ȃ玸�s
					false�Ȃ琬��
	*/
	if (FAILED(result))
	{
		return false;
	}
/*
	// ��������
	if (SUCCEEDED(result))
	{

	}
*/
	return true;
}

void ReleaseDirectGraphics()
{
	if (g_Device != nullptr)
	{
		// Relase => ����֐�
		g_Device->Release();
		// �����̕s���A�N�Z�X�Ή�
		g_Device = nullptr;
	}

	if (g_Interface != nullptr)
	{
		// Release => ����֐�
		g_Interface->Release();
		// �����̕s���A�N�Z�X�Ή�
		g_Interface = nullptr;
	}
}


void StartDrawing()
{
	//�e�N�X�`���u�����h�̐ݒ�(�ȉ��̐ݒ�ł͒��_�J���[�ƃe�N�X�`���̃��̃u�����h�������Ă���)
	/*
		SetTextureStageState
			���F�X�e�[�W��ID
			���F�ݒ�̎��
			��O�F�ݒ�̏��
	*/
	/*
		SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			���F0
			���FD3DTSS_ALPHAOP <= ���l�̍����Ɋւ���ݒ������
			��O�FD3DTOP_MODULATE <= �����͏�Z���g�p����
	*/
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	/*
		SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			���F0
			���FD3DTSS_COLORARG1 <= ��Z�̒l�̐ݒ�
			��O�FD3DTA_TEXTURE <= �e�N�X�`���J���[���g�p����
	*/
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	/*
		SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			���F0
			���FD3DTSS_COLORARG2 <= ��Z�̒l�̐ݒ�
			��O�FD3DTA_DIFFUSE <= Stage�ɑ��M����Ă��������J���[
	*/
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	//���l��L���ɂ���
	/*
		SetRenderState <= �`��̐ݒ��ύX����
			���F�ύX�������ݒ�̎��
			���F�ݒ�̏ڍ�
	*/
	/*
		SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			���FD3DRS_ALPHABLENDENABLE	<= ���u�����h�̐ݒ��ύX����
			���Ftrue						<= �L��(false�Ŗ���)
	*/
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	/*
		SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			���FD3DRS_SRCBLEND			<= �o�b�t�@�ɑ��M�����F�̐ݒ�
			���FD3DBLEND_SRCALPHA			<= �������̂܂܎g��
	*/
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	/*
		SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			���FD3DRS_DESTBLEND			<= �o�b�t�@�̐F�̐ݒ�
			���FD3DBLEND_INVSRCALPHA		<= SRC(�o�b�t�@�ϑ��M�����F)�̃A���t�@�𗘗p����(1 - srcalpha)
	*/
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//���C�e�B���O�𖳌��ɂ���
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�o�b�t�@�̎g�p��ON�ɂ���
	g_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// �o�b�N�o�b�t�@���N���A����
	g_Device->Clear(
		0,										// 0�Œ�
		nullptr,								// nullptr�Œ�
		//D3DCLEAR_ZBUFFER => Z�o�b�t�@���N���A�Ώۂɉ�����
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		// D3DCLEAR_TARGET�Œ�
		D3DCOLOR_XRGB(255, 255, 0),				// �h��Ԃ��F
		1.0f,									// 1.0f�Œ�
		0										// 0�Œ�
	);

	// DirectX�ŕ`����J�n����
	g_Device->BeginScene();
}

void FinishDrawing()
{
	// DirecX�̕`����I������
	g_Device->EndScene();

	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�ɓ]������
	g_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void DrawPorigon()
{
	CustomVertex vertices[] =
	{
		{ 300.0f, 100.0f, 0.0f, 1.0f, 0xffffff },
		{ 500.0f, 300.0f, 0.0f, 1.0f, 0xffffff },
		{ 50.0f, 300.0f, 0.0f, 1.0f, 0xffffff },
	};

	// DirectX�ɒ��_�\������ʒm
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,			// �|���S���̍���
		1,							// �|���S���̐�
		vertices,					// ���_���X�g
		sizeof(CustomVertex)		// 1���_�̃T�C�Y
	);
}

void DrawPorigonWithTriangleList()
{
	CustomVertex vertices[] =
	{
		// 1��
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0xff0000 },
		{ 100.0f, 0.0f, 0.0f, 1.0f, 0xff0000 },
		{ 0.0f, 100.0f, 0.0f, 1.0f, 0xff0000 },

		// 2��
		{ 100.0f, 0.0f, 0.0f, 1.0f, 0x00ff00 },
		{ 100.0f, 100.0f, 0.0f, 1.0f, 0x00ff00 },
		{ 0.0f, 100.0f, 0.0f, 1.0f, 0x00ff00 },
	};

	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		2,						// �`�悷��|���S���� => 2
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawPorigonWithTriangleStrip()
{
	CustomVertex vertices[] =
	{
		{ 300.0f, 0.0f, 0.0f, 1.0f, 0xffffff },
		{ 350.0f, 0.0f, 0.0f, 1.0f, 0x808080 },
		{ 300.0f, 200.0f, 0.0f, 1.0f, 0x808080 },
		{ 350.0f, 200.0f, 0.0f, 1.0f, 0x000000 }
	};

	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,	// D3DPT_TRIANGLESTRIP�Ń|���S���쐬
		2,
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawPorigonWithTriangleFan(TextureID tex_id)
{
	if (g_Textures[tex_id] == nullptr)
	{
		return;
	}

	CustomVertexTex vertices[] = {
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0x80ffffff, 0.0f, 0.0f },
		{ 250.0f, 0.0f, 0.0f, 1.0f, 0x80ffffff, 1.0f, 0.0f },
		{ 250.0f, 450.0f, 0.0f, 1.0f, 0x80ffffff, 1.0f, 1.0f },
		{ 0.0f, 450.0f, 0.0f, 1.0f, 0x80ffffff, 0.0f, 1.0f },
	};

	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	/*
		DirectX(Computer)�ɍ���̃|���S���`��Ŏg�p����e�N�X�`����������
	*/
	g_Device->SetTexture(0, g_Textures[tex_id]);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertices,
		sizeof(CustomVertex)
	);
}

void SetUpView()
{
	D3DXMATRIX matrix;//�J��������ۑ����邽�߂̍s��
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(0.0f, 0.0f, -200.0f);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//LH => Left Hand => ����n�ŃJ�������W��ݒ肷��
	D3DXMatrixLookAtLH( //�J�����������s����쐬����
		&matrix,
		&camera_pos,
		&eye_pos,
		&up_vec
	);

	g_Device->SetTransform(D3DTS_VIEW, &matrix);
}

void SetUpProjection()
{
	D3DXMATRIX matrix;//�v���W�F�N�V��������ۑ����邽�߂̍s��
	float aspect = 640.0f / 480.0f;//�A�X�y�N�g��(�E�C���h�E�T�C�Y�̉�/�c)

	//Perspective => �������e
	D3DXMatrixPerspectiveFovLH(
		&matrix,		//���ʕۑ��p�̍s��̐ݒ�
		D3DXToRadian(60.0f),
		aspect,			//�A�X�y�N�g��
		0.1f,			//�������near�l
		1000.0f			//�������far�l
	);

	//SetTransform => ���W�ϊ��n�̏���DirectX�ɒʒm����
	g_Device->SetTransform(D3DTS_PROJECTION, &matrix);
}

void Draw3DPorigon()
{
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	CustomVertex3D vertices[]
	{
		{-1.0f, -1.0f, 0.0f, 0xffff0000},
		{0.0f, 1.0f, 0.0f, 0xffff0000},
		{1.0f, 1.0f, 0.0f, 0xffff0000}
	};

	g_Device->SetFVF(FVF_3D_VERTEX);

	g_Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, vertices, sizeof(CustomVertex3D));
}

bool LoadTexture(TextureID tex_id)
{
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,					// DirectX9�̃f�o�C�X
		g_TextureNameList[tex_id],	// �t�@�C����
		&g_Textures[tex_id]			// �ǂݍ��܂ꂽ�e�N�X�`����񂪕ۑ������ϐ�
	);

	// FAILED => HRESULT�^�̕ϐ����w�肵�āA���̒l�����s���Ă�����true
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void ReleaseTexture()
{
	for (int i = 0; i < TextureID::TexIDMax; i++)
	{
		if (g_Textures[i] != nullptr)
		{
			g_Textures[i]->Release();
			g_Textures[i] = nullptr;
		}
	}
}

bool LoadXFile(LPCWSTR file_name)
{
	HRESULT hr = D3DXLoadMeshFromX(
		file_name,				//�t�@�C����
		D3DXMESH_SYSTEMMEM,		//���b�V���쐬�̃I�v�V����(D3DXMESH_SYSTEMMEM�Œ�)
		g_Device,				//DirectGraphics�̃f�o�C�X
		nullptr,				//�����nullptr�ŗǂ�
		&g_XFile.Materials,		//�}�e���A���ۑ��p
		nullptr,				//�����nullptr�ŗǂ�
		&g_XFile.MaterialNum,	//�}�e���A���̐��ۑ��p
		&g_XFile.Meshes			//���b�V���f�[�^�ۑ��p
	);
	if (FAILED(hr))
	{
		return false;
	}

	//�}�e���A���̐������e�N�X�`����ۑ��ł���悤�ɂ���
	g_XFile.Textures = new LPDIRECT3DTEXTURE9[g_XFile.MaterialNum];

	//�o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���ĕۑ�
	/*
		Materials�̌^���m�F����Ƃ킩�邪�AMaterials��LPD3DXBUFFER�^�ɂȂ��Ă���
		Buffer�Ƃ����ėp�I�Ƀf�[�^��ۑ�������Ƀ}�e���A�����ۑ�����Ă��邽�߁A
		Material�{���̌^�ɖ߂��K�v������
		
		GetBufferPointer���g�p�����Buffer�̐擪�̃|�C���^���擾�ł���
	*/
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();

	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		if (materials[i].pTextureFilename != nullptr)
		{
			LPSTR char_file_name = materials[i].pTextureFilename;

			//LPSTR(char*) => LPWSTR(wchar_t*)�ϊ�
			wchar_t wtext[256];
			size_t ret;
			mbstowcs_s(&ret, wtext, char_file_name, strlen(char_file_name) + 1);
			LPWSTR file_name = wtext;

			D3DXCreateTextureFromFile(
				g_Device,
				file_name,
				&g_XFile.Textures[i]
			);
		}
		else
		{
			g_XFile.Textures[i] = nullptr;
		}
	}

	return true;
}

void DrawXFile(float pos_x, float pos_y, float pos_z, float angle)
{
	/*float pos_x = 0.0f;
	float pos_y = 0.0f;
	float pos_z = 0.0f;*/

	float scale_x = 1.0f;
	float scale_y = 1.0f;
	float scale_z = 1.0f;

	float rad_x = D3DXToRadian(0.0f);
	float rad_y = D3DXToRadian(angle);
	float rad_z = D3DXToRadian(0.0f);

	D3DXMATRIX matrix;
	D3DXMATRIX scale_matrix;
	D3DXMATRIX trans_matrix;

	D3DXMATRIX rotate_x_matrix;
	D3DXMATRIX rotate_y_matrix;
	D3DXMATRIX rotate_z_matrix;

	D3DXMatrixIdentity(&matrix);

	//�g�k
	D3DXMatrixScaling(&scale_matrix, scale_x, scale_y, scale_z);

	//��]
	D3DXMatrixRotationX(&rotate_x_matrix, rad_x);
	D3DXMatrixRotationY(&rotate_y_matrix, rad_y);
	D3DXMatrixRotationZ(&rotate_z_matrix, rad_z);

	//�ړ�
	D3DXMatrixTranslation(&trans_matrix, pos_x, pos_y, pos_z);

	matrix *= scale_matrix;

	matrix *= trans_matrix;

	matrix *= rotate_x_matrix;
	matrix *= rotate_y_matrix;
	matrix *= rotate_z_matrix;

	g_Device->SetTransform(D3DTS_WORLD, &matrix);

	//�`�揈��
	/*
		XFile�̕`��̓}�e���A���P�ʂōs��
		�e�}�e���A���P�ʂŃ}�e���A���̐ݒ�Ƃ��̃}�e���A���Ŏg���Ă���e�N�X�`���̐ݒ���s��
		���̐ݒ肪���������烁�b�V����`�悷��
	*/
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();
	for (DWORD i = 0; i < g_XFile.MaterialNum; i++)
	{
		//�}�e���A���̐ݒ�
		g_Device->SetMaterial(&materials[i].MatD3D);
		//�e�N�X�`���̐ݒ�
		g_Device->SetTexture(0, g_XFile.Textures[i]);
		//���b�V����`��
		g_XFile.Meshes->DrawSubset(i);
	}
}

void UpdateTPSCamera(float target_pos_x, float target_pos_y, float target_pos_z, float angle)
{
	// �^�[�Q�b�g�̌����x�N�g�������߂�
	float vec_x = sinf(D3DXToRadian(angle));
	float vec_z = cosf(D3DXToRadian(angle));

	// �x�N�g���̔��]
	vec_x *= -1.0f;
	vec_z *= -1.0f;

	// �x�N�g���ɋ������|���Ē�����ω�������
	float distance = 200.0f;
	vec_x *= distance;
	vec_z *= distance;

	// �^�[�Q�b�g�̈ʒu�Ƀx�N�g�������Z�����l��V�����J�����̍��W�Ƃ���
	float new_campos_x = target_pos_x + vec_x;
	float new_campos_y = target_pos_y;
	float new_campos_z = target_pos_z + vec_z;

	D3DXMATRIX matrix; // �J��������ۑ����邽�߂̍s��
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(new_campos_x, new_campos_y, new_campos_z);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(target_pos_x, target_pos_y, target_pos_z);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	/*
		D3DXMatrixLookAtLH
			�J�����������s����쐬����
	*/
	// L => Left H => Hand LH => ����n�ŃJ�������W��ݒ肷��
	D3DXMatrixLookAtLH(
		&matrix,		// ���ʕۑ��p�s��̎w��
		&camera_pos,	// �J�������W
		&eye_pos,		// �����_���W
		&up_vec			// ���[���h���W��ɂ�����J�����̏�����̃x�N�g��
	);

	// SetTransform => ���W�ϊ��n�̏���DirectX�ɒʒm����
	g_Device->SetTransform(D3DTS_VIEW, &matrix);
}

void ReleaseXFile()
{
	//�e�N�X�`���̊J��
	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		if (g_XFile.Textures[i] != nullptr)
		{
			g_XFile.Textures[i]->Release();
			g_XFile.Textures[i] = nullptr;
		}
	}

	//�e�N�X�`���z����J������
	delete[] g_XFile.Textures;

	//�}�e���A���̊J��
	if (g_XFile.Materials != nullptr)
	{
		g_XFile.Materials->Release();
		g_XFile.Materials = nullptr;
	}

	//���b�V���̊J��
	if (g_XFile.Meshes != nullptr)
	{
		g_XFile.Meshes->Release();
		g_XFile.Meshes = nullptr;
	}
}
