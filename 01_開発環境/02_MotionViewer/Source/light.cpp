//****************************************
// 
// ライト
// Author Tetsuji Yamamoto
// 
//****************************************
#include "light.h"
#include "renderer.h"

//****************************************
// ライトコンストラクタ
//****************************************
CLight::CLight()
{
	ZeroMemory(m_aLight, sizeof(m_aLight));
}

//****************************************
// ライトデストラクタ
//****************************************
CLight::~CLight()
{

}

//****************************************
// ライト初期化処理
//****************************************
HRESULT CLight::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	//ライトの方向ベクトル
	D3DXVECTOR3 vecDir[3];

	for (int nCntLight = 0; nCntLight < 3; nCntLight++)
	{
		//ライトをクリアする
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//ライトの方向を設定
		vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
		vecDir[1] = D3DXVECTOR3(-12.0f, -14.0f, 15.0f);
		vecDir[2] = D3DXVECTOR3(-5.0f, -5.57f, -3.3f);

		//vecDir[0] = D3DXVECTOR3(0.0f, -1.0f, 0.0);
		//vecDir[1] = D3DXVECTOR3(0.0f, -1.0f, 0.0);
		//vecDir[2] = D3DXVECTOR3(0.0f, -1.0f, 0.0);

		//正規化する(大きさ１のベクトルにする)
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);
		m_aLight[nCntLight].Direction = vecDir[nCntLight];

		//ライトを設定する
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//****************************************
// ライト終了処理
//****************************************
void CLight::Uninit(void)
{

}

//****************************************
// ライト更新処理
//****************************************
void CLight::Update(void)
{

}