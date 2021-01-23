#ifndef DIRECT_INPUT_H_
#define DIRECT_INPUT_H_

#include <dinput.h>

enum InputState
{
	NoHold,			//押されていない
	Pushed,			//押した
	Held,			//押している
	Released,		//離した
};

bool InitDirectInput();
void ReleaseDirectInput();
void UpdateDirectInput();
//キーが押されているかをチェックする
bool IsKeyHeld(int key);
bool IsKeyPushed(int key);
bool IsKeyReleased(int key);

#endif
