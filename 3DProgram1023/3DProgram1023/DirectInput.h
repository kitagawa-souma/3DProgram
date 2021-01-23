#ifndef DIRECT_INPUT_H_
#define DIRECT_INPUT_H_

#include <dinput.h>

enum InputState
{
	NoHold,			//������Ă��Ȃ�
	Pushed,			//������
	Held,			//�����Ă���
	Released,		//������
};

bool InitDirectInput();
void ReleaseDirectInput();
void UpdateDirectInput();
//�L�[��������Ă��邩���`�F�b�N����
bool IsKeyHeld(int key);
bool IsKeyPushed(int key);
bool IsKeyReleased(int key);

#endif
