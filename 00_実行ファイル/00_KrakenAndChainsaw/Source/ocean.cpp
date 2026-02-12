//*********************************************************************
// 
// 海
// Author Tetsuji Yamamoto
// 
//*********************************************************************
#include "ocean.h"
#include "compMeshField.h"

//*********************************************************************
// 初期化
//*********************************************************************
HRESULT COcean::Init(void)
{
	// メッシュフィールド用引数
	CMeshField::MeshFIeldArgument meshFiledArgu;
	meshFiledArgu.fWidth = WIDTH;
	meshFiledArgu.fHeight = DIPS;
	meshFiledArgu.posStart.x = -WIDTH * Parameters::HALF;
	meshFiledArgu.posStart.z = DIPS * Parameters::HALF;
	meshFiledArgu.texturePass = "data\\texture\\river000.jpg";
	meshFiledArgu.col = Color::_DARKCYAN;
	meshFiledArgu.col.a = ALPHA;

	// メッシュフィールド
	AddComponent<CMeshField>(meshFiledArgu);

	return S_OK;
}

//*********************************************************************
// 終了
//*********************************************************************
void COcean::Uninit(void)
{
	// コンポーネント破棄
	CObject::Release();
}

//*********************************************************************
// 更新
//*********************************************************************
void COcean::Update(void)
{
	// コンポーネント更新
	CObject::UpdateComponent();
}

//*********************************************************************
// 描画
//*********************************************************************
void COcean::Draw(void)
{
	// コンポーネント描画
	CObject::DrawComponent();
}

//*********************************************************************
// 生成
//*********************************************************************
COcean* COcean::Create(void)
{
	// インスタンス生成
	COcean* pOcean = new COcean;

	// 初期化
	if (pOcean != nullptr) 
	{
		pOcean->Init();
	}

	// 返す
	return pOcean;
}