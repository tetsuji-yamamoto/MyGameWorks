//********************************************************
// 
// 特定部位モーションコン親ポーネント
// Author Tetsuji Yamamoto
// 
//********************************************************
#include "compPartMotion.h"
#include "object.h"
#include "myMath.h"
#include "vector.h"
#include "quaternion.h"

using namespace my_math;
using namespace my_vector;
using namespace Quaternion;

//********************************************************
// コンストラクタ
//********************************************************
CPartMotion::CPartMotion()
{
	m_motionInfo.bEndMotion = true;	// モーションを終っている
	m_bEndBlend = false;			// ブレンド終了フラグ
	m_nTimeBlend;					// ブレンド時間
	m_nCntBlend = 0;				// 時間カウンタ
}

//********************************************************
// デストラクタ
//********************************************************
CPartMotion::~CPartMotion()
{

}

//********************************************************
// 初期化
//********************************************************
void CPartMotion::Init(void)
{
	
}

//********************************************************
// 終了
//********************************************************
void CPartMotion::Uninit(void)
{
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CPartMotion::Update(void)
{
	// モーションが終っていたら処理しない
	if (m_motionInfo.bEndMotion == true)return;

	// モーション情報更新
	UpdateMotionInfo(m_motion,m_motionInfo);
	UpdateMotionInfo(m_motionBlend,m_motionInfoBlend);

	// ブレンドタイムをカウントアップして最大まで達したらブレンド終了フラグを立てる
	if (CountClamp(m_nCntBlend, 0, m_nTimeBlend) == true)
	{
		m_bEndBlend = true;
	}

	// モーションの更新
	UpdateMotion();
}

//********************************************************
// 描画
//********************************************************
void CPartMotion::Draw(void)
{

}

//********************************************************
// モーションの設定するキー作成
//********************************************************
CMotionSkin::KEY CPartMotion::CreateMotionKey(const int nIdxBone, CMotionSkin::MOTION rMotion, CMotionSkin::MOTIONINFO& rMotionInfo)
{
	// 出力用変数
	CMotionSkin::KEY keyOut;

	// キーセットが無ければ処理しない
	if (rMotion.vKeySet.empty() == true)return keyOut;

	int nKeySetNow = rMotionInfo.nKeySetNow;						// 今のキー
	int nKeySetNext = rMotionInfo.nKeySetNext;						// 今のキー
	int nFrame = rMotion.vKeySet[nKeySetNow].nFlame;				// フレーム数
	CMotionSkin::KEYSET keySetNow = rMotion.vKeySet[nKeySetNow];	// キーセット
	CMotionSkin::KEYSET keySetNext = rMotion.vKeySet[nKeySetNext];	// キーセット

	// 今と次の位置と向きを取得
	D3DXVECTOR3 posNow, posNext;
	D3DXQUATERNION quatNow, quatNext;
	posNow.x = keySetNow.vKey[nIdxBone].fPosX;
	posNow.y = keySetNow.vKey[nIdxBone].fPosY;
	posNow.z = keySetNow.vKey[nIdxBone].fPosZ;
	posNext.x = keySetNext.vKey[nIdxBone].fPosX;
	posNext.y = keySetNext.vKey[nIdxBone].fPosY;
	posNext.z = keySetNext.vKey[nIdxBone].fPosZ;
	quatNow = keySetNow.vKey[nIdxBone].quat;
	quatNext = keySetNext.vKey[nIdxBone].quat;

	// パラメータ算出
	float fPalam = Palam(m_motionInfo.nCounterMotion, nFrame);

	// 位置を線形補間
	D3DXVECTOR3 posOut = Vec3Lerp(posNow, posNext, fPalam);

	// 向きを球面線形補間
	D3DXQUATERNION quatOut = QuatSlerp(quatNow, quatNext, fPalam);

	// キーの値を設定
	keyOut.fPosX = posOut.x;
	keyOut.fPosY = posOut.y;
	keyOut.fPosZ = posOut.z;
	keyOut.quat = quatOut;

	// 返す
	return keyOut;
}

//********************************************************
// モーション情報の更新
//********************************************************
void CPartMotion::UpdateMotion()
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// モーション情報取得
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();

	// ボーンのインデックス分回す
	for (auto iterBoneIdx : m_vBoneIdx)
	{
		// 設定用変数
		D3DXVECTOR3 posSet;
		D3DXQUATERNION quatSet;

		// 設定したいキーを取得
		CMotionSkin::KEY key = CreateMotionKey(iterBoneIdx,m_motion, m_motionInfo);
		CMotionSkin::KEY keyBlend = CreateMotionKey(iterBoneIdx,m_motionBlend, m_motionInfoBlend);

		// ブレンドをするのであれば
		if (m_bEndBlend == false)
		{
			// ブレンドパラメータ算出
			float fPlam = Palam(m_nCntBlend, m_nTimeBlend);
			float fPalamBlend = 1.0f - fPlam;

			// ブレンドした値を設定
			posSet.x = (key.fPosX * fPlam) + (keyBlend.fPosX * fPalamBlend);
			posSet.y = (key.fPosY * fPlam) + (keyBlend.fPosY * fPalamBlend);
			posSet.z = (key.fPosZ * fPlam) + (keyBlend.fPosZ * fPalamBlend);
			quatSet = (key.quat * fPlam) + (keyBlend.quat * fPalamBlend);
		}
		else
		{
			// そのまま値を設定
			posSet.x = key.fPosX;
			posSet.y = key.fPosY;
			posSet.z = key.fPosZ;
			quatSet = key.quat;
		}

		// 位置と向きを設定
		pMotionSkin->SetBonePos(iterBoneIdx, posSet);
		pMotionSkin->SetBoneRotate(iterBoneIdx, quatSet);
	}
}

//********************************************************
// モーション情報の更新
//********************************************************
void CPartMotion::UpdateMotionInfo(CMotionSkin::MOTION rMotion, CMotionSkin::MOTIONINFO& rMotionInfo)
{
	// モーションが終っていたら処理しない
	if (rMotionInfo.bEndMotion == true)return;

	bool bLoop = rMotion.bLoop;								// ループ
	int nKeySetNow = rMotionInfo.nKeySetNow;					// 今のキー
	int nFrame = rMotion.vKeySet[nKeySetNow].nFlame;			// フレーム数
	CMotionSkin::KEYSET keySet = rMotion.vKeySet[nKeySetNow];	// キーセット

	// モーションをカウントアップして最大値まで達したら
	if (CountWrap(rMotionInfo.nCounterMotion, 0, nFrame) == true)
	{
		// キーセットの最大値を計算(ゼロから数えた値にする)
		int nMaxNumKeySet = Decrement((int)rMotion.vKeySet.size(), 0);

		// キーセットをカウントアップ
		rMotionInfo.nKeySetNow++;
		rMotionInfo.nKeySetNow = Wrap(rMotionInfo.nKeySetNow, 0, nMaxNumKeySet);
		bool bMaxNow = Max(rMotionInfo.nKeySetNow, nMaxNumKeySet);

		// 次のキー設定
		rMotionInfo.nKeySetNext++;
		rMotionInfo.nKeySetNext = Wrap(rMotionInfo.nKeySetNext, 0, nMaxNumKeySet);
		bool bMaxNext = Max(rMotionInfo.nKeySetNext, nMaxNumKeySet);


		// キーが最大数までカウントされた
		if (bMaxNow == true)
		{
			// ループしないのであれば
			if (bLoop == false)
			{
				// キーの数を最大値に設定しておく
				rMotionInfo.nKeySetNow = nMaxNumKeySet;
				rMotionInfo.nKeySetNext = nMaxNumKeySet;

				// モーションが終ったことにする
				rMotionInfo.bEndMotion = true;
			}
		}
	}
}

//********************************************************
// モーション設定
//********************************************************
void CPartMotion::SetMotion(const int nTypeMotion, const int nTimeBlend)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// モーション情報取得
	CMotionSkin* pMotionskin = pOwner->GetComponent<CMotionSkin>();

	// 取得できなければ処理しない
	if (pMotionskin == nullptr)return;

	// ブレンド用モーション引き継ぎ
	m_motionBlend = m_motion;

	// ブレンド用モーション情報引き継ぎ
	m_motionInfoBlend = m_motionInfo;

	// ブレンドする時間設定
	m_nTimeBlend = nTimeBlend;

	// ブレンドが終っていない状態にする
	m_bEndBlend = false;

	// ブレンドカウンターリセット
	m_nCntBlend = 0;

	// モーション取得
	m_motion = pMotionskin->GetMotion(nTypeMotion);

	// キーセットの最大値を計算(ゼロから数えた値にする)
	int nMaxNumKeySet = Decrement((int)m_motion.vKeySet.size(), 0);

	// モーションタイプ設定
	m_motionInfo.nTypeMotionNow = nTypeMotion;
	
	// モーションを終っていない状態にする
	m_motionInfo.bEndMotion = false;

	// モーションカウンターリセット
	m_motionInfo.nCounterMotion = 0;

	// 今のキーセットゼロ
	m_motionInfo.nKeySetNow = 0;

	// 次のキーセット設定
	m_motionInfo.nKeySetNext = 1;
	m_motionInfo.nKeySetNext = Clamp(m_motionInfo.nKeySetNext, 0, nMaxNumKeySet);
}