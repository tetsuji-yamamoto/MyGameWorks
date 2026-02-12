//*****************************************
// 
// デバッグ用点
// Author Tetsuji Yamamoto
// 
//*****************************************
#ifndef DEBUGPOINT_H_
#define DEBUGPOINT_H_

class CSphere;

// デバッグ用点クラス
class CDebugPoint
{
public:
	// ポイント情報
	typedef struct
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXCOLOR col;		// 色
		float fRadius;		// 半径
		int nDeleteTime;	// 消える時間
	}POINT;

	// 生成処理
	static void Create(void)
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CDebugPoint;
			m_pInstance->Init();
		}
	}

	// 破棄処理
	static void Delete(void)
	{
		if (m_pInstance != nullptr)
		{
			m_pInstance->Uninit();
			m_pInstance = nullptr;
		}
	}

	void Init(void);
	void Uninit(void);
	void Draw(void);
	void AddPoint(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fRadius = 10.0f, int nDeleteTime = 0);

	void SwitchIsDisp(void) { m_bDisp = !m_bDisp; }
	void SwitchIsZtest(void) { m_bZtest = !m_bZtest; }

	// 取得処理
	static CDebugPoint* GetInstance(void)
	{
		return m_pInstance;
	}

private:
	CDebugPoint();
	~CDebugPoint();
	
	static CDebugPoint* m_pInstance;
	CSphere* m_pSpher;				// 球
	std::vector<POINT> m_vPoint;	// 位置情報
	bool m_bZtest;					// Zテストの有無
	bool m_bDisp;					// 表示の有無
};

#endif