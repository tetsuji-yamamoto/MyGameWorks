//**********************************************
// 
// レンダラー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "main.h"

// レンダラークラス
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd,BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
	LPDIRECT3D9 m_pD3D;						//Direct3Dオブジェクトへのポインタ
	static LPDIRECT3DDEVICE9 m_pD3DDevice;	//Direct3Dデバイスへのポインタ
};
#endif