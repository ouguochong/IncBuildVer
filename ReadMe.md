##��Ŀ˵��
----
����Ŀ���� Visual Studio ������Ŀʱ���Զ����ӱ���汾�š�
 
##ʹ�÷���
---
1.������Ŀ������Ŀ�ִ���ļ���IncBuildVer.exe ����ϵͳĿ¼��Windows��System32���£�

2.������ĿĿ¼�µ� Version.h ��ӵ������Ŀ�У�

3.�������Ŀ�е���Դ�ű��ļ���<��Ŀ��>.rc�������¸��ģ�

����ǰ

    /////////////////////////////////////////////////////////////////////////////
    //
    // Version
    //
    VS_VERSION_INFO VERSIONINFO
    FILEVERSION 1, 0, 0, 1
    PRODUCTVERSION 1, 0, 0, 1
    ...
            VALUE "FileVersion", ��1.0.0.1��
            ...
            VALUE "ProductVersion", ��1.0.0.1��

���ĺ�

    /////////////////////////////////////////////////////////////////////////////
    //
    // Version
    //
    VS_VERSION_INFO VERSIONINFO
    FILEVERSION MARJOR_VER_NUM, MINOR_VER_NUM, REVISION_VER_NUM, BUILD_VER_NUM
    PRODUCTVERSION MARJOR_VER_NUM, MINOR_VER_NUM, REVISION_VER_NUM, BUILD_VER_NUM
    ...
            VALUE "FileVersion", VERSION_STR
            ...
            VALUE "ProductVersion", VERSION_STR
            
4.����Ŀ���ԡ���Ŀ - ���� - �������� - �����¼� - Ԥ�������¼����ġ������С�������£�

    IncBuildVer $(ProjectDir)Version.h
    
##ע������
---
�޸� Version.h ʱ��Ҫȷ�����ļ��ı��뷽ʽΪ��ANSI��

##License
---
[MIT](https://github.com/songbaoming/IncBuildVer/blob/master/LICENSE)