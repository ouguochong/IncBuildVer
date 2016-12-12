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
	do{
		// �жϲ��������Ƿ���ڵ���3
		if (nArgc < 3)
			break;
		// �޸ĳ���·��Ϊ�����ļ�·��
		auto p = _tcsrchr(ppArgv[0], TEXT('.'));
		if (!p)
			break;
		_tcscpy_s(p, 5, TEXT(".ini"));
		// ��ȡ�ϴα���ĳ�������
		TCHAR szType[10];
		GetPrivateProfileString(TEXT("LASTBUILD"), TEXT("TYPE"), TEXT("Debug"), szType, 10, ppArgv[0]);
		
		if (_tcsicmp(szType, ppArgv[1])){
			// ���α������ͺ��ϴβ�ͬ��д�뱾�α�������
			WritePrivateProfileString(TEXT("LASTBUILD"), TEXT("TYPE"), ppArgv[1], ppArgv[0]);
			break;
		}
		
		// �Ա���汾�ż�һ
		CVerFile file;
		file.IncBuildVer(ppArgv[2]);
	} while (false);

	LocalFree(ppArgv);
	return 0;
}
