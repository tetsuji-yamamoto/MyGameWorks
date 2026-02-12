//**********************************************
// 
// スタートボタン
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _SSTART_BUTTON_H_
#define _SSTART_BUTTON_H_

#include "object.h"
#include "button.h"

// スタートボタンクラス
class CStart_button : public CObject
{
public:
	CStart_button();
	~CStart_button();

	HRESULT	Init	(void);
	void	Init	(D3DXVECTOR3 pos);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

private:
	CButton m_button;	// ボタン
};

#endif
