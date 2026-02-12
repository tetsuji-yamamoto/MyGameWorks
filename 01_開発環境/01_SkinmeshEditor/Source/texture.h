//******************************************
// 
// テクスチャ
// Author Tetsuji Yamamoto
// 
//******************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"
#include <vector>
#include <map>
#include <string>

// テクスチャクラス
class CTexture
{
public:
	CTexture();
	~CTexture();

	void Uninit(void);
	HRESULT Load(void);
	void UnloadOll(void);
	void Unload(const char* pTexturePass);
	std::string Register(const char* pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(const char* pTexturePass);
	static CTexture* Create();
	static void DebugTexture();

private:
	std::map<std::string, LPDIRECT3DTEXTURE9>m_mTextureInfo;
	static int m_nNumAll;
};

#endif // !_TEXTURE_H_
