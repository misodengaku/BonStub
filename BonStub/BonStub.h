#include "stdafx.h"

enum {
	ERR_NOERROR,		// エラーなし
	ERR_NOTLOADED,		// ライブラリが読み込まれていない
	ERR_DRIVER,			// ドライバエラー
	ERR_TUNEROPEN,		// チューナオープンエラー
	ERR_TUNER,			// チューナエラー
	ERR_NOTOPEN,		// チューナが開かれていない
	ERR_ALREADYOPEN,	// チューナが既に開かれている
	ERR_NOTPLAYING,		// 再生されていない
	ERR_ALREADYPLAYING,	// 既に再生されている
	ERR_TIMEOUT,		// タイムアウト
	ERR_PENDING,		// 前の処理が終わっていない
	ERR_INTERNAL		// 内部エラー
};


typedef IBonDriver* (PFCREATEBONDRIVER)(void);

bool LoadBonDriver(LPCTSTR pszFileName);
bool UnloadBonDriver();
bool IsBonDriverLoaded();
bool OpenTuner();
bool CloseTuner();