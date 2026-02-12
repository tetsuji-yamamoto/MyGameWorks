//********************************************************
// 
// 特定部位モーションコンポーネント
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_PART_MOTION_H_
#define _COMPONENT_PART_MOTION_H_

#include "component.h"
#include "compMotionSkin.h"

// 特定部位モーションクラス
class CPartMotion : public CComponent
{
public:
	CPartMotion();
	~CPartMotion();

	void Init	(void);
	void Uninit	(void);
	void Update	(void);
	void Draw	(void);

	void PushBackBoneIdx(const int nBoneIdx) { m_vBoneIdx.push_back(nBoneIdx); }

	// セッター
	void SetMotion(const int nTypeMotion,const int nTimeBlend);

	// ゲッター
	CMotionSkin::MOTION		GetMotion		(void)	{ return m_motion; }
	std::vector<int>		GetBoneIdx		(void)	{ return m_vBoneIdx;}
	bool					GetIsMotionEnd	(void)	{ return m_motionInfo.bEndMotion; }

private:
	void				UpdateMotionInfo	(CMotionSkin::MOTION rMotion,CMotionSkin::MOTIONINFO& rMotionInfo);
	CMotionSkin::KEY	CreateMotionKey		(const int nIdxBone,CMotionSkin::MOTION rMotion, CMotionSkin::MOTIONINFO& rMotionInfo);
	void				UpdateMotion		();

	CMotionSkin::MOTION			m_motion;			// モーション
	CMotionSkin::MOTION			m_motionBlend;		// ブレンド用モーション
	CMotionSkin::MOTIONINFO		m_motionInfo;		// モーション情報
	CMotionSkin::MOTIONINFO		m_motionInfoBlend;	// ブレンド用モーション情報
	int							m_nTimeBlend;		// ブレンドする時間
	int							m_nCntBlend;		// ブレンドカウンタ
	bool						m_bEndBlend;		// ブレンド終了フラグ
	std::vector<int>			m_vBoneIdx;			// ボーンインデックス
};
#endif
