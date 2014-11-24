// BonStub.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "BonStub.h"

HINSTANCE lib;
IBonDriver *tuner;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc;
	LPTSTR *argv;
	argv = CommandLineToArgvW(GetCommandLine(), &argc);
	if (argc != 2)
	{
		printf("argument is invalid.\n");
		return 1;
	}
	
	BOOL isBonDriverExists = PathFileExists(argv[1]);
	if (!isBonDriverExists)
	{
		printf("%s is invalid path.\n", argv[1]);
		return 1;
	}

	if (LoadBonDriver(argv[1]))
	{
		if (OpenTuner())
		{
			getchar();
			// 適当
			CloseTuner();
			UnloadBonDriver();
		}
	}

	return 0;
}

bool LoadBonDriver(LPCTSTR pszFileName)
{
	printf("BonDriver \"%s\" を読み込みます...\n", pszFileName);

	lib = LoadLibrary(pszFileName);

	if (lib == NULL) {
		const DWORD ErrorCode = GetLastError();

		printf("\"%s\" が読み込めません。\n", pszFileName);

		switch (ErrorCode) {
		case ERROR_MOD_NOT_FOUND:
			printf("ファイルが見つかりません。\n");
			break;

		case ERROR_BAD_EXE_FORMAT:
#ifndef _WIN64
			printf("32ビット用の BonDriver ではないか、ファイルが破損している可能性があります。\n");
#else
			printf("64ビット用の BonDriver ではないか、ファイルが破損している可能性があります。\n");
#endif
			break;

		case ERROR_SXS_CANT_GEN_ACTCTX:
			printf("この BonDriver に必要なランタイムがインストールされていない可能性があります。\n");
			break;

		default:
			printf("エラーコード: 0x%x\n", ErrorCode);
		}
		return false;
	}

	printf("BonDriver を読み込みました。\n");

	return true;
}

bool UnloadBonDriver()
{
	if (lib != NULL) {
		CloseTuner();

		printf("BonDriver を解放します...\n");
		FreeLibrary(lib);
		lib = NULL;
		printf("BonDriver を解放しました。\n");
	}

	return true;
}

bool IsBonDriverLoaded()
{
	return lib != NULL;
}

bool OpenTuner()
{
	if (lib == NULL) {
		printf("BonDriverが読み込まれていません。\n");
		return false;
	}

	// オープンチェック
	if (tuner) {
		printf("チューナは既に開かれています。\n");
		return false;
	}

	printf("チューナを開いています...\n");

	// ドライバポインタの取得

	

	PFCREATEBONDRIVER *pfCreateBonDriver =
		(PFCREATEBONDRIVER*)::GetProcAddress(lib, "CreateBonDriver");
	if (pfCreateBonDriver == NULL) {
		printf("CreateBonDriver() のアドレスを取得できません。\n");
		printf("指定された DLL が BonDriver ではありません。\n");
		return false;
	}
	tuner = pfCreateBonDriver();
	if (tuner == NULL) {
		printf("IBonDriver を取得できません。\n");
		printf("CreateBonDriver() の呼び出しで NULL が返されました。\n");
		return false;
	}

	BOOL bTunerOpened;

	try {
		// チューナを開く
		bTunerOpened = tuner->OpenTuner();

		if (!bTunerOpened) {
			printf("チューナを開けません。\n");
			printf("BonDriver にチューナを開くよう要求しましたがエラーが返されました。\n");
			throw ERR_TUNEROPEN;
		}

	}
	catch (...) {
		if (bTunerOpened)
			tuner->CloseTuner();
		tuner->Release();
		tuner = NULL;
		return false;
	}

	printf("チューナを開きました。\n");

	return true;
}

bool CloseTuner(void)
{
	if (tuner) {
		// チューナを閉じる
		printf("チューナを閉じています...\n");
		tuner->CloseTuner();

		// ドライバインスタンス開放
		printf("BonDriver インターフェースを解放しています...\n");
		tuner->Release();
		tuner = NULL;
		printf("BonDriver インターフェースを解放しました。\n");
	}
	return true;
}