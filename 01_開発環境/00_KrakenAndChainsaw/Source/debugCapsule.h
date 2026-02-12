//*****************************************
// 
// デバッグ用カプセル
// Author Tetsuji Yamamoto
// 
//*****************************************
#ifndef DEBUGCAPSULE_H_
#define DEBUGCAPSULE_H_

// 前方宣言
class CShapeCapsule;

// デバッグ用カプセルクラス
class CDebugCapsule
{
public:
	// ポイント情報
	typedef struct 
	{
		D3DXQUATERNION quat;// クォータニオン
		D3DXVECTOR3 pos;	// 位置
		D3DXCOLOR col;		// 色
		D3DXMATRIX mtx;		// マトリックス
		float fHeight;		// 高さ
		float fRadius;		// 半径
		int nDeleteTime;	// 消える時間
	}CAPSULE;

	// 生成処理
	static void Create(void)
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CDebugCapsule;
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
	void AddCapsule(const D3DXVECTOR3 posOffset,const D3DXQUATERNION quat,const D3DXCOLOR col,const D3DXMATRIX mtxPerent,const float fHeight,const float fRadius,const int nDeleteTime = 0);

	void SwitchIsDisp(void) { m_bDisp = !m_bDisp; }
	void SwitchIsZtest(void) { m_bZtest = !m_bZtest; }
	void SwitchIsWire(void) { m_bWire = !m_bWire; }

	// 取得処理
	static CDebugCapsule* GetInstance(void)
	{
		return m_pInstance;
	}

private:
	CDebugCapsule();
	~CDebugCapsule();

	static CDebugCapsule* m_pInstance;
	CShapeCapsule* m_pCapsule;			// カプセル
	std::vector<CAPSULE> m_vCapsule;	// カプセル情報
	bool m_bZtest;						// Zテストの有無
	bool m_bDisp;						// 表示の有無
	bool m_bWire;						// ワイヤー表示の有無
};

#endif