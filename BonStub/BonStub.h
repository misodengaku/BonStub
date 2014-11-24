#include "stdafx.h"

enum {
	ERR_NOERROR,		// �G���[�Ȃ�
	ERR_NOTLOADED,		// ���C�u�������ǂݍ��܂�Ă��Ȃ�
	ERR_DRIVER,			// �h���C�o�G���[
	ERR_TUNEROPEN,		// �`���[�i�I�[�v���G���[
	ERR_TUNER,			// �`���[�i�G���[
	ERR_NOTOPEN,		// �`���[�i���J����Ă��Ȃ�
	ERR_ALREADYOPEN,	// �`���[�i�����ɊJ����Ă���
	ERR_NOTPLAYING,		// �Đ�����Ă��Ȃ�
	ERR_ALREADYPLAYING,	// ���ɍĐ�����Ă���
	ERR_TIMEOUT,		// �^�C���A�E�g
	ERR_PENDING,		// �O�̏������I����Ă��Ȃ�
	ERR_INTERNAL		// �����G���[
};


typedef IBonDriver* (PFCREATEBONDRIVER)(void);

bool LoadBonDriver(LPCTSTR pszFileName);
bool UnloadBonDriver();
bool IsBonDriverLoaded();
bool OpenTuner();
bool CloseTuner();