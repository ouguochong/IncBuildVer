////////////////////////////////////////////////////////////////////////////////
// �ļ�����VerFile.h
// ���ߣ�Song Baoming,2016
// ��;������ Version.h �ļ������ӱ���汾���� CVerFile ��ͷ�ļ�
// License��https://github.com/songbaoming/IncBuildVer/blob/master/LICENSE
////////////////////////////////////////////////////////////////////////////////

#pragma once

#define	CP_UTF16				1200
#define	CP_UTF16B				1201



class CVerFile
{
public:
	CVerFile();
	~CVerFile();

	// ���ܣ���ָ���ļ��б���汾�ź��ֵ��һ
	// ������LPCTSTR lpszFilePath��Ŀ���ļ�������·��
	// ����ֵ���ɹ�ʱ���� true�����򷵻� false
	bool IncBuildVer(LPCTSTR lpszFilePath);

protected:
	DWORD m_dwOffset; // ��Ч�ļ���������ļ���ͷ��ƫ���ֽ���
	DWORD m_dwCode; // �ļ����ַ������ʽ
	HANDLE m_hFile; // Ŀ���ļ����
	LPTSTR m_pContent; // ת��Ϊ Unicode �����ʽ����ļ����ݵ�ָ��

	// ��ȡָ���ļ����ݣ���ת��Ϊ Unicode ����
	bool GetFileContent(LPCTSTR lpszFilePath);

	// ��ȡ���ļ����ݵı����ʽ
	bool GetFileCode(LPCSTR pData, LONGLONG llLen);
	// �жϸ����������Ƿ�Ϊutf-8����
	bool IsCodeUtf8(LPCSTR pString, LONGLONG llLen);
	// �жϸ��������Ƿ�Ϊutf-16��ʽ�ı��룬������򷵻��ַ����룬���򷵻���
	int IsCodeUtf16(LPCSTR pString, LONGLONG llLen);

	// �Ӹ����İ汾��֮ǰ���ַ�����������ԭ�ļ����뷽ʽ���ļ����ݵ�ƫ������
	// ���㲢�����ļ�ָ���λ�ã���������汾��֮ǰ���ᱻ���ĵ����ݣ�
	// ���ں���д�룬������ I/O ���Ч��
	bool SetFilePtrWithString(LPCTSTR lpszProBuildVer, DWORD dwLen);
	// �����������ݣ�ת��Ϊԭ�ļ������ʽ��д���ļ�
	bool WriteContent(LPCTSTR lpszContent);
};

