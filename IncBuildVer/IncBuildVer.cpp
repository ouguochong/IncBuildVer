////////////////////////////////////////////////////////////////////////////////
// �ļ�����IncBuildVer.cpp
// ���ߣ�Song Baoming,2016
// ��;������ָ���� Version.h �ļ����Զ����ӱ���汾�ŵĳ�����ں�������
// License��https://github.com/songbaoming/IncBuildVer/blob/master/LICENSE
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Shellapi.h"
#include "IncBuildVer.h"
#include "VerFile.h"




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ��ȡ���������� Version.h ������·��
	int nArgc;
	LPWSTR *ppArgv = CommandLineToArgvW(GetCommandLine(), &nArgc);
	if (nArgc > 1) {
		CVerFile file;
		file.IncBuildVer(ppArgv[1]);
	}

	LocalFree(ppArgv);
	return 0;
}
