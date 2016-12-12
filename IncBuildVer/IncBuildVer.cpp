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
		// ���� version.h ·����ȡ����Ŀ�µ����������ͼ�¼�ļ�·��
		TCHAR szLogPath[MAX_PATH];
		_tcscpy_s(szLogPath, ppArgv[2]);
		auto p = _tcsrchr(szLogPath, TEXT('\\'));
		if (!p)
			break;
		p[1] = 0;
		_tcscat_s(szLogPath, TEXT("ibv_lbt.log"));
		// ��ȡ������ĳ�������
		TCHAR szType[10];
		GetPrivateProfileString(TEXT("LASTBUILD"), TEXT("TYPE"), TEXT(""), szType, 10, szLogPath);
		
		if (_tcsicmp(szType, ppArgv[1])){
			// ���α������ͺ����ͬ��д�뱾�α�������
			WritePrivateProfileString(TEXT("LASTBUILD"), TEXT("TYPE"), ppArgv[1], szLogPath);
			if(szType[0])
				break; // ����������Ϊ��ʱ��˵���ǵ�һ�α��룬��Ҫ�԰汾�ż�һ�����Բ�Ϊ��ʱ������һ����
		}
		
		// �Ա���汾�ż�һ
		CVerFile file;
		file.IncBuildVer(ppArgv[2]);
	} while (false);

	LocalFree(ppArgv);
	return 0;
}
