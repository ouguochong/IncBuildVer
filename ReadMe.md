##项目说明
该项目用于 Visual Studio 编译项目时，自动增加编译版本号。
 
##使用方法
1.将该项目编译出的可执行文件：IncBuildVer.exe 放入系统目录（Windows或System32）下；

2.将该项目目录下的 Version.h 添加到你的项目中；

3.将你的项目中的资源脚本文件（<项目名>.rc）做如下更改：

更改前

    /////////////////////////////////////////////////////////////////////////////
    //
    // Version
    //
    VS_VERSION_INFO VERSIONINFO
    FILEVERSION 1, 0, 0, 1
    PRODUCTVERSION 1, 0, 0, 1
    ...
            VALUE "FileVersion", “1.0.0.1”
            ...
            VALUE "ProductVersion", “1.0.0.1”

更改后

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
            
4.在项目属性“项目 - 属性 - 配置属性 - 生成事件 - 预先生成事件”的“命令行”添加如下：

    IncBuildVer $(ProjectDir)Version.h
    
##注意事项
修改 Version.h 时，要确保该文件的编码方式为“ANSI”

##License
[MIT](https://github.com/songbaoming/IncBuildVer/blob/master/LICENSE)
