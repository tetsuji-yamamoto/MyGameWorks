#include "effect.h"
#include "easing.h"
#include "texture.h"

//--------------------------------------------------------------------
// エフェクト

//********************************************************************
// エフェクト
//********************************************************************
CEffect2D::CEffect2D(int nPriority) :CObject2D(nPriority)
{
	m_nLife = 0;
	m_nMaxLife = 0;
	m_fMaxWidth = 0.0f;
	m_fMaxHeight = 0.0f;
	m_MaxCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_move = InitValu::VEC3_NULL;
}

//********************************************************************
// エフェクト
//********************************************************************
CEffect2D::~CEffect2D()
{
}

//********************************************************************
// エフェクト
//********************************************************************
HRESULT CEffect2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move,D3DXCOLOR col, int nLife, float fWidth, float fHeight)
{
	CObject2D::Init(pos, rot, col, fWidth, fHeight);
	m_nLife = nLife;
	m_nMaxLife = nLife;
	m_fMaxWidth = fWidth;
	m_fMaxHeight = fHeight;
	m_MaxCol = col;
	m_move = move;
	m_MaxMove = move;

	return S_OK;
}

//********************************************************************
// エフェクト
//********************************************************************
void CEffect2D::Uninit(void)
{
	CObject2D::Uninit();
}

//********************************************************************
// エフェクト
//********************************************************************
void CEffect2D::Update(void) 
{
	m_nLife--;
}

//********************************************************************
// エフェクト
//********************************************************************
void CEffect2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CObject2D::Draw();

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//********************************************************************
// エフェクト
//********************************************************************
void CEffect2D::SetEffectType(CEffect2D::TYPE type)
{
	CEffect2D::m_type = type;
}

//--------------------------------------------------------------------
// エフェクトノーマル

//********************************************************************
// エフェクトノーマル
//********************************************************************
CEffect2DNomal::CEffect2DNomal()
{
}

//********************************************************************
// エフェクトノーマル
//********************************************************************
CEffect2DNomal::~CEffect2DNomal()
{
}

//********************************************************************
// エフェクトノーマル
//********************************************************************
HRESULT CEffect2DNomal::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight)
{
	// エフェクト2Dクラス初期化
	CEffect2D::Init(pos, InitValu::VEC3_NULL,move, col, nLife, fWidth, fHeight);	
	
	// オブジェクトタイプ設定
	CObject::SetType(CObject::TYPE_EFFECT);

	// エフェクトタイプ設定
	CEffect2D::SetEffectType(CEffect2D::TYPE_NOMAL);		

	// テクスチャンのインデックス設定
	CObject2D::SetIdxTexture(CTexture::TEXTURE_SHADOW);

	return S_OK;
}

//********************************************************************
// エフェクトノーマル
//********************************************************************
void CEffect2DNomal::Uninit(void)
{
	CObject2D::Uninit();
}

//********************************************************************
// エフェクトノーマル
//********************************************************************
void CEffect2DNomal::Update(void)
{
	CEffect2D::Update();
	
	float fWidth = CObject2D::GetWidth();			// 幅取得
	float fHeight = CObject2D::GetHeight();			// 高さ取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();		// 位置取得
	float fMaxWidth = CEffect2D::GetMaxWidth();		// 最大幅取得
	float fMaxHeight = CEffect2D::GetMaxHeight();	// 最大高さ取得
	int nNowLife = CEffect2D::GetLife();			// 今の寿命取得
	int nMaxLife = CEffect2D::GetMaxLife();			// 最大寿命取得
	D3DXCOLOR maxCol = CEffect2D::GetMaxColor();	// 最大色取得
	D3DXVECTOR3 maxMove = CEffect2D::GetMaxMove();
	D3DXVECTOR3 move = CEffect2D::GetMove();

	if (nNowLife <= 0)
	{// 寿命がなくなった
		// 終了
		CEffect2DNomal::Uninit();
		return;
	}

	float fMugLife = (float)nNowLife / (float)nMaxLife;
	float fNowWidth = fMaxWidth * fMugLife;
	float fNowHeight = fMaxHeight * fMugLife;
	D3DXCOLOR nowCol = maxCol;
	nowCol.a = maxCol.a * SelectEasing(EASING_TYPE_EaseInOutCirc,fMugLife);

	move = maxMove * fMugLife;
	pos += move;

	CObject2D::SetWidth(fNowWidth);		// 幅設定
	CObject2D::SetHeight(fNowHeight);	// 高さ設定
	CObject2D::SetColor(nowCol);		// 色設定
	CObject2D::SetPosition(pos);		// 位置設定

	CObject2D::Update();				// ポリゴンの更新
}

//********************************************************************
// エフェクトノーマル
//********************************************************************
void CEffect2DNomal::Draw(void)
{
	CEffect2D::Draw();
}

//****************************************************************************
// エフェクトノーマルクリエイト処理
//****************************************************************************
CEffect2DNomal* CEffect2DNomal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight)
{
	CEffect2DNomal* pEffectN = NULL;
	int nNumAll = GetObujectNumAll();

	// エフェクトノーマルの生成
	if (pEffectN == NULL)
	{
		pEffectN = new CEffect2DNomal;
		if (pEffectN != NULL)
		{
			// 初期化処理
			pEffectN->Init(pos, move,col, nLife, fWidth, fHeight);						// 初期化処理
		}
		else
		{
			MessageBox(NULL, " CEffect2DNomal* CEffect2DNomal::Create() > if ( pEffectN != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CEffect2DNomal* CEffect2DNomal::Create() > if ( pEffectN == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pEffectN;
}

//--------------------------------------------------------------------
// エフェクトショック

//********************************************************************
// エフェクトショック
//********************************************************************
CEffect2DShaking::CEffect2DShaking()
{
	m_fAddRot = 0.0f;
	m_fStartRot = 0.0f;
	m_fShakRadius = 0.0f;
}

//********************************************************************
// エフェクトショック
//********************************************************************
CEffect2DShaking::~CEffect2DShaking()
{
}

//********************************************************************
// エフェクトショック
//********************************************************************
HRESULT CEffect2DShaking::Init(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float fWidth, float fHeight, float fShakRadius, float fAddRot)
{
	D3DXVECTOR3 rot = InitValu::VEC3_NULL;
	D3DXVECTOR3 move = InitValu::VEC3_NULL;
	CEffect2D::Init(pos, rot, move, col, nLife, fWidth, fHeight);	// 初期化
	CObject::SetType(CObject::TYPE_EFFECT);							// オブジェクトタイプ設定
	CEffect2D::SetEffectType(CEffect2D::TYPE_SHAKING);				// エフェクトタイプ設定

	int addrot = (int)(fAddRot * 100.0f);
	int nShakRadius = (int)(fShakRadius * 100.0f);

	m_fAddRot = (float)(rand() % addrot) * 0.001f;
	m_fStartRot = (float)(rand() % 628 - 314) * 0.001f;
	m_fShakRadius = (float)(rand() % nShakRadius) * 0.001f;

	return S_OK;
}

//********************************************************************
// エフェクトショック
//********************************************************************
void CEffect2DShaking::Uninit(void)
{
	CObject2D::Uninit();
}

//********************************************************************
// エフェクトショック
//********************************************************************
void CEffect2DShaking::Update(void)
{
	CEffect2D::Update();

	float fWidth = CObject2D::GetWidth();			// 幅取得
	float fHeight = CObject2D::GetHeight();			// 高さ取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();		// 位置取得
	D3DXVECTOR3 rot = CObject2D::GetRotation();		// 向き取得
	float fMaxWidth = CEffect2D::GetMaxWidth();		// 最大幅取得
	float fMaxHeight = CEffect2D::GetMaxHeight();	// 最大高さ取得
	int nNowLife = CEffect2D::GetLife();			// 今の寿命取得
	int nMaxLife = CEffect2D::GetMaxLife();			// 最大寿命取得
	D3DXCOLOR maxCol = CEffect2D::GetMaxColor();	// 最大色取得

	if (nNowLife <= 0)
	{// 寿命がなくなった
		// 終了
		CEffect2DShaking::Uninit();
		return;
	}

	float fMugLife = (float)nNowLife / (float)nMaxLife;
	float fNowWidth = fMaxWidth * fMugLife;
	float fNowHeight = fMaxHeight * fMugLife;
	D3DXCOLOR nowCol = maxCol;
	nowCol.a = maxCol.a * SelectEasing(EASING_TYPE_EaseInOutCirc, fMugLife);

	D3DXVECTOR3 move = InitValu::VEC3_NULL;

	CObject2D::SetWidth(fNowWidth);		// 幅設定
	CObject2D::SetHeight(fNowHeight);	// 高さ設定
	CObject2D::SetPosition(pos);		// 位置設定
	CObject2D::SetColor(nowCol);		// 色設定
	CObject2D::Update();				// ポリゴンの更新
}

//********************************************************************
// エフェクトショック
//********************************************************************
void CEffect2DShaking::Draw(void)
{
	CEffect2D::Draw();
}

//****************************************************************************
// エフェクトショッククリエイト処理
//****************************************************************************
CEffect2DShaking* CEffect2DShaking::Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float fWidth, float fHeight, float fShakRadius, float fAddRot)
{
	CEffect2DShaking* pEffectS = NULL;
	int nNumAll = GetObujectNumAll();

	// エフェクトショックの生成
	if (pEffectS == NULL)
	{
		pEffectS = new CEffect2DShaking;
		if (pEffectS != NULL)
		{
			// 初期化処理
			pEffectS->Init(pos, col, nLife, fWidth, fHeight, fShakRadius, fAddRot);			// 初期化処理
		}
		else
		{
			MessageBox(NULL, " CEffect2DShaking* CEffect2DShaking::Create() > if ( pEffectS != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CEffect2DShaking* CEffect2DShaking::Create() > if ( pEffectS == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pEffectS;
}