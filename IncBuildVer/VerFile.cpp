////////////////////////////////////////////////////////////////////////////////
// �ļ�����VerFile.cpp
// ���ߣ�Song Baoming,2016
// ��;������ resource.h �� *.rc �ļ������ӱ���汾���� CVerFile �ķ��������ļ�
// License��https://github.com/songbaoming/IncBuildVer/blob/master/LICENSE
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VerFile.h"



#define MAJOR				0
#define MINOR				1
#define REVISION			2
#define BUILD				3







CVerFile::CVerFile()
	: m_dwOffset(0)
	, m_dwCode(0)
	, m_hFile(nullptr)
	, m_pContent(nullptr)
{
}


CVerFile::~CVerFile()
{
	if (m_pContent)
		delete[] m_pContent;
	if (m_hFile && m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);
}

// ��ָ���ļ��б���汾�ź��ֵ��һ
bool CVerFile::IncBuildVer(LPCTSTR lpszDirPath)
{
	TCHAR szFilePath[MAX_PATH];
	_stprintf_s(szFilePath, TEXT("%sresource.h"), lpszDirPath);
	// resource.h
	if (!IncResourceVer(szFilePath))
		return false;
	// .rc�ļ�·��
	auto pos = _tcsrchr(lpszDirPath, TEXT('\\'));
	if (!pos)
		return false;
	while (pos != lpszDirPath && *(pos - 1) != TEXT('\\'))
		--pos;
	if (pos == lpszDirPath)
		return false;
	_stprintf_s(szFilePath, TEXT("%s%s"), lpszDirPath, pos);
	szFilePath[_tcslen(szFilePath) - 1] = 0;
	_tcscat_s(szFilePath, TEXT(".rc"));
	// .rc
	if (!IncRCFileVer(szFilePath))
		return false;

	return true;
}

bool CVerFile::IncResourceVer(LPCTSTR lpszFilePath)
{
	static LPCTSTR pszVerSign[] = {
		TEXT("MAJOR_VER_NUM"),
		TEXT("MINOR_VER_NUM"),
		TEXT("REVISION_VER_NUM"),
		TEXT("BUILD_VER_NUM"),
		NULL
	};

	bool bRes = true;
	
	// ��ȡ�ļ�����
	if (!GetFileContent(lpszFilePath))
		return false;

	auto pszBegin = m_pContent;
	for (int i = 0; pszVerSign[i]; ++i){
		pszBegin = _tcsstr(m_pContent, pszVerSign[i]);
		if (!pszBegin)
			return false;
		pszBegin += _tcslen(pszVerSign[i]);
		m_dwVersion[i] = _ttoi(pszBegin);
	}
	// ��ת������汾�ŵ�λ��
	while (*pszBegin && !_istdigit(*pszBegin)) ++pszBegin;
	if (!*pszBegin)
		return false;
	auto pszEnd = pszBegin;
	while (*pszEnd && _istdigit(*pszEnd)) ++pszEnd;
	// ���ݾͰ汾�ż����°汾�ţ���ת��Ϊ�ַ���
	//DWORD dwBuildVer = _ttoi(pszBegin);
	TCHAR szNewVer[20];
	_stprintf_s(szNewVer, TEXT("%u"), ++m_dwVersion[BUILD]);

	// ʹ�ð汾����ǰ���ַ����ض�λ�ļ�ָ��
	if (!SetFilePtrWithString(m_pContent, pszBegin - m_pContent))
		return false;
	// д���°汾��
	if (!WriteContent(szNewVer, _tcslen(szNewVer)))
		return false;
	// �жϰ汾�ų����Ƿ�һ�£�ֻ�в�һ��ʱ����Ҫ����д��汾�ź������
	if (pszEnd - pszBegin != _tcslen(szNewVer)){
		bRes = WriteContent(pszEnd, _tcslen(pszEnd));
		SetEndOfFile(m_hFile);
	}
	return bRes;
}

bool CVerFile::IncRCFileVer(LPCTSTR lpszFilePath)
{
	static LPCTSTR pszSign[] = {
		TEXT("FILEVERSION"),
		TEXT("PRODUCTVERSION"),
		TEXT("\"FileVersion\","),
		TEXT("\"ProductVersion\","),
		NULL
	};
	bool bRes = false;

	// ��ȡ�ļ�����
	if (!GetFileContent(lpszFilePath))
		return false;

	auto pszBegin = m_pContent;
	auto pszEnd = pszBegin;
	TCHAR szText[100];
	for (int i = 0; pszSign[i]; ++i){

		pszEnd = _tcsstr(pszBegin, pszSign[i]);
		if (!pszEnd)
			return false;
		pszEnd += _tcslen(pszSign[i]);
		while (*pszEnd && !_istgraph(*pszEnd)) ++pszEnd;
		if (!*pszEnd)
			return false;

		if (i == 0)
			bRes = SetFilePtrWithString(pszBegin, pszEnd - pszBegin);
		else
			bRes = WriteContent(pszBegin, pszEnd - pszBegin);
		if (!bRes)
			return false;
		
		if (!i){
			_stprintf_s(szText, TEXT("%u,%u,%u,%u"), m_dwVersion[MAJOR], m_dwVersion[MINOR]
				, m_dwVersion[REVISION], m_dwVersion[BUILD]);
		}else if(i == 2){
			_stprintf_s(szText, TEXT("\"%u.%u.%u.%u\""), m_dwVersion[MAJOR], m_dwVersion[MINOR]
				, m_dwVersion[REVISION], m_dwVersion[BUILD]);
		}
		if (!WriteContent(szText, _tcslen(szText)))
			return false;

		while (*pszEnd && _istprint(*pszEnd)) ++pszEnd;
		if (!*pszEnd)
			return false;
		pszBegin = pszEnd;
	}
	// д��ʣ������
	bRes = WriteContent(pszEnd, _tcslen(pszEnd));
	SetEndOfFile(m_hFile);

	return bRes;
}

// ��ȡָ���ļ����ݣ���ת��Ϊ Unicode ����
bool CVerFile::GetFileContent(LPCTSTR lpszFilePath)
{
	// ���ļ�
	if (m_hFile)
		CloseHandle(m_hFile);
	m_hFile = CreateFile(lpszFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
		nullptr, OPEN_EXISTING, 0, nullptr);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return false;
	LARGE_INTEGER large;
	GetFileSizeEx(m_hFile, &large);
	if (!large.QuadPart)
		return false;
	// �����ļ�ӳ�䲢ӳ�䵽�ڴ�ҳ
	HANDLE hMapping = CreateFileMapping(m_hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
	if (!hMapping)
		return false;
	auto pData = (char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	CloseHandle(hMapping);
	if (!pData)
		return false;
	// ��ȡ�ļ�����
	GetFileContentCode(pData, large.QuadPart);
	// �����ļ����뽫�ļ�����ת���� unicode
	if (m_pContent)
		delete[] m_pContent;
	m_pContent = FileContentToUnicode(pData + m_dwOffset, large.QuadPart - m_dwOffset);

	UnmapViewOfFile(pData);
	return m_pContent != nullptr;
}

// ��ȡ���ļ����ݵı����ʽ
bool CVerFile::GetFileContentCode(LPCSTR pData, LONGLONG llLen)
{
	if (llLen > 3) {
		DWORD dwBOM = MAKELONG(MAKEWORD(pData[0], pData[1]), MAKEWORD(pData[2], 0));
		if (dwBOM == 0xBFBBEF) { // utf-8
			m_dwCode = CP_UTF8;
			m_dwOffset = 3;
		}
		else if (LOWORD(dwBOM) == 0xFFFE) { // utf-16 big endian
			m_dwCode = CP_UTF16B;
			m_dwOffset = 2;
		}
		else if (LOWORD(dwBOM) == 0xFEFF) { // utf-16
			m_dwCode = CP_UTF16;
			m_dwOffset = 2;
		}
	}
	if(!m_dwCode) {
		if (IsCodeUtf8(pData, llLen))
			m_dwCode = CP_UTF8;
		else
			m_dwCode = IsCodeUtf16(pData, llLen);
	}
	
	return true;
}

// �жϸ����������Ƿ�Ϊutf-8����
bool CVerFile::IsCodeUtf8(LPCSTR pString, LONGLONG llLen)
{
	bool bRet = false;
	auto pData = (PBYTE)pString;
	for (LONGLONG i = 0; i < llLen; ++i) {
		if (pData[i] > 0x80) {
			int nCount = 1;
			while ((pData[i] << nCount) & 0x80) ++nCount;
			if (nCount > 6 || i + nCount > llLen)
				return false;
			for (int j = 1; j < nCount; ++j) {
				if ((pData[i + j] & 0xc0) != 0x80)
					return false;
			}
			//i += nCount;
			//bRet = true;
			return true;
		}
		//else
		//	++i;
	}
	return bRet;
}

// �жϸ��������Ƿ�Ϊutf-16��ʽ�ı��룬������򷵻��ַ����룬���򷵻���
int CVerFile::IsCodeUtf16(LPCSTR pString, LONGLONG llLen)
{
	if (llLen % 2)
		return 0;
	for (LONGLONG i = 0; i + 1 < llLen; ++i) {
		if (!pString[i])
			return CP_UTF16B;
		else if (!pString[i + 1])
			return CP_UTF16;
	}
	return 0;
}

LPTSTR CVerFile::FileContentToUnicode(LPCSTR lpszSrc, LONGLONG llLen)
{
	LPTSTR pContent = nullptr;
	if (m_dwCode == CP_UTF16) {
		pContent = new TCHAR[llLen / sizeof(TCHAR) + 1];
		if (pContent) {
			memset(pContent, 0, llLen + sizeof(TCHAR));
			memcpy_s(pContent, llLen + sizeof(TCHAR), lpszSrc, llLen);
		}
	}
	else if (m_dwCode == CP_UTF16B) {
		pContent = new TCHAR[llLen / sizeof(TCHAR)+1];
		if (pContent) {
			memset(pContent, 0, llLen + sizeof(TCHAR));
			char *pDst = (char*)pContent;
			const char *pSrc = lpszSrc;
			for (LONGLONG i = 0; i + 1 < llLen; i += 2) {
				pDst[i] = pSrc[i + 1];
				pDst[i + 1] = pSrc[i];
			}
		}
	}
	else {
		DWORD dwLen = MultiByteToWideChar(m_dwCode, 0, lpszSrc, llLen, nullptr, 0);
		if (dwLen) {
			pContent = new TCHAR[dwLen];
			if (pContent)
				MultiByteToWideChar(m_dwCode, 0, lpszSrc, llLen, pContent, dwLen);
		}
	}
	return pContent;
}

// �Ӹ����İ汾��֮ǰ���ַ�����������ԭ�ļ����뷽ʽ���ļ����ݵ�ƫ������
// ���㲢�����ļ�ָ���λ�ã���������汾��֮ǰ���ᱻ���ĵ����ݣ�
// ���ں���д�룬������ I/O ���Ч��
bool CVerFile::SetFilePtrWithString(LPCTSTR lpszProBuildVer, DWORD dwLen)
{
	LARGE_INTEGER large = { m_dwOffset };

	if (m_dwCode != CP_UTF16 && m_dwCode != CP_UTF16B) {
		int nLen = WideCharToMultiByte(m_dwCode, 0, lpszProBuildVer, dwLen, nullptr, 0, nullptr, nullptr);
		if (!nLen)
			return false;
		large.QuadPart += nLen;
	}
	else
		large.QuadPart += dwLen * sizeof(TCHAR);
	
	return TRUE == SetFilePointerEx(m_hFile, large, nullptr, FILE_BEGIN);
}

// �����������ݣ�ת��Ϊԭ�ļ������ʽ��д���ļ�
bool CVerFile::WriteContent(LPCTSTR lpszContent, DWORD dwLen)
{
	DWORD dwWriten;

	if (m_dwCode == CP_UTF16) {
		return WriteFile(m_hFile, lpszContent, dwLen * sizeof(TCHAR), &dwWriten, nullptr);
	}
	else if (m_dwCode == CP_UTF16B) {
		DWORD dwBytes = dwLen * sizeof(TCHAR);
		auto pDst = new char[dwBytes];
		if (pDst) {
			auto pSrc = (const char*)lpszContent;
			for (DWORD i = 0; i + 1 < dwBytes; i += 2) {
				pDst[i] = pSrc[i + 1];
				pDst[i + 1] = pSrc[i];
			}
			auto bRes = WriteFile(m_hFile, pDst, dwBytes, &dwWriten, nullptr);
			delete[] pDst;
			return bRes;
		}
	}
	else {
		int nLen = WideCharToMultiByte(m_dwCode, 0, lpszContent, dwLen, nullptr, 0, nullptr, nullptr);
		if (!nLen)
			return false;
		auto pBuff = new char[nLen];
		if (pBuff) {
			WideCharToMultiByte(m_dwCode, 0, lpszContent, dwLen, pBuff, nLen, nullptr, nullptr);
			auto bRes = WriteFile(m_hFile, pBuff, nLen - 1, &dwWriten, nullptr);
			delete[] pBuff;
			return bRes;
		}
	}
	
	return false;
}
