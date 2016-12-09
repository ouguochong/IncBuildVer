////////////////////////////////////////////////////////////////////////////////
// �ļ�����VerFile.cpp
// ���ߣ�Song Baoming,2016
// ��;������ Version.h �ļ������ӱ���汾���� CVerFile �ķ��������ļ�
// License��https://github.com/songbaoming/IncBuildVer/blob/master/LICENSE
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VerFile.h"


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
bool CVerFile::IncBuildVer(LPCTSTR lpszFilePath)
{
	static LPCTSTR pszSign = TEXT("BUILD_VER_NUM");
	// ��ȡ�ļ�����
	if (!GetFileContent(lpszFilePath))
		return false;
	// ��ת������汾�ŵ�λ��
	auto pszBegin = _tcsstr(m_pContent, pszSign);
	if (!pszBegin)
		return false;
	pszBegin += _tcslen(pszSign);
	while (*pszBegin && !_istdigit(*pszBegin)) ++pszBegin;
	if (!*pszBegin)
		return false;
	auto pszEnd = pszBegin;
	while (*pszEnd && _istdigit(*pszEnd)) ++pszEnd;
	// ���ݾͰ汾�ż����°汾�ţ���ת��Ϊ�ַ���
	DWORD dwBuildVer = _ttoi(pszBegin);
	TCHAR szNewVer[20];
	_stprintf_s(szNewVer, TEXT("%u"), ++dwBuildVer);

	// ʹ�ð汾����ǰ���ַ����ض�λ�ļ�ָ��
	if (!SetFilePtrWithString(m_pContent, pszBegin - m_pContent))
		return false;
	// д���°汾��
	if (!WriteContent(szNewVer))
		return false;
	// �жϰ汾�ų����Ƿ�һ�£�ֻ�в�һ��ʱ����Ҫ����д��汾�ź������
	if (pszEnd - pszBegin != _tcslen(szNewVer))
		return WriteContent(pszEnd);

	return true;
}

// ��ȡָ���ļ����ݣ���ת��Ϊ Unicode ����
bool CVerFile::GetFileContent(LPCTSTR lpszFilePath)
{
	// ���ļ�
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
	GetFileCode(pData, large.QuadPart);
	// �����ļ����뽫�ļ�����ת���� unicode
	if (m_dwCode == CP_UTF16) {
		m_pContent = new TCHAR[large.QuadPart / sizeof(TCHAR) + 1];
		if (m_pContent) {
			memset(m_pContent, 0, large.QuadPart + sizeof(TCHAR));
			memcpy_s(m_pContent, large.QuadPart + sizeof(TCHAR), pData + m_dwOffset, large.QuadPart - m_dwOffset);
		}
	}
	else if (m_dwCode == CP_UTF16B) {
		m_pContent = new TCHAR[large.QuadPart / sizeof(TCHAR) + 1];
		if (m_pContent) {
			memset(m_pContent, 0, large.QuadPart + sizeof(TCHAR));
			char *pDst = (char*)m_pContent;
			char *pSrc = pData + m_dwOffset;
			for (LONGLONG i = 0; i + 1 < large.QuadPart; i += 2) {
				pDst[i] = pSrc[i + 1];
				pDst[i + 1] = pSrc[i];
			}
		}
	}
	else {
		DWORD dwLen = MultiByteToWideChar(m_dwCode, 0, pData + m_dwOffset, -1, nullptr, 0);
		if (dwLen) {
			m_pContent = new TCHAR[dwLen];
			if (m_pContent)
				MultiByteToWideChar(m_dwCode, 0, pData + m_dwOffset, -1, m_pContent, dwLen);
		}
	}

	UnmapViewOfFile(pData);
	return m_pContent != nullptr;
}

// ��ȡ���ļ����ݵı����ʽ
bool CVerFile::GetFileCode(LPCSTR pData, LONGLONG llLen)
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
bool CVerFile::WriteContent(LPCTSTR lpszContent)
{
	DWORD dwWriten;

	if (m_dwCode == CP_UTF16) {
		return WriteFile(m_hFile, lpszContent, _tcslen(lpszContent) * sizeof(TCHAR), &dwWriten, nullptr);
	}
	else if (m_dwCode == CP_UTF16B) {
		DWORD dwLen = _tcslen(lpszContent) * sizeof(TCHAR);
		auto pDst = new char[dwLen];
		if (pDst) {
			auto pSrc = (const char*)lpszContent;
			for (DWORD i = 0; i + 1 < dwLen; i += 2) {
				pDst[i] = pSrc[i + 1];
				pDst[i + 1] = pSrc[i];
			}
			auto bRes = WriteFile(m_hFile, pDst, dwLen, &dwWriten, nullptr);
			delete[] pDst;
			return bRes;
		}
	}
	else {
		int nLen = WideCharToMultiByte(m_dwCode, 0, lpszContent, -1, nullptr, 0, nullptr, nullptr);
		if (!nLen)
			return false;
		auto pBuff = new char[nLen];
		if (pBuff) {
			WideCharToMultiByte(m_dwCode, 0, lpszContent, -1, pBuff, nLen, nullptr, nullptr);
			auto bRes = WriteFile(m_hFile, pBuff, nLen - 1, &dwWriten, nullptr);
			delete[] pBuff;
			return bRes;
		}
	}
	
	return false;
}
