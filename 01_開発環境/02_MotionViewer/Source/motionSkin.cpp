//*******************************************************************************************
// 
// モーション(スキンメッシュ)
// Author Tetsuji Yamamoto
// 
//*******************************************************************************************
#include "motionSkin.h"
#include "manager.h"
#include "myMath.h"
#include "compWeapon.h"
#include "compCollisionModel.h"

using namespace std;
using namespace nlohmann;

//*******************************************************************************************
// モーション(スキンメッシュ)コンストラクタ
//*******************************************************************************************
CMotionSkin::CMotionSkin()
{
	m_pPlayer = nullptr;				// エンティティ
	m_nNumBone = 0;							// ボーン数
	m_nNumVtx = 0;							// 頂点数
	m_nNumMotion = 0;					// モーションの総数

	ZeroMemory(&m_motionInfo,sizeof(MOTIONINFO)) ;		// モーションで使う情報
	ZeroMemory(&m_motionInfoBlend,sizeof(MOTIONINFO)) ;	// モーションで使う情報ブレンド用

	m_bBlend = false;						// モーションブレンドの有無
	m_nBlendTime = 0;						// ブレンド時間
	m_nCntBlend = 0;						// ブレンドカウンター
	m_nTypeBlendMotion = 0;					// ブレンドするモーションタイプ

	ZeroMemory(&m_model,sizeof(m_model));	// モデル情報
}

//*******************************************************************************************
// モーション(スキンメッシュ)デストラクタ
//*******************************************************************************************
CMotionSkin::~CMotionSkin()
{

}

//*******************************************************************************************
// モーション(スキンメッシュ)初期化バイナリ
//*******************************************************************************************
void CMotionSkin::InitBinary(std::string motionFilePathBinary)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// 読み取り用としてファイルを開く
	ifstream ifs(motionFilePathBinary, ios::binary);

	// ファイルが開けなかった
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "CMotionSkin::Init() > ファイルが開けませんでした", "Error", MB_OK);
		return;
	}

	// エンティティ破棄
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	// エンティティ生成
	m_pPlayer = new CEntity;

	// ファイルが開けたら
	string str;				// 文字列
	CBone::BONE boneRead;	// 読込用のボーン
	VERTEXSKIN vtxSkinRead;	// 頂点読込
	MOTION motionRead;		// モーション読込
	KEYSET keySetRead;		// モーション読込
	KEY keyRead;			// モーション読込

	// Xファイルの相対パスの文字数
	int nXfilePathSize;
	ifs.read((char*)&nXfilePathSize, sizeof(int));

	// 武器コンポーネント数読込
	int nNumCompWeapon;
	ifs.read((char*)&nNumCompWeapon, sizeof(int));

	//	当たり判定用モデルコンポーネント数読込
	int nNumCompCollModel;
	ifs.read((char*)&nNumCompCollModel, sizeof(int));

	// ボーンの数
	ifs.read((char*)&m_nNumBone, sizeof(int));

	// 頂点数
	ifs.read((char*)&m_nNumVtx, sizeof(int));

	// モーション数
	ifs.read((char*)&m_nNumMotion, sizeof(int));

	// Xファイルの相対パス
	for (int nCntFilePath = 0; nCntFilePath < nXfilePathSize; nCntFilePath++)
	{
		char cData;
		ifs.read((char*)&cData, sizeof(char));
		m_sXFilePass += cData;
	}

	// 武器コンポーネント読込
	for (int nCntCompWeapon = 0; nCntCompWeapon < nNumCompWeapon; nCntCompWeapon++)
	{
		CWeapon* weaponRead = new CWeapon;
		string name;
		string XFilePath;
		int nIdxBone;
		D3DXVECTOR3 pos;
		D3DXQUATERNION quat;

		// 名前文字数読込
		int nNumName = 0;
		ifs.read((char*)&nNumName, sizeof(int));

		// 文字読込
		for (int nCntName = 0; nCntName < nNumName; nCntName++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			name += cData;
		}

		// Xファイルパス文字数読込
		int nNumXFilePath = 0;
		ifs.read((char*)&nNumXFilePath, sizeof(int));

		// 文字読込
		for (int nCntXFilePath = 0; nCntXFilePath < nNumXFilePath; nCntXFilePath++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			XFilePath += cData;
		}

		// ボーンインデックス読み取り
		ifs.read((char*)&nIdxBone, sizeof(int));

		// 位置読み取り
		ifs.read((char*)&pos, sizeof(D3DXVECTOR3));

		// 向き読み取り
		ifs.read((char*)&quat, sizeof(D3DXQUATERNION));

		// 情報設定
		weaponRead->SetName(name);
		weaponRead->SetXFilePath(XFilePath);
		weaponRead->SetIdxBone(nIdxBone);
		weaponRead->SetPosition(pos);
		weaponRead->SetQuat(quat);

		// 初期化
		weaponRead->Init();

		// コンポーネントに要素追加
		m_pPlayer->AddComponentPushBack(weaponRead);
	}

	// 当たり判定用モデルコンポーネント読込
	for (int nCntCompCollModel = 0; nCntCompCollModel < nNumCompCollModel; nCntCompCollModel++)
	{
		CCollisionModel* collModelRead = new CCollisionModel;
		string name;
		string XFilePath;
		int nIdxBone;
		D3DXVECTOR3 pos;
		D3DXQUATERNION quat;

		// 名前文字数読込
		int nNumName = 0;
		ifs.read((char*)&nNumName, sizeof(int));

		// 文字読込
		for (int nCntName = 0; nCntName < nNumName; nCntName++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			name += cData;
		}

		// Xファイルパス文字数読込
		int nNumXFilePath = 0;
		ifs.read((char*)&nNumXFilePath, sizeof(int));

		// 文字読込
		for (int nCntXFilePath = 0; nCntXFilePath < nNumXFilePath; nCntXFilePath++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			XFilePath += cData;
		}

		// ボーンインデックス読み取り
		ifs.read((char*)&nIdxBone, sizeof(int));

		// 位置読み取り
		ifs.read((char*)&pos, sizeof(D3DXVECTOR3));

		// 向き読み取り
		ifs.read((char*)&quat, sizeof(D3DXQUATERNION));

		// 情報設定
		collModelRead->SetName(name);
		collModelRead->SetXFilePath(XFilePath);
		collModelRead->SetIdxBone(nIdxBone);
		collModelRead->SetPosition(pos);
		collModelRead->SetQuat(quat);

		// 初期化
		collModelRead->Init();

		// コンポーネントに要素追加
		m_pPlayer->AddComponentPushBack(collModelRead);
	}

	// ボーン情報
	for (int nCntBone = 0; nCntBone < m_nNumBone; nCntBone++)
	{
		CBone::BONE bone;
		ifs.read((char*)&bone, sizeof(CBone::BONE));

		// 要素追加
		m_vBone.push_back(bone);

		// ボーンモデルの生成
		CBone* pBoneModel = CBone::Create();

		// リストに追加
		m_vpBoneModel.push_back(pBoneModel);
	}

	// 頂点情報
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		VERTEXSKIN vtxEdit;
		ifs.read((char*)&vtxEdit, sizeof(VERTEXSKIN));

		// 要素追加
		m_vVtxSkin.push_back(vtxEdit);
	}

	// モーション情報
	for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
	{
		MOTION motion;

		// ループ
		int nLoop = 0;
		ifs.read((char*)&nLoop, sizeof(int));
		motion.bLoop = (bool)nLoop;

		// キーセットの数
		ifs.read((char*)&motion.nNumKeyInfo, sizeof(int));

		// キーセット
		for (int nCntKeySet = 0; nCntKeySet < motion.nNumKeyInfo; nCntKeySet++)
		{
			KEYSET keySet;

			// フレーム数
			ifs.read((char*)&keySet.nFlame, sizeof(int));

			// キー数
			ifs.read((char*)&keySet.nNumkey, sizeof(int));

			// キー
			for (int nCntKey = 0; nCntKey < keySet.nNumkey; nCntKey++)
			{
				KEY key;
				ifs.read((char*)&key.fPosX, sizeof(float));
				ifs.read((char*)&key.fPosY, sizeof(float));
				ifs.read((char*)&key.fPosZ, sizeof(float));
				ifs.read((char*)&key.quat, sizeof(D3DXQUATERNION));

				// 要素追加
				keySet.vKey.push_back(key);
			}

			// 要素追加
			motion.vKeySet.push_back(keySet);
		}

		// 要素追加
		m_vMotion.push_back(motion);
	}

	// ファイルを閉じる
	ifs.close();

	// Xファイル読込
	HRESULT hr;
	hr = D3DXLoadMeshFromX(
		m_sXFilePass.c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&m_model.pBuffMat,
		NULL,
		&m_model.dwNumMat,
		&m_model.pMesh
	);

	if (FAILED(hr))
	{// 失敗したら
		MessageBox(NULL, "CMotionSkin::Init() > モデルを読み込めませんでした", "Error", MB_OK);

		// 結果を返す
		return;
	}

	//マテリアル辺のポインタを取得
	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

	// テクスチャインデックスのメモリ確保
	m_model.apIdxTexture = new int[(int)m_model.dwNumMat];

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMat; nCntMat++)
	{
		m_model.apIdxTexture[nCntMat] = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
	}

	// ボーンの親と子の設定
	for (auto& iterBone : m_vBone)
	{
		// 子のポインタを設定
		if (iterBone.nIdxChild != -1)
		{
			auto iterBoneChild = m_vBone.begin() + iterBone.nIdxChild;
			iterBone.pBoneChild = &*iterBoneChild;
		}

		// 親のポインタを設定
		if (iterBone.nIdxParent != -1)
		{
			auto iterBoneParent = m_vBone.begin() + iterBone.nIdxParent;
			iterBone.pBoneParent = &*iterBoneParent;
		}
	}

	// ボーンの親と子の設定
	for (auto& iterBone : m_vBoneReset)
	{
		// 子のポインタを設定
		if (iterBone.nIdxChild != -1)
		{
			auto iterBoneChild = m_vBoneReset.begin() + iterBone.nIdxChild;
			iterBone.pBoneChild = &*iterBoneChild;
		}

		// 親のポインタを設定
		if (iterBone.nIdxParent != -1)
		{
			auto iterBoneParent = m_vBoneReset.begin() + iterBone.nIdxParent;
			iterBone.pBoneParent = &*iterBoneParent;
		}
	}

	// コンポーネントの各種設定
	for (auto& iterComp : m_pPlayer->GetComponentVector())
	{
		// 武器の設定
		if (auto* weapon = dynamic_cast<CWeapon*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = weapon->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					weapon->SetParentBone(&iterBone);
					break;
				}
			}
		}
		// 当たり判定用モデルの設定
		else if (auto* collModel = dynamic_cast<CCollisionModel*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = collModel->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					collModel->SetParentBone(&iterBone);
					break;
				}
			}
		}
	}
}

//*******************************************************************************************
// モーション(スキンメッシュ)初期化
//*******************************************************************************************
void CMotionSkin::Init(std::string motionFilePath)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// 読み取り用としてファイルを開く
	ifstream ifs(CManager::GetFilePathMotionSkinRead().c_str());

	// ファイルが開けなかった
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "CEdit::Read() > ファイルが開けませんでした", "Error", MB_OK);
		return;
	}

	// エンティティ破棄
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	// エンティティ生成
	m_pPlayer = new CEntity;

	// ファイルが開けたら
	string str;				// 文字列
	char cData;				// 一文字
	CBone::BONE boneRead;	// 読込用のボーン
	VERTEXSKIN vtxSkinRead;	// 頂点読込
	MOTION motionRead;		// モーション読込
	KEYSET keySetRead;		// モーション読込
	KEY keyRead;			// モーション読込
	int nCntVtx = 0;		// 頂点カウンター
	D3DXVECTOR3 posRead;		// 読込位置
	D3DXQUATERNION quatRead;	// 読込向き
	string nameRead;			// 名前読込文字列
	string filePathRead;		// ファイルパス読込文字列
	int nDataRead;				// int読込用

	// 一文字ずつ読み取る
	while (ifs >> cData)
	{
		// コメント文字
		if (cData == '#')
		{
			// 一行読む
			getline(ifs, str);

			// 文字列削除
			str.clear();

			continue;
		}
		// 改行文字
		else if (cData == '\n')
		{
			// 一行読む
			getline(ifs, str);

			// 文字列削除
			str.clear();

			continue;
		}
		// タブ文字
		else if (cData == '\t')
		{
			continue;
		}

		// 文字列に一文字追加
		str += cData;

		// Xファイル
		if (str == "XFILEPASS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// Xファイルパス読み取り
			CMyMath::ReadToSpace(ifs, str);

			// Xファイル読込
			HRESULT hr;
			hr = D3DXLoadMeshFromX(
				str.c_str(),
				D3DXMESH_MANAGED,
				pDevice,
				NULL,
				&m_model.pBuffMat,
				NULL,
				&m_model.dwNumMat,
				&m_model.pMesh
			);

			if (FAILED(hr))
			{// 失敗したら
				MessageBox(NULL, "CMotionSkin::Init() > モデルを読み込めませんでした", "Error", MB_OK);

				// 結果を返す
				return;
			}

			//マテリアル辺のポインタを取得
			D3DXMATERIAL* pMat;
			pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

			// テクスチャインデックスのメモリ確保
			m_model.apIdxTexture = new int[(int)m_model.dwNumMat];

			// マテリアルの数分回す
			for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMat; nCntMat++)
			{
				m_model.apIdxTexture[nCntMat] = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
			}

			// ファイルパス設定
			m_sXFilePass = str;

			// 文字列削除
			str.clear();
		}
		// 武器セット
		else if (str == "WEAPON_SET")
		{
			// 読込情報リセット
			posRead = InitValu::VEC3_NULL;	// 読込位置
			quatRead = InitValu::QUAT_NULL;	// 読込向き
			nameRead.clear();				// 名前読込文字列
			filePathRead.clear();			// ファイルパス読込文字列

			// 文字列削除
			str.clear();
		}
		// 武器の名前
		else if (str == "WEAPON_NAME")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 名前読み取り
			CMyMath::ReadToSpace(ifs, nameRead);

			// 文字列削除
			str.clear();
		}
		// 武器のファイルパス
		else if (str == "WEAPON_XFILEPATH")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ファイルパス読み取り
			CMyMath::ReadToSpace(ifs, filePathRead);

			// 文字列削除
			str.clear();
		}
		// 武器のボーンインデックス
		else if (str == "WEAPON_BONEIDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置取得
			CMyMath::ReadToSpace(ifs, str);
			nDataRead = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 武器の位置
		else if (str == "WEAPON_OFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			posRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 武器の向き
		else if (str == "WEAPON_QUAT")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			quatRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.z = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.w = stof(str);

			// 文字列削除
			str.clear();
		}
		// 武器の設定終わり
		else if (str == "WEAPON_END")
		{
			// コリジョン用モデルインスタンス生成
			CWeapon* pWeapon = new CWeapon;

			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 情報設定
			pWeapon->SetName(nameRead);
			pWeapon->SetXFilePath(filePathRead);
			pWeapon->SetIdxBone(nDataRead);
			pWeapon->SetPosition(posRead);
			pWeapon->SetQuat(quatRead);

			// 初期化
			pWeapon->Init();

			// コンポーネントリストに追加
			m_pPlayer->AddComponentPushBack(pWeapon);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルセット
		else if (str == "COLLISION_SET")
		{
			// 読込情報リセット
			posRead = InitValu::VEC3_NULL;	// 読込位置
			quatRead = InitValu::QUAT_NULL;	// 読込向き
			nameRead.clear();				// 名前読込文字列
			filePathRead.clear();			// ファイルパス読込文字列

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの名前
		else if (str == "COLLISION_NAME")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 名前読み取り
			CMyMath::ReadToSpace(ifs, nameRead);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルのファイルパス
		else if (str == "COLLISION_XFILEPATH")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ファイルパス読み取り
			CMyMath::ReadToSpace(ifs, filePathRead);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルのボーンインデックス
		else if (str == "COLLISION_BONEIDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置取得
			CMyMath::ReadToSpace(ifs, str);
			nDataRead = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの位置
		else if (str == "COLLISION_OFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			posRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの向き
		else if (str == "COLLISION_QUAT")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			quatRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.z = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.w = stof(str);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの設定終わり
		else if (str == "COLLISION_END")
		{
			// コリジョン用モデルインスタンス生成
			CCollisionModel* pCollModel = new CCollisionModel;

			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 情報設定
			pCollModel->SetName(nameRead);
			pCollModel->SetXFilePath(filePathRead);
			pCollModel->SetIdxBone(nDataRead);
			pCollModel->SetPosition(posRead);
			pCollModel->SetQuat(quatRead);

			// 初期化
			pCollModel->Init();

			// コンポーネントリストに追加
			m_pPlayer->AddComponentPushBack(pCollModel);

			// 文字列削除
			str.clear();
		}
		// ボーンの数読込
		else if (str == "NUMBONE")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			m_nNumBone = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 頂点の数読込
		else if (str == "NUMVTX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点の数読込読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			m_nNumVtx = stoi(str);

			// 読み取った頂点の数分回す
			for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
			{
				// 頂点情報初期化
				VERTEXSKIN vtxSkin;
				ZeroMemory(&vtxSkin, sizeof(vtxSkin));
				for (int nCntvtxSkin = 0; nCntvtxSkin < IDX_BONE_NUM; nCntvtxSkin++)
				{
					vtxSkin.aIdxBone[nCntvtxSkin] = -1;
					vtxSkin.aVtxWaght[nCntvtxSkin] = 0.0f;
				}

				// 要素を追加
				m_vVtxSkin.push_back(vtxSkin);
			}

			// 文字列削除
			str.clear();
		}
		// ボーン開始読込
		else if (str == "BONE_SET")
		{
			// 文字列削除
			str.clear();
		}
		// ボーンインデックス
		else if (str == "BONE_IDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			boneRead.nIdx = stoi(str);

			// 文字列削除
			str.clear();
		}
		// ボーンインデックス(子)
		else if (str == "BONE_CHILD_IDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス(子)読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			boneRead.nIdxChild = stoi(str);

			// 文字列削除
			str.clear();
		}
		// ボーンインデックス(親)
		else if (str == "BONE_PARENT_IDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス(親)読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			boneRead.nIdxParent = stoi(str);

			// 文字列削除
			str.clear();
		}
		// ボーンの位置
		else if (str == "BONE_POS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置読み取り
			CMyMath::ReadToSpace(ifs, str);
			boneRead.pos.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.pos.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.pos.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// ボーンの位置
		else if (str == "BONE_OFFSETPOS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置読み取り
			CMyMath::ReadToSpace(ifs, str);
			boneRead.posOffset.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.posOffset.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.posOffset.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// ボーン終了
		else if (str == "BONE_END")
		{
			// 要素を追加する
			// ワールドマトリックス初期化
			D3DXMatrixIdentity(&boneRead.mtxWorld);
			D3DXMatrixIdentity(&boneRead.mtxRot);
			boneRead.pBoneChild = NULL;
			boneRead.pBoneParent = NULL;
			D3DXQuaternionIdentity(&boneRead.quat);
			m_vBone.push_back(boneRead);

			// リセット用ボーン追加
			m_vBoneReset.push_back(boneRead);

			// ボーンモデルの生成
			CBone* pBoneModel = CBone::Create();

			// リストに追加
			m_vpBoneModel.push_back(pBoneModel);

			// 読込情報リセット
			ZeroMemory(&boneRead, sizeof(boneRead));

			// 文字列削除
			str.clear();
		}
		// 頂点開始
		else if (str == "VTX_SET")
		{
			// 文字列削除
			str.clear();
		}
		// 頂点ボーンインデックス0
		else if (str == "VTX_IDXBONE_0")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点ボーンインデックス0読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxSkinRead.aIdxBone[0] = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 頂点ボーンインデックス1
		else if (str == "VTX_IDXBONE_1")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点ボーンインデックス1読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxSkinRead.aIdxBone[1] = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 頂点重さ0
		else if (str == "VTX_WAGHT_0")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点重さ0読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxSkinRead.aVtxWaght[0] = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点重さ1
		else if (str == "VTX_WAGHT_1")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点重さ1読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxSkinRead.aVtxWaght[1] = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点法線オフセット
		else if (str == "VTX_NOROFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.norOffSet.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.norOffSet.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.norOffSet.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点位置オフセット
		else if (str == "VTX_POSOFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.posOffSet.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.posOffSet.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.posOffSet.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点終了
		else if (str == "VTX_END")
		{
			// 編集用頂点のイテレータ取得
			auto iterVtxEdit = m_vVtxSkin.begin() + nCntVtx;

			// 読み取った情報設定
			(*iterVtxEdit) = vtxSkinRead;

			// カウンター加算
			nCntVtx++;

			// 文字列削除
			str.clear();
		}
		// モーション開始
		else if (str == "MOTION_SET")
		{
			// モーションの総数増加
			m_nNumMotion++;

			// モーション読込情報初期化
			motionRead.bLoop = false;
			motionRead.nNumKeyInfo = 0;
			motionRead.vKeySet.clear();

			// 文字列削除
			str.clear();
		}
		// ループの有無
		else if (str == "MOTION_LOOP")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ループ読み取り
			CMyMath::ReadToSpace(ifs, str);

			// bool型に変換
			motionRead.bLoop = (bool)stoi(str);

			// 文字列削除
			str.clear();
		}
		// キーセットの数
		else if (str == "MOTION_NUMKEYINFO")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// キーセット読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			motionRead.nNumKeyInfo = stoi(str);

			// 文字列削除
			str.clear();
		}
		// キーセット開始
		else if (str == "KEYSET_SET")
		{
			// 読み取り用キーセット初期化
			keySetRead.nFlame = 10;
			keySetRead.nNumkey = 0;
			keySetRead.vKey.clear();

			// 文字列削除
			str.clear();
		}
		// フレーム数
		else if (str == "KEYSET_FLAME")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// フレーム数読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			keySetRead.nFlame = stoi(str);

			// 文字列削除
			str.clear();
		}
		// キー数
		else if (str == "KEYSET_NUMKEY")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// キー読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			keySetRead.nNumkey = stoi(str);

			// 文字列削除
			str.clear();
		}
		// キー開始
		else if (str == "KEY_SET")
		{
			// 読込用キー初期化
			keyRead.fPosX = 0.0f;
			keyRead.fPosY = 0.0f;
			keyRead.fPosZ = 0.0f;
			D3DXQuaternionIdentity(&keyRead.quat);

			// 文字列削除
			str.clear();
		}
		// 位置
		else if (str == "KEY_POS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			keyRead.fPosX = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.fPosY = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.fPosZ = stof(str);

			// 文字列削除
			str.clear();
		}
		// 向き
		else if (str == "KEY_QUAT")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			keyRead.quat.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.quat.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.quat.z = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.quat.w = stof(str);

			// 文字列削除
			str.clear();
		}
		// キー終了
		else if (str == "KEY_END")
		{
			// キーセットに要素追加
			keySetRead.vKey.push_back(keyRead);

			// 文字列削除
			str.clear();
		}
		// キーセット終了
		else if (str == "KEYSET_END")
		{
			// モーションに要素追加
			motionRead.vKeySet.push_back(keySetRead);

			// 文字列削除
			str.clear();
		}
		// モーション終了
		else if (str == "MOTION_END")
		{
			// モーション情報に要素追加
			m_vMotion.push_back(motionRead);

			// 文字列削除
			str.clear();
		}
	}

	// ファイルを閉じる
	ifs.close();

	// ボーンの親と子の設定
	for (auto& iterBone : m_vBone)
	{
		// 子のポインタを設定
		if (iterBone.nIdxChild != -1)
		{
			auto iterBoneChild = m_vBone.begin() + iterBone.nIdxChild;
			iterBone.pBoneChild = &*iterBoneChild;
		}

		// 親のポインタを設定
		if (iterBone.nIdxParent != -1)
		{
			auto iterBoneParent = m_vBone.begin() + iterBone.nIdxParent;
			iterBone.pBoneParent = &*iterBoneParent;
		}
	}

	// ボーンの親と子の設定
	for (auto& iterBone : m_vBoneReset)
	{
		// 子のポインタを設定
		if (iterBone.nIdxChild != -1)
		{
			auto iterBoneChild = m_vBoneReset.begin() + iterBone.nIdxChild;
			iterBone.pBoneChild = &*iterBoneChild;
		}

		// 親のポインタを設定
		if (iterBone.nIdxParent != -1)
		{
			auto iterBoneParent = m_vBoneReset.begin() + iterBone.nIdxParent;
			iterBone.pBoneParent = &*iterBoneParent;
		}
	}

	// コンポーネントの各種設定
	for (auto& iterComp : m_pPlayer->GetComponentVector())
	{
		// 武器の設定
		if (auto* weapon = dynamic_cast<CWeapon*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = weapon->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					weapon->SetParentBone(&iterBone);
					break;
				}
			}
		}
		// 当たり判定用モデルの設定
		else if (auto* collModel = dynamic_cast<CCollisionModel*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = collModel->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					collModel->SetParentBone(&iterBone);
					break;
				}
			}
		}
	}
}

//*******************************************************************************************
// モーション(スキンメッシュ)終了
//*******************************************************************************************
void CMotionSkin::Uninit(void)
{
	// エンティティ破棄
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	// モーション情報破棄
	m_vMotion.clear();	

	// ボーン破棄
	m_vBone.clear();

	// リセット用ボーン破棄
	m_vBoneReset.clear();

	// ボーンのモデル破棄
	for (auto& iter : m_vpBoneModel)
	{
		iter->Uninit();
	}
	m_vpBoneModel.clear();

	// 頂点スキンメッシュ用破棄
	m_vVtxSkin.clear();
	
	// テクスチャインデックスの破棄
	if (m_model.apIdxTexture != NULL)
	{
		delete[] m_model.apIdxTexture;
		m_model.apIdxTexture = NULL;
	}

	// マテリアルバッファの破棄
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}

	// メッシュの破棄
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	// 自身の破棄
	delete this;
}

//*******************************************************************************************
// モーション(スキンメッシュ)更新
//*******************************************************************************************
void CMotionSkin::Update(void)
{
	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ボーンの数分回す
	for (auto& iterBone : m_vBone)
	{
		// ワールドマトリックス初期化
		D3DXMatrixIdentity(&iterBone.mtxWorld);

		// 向きを追加
		D3DXMatrixRotationQuaternion(&mtxRot, &iterBone.quat);
		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &mtxRot);

		// 位置を追加
		D3DXMatrixTranslation(&mtxTrans, iterBone.pos.x, iterBone.pos.y, iterBone.pos.z);
		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &mtxTrans);

		// 親がいれば親のマトリックスを掛ける
		if (iterBone.pBoneParent != NULL)
		{
			D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &iterBone.pBoneParent->mtxWorld);
		}
		// いなければこのオブジェクトのマトリックスを掛ける
		else
		{
			D3DXMatrixMultiply(&iterBone.mtxWorld, &m_model.mtxWorld, &iterBone.mtxWorld);
		}
	}

	// エンティティ更新
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Update();
	}

	// ボーンのモデルの位置を設定する
	BoneModelSettingPosition();

	// 頂点バッファのポインタ
	BYTE* pVtxBuff = NULL;

	// 頂点フォーマットのサイズデータ
	WORD sizeFVF;

	// 頂点フォーマットのサイズを取得
	sizeFVF = (WORD)D3DXGetFVFVertexSize(m_model.pMesh->GetFVF());

	// 頂点バッファのロック
	m_model.pMesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&pVtxBuff);

	// 頂点情報のイテレーター
	auto iterVtxSkin = m_vVtxSkin.begin();

	// モデルの頂点数分回す
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++, iterVtxSkin++)
	{
		// 計算用変数
		D3DXVECTOR3 posHope, offSet, offSetEX, posLocal;
		D3DXVECTOR3 aNorOffset[IDX_BONE_NUM];
		D3DXMATRIX mtx, mtxOffset;

		// 一回でも設定下したかどうか
		bool bSet = false;

		// 位置をゼロにする
		posLocal = InitValu::VEC3_NULL;

		// 位置のポインタ設定
		D3DXVECTOR3* pPos = (D3DXVECTOR3*)pVtxBuff;

		offSet.x = (*iterVtxSkin).posOffSet.x;
		offSet.y = (*iterVtxSkin).posOffSet.y;
		offSet.z = (*iterVtxSkin).posOffSet.z;

		// ボーンインデックスの数分回す
		for (int nCntIdxBone = 0; nCntIdxBone < IDX_BONE_NUM; nCntIdxBone++)
		{
			aNorOffset[nCntIdxBone].x = (*iterVtxSkin).norOffSet.x;
			aNorOffset[nCntIdxBone].y = (*iterVtxSkin).norOffSet.y;
			aNorOffset[nCntIdxBone].z = (*iterVtxSkin).norOffSet.z;

			int nIdxBone = (*iterVtxSkin).aIdxBone[nCntIdxBone];
			float fWeight = (*iterVtxSkin).aVtxWaght[nCntIdxBone];

			// 親ボーンが存在しないのであれば処理しない
			if (nIdxBone == -1)
			{
				aNorOffset[nCntIdxBone] = InitValu::VEC3_NULL;
				continue;
			}

			// セットした状態にする
			bSet = true;

			// ボーン情報取得
			auto iterBone = m_vBone.begin() + nIdxBone;

			// ボーンのマトリックス
			mtx = iterBone->mtxWorld;

			// 原点に移動
			offSetEX.x = offSet.x - (*iterBone).posOffset.x;
			offSetEX.y = offSet.y - (*iterBone).posOffset.y;
			offSetEX.z = offSet.z - (*iterBone).posOffset.z;

			// ボーンの回転を掛ける
			posHope.x = (offSetEX.x * mtx._11) + (offSetEX.y * mtx._21) + (offSetEX.z * mtx._31) + mtx._41;
			posHope.y = (offSetEX.x * mtx._12) + (offSetEX.y * mtx._22) + (offSetEX.z * mtx._32) + mtx._42;
			posHope.z = (offSetEX.x * mtx._13) + (offSetEX.y * mtx._23) + (offSetEX.z * mtx._33) + mtx._43;

			aNorOffset[nCntIdxBone].x = (aNorOffset[nCntIdxBone].x * mtx._11) + (aNorOffset[nCntIdxBone].y * mtx._21) + (aNorOffset[nCntIdxBone].z * mtx._31);
			aNorOffset[nCntIdxBone].y = (aNorOffset[nCntIdxBone].x * mtx._12) + (aNorOffset[nCntIdxBone].y * mtx._22) + (aNorOffset[nCntIdxBone].z * mtx._32);
			aNorOffset[nCntIdxBone].z = (aNorOffset[nCntIdxBone].x * mtx._13) + (aNorOffset[nCntIdxBone].y * mtx._23) + (aNorOffset[nCntIdxBone].z * mtx._33);

			// 重さを掛ける
			posHope *= fWeight;

			// 頂点座標に足す
			posLocal += posHope;
		}
		
		if (bSet == true)
		{
			// 計算した値を代入
			*pPos = posLocal;
		}
		else
		{
			// オフセット位置を設定
			*pPos = offSet;
		}

		// 位置のサイズ分ずらす
		pVtxBuff += sizeof(D3DXVECTOR3);

		// 法線のポインタを取得
		D3DXVECTOR3* pNor = (D3DXVECTOR3*)pVtxBuff;

		// ゼロに設定して
		*pNor = InitValu::VEC3_NULL;

		// ボーンインデックスの数分回す
		for (int nCntIdxBone = 0; nCntIdxBone < IDX_BONE_NUM; nCntIdxBone++)
		{
			// 計算した値を足していく
			*pNor += aNorOffset[nCntIdxBone];
		}

		// 正規化する
		D3DXVec3Normalize(pNor, pNor);

		// 残りのサイズ分ずらす
		pVtxBuff += sizeof(D3DXVECTOR3);
		pVtxBuff += sizeof(D3DXVECTOR2);
	}
}

//*******************************************************************************************
// モーション(スキンメッシュ)描画
//*******************************************************************************************
void CMotionSkin::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_model.mtxWorld);

	// 向きを追加
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_model.rot.y, m_model.rot.x, m_model.rot.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, m_model.pos.x, m_model.pos.y, m_model.pos.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_model.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_model.apIdxTexture[nCntMat]));

		// モデル(パーツの描画)
		m_model.pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// エンティティ描画
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Draw();
	}
}

//*******************************************************************************************
// モーション(スキンメッシュ)生成
//*******************************************************************************************
CMotionSkin* CMotionSkin::Create(std::string motionFilePath)
{
	CMotionSkin* pMotionSkin = new CMotionSkin;

	if (pMotionSkin != NULL)
	{
		// 初期化処理
		pMotionSkin->Init(motionFilePath);
	}

	return  pMotionSkin;
}

//*******************************************************************************************
// モーション(スキンメッシュ)生成バイナリ
//*******************************************************************************************
CMotionSkin* CMotionSkin::CreateBinary(std::string motionFilePathBinary)
{
	CMotionSkin* pMotionSkin = new CMotionSkin;

	if (pMotionSkin != NULL)
	{
		// 初期化処理
		pMotionSkin->Init(motionFilePathBinary);
	}

	return  pMotionSkin;
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンのモデルの配置
//***************************************************************************************
void CMotionSkin::BoneModelSettingPosition(void)
{
	auto iterBone = m_vBone.begin();

	// ボーンのモデル分位置設定する
	for (auto iterBoneModel : m_vpBoneModel)
	{
		// 位置を取得
		D3DXVECTOR3 pos;
		pos.x = (*iterBone).mtxWorld._41;
		pos.y = (*iterBone).mtxWorld._42;
		pos.z = (*iterBone).mtxWorld._43;

		// 向き行列
		D3DXMATRIX mtxRot = (*iterBone).mtxWorld;
		mtxRot._41 = 0.0f;
		mtxRot._42 = 0.0f;
		mtxRot._43 = 0.0f;

		// 回転行列を設定
		iterBoneModel->SetMatrixRotate(mtxRot);

		// 位置設定
		iterBoneModel->SetPosition(pos);

		// 頂点位置を初期値に戻す
		iterBoneModel->ResetVertex();

		// 親のポインタを取得
		CBone::BONE* pBoneChild = (*iterBone).pBoneChild;

		// 親ボーンが存在する
		if (pBoneChild != nullptr)
		{
			// 親の位置取得
			D3DXVECTOR3 posChild;
			posChild.x = pBoneChild->mtxWorld._41;
			posChild.y = pBoneChild->mtxWorld._42;
			posChild.z = pBoneChild->mtxWorld._43;

			// 自身から親までのベクトルを取る
			D3DXVECTOR3 vecPos = posChild - pos;

			float fLength = D3DXVec3Length(&vecPos);

			// 長さを設定
			iterBoneModel->SetHeight(fLength);

			D3DXQUATERNION quat;
			D3DXVECTOR3 vecAxis;
			D3DXVECTOR3 vecY = { 0.0f,1.0f,0.0f };

			// 外積で回転軸を求める
			D3DXVec3Normalize(&vecPos, &vecPos);
			D3DXVec3Cross(&vecAxis, &vecY, &vecPos);

			if ((int)(D3DXVec3Length(&vecAxis) * 100.0f) == 0)
			{
				vecAxis = { 1.0f,0.0f,0.0f };
			}

			// ベクトルのなす角を求める
			float fAngle = CMyMath::VectorAngleToRadian(vecY, vecPos);

			// クオータニオンを求める
			D3DXQuaternionRotationAxis(&quat, &vecAxis, fAngle);

			// クオータニオンから回転行列を求める
			D3DXMatrixRotationQuaternion(&mtxRot, &quat);

			// 回転行列を設定
			iterBoneModel->SetMatrixRotate(mtxRot);
		}

		// 親のポインタを取得
		CBone::BONE* pBoneParent = (*iterBone).pBoneParent;

		// 親ボーンが存在する
		if (pBoneParent != nullptr)
		{
			// 親の位置取得
			D3DXVECTOR3 posParent;
			posParent.x = pBoneParent->mtxWorld._41;
			posParent.y = pBoneParent->mtxWorld._42;
			posParent.z = pBoneParent->mtxWorld._43;

			// 自身から親までのベクトルを取る
			D3DXVECTOR3 vecPos = posParent - pos;

			// ボーンの逆行列作成
			D3DXMATRIX mtxInvers;
			D3DXMATRIX mtxRotBone = iterBoneModel->GetMatrixRotate();
			D3DXMatrixInverse(&mtxInvers, nullptr, &mtxRotBone);

			// 逆行列を掛けた位置にする
			vecPos = CMyMath::MatrixMulRotation(vecPos, mtxInvers);

			// お尻を設定
			iterBoneModel->SetBack(vecPos);
		}

		// ボーンを一つ進める
		iterBone++;
	}
}

//***************************************************************************************
// モーション(スキンメッシュ)すべてのボーンのモデルの色を変える
//***************************************************************************************
void CMotionSkin::BoneModelOllSetColor(int nSelectBoneIdx,D3DXCOLOR colBone, D3DXCOLOR colBoneParent, D3DXCOLOR colBoneSelect)
{
	// ボーンが無い、ボーンの数の範囲を超えていれば処理しない
	if (m_vpBoneModel.size() == 0 || nSelectBoneIdx < 0 || nSelectBoneIdx > (int)m_vpBoneModel.size())
	{
		return;
	}

	// ボーンのモデルに色をつける
	for (auto iterBoneModel : m_vpBoneModel)
	{
		// 通常色に設定
		iterBoneModel->SetColor(colBone);
	}

	// 選んでいるボーンとその親のそれぞれのモデルを取得
	auto iterTouchBoneModel = m_vpBoneModel.begin() + nSelectBoneIdx;
	auto iterTouchBone = m_vBone.begin() + nSelectBoneIdx;

	// 今選んでいるボーンの色を設定する
	(*iterTouchBoneModel)->SetColor(colBoneSelect);

	// 親がいなければ処理はここまで
	if (iterTouchBone->pBoneParent == NULL)
	{
		return;
	}

	// 親のボーンの色を変える
	auto iterTouchBoneModelParent = m_vpBoneModel.begin() + iterTouchBone->pBoneParent->nIdx;
	(*iterTouchBoneModelParent)->SetColor(colBoneParent);
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの回転処理
//***************************************************************************************
void CMotionSkin::RotateBone(int nSelectBone,float fAngle, D3DXVECTOR3 vecAxis)
{
	// ボーンが無ければ処理しない
	if (m_vBone.size() == 0)
	{
		return;
	}

	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 計算用変数
	D3DXQUATERNION quat;

	// クオータニオンを求める
	D3DXQuaternionRotationAxis(&quat, &vecAxis, fAngle);
	
	// クオータニオン同士を掛けて設定
	D3DXQuaternionMultiply(&(*iterBone).quat, &quat, &(*iterBone).quat);
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの移動処理
//***************************************************************************************
void CMotionSkin::PositionBone(int nSelectBone, D3DXVECTOR3 addPos)
{
	// ボーンが無ければ処理しない
	if (m_vBone.size() == 0)
	{
		return;
	}

	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 向きを追加
	(*iterBone).pos += addPos;
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの回転のセット処理
//***************************************************************************************
void CMotionSkin::SetRotateBone(int nSelectBone, D3DXQUATERNION quat)
{
	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 向きを追加
	(*iterBone).quat = quat;
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの回転のセット処理
//***************************************************************************************
void CMotionSkin::SetRotateBoneNotParent(int nSelectBone, D3DXQUATERNION quat)
{
	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 向きを追加
	(*iterBone).quat = quat;
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの回転のリセット処理
//***************************************************************************************
void CMotionSkin::ReSetRotateBone(int nSelectBone)
{
	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 向きをリセット
	(*iterBone).quat = InitValu::QUAT_NULL;
}

//***************************************************************************************
// モーションボーンの位置回転のリセット処理
//***************************************************************************************
void CMotionSkin::ReSetBone(void)
{
	// ボーンリセットの情報
	auto iterBoneReset = m_vBoneReset.begin();

	// すべてのボーンを設定
	for (auto& iterBone : m_vBone)
	{
		iterBone.pos = (*iterBoneReset).pos;
		iterBone.quat = (*iterBoneReset).quat;

		// リセットボーンの情報を一つ進める
		iterBoneReset++;
	}
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの位置のセット処理
//***************************************************************************************
void CMotionSkin::SetPositionBone(int nSelectBone, D3DXVECTOR3 pos)
{
	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 位置を追加
	(*iterBone).pos = pos;
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの位置のセット処理
//***************************************************************************************
void CMotionSkin::SetPositionBoneChild(int nSelectBone, D3DXVECTOR3 pos)
{
	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	if (iterBone->pBoneChild == nullptr)return;

	// 位置を追加
	iterBone->pBoneChild->pos = pos;
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの位置のセット処理X
//***************************************************************************************
void CMotionSkin::SetPositionBoneX(int nSelectBone, float fX)
{
	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 位置を追加
	(*iterBone).pos.x = fX;
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの位置のセット処理Y
//***************************************************************************************
void CMotionSkin::SetPositionBoneY(int nSelectBone, float fY)
{
	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 位置を追加
	(*iterBone).pos.y = fY;
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの位置のセット処理Z
//***************************************************************************************
void CMotionSkin::SetPositionBoneZ(int nSelectBone, float fZ)
{
	// 回転行列を設定
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 位置を追加
	(*iterBone).pos.z = fZ;
}

//***************************************************************************************
// モーション(スキンメッシュ)ボーンの位置のリセット処理
//***************************************************************************************
void CMotionSkin::ReSetPositionBone(int nSelectBone)
{
	// ボーン情報を取得
	auto iterBone = m_vBone.begin() + nSelectBone;
	auto iterBoneReset = m_vBoneReset.begin() + nSelectBone;

	// 位置を設定
	(*iterBone).pos = (*iterBoneReset).pos;
}

//*************************************
// モーションクラスセットモーションブレンド
//*************************************
void CMotionSkin::SetMotion(int nTypeMotion, int nBlendTime)
{
	// モーションが無ければ設定しない
	if (nTypeMotion < 0 || nTypeMotion >= m_nNumMotion)
	{
		return;
	}

	// モーション情報取得
	auto iterMotionInfo = m_vMotion.begin() + m_motionInfo.nTypeMotionNow;

	// ループの有無を取得する
	bool bLoop = (*iterMotionInfo).bLoop;

	// モーションの情報を受け継ぎ
	m_motionInfoBlend = m_motionInfo;	

	m_motionInfo.bEndMotion = false;			// 終っていない状態にする
	m_motionInfo.nCounterMotion = 0;			// モーションカウンターリセット
	m_motionInfo.nKeyNow = 0;					//　今のキーリセット 
	m_motionInfo.nTypeMotionNow = nTypeMotion;	//　今のモーションタイプを変更
	m_nCntBlend = 0;							// ブレンドカウンターゼロ
	m_nBlendTime = nBlendTime;					// ブレンド時間
	m_bBlend = true;							// ブレンドする状態にする

	// モーション情報再設定
	iterMotionInfo = m_vMotion.begin() + m_motionInfo.nTypeMotionNow;
	
	// キーの総数取得
	int nNumKeySet = (int)(*iterMotionInfo).vKeySet.size();

	// キーセットを設定
	m_motionInfo.nKeySetNow = 0;
	m_motionInfo.nKeySetNext = 1;
	int nMax = (int)(*iterMotionInfo).vKeySet.size() - 1;
	nMax = Clamp(nMax, 0, nMax);

	// 繰り返す
	m_motionInfo.nKeySetNext = Clamp(m_motionInfo.nKeySetNext, 0, nMax);

	// ブレンド時間は下限を超えないようにする
	if (m_nBlendTime <= 0)
	{
		m_nBlendTime = 1;
	}
}

//**************************************************************************
// モーションクラスキーの更新
//**************************************************************************
void CMotionSkin::UpdateMotionGetKey(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutRot, D3DXQUATERNION* pOutQuat, MOTIONINFO* pMotionInfo)
{
	// モーションが無ければ処理しない
	if (m_vMotion.empty() == true)return;
	
	// モーション情報取得
	auto iterMotionInfo = m_vMotion.begin() + pMotionInfo->nTypeMotionNow;
	
	// キーが無ければ処理しない
	if ((*iterMotionInfo).vKeySet.empty() == true)return;
	
	auto iterKeyInfoNow = (*iterMotionInfo).vKeySet.begin() + pMotionInfo->nKeySetNow;	// キー情報取得
	bool bLoop = (*iterMotionInfo).bLoop;												// ループの有無取得
	int nNumKeyInfo = (*iterMotionInfo).nNumKeyInfo;									// キーの総数取得
	int nFrame = (*iterKeyInfoNow).nFlame;												// フレーム数取得

	float fMugFlame = (float)pMotionInfo->nCounterMotion / (float)nFrame;	// フレーム倍率を求める
	D3DXVECTOR3 posKeyNow;													// 今のキーの位置
	D3DXVECTOR3 posKeyNext;													// 次のキーの位置
	D3DXVECTOR3 posDiff;													// キーの位置の差分
	D3DXQUATERNION quatNow, quatNext;										// 今と次のクォータニオン

	if ((int)(*iterMotionInfo).vKeySet.size() <= pMotionInfo->nKeySetNext)
	{
		pMotionInfo->nKeySetNext--;
	}

	// 今と先のキーを取得
	iterKeyInfoNow = (*iterMotionInfo).vKeySet.begin() + pMotionInfo->nKeySetNow;
	auto iterKeyInfoNext = (*iterMotionInfo).vKeySet.begin() + pMotionInfo->nKeySetNext;
	auto iterKeyNow = (*iterKeyInfoNow).vKey.begin() + pMotionInfo->nKeyNow;
	auto iterKeyNext = (*iterKeyInfoNext).vKey.begin() + pMotionInfo->nKeyNow;

	posKeyNow.x = (*iterKeyNow).fPosX;	// 今の位置X取得
	posKeyNow.y = (*iterKeyNow).fPosY;	// 今の位置Y取得
	posKeyNow.z = (*iterKeyNow).fPosZ;	// 今の位置Z取得
	quatNow = (*iterKeyNow).quat;		// 今のクォータニオン

	posKeyNext.x = (*iterKeyNext).fPosX;	// 次の位置X取得
	posKeyNext.y = (*iterKeyNext).fPosY;	// 次の位置Y取得
	posKeyNext.z = (*iterKeyNext).fPosZ;	// 次の位置Z取得
	quatNext = (*iterKeyNext).quat;			// 次のクォータニオン

	// モーションが終わっている場合は値をそのまま返す
	if (pMotionInfo->bEndMotion == true)
	{
		*pOutPos = posKeyNow;
		*pOutQuat = quatNow;
		return;
	}

	// 位置と向きの差分を求める
	posDiff = posKeyNext - posKeyNow;

	// 位置と向きの差分にフレーム倍率を掛けそれを足し位置を求める
	*pOutPos = posKeyNow + (posDiff * fMugFlame);

	// クォータニオンの球面線形補間の計算
	CMyMath::SlerpQuaternion(pOutQuat, quatNow, quatNext, fMugFlame);
}

//**************************************************************************
// モーションクラスモーションの更新
//**************************************************************************
void CMotionSkin::UpdateMotion(CMotionSkin* pBlendMotion)
{
	// モデルカウンター
	int nCntModel = 0;

	// モーションが終っていたら処理しない
	if (m_motionInfo.bEndMotion == true)return;

	// ボーンの数分回す
	for (auto iterBone = m_vBone.begin(); iterBone != m_vBone.end(); iterBone++, nCntModel++)
	{
		// モーション用ベクトル
		D3DXVECTOR3 outPos, outRot;

		// モーション用クォータニオン
		D3DXQUATERNION outQuat;

		// ブレンドモーション用ベクトル
		D3DXVECTOR3 blendPos, blendRot;

		// ブレンドモーション用クォータニオン
		D3DXQUATERNION blendQuat;

		// ボーンの位置と向きを取得
		D3DXVECTOR3 pos = iterBone->pos;
		D3DXQUATERNION quat = iterBone->quat;

		// モーションのアップデート
		UpdateMotionGetKey(&outPos, &outRot, &outQuat, &m_motionInfo);
		UpdateMotionGetKey(&blendPos, &blendRot,&blendQuat,&m_motionInfoBlend);

		// モーションブレンドをする状態であれば
		if (m_bBlend == true)
		{
			// ブレンド時間の倍率を出す
			float fMugBlendTime = (float)m_nCntBlend / (float)m_nBlendTime;
			
			outPos *= fMugBlendTime;
			outRot *= fMugBlendTime;
			outQuat *= fMugBlendTime;
			blendPos *= 1.0f - fMugBlendTime;
			blendRot *= 1.0f - fMugBlendTime;
			blendQuat *= 1.0f - fMugBlendTime;
			
			// 算出された値を追加
			pos = outPos + blendPos;
			quat = outQuat + blendQuat;
		}
		else
		{
			// 取得した値を追加
			pos = outPos;
			quat = outQuat;
		}

		// 位置と向きを設定
		(*iterBone).pos = pos;
		(*iterBone).quat = quat;

		// キー番号プラス
		m_motionInfo.nKeyNow++;
		m_motionInfoBlend.nKeyNow++;
	}

	// キー番号リセット
	m_motionInfo.nKeyNow = 0;
	m_motionInfoBlend.nKeyNow = 0;

	// モーションカウンターをカウントアップ
	m_motionInfo.nCounterMotion++;;
	m_motionInfoBlend.nCounterMotion++;

	// ブレンドモーションカウンタがブレンド時間より大きければ
	if (m_nCntBlend > m_nBlendTime)
	{
		// モーションブレンドをしない状態にする
		m_bBlend = false;
	}
	// ブレンドモーションカウンタがあれば
	else
	{
		// モーションブレンドカウントアップ
		m_nCntBlend++;
	}

	auto iterMotionInfo = m_vMotion.begin() + m_motionInfo.nTypeMotionNow;				// モーション情報取得
	auto iterKeyInfoNow = (*iterMotionInfo).vKeySet.begin() + m_motionInfo.nKeySetNow;	// キー情報取得
	bool bLoop = (*iterMotionInfo).bLoop;												// ループの有無取得
	int nNumKeySet = (int)(*iterMotionInfo).vKeySet.size() - 1;							// キーの総数取得
	nNumKeySet = Clamp(nNumKeySet, 0, nNumKeySet);
	int nFrame = (*iterKeyInfoNow).nFlame;												// フレーム数取得

	// モーションカウンターがフレーム数を超えたら次のキーにする
	if (m_motionInfo.nCounterMotion > nFrame)
	{
		// 次のキー設定(クランプ)
		m_motionInfo.nKeySetNow++;
		m_motionInfo.nKeySetNow = Wrap(m_motionInfo.nKeySetNow, 0, nNumKeySet);
		m_motionInfo.nKeySetNext = m_motionInfo.nKeySetNow + 1;
		m_motionInfo.nKeySetNext = Clamp(m_motionInfo.nKeySetNext, 0, nNumKeySet);

		// モーションカウンターをクランプ
		m_motionInfo.nCounterMotion = Wrap(m_motionInfo.nCounterMotion, 0, nFrame);

		// キーセットが限界まできたら
		if (m_motionInfo.nKeySetNow >= nNumKeySet)
		{
			// ループする場合(モーションが終っていない場合)
			if (bLoop == true && m_motionInfo.bEndMotion == false)
			{
				// ループのつなぎに設定する
				m_motionInfo.nKeySetNow = nNumKeySet;
				m_motionInfo.nKeySetNext = 0;

				// モーションカウンターを繰り返す
				m_motionInfo.nCounterMotion = 0;
			}
			// ループしない場合
			else
			{
				// モーションカウンターせき止め
				m_motionInfo.nCounterMotion = nFrame;

				// モーションが終わった状態にする
				m_motionInfo.bEndMotion = true;
			}
		}
	}
}

//**************************************************************************
// モーションクラスモーションを増やす処理
//**************************************************************************
void CMotionSkin::AddMotion(void)
{
	// モーション情報初期化
	MOTION motion;
	motion.bLoop = false;
	motion.nNumKeyInfo = 0;

	// モーション数追加
	m_nNumMotion++;

	// 要素追加
	m_vMotion.push_back(motion);
}

//**************************************************************************
// モーションクラスモーションを減らす処理
//**************************************************************************
void CMotionSkin::PullMotion(void)
{
	if (m_vMotion.size() != 0)
	{
		auto iterMotion = m_vMotion.end() - 1;

		// 要素削除
		m_vMotion.erase(iterMotion);
	}
}

//**************************************************************************
// モーションクラスキーセットを増やす処理
//**************************************************************************
void CMotionSkin::AddKeySet(int nMotion)
{
	// モーションが無ければ処理しない
	if (m_vMotion.size() != NULL || (int)m_vMotion.size() < nMotion || 0 > nMotion)
	{
		// キーセット情報初期化
		KEYSET keySet;
		keySet.nFlame = 10;
		keySet.nNumkey = m_nNumBone;
		
		// リセット用ボーン情報取得
		auto iterBoneReset = m_vBoneReset.begin();

		// ボーンの数分キーを生成
		for (int nCntKey = 0; nCntKey < m_nNumBone; nCntKey++)
		{
			CMotionSkin::KEY key;

			// 位置と向きを初期値に設定
			key.fPosX = (*iterBoneReset).pos.x;
			key.fPosY = (*iterBoneReset).pos.y;
			key.fPosZ = (*iterBoneReset).pos.z;
			key.quat = (*iterBoneReset).quat;

			// 要素追加
			keySet.vKey.push_back(key);

			// 情報をずらす
			iterBoneReset++;
		}

		// 要素追加
		auto iterMotion = m_vMotion.begin() + nMotion;
		(*iterMotion).nNumKeyInfo++;
		(*iterMotion).vKeySet.push_back(keySet);
	}
}

//**************************************************************************
// モーションクラスキーセットを減らす処理
//**************************************************************************
void CMotionSkin::PullKeySet(int nMotion)
{
	// モーションが無ければ処理しない
	if (m_vMotion.size() != NULL || (int)m_vMotion.size() < nMotion || 0 > nMotion)
	{
		// 要素追加
		auto iterMotion = m_vMotion.begin() + nMotion;
		// キーが無ければ処理しない
		if ((*iterMotion).vKeySet.size() != NULL)
		{
			// キーセット情報取得
			auto iterKeyset = (*iterMotion).vKeySet.end() - 1;

			// 要素削除
			(*iterMotion).vKeySet.erase(iterKeyset);

			// キーセット数を減らす
			(*iterMotion).nNumKeyInfo--;
		}
	}
}

//**************************************************************************
// モーションクラスボーンの情報を取得する処理
//**************************************************************************
CBone::BONE* CMotionSkin::GetBone(int nBone)
{
	// ボーンが無ければ処理しない
	if (m_vBone.size() != 0 || (int)m_vBone.size() < nBone || 0 > nBone)
	{
		auto iterBone = m_vBone.begin() + nBone;
		return &*iterBone;
	}

	return NULL;
}

//**************************************************************************
// モーションクラスボーンの情報を取得する処理
//**************************************************************************
D3DXVECTOR3 CMotionSkin::GetPositionBone(int nSelectBone) 
{
	auto iterBone = m_vBone.begin() + nSelectBone; 
	return { (*iterBone).mtxWorld._41,(*iterBone).mtxWorld._42,(*iterBone).mtxWorld._43 }; 
}

//**************************************************************************
// モーションクラスボーンの情報を取得する処理
//**************************************************************************
D3DXVECTOR3 CMotionSkin::GetPositionBoneParent(int nSelectBone) 
{
	auto iterBone = m_vBone.begin() + nSelectBone; 
	if ((*iterBone).pBoneParent == nullptr)return InitValu::VEC3_NULL;
	return 
	{ 
		(*iterBone).pBoneParent->mtxWorld._41,
		(*iterBone).pBoneParent->mtxWorld._42,
		(*iterBone).pBoneParent->mtxWorld._43
	};
}

//**************************************************************************
// モーションクラスボーンをキーセットに適用する処理
//**************************************************************************
bool CMotionSkin::ApplicableKeySetBoneToKey(int nMotion, int nKeySet)
{
	// モーションがあれば処理する
	if (m_vMotion.size() != 0)
	{
		// 選択中のモーション情報取得
		auto iterMotion = m_vMotion.begin() + nMotion;

		// キーセットがあれば処理する
		if ((*iterMotion).vKeySet.size() != 0)
		{
			// 選択中のキーセット取得
			auto iterKeySet = (*iterMotion).vKeySet.begin() + nKeySet;

			// キーカウンター
			int nCntKey = 0;

			// キーの数分回す
			for (auto& iterKey : (*iterKeySet).vKey)
			{
				// ボーン情報取得
				auto iterBone = m_vBone.begin() + nCntKey;

				// キーカウンターカウントアップ
				nCntKey++;

				// 位置と向きを設定
				iterKey.fPosX = (*iterBone).pos.x;
				iterKey.fPosY = (*iterBone).pos.y;
				iterKey.fPosZ = (*iterBone).pos.z;
				iterKey.quat = (*iterBone).quat;
			}
		}

		return true;
	}

	return false;
}

//**************************************************************************
// モーションクラスボーンにキーセットを適用する処理
//**************************************************************************
bool CMotionSkin::ApplicableKeySetKeyToBone(int nMotion, int nKeySet)
{
	// モーションがあれば,値が範囲内であれば処理する
	if (m_vMotion.size() != 0 || (int)m_vMotion.size() < nMotion || 0 > nMotion)
	{
		// 選択中のモーション情報取得
		auto iterMotion = m_vMotion.begin() + nMotion;

		// キーセットがあれば処理する
		if ((*iterMotion).vKeySet.size() != 0)
		{
			// 選択中のキーセット取得
			auto iterKeySet = (*iterMotion).vKeySet.begin() + nKeySet;

			// キーカウンター
			int nCntKey = 0;

			// キーの数分回す
			for (auto& iterKey : (*iterKeySet).vKey)
			{
				// ボーン情報取得
				auto iterBone = m_vBone.begin() + nCntKey;

				// キーカウンターカウントアップ
				nCntKey++;

				// 位置と向きを設定
				(*iterBone).pos.x = iterKey.fPosX;
				(*iterBone).pos.y = iterKey.fPosY;
				(*iterBone).pos.z = iterKey.fPosZ;
				(*iterBone).quat = iterKey.quat;
			}
		}
		// なければ初期値にする
		else
		{
			// ボーンカウンター
			int nCntBone = 0;

			// キーの数分回す
			for (auto& iterBone : m_vBone)
			{
				// ボーンの位置と回転を初期値に戻す
				ReSetPositionBone(nCntBone);
				ReSetRotateBone(nCntBone);
				
				// ボーンカウンタ増加
				nCntBone++;
			}
		}

		return true;
	}

	return false;
}

//**************************************************
// 編集ボーンのサイズ変更処理
//**************************************************
void CMotionSkin::BoneScaling(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// 距離取得
	float fDistance = pCamera->GetDistance();

	// 距離係数を計算
	float fPalamDistance = (fDistance / 50.0f);

	if (fPalamDistance <= 0.2f)
	{
		fPalamDistance = 0.0f;
	}

	m_fMovementBone = ADDMOVEMENT * fPalamDistance;

	// すべてのボーンに適用
	for (auto& iterBoneModel : m_vpBoneModel)
	{
		float fRadius = CBone::BONE_MODEL_RADIUS * fPalamDistance;
		iterBoneModel->SetRadius(fRadius);
	}
}

//**************************************************
// 編集ボーンのサイズ変更処理
//**************************************************
D3DXVECTOR3 CMotionSkin::GetBoneVec3(int nSelectBone)
{
	// ボーンの情報取得
	auto iterBone = m_vBone.begin() + nSelectBone;

	// 自分の位置特定
	D3DXVECTOR3 posMy;
	posMy.x = iterBone->mtxWorld._41;
	posMy.y = iterBone->mtxWorld._42;
	posMy.z = iterBone->mtxWorld._43;

	// 親がいたら
	if (iterBone->pBoneParent != nullptr)
	{
		D3DXVECTOR3 posParent;
		posParent.x = iterBone->pBoneParent->mtxWorld._41;
		posParent.y = iterBone->pBoneParent->mtxWorld._42;
		posParent.z = iterBone->pBoneParent->mtxWorld._43;
		D3DXVECTOR3 vec = posMy - posParent;

		return vec;
	}
	// 子がいたら
	else if (iterBone->pBoneChild != nullptr)
	{
		// この位置特定
		D3DXVECTOR3 posChild;
		posChild.x = iterBone->pBoneChild->mtxWorld._41;
		posChild.y = iterBone->pBoneChild->mtxWorld._42;
		posChild.z = iterBone->pBoneChild->mtxWorld._43;
		D3DXVECTOR3 vec = posChild - posMy;

		return vec;
	}
	

	// 誰もいない
	return InitValu::VEC3_NULL;
}

