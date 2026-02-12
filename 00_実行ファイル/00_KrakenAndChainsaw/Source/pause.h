//***********************************************************
// 
// ポーズ
// Author Tetsuji Yamamoto
// 
//***********************************************************
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "object.h"

// ポーズクラス
class CPause
{
public:
	CPause();
	~CPause();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause* Create(void);

	// セッター
	void SetPause(bool bPause) { m_bPause = bPause; }
	
	// ゲッター
	bool GetPause(void) { return m_bPause; }

private:
	bool m_bPause;
};

#endif // !_PAUSE_H_
