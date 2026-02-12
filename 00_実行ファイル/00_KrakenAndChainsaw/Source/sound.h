//****************************************************************************
// 
// サウンド処理
// Author tetuji yamamoto
// 
//****************************************************************************
#ifndef _SOUND_H_

// サウンドクラス
class CSound
{
public:

	// サウンドタイプ
	typedef enum
	{
		SOUND_LABEL_BGM_WAVE = 0,				// BGM波
		SOUND_LABEL_SE_CHAINSAW_ENGIN_STARTUP,	// SEチェーンソー起動音
		SOUND_LABEL_BGM_CHAINSAW_ENGIN_IDLING,	// BGMチェーンソーアイドリング音
		SOUND_LABEL_BGM_CHAINSAW_CUT,			// BGMチェンソーで切る音
		SOUND_LABEL_SE_CHAINSAW_ENGIN_STOP,		// SEチェーンソー停止音
		SOUND_LABEL_MAX

	} SOUND_LABEL;

	// サウンド情報の構造体
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;

	CSound();
	~CSound();

	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	// サウンドの情報
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{"data\\sound\\Wave.wav",-1},					// BGM波
		{"data\\sound\\Chainsaw_Engin_Start.wav",0},	// SEチェーンソー起動音
		{"data\\sound\\Chainsaw_Idling.wav",-1},		// BGMチェーンソーアイドリング音
		{"data\\sound\\Chainsaw_Cut.wav",-1},			// BGMチェンソーで切る音
		{"data\\sound\\Chainsaw_Engin_Stop.wav",0},		// SEチェーンソー停止音
	};
};
#endif