//////////////////////////////////////////////////////////////
// �ļ�����Version.h
// ���ߣ�Song Baoming,2016
// ��;���������汾�ŵĺ꣬���ڱ���ʱ�Զ����ӱ���汾��
// License��https://github.com/songbaoming/IncBuildVer/blob/master/LICENSE
//////////////////////////////////////////////////////////////

#pragma once

// �汾��
#define MARJOR_VER_NUM					1 // ���汾��
#define MINOR_VER_NUM					0 // �ΰ汾��
#define REVISION_VER_NUM				0 // �����汾��
#define BUILD_VER_NUM					4 // ����汾��


// �汾���ַ���
#define TOSTR0(arg) #arg
#define TOSTR(arg) TOSTR0(arg)
#define VERSION_STR TOSTR(MARJOR_VER_NUM) "." TOSTR(MINOR_VER_NUM) "." TOSTR(REVISION_VER_NUM) "." TOSTR(BUILD_VER_NUM)
