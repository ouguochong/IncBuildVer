////////////////////////////////////////////////////////////////////////////////
// �ļ�����IncBuildVer.cpp
// ���ߣ�Song Baoming,2016
// ��;������ָ���� resource.h �� *.rc �ļ����Զ����ӱ���汾�ŵĳ�����ں�������
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

	// ��ȡ�����в�������
	int nArgc;
	LPWSTR *ppArgv = CommandLineToArgvW(GetCommandLine(), &nArgc);
	do{
		// �ж������в��������Ƿ���ڵ���3
		if (nArgc < 3)
			break;
		
		// �Ա���汾�ż�һ
		CVerFile file;
		file.IncBuildVer(ppArgv[2]);
	} while (false);

	LocalFree(ppArgv);
	return 0;
}
