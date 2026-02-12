//******************************************
// 
// テクスチャ
// Author Tetsuji Yamamoto
// 
//******************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"
#include "FIleName.h"
#include <vector>


#define TEXTURE_FILE_PASS_RANNIGMAN "data\\texture\\runningman102.png"
#define TEXTURE_FILE_PASS_ENEMY_TETSUJI "data\\texture\\ore_001.png"
#define TEXTURE_FILE_PASS_ENEMY_CANNING "data\\texture\\koikasyouyuni_001.png"
#define TEXTURE_FILE_PASS_BULLET "data\\texture\\bullet000.png"
#define TEXTURE_FILE_PASS_BG_0 "data\\texture\\bg100.png"
#define TEXTURE_FILE_PASS_BG_1 "data\\texture\\bg101.png"
#define TEXTURE_FILE_PASS_BG_2 "data\\texture\\bg102.png"
#define TEXTURE_FILE_PASS_NUMBER "data\\texture\\number000.png"
#define TEXTURE_FILE_EXPLOSION "data\\texture\\explosion000.png"
#define TEXTURE_FILE_SHADOW "data\\texture\\shadow000.jpg"

#define TEXTURE_ADD_NUM (10)

// テクスチャクラス
class CTexture
{
public:

	typedef enum
	{
		TEXTURE_RANNINGMAN = 0,
		TEXTURE_ENEMY_CANNING,
		TEXTURE_ENEMY_TETSUJI,
		TEXTURE_BULLET,
		TEXTURE_BG_0,
		TEXTURE_BG_1,
		TEXTURE_BG_2,
		TEXTURE_NUMBER,
		TEXTURE_EXPROSION,
		TEXTURE_SHADOW,
		TEXTURE_MAX
	}TEXTURE;

#define MAX_TEXTURE_NUM (TEXTURE_MAX + TEXTURE_ADD_NUM)

	CTexture();
	~CTexture();

	HRESULT Load(void);
	void Unload(void);
	void Unload(int nIdx);
	int Register(const char* pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	static CTexture* Create();
	static void DebugTexture();
private:
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE_NUM];
	static int m_nNumAll;

	std::vector<CFileName> m_aFileName;	// ファイル名保存用
};

#endif // !_TEXTURE_H_
