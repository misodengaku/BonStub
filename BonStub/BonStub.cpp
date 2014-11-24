// BonStub.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
			// �K��
			CloseTuner();
			UnloadBonDriver();
		}
	}

	return 0;
}

bool LoadBonDriver(LPCTSTR pszFileName)
{
	printf("BonDriver \"%s\" ��ǂݍ��݂܂�...\n", pszFileName);

	lib = LoadLibrary(pszFileName);

	if (lib == NULL) {
		const DWORD ErrorCode = GetLastError();

		printf("\"%s\" ���ǂݍ��߂܂���B\n", pszFileName);

		switch (ErrorCode) {
		case ERROR_MOD_NOT_FOUND:
			printf("�t�@�C����������܂���B\n");
			break;

		case ERROR_BAD_EXE_FORMAT:
#ifndef _WIN64
			printf("32�r�b�g�p�� BonDriver �ł͂Ȃ����A�t�@�C�����j�����Ă���\��������܂��B\n");
#else
			printf("64�r�b�g�p�� BonDriver �ł͂Ȃ����A�t�@�C�����j�����Ă���\��������܂��B\n");
#endif
			break;

		case ERROR_SXS_CANT_GEN_ACTCTX:
			printf("���� BonDriver �ɕK�v�ȃ����^�C�����C���X�g�[������Ă��Ȃ��\��������܂��B\n");
			break;

		default:
			printf("�G���[�R�[�h: 0x%x\n", ErrorCode);
		}
		return false;
	}

	printf("BonDriver ��ǂݍ��݂܂����B\n");

	return true;
}

bool UnloadBonDriver()
{
	if (lib != NULL) {
		CloseTuner();

		printf("BonDriver ��������܂�...\n");
		FreeLibrary(lib);
		lib = NULL;
		printf("BonDriver ��������܂����B\n");
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
		printf("BonDriver���ǂݍ��܂�Ă��܂���B\n");
		return false;
	}

	// �I�[�v���`�F�b�N
	if (tuner) {
		printf("�`���[�i�͊��ɊJ����Ă��܂��B\n");
		return false;
	}

	printf("�`���[�i���J���Ă��܂�...\n");

	// �h���C�o�|�C���^�̎擾

	

	PFCREATEBONDRIVER *pfCreateBonDriver =
		(PFCREATEBONDRIVER*)::GetProcAddress(lib, "CreateBonDriver");
	if (pfCreateBonDriver == NULL) {
		printf("CreateBonDriver() �̃A�h���X���擾�ł��܂���B\n");
		printf("�w�肳�ꂽ DLL �� BonDriver �ł͂���܂���B\n");
		return false;
	}
	tuner = pfCreateBonDriver();
	if (tuner == NULL) {
		printf("IBonDriver ���擾�ł��܂���B\n");
		printf("CreateBonDriver() �̌Ăяo���� NULL ���Ԃ���܂����B\n");
		return false;
	}

	BOOL bTunerOpened;

	try {
		// �`���[�i���J��
		bTunerOpened = tuner->OpenTuner();

		if (!bTunerOpened) {
			printf("�`���[�i���J���܂���B\n");
			printf("BonDriver �Ƀ`���[�i���J���悤�v�����܂������G���[���Ԃ���܂����B\n");
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

	printf("�`���[�i���J���܂����B\n");

	return true;
}

bool CloseTuner(void)
{
	if (tuner) {
		// �`���[�i�����
		printf("�`���[�i����Ă��܂�...\n");
		tuner->CloseTuner();

		// �h���C�o�C���X�^���X�J��
		printf("BonDriver �C���^�[�t�F�[�X��������Ă��܂�...\n");
		tuner->Release();
		tuner = NULL;
		printf("BonDriver �C���^�[�t�F�[�X��������܂����B\n");
	}
	return true;
}