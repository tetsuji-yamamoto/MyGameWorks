//**************************************************
// 
// XYZ軸
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "XYZAxis.h"
#include "line.h"

// 静的メンバー変数宣言
bool CXYZAxis::m_bDisp = true;	// 表示切替

//**************************************************
// XYZ軸デストラクタ
//**************************************************
CXYZAxis::CXYZAxis() : CObject(PRIORITY_5)
{
	ZeroMemory(m_aVtx, sizeof(VERTEX_LINE));	// 頂点情報
	m_pos = InitValu::VEC3_NULL;					// 位置
}

//**************************************************
// XYZ軸コンストラクタ
//**************************************************
CXYZAxis::~CXYZAxis()
{
	
}

//**************************************************
// XYZ軸初期化
//**************************************************
HRESULT CXYZAxis::Init(void)
{
	return S_OK;
}

//**************************************************
// XYZ軸初期化
//**************************************************
void CXYZAxis::Init(D3DXVECTOR3 pos)
{
	// X軸
	m_aVtx[0].col = InitValu::COLOR_YELLOW;
	m_aVtx[0].pos = pos;
	m_aVtx[1].col = InitValu::COLOR_YELLOW;
	m_aVtx[1].pos = pos;
	m_aVtx[1].pos.x += LINELENGTH;

	// Y軸
	m_aVtx[2].col = InitValu::COLOR_CYAN;
	m_aVtx[2].pos = pos;
	m_aVtx[3].col = InitValu::COLOR_CYAN;
	m_aVtx[3].pos = pos;
	m_aVtx[3].pos.y += LINELENGTH;

	// Z軸
	m_aVtx[4].col = InitValu::COLOR_MAGENTA;
	m_aVtx[4].pos = pos;
	m_aVtx[5].col = InitValu::COLOR_MAGENTA;
	m_aVtx[5].pos = pos;
	m_aVtx[5].pos.z += LINELENGTH;

	// クオータニオンの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
}

//**************************************************
// XYZ軸終了
//**************************************************
void CXYZAxis::Uninit(void)
{
	Release();
}

//**************************************************
// XYZ軸更新
//**************************************************
void CXYZAxis::Update(void)
{

}

//**************************************************
// XYZ軸描画
//**************************************************
void CXYZAxis::Draw(void)
{
	// 表示しない場合ここで止める
	if (m_bDisp == false)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// Zバッファに書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetFVF(FVF_VERTEX_LINE);

	//ポリゴンの描画
	pDevice->DrawPrimitiveUP(
		D3DPT_LINELIST,
		3,
		&m_aVtx[0],
		sizeof(VERTEX_LINE));

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//***************************************************************
// XYZ軸生成
//***************************************************************
CXYZAxis* CXYZAxis::Create(D3DXVECTOR3 pos)
{
	// XYZ軸インスタンス生成
	CXYZAxis* pXYZAxis = new CXYZAxis;

	// 初期化
	if (pXYZAxis != nullptr)
	{
		pXYZAxis->Init(pos);
	}

	return pXYZAxis;
}

void CXYZAxis::Scaling(float fPlam)
{
	float fLength = LINELENGTH * fPlam;

	if (fPlam <= 0.5f)
	{
		fLength = LINELENGTH;
	}

	// X軸
	m_aVtx[1].pos.x = LINELENGTH * fPlam;

	// Y軸
	m_aVtx[3].pos.y = LINELENGTH * fPlam;

	// Z軸
	m_aVtx[5].pos.z = LINELENGTH * fPlam;
}