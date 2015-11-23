// PFlistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PFlist.h"
#include "PFlistDlg.h"



#include "fstream"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif










std::string FileDigest(const std::string& file) {

	ifstream in(file.c_str(), std::ios::binary);
	if (!in) {
		return "";
	}

	MD5 md5;
	std::streamsize length;
	char buffer[1024];
	while (!in.eof()) {
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0) {
			md5.update(buffer, length);
		}
	}
	in.close();
	return md5.toString();
}


std::string FileDigest(const std::string& file,MD5 &md5) {

	md5.reset();

	ifstream in(file.c_str(), std::ios::binary);
	if (!in) {
		return "";
	}

	//MD5 md5;
	std::streamsize length;
	char buffer[1024];
	while (!in.eof()) {
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0) {
			md5.update(buffer, length);
		}
	}
	in.close();
	return md5.toString();
}






//#include <windows.h>
#include <stdio.h>
#include <string.h>
#define LEN MAX_PATH
// 深度优先递归遍历目录中所有的文件
BOOL  DirectoryList(LPCSTR Path)
{
	WIN32_FIND_DATAA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[LEN];
	// 构造路径
	char FullPathName[LEN];
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFileA(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		printf("搜索失败!");
		return 0;
	}
	while(::FindNextFileA(hError, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0 
			|| strcmp(FindData.cFileName, "..") == 0 )
		{
			continue;
		}

		// 构造完整路径
		sprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		FileCount++;
		// 输出本级的文件
		printf("\n%d  %s  ", FileCount, FullPathName);

		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			printf("<Dir>");
			DirectoryList(FullPathName);
		}



	}
	return 0;
}


// 深度优先递归遍历目录中所有的文件
BOOL  DirectoryList(LPCSTR Path,std::string &info)
{
	WIN32_FIND_DATAA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[LEN];
	// 构造路径
	char FullPathName[LEN];
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFileA(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		//printf("搜索失败!");
		info.append(FilePathName);
		info.append("搜索失败!");
		return 0;
	}
	while(::FindNextFileA(hError, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0 
			|| strcmp(FindData.cFileName, "..") == 0 )
		{
			continue;
		}

		// 构造完整路径
		sprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		FileCount++;
		// 输出本级的文件
		//printf("\n%d  %s  ", FileCount, FullPathName);
		info.append("\n");
		//info.append(FileCount);//NO 不能用数字！  看来得用stringstream啊！！
		//info.append("  ");
		info.append(FullPathName);


		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//printf("<Dir>");
			info.append("<Dir>");
			DirectoryList(FullPathName,info);
		}



	}
	return 0;
}





/***
filelist.dat结构：

{
	"baseUrl":"http://localhost/zendworkspace/newVersionCenter",
	"comment":"我是一个小小的fileList文件！",
	"directFileUrl":"http://localhost/zendworkspace/newVersionCenter/7-4/plist.txt",
	"directFileType":"plist",
	"fileList":[
					{
					"url":"/7-4/7-4.rar",
					"path":"/7-4.rar",
					"md5":"D0D15AE748529985776176253D2E11F4",
					"size":"1417316"
					}
			]
}

plist结构：

*7-4.rar	D0D15AE748529985776176253D2E11F4	1417316







*/

// 深度优先递归遍历目录中所有的文件
BOOL  DirectoryList(LPCSTR Path,MD5 &md5,std::ofstream &plist,std::ofstream &filelist)
{
	WIN32_FIND_DATAA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[LEN];
	// 构造路径
	char FullPathName[LEN];
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFileA(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		//printf("搜索失败!");
		//info.append(FilePathName);
		//info.append("搜索失败!");
		AfxMessageBox(_T("搜索失败!"));
		return 0;
	}
	while(::FindNextFileA(hError, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0 
			|| strcmp(FindData.cFileName, "..") == 0 )
		{
			continue;
		}

		// 构造完整路径
		sprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		FileCount++;
		// 输出本级的文件
		//printf("\n%d  %s  ", FileCount, FullPathName);
		//info.append("\n");
		//info.append(FileCount);//NO 不能用数字！  看来得用stringstream啊！！
		//info.append("  ");
		//info.append(FullPathName);


		//设置plist的每一项！
		std::string md5str=FileDigest(std::string(FullPathName),md5);
		plist<<"*";
		plist<<FindData.cFileName;
		//of<<'\9';不行 \0x09不行 \t和\x09是可以的！
		plist<<'\x09';//09 /t
		plist<<md5str;
		plist<<'\x09';//09 /t
		plist<<((((unsigned __int64)FindData.nFileSizeHigh)<<32)|FindData.nFileSizeLow);
		//of<<'\x0d';//0d  我明明输入一个od 怎么变成了两个？！！
		plist<<'\x0a';//仅仅输入 '\x0a'  就有了 OD和OA!!!!


		//设置filelist中的每一项
		filelist<<"{";
		filelist<<"\"url\""<<":"<<"\"/"<<FindData.cFileName<<"\""<<",";
		filelist<<"\"path\""<<":"<<"\"/"<<FindData.cFileName<<"\""<<",";
		filelist<<"\"md5\""<<":"<<"\""<<md5str<<"\""<<",";
		filelist<<"\"size\""<<":"<<"\""<<((((unsigned __int64)FindData.nFileSizeHigh)<<32)|FindData.nFileSizeLow)<<"\"";
		filelist<<"}";
		filelist<<",";
	
		



		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//printf("<Dir>");
			//info.append("<Dir>");
			DirectoryList(FullPathName,md5,plist,filelist);
		}



	}
	return 0;
}











// father中有\\ 用它来拼url还是不爽！！  还是都用/算了！！！

//Dir为当前的文件夹全路径！ father为相对父文件夹名！
// 深度优先递归遍历目录中所有的文件
BOOL  DirectoryList(LPCSTR Dir,LPCSTR father,MD5 &md5,std::ofstream &plist,std::ofstream &filelist)
{
	WIN32_FIND_DATAA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[LEN];
	// 构造路径
	char FullPathName[LEN];
	strcpy(FilePathName, Dir);
	strcat(FilePathName, "/*.*");
	hError = FindFirstFileA(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		//printf("搜索失败!");
		//info.append(FilePathName);
		//info.append("搜索失败!");
		AfxMessageBox(_T("搜索失败!"));
		return 0;
	}
	while(::FindNextFileA(hError, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0 
			|| strcmp(FindData.cFileName, "..") == 0 )
		{
			continue;
		}

		if(father==""){
		

			if (strcmpi(FindData.cFileName, "plist.txt") == 0 
				|| strcmpi(FindData.cFileName, "fileList.dat") == 0 )
			{
				continue;
			}
		
		
		}




		// 构造完整路径
		sprintf(FullPathName, "%s/%s", Dir,FindData.cFileName);

		if(!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
				
				//FileCount++;
				// 输出本级的文件
				//printf("\n%d  %s  ", FileCount, FullPathName);
				//info.append("\n");
				//info.append(FileCount);//NO 不能用数字！  看来得用stringstream啊！！
				//info.append("  ");
				//info.append(FullPathName);






				//设置plist的每一项！
				std::string md5str=FileDigest(std::string(FullPathName),md5);
				plist<<"*";
				if(father!="")plist<<father<<"/"<<FindData.cFileName;
				else plist<<FindData.cFileName;

				//of<<'\9';不行 \0x09不行 \t和\x09是可以的！
				plist<<'\x09';//09 /t
				plist<<md5str;
				plist<<'\x09';//09 /t
				plist<<((((unsigned __int64)FindData.nFileSizeHigh)<<32)|FindData.nFileSizeLow);
				//of<<'\x0d';//0d  我明明输入一个od 怎么变成了两个？！！
				plist<<'\x0a';//仅仅输入 '\x0a'  就有了 OD和OA!!!!







				//设置filelist中的每一项
				filelist<<"{";

				if(father!=""){
					filelist<<"\"url\""<<":"<<"\"/"<<father<<"/"<<FindData.cFileName<<"\""<<",";
					filelist<<"\"path\""<<":"<<"\"/"<<father<<"/"<<FindData.cFileName<<"\""<<",";
				
				}
				else {
					filelist<<"\"url\""<<":"<<"\"/"<<FindData.cFileName<<"\""<<",";
					filelist<<"\"path\""<<":"<<"\"/"<<FindData.cFileName<<"\""<<",";
				
				}

				filelist<<"\"md5\""<<":"<<"\""<<md5str<<"\""<<",";
				filelist<<"\"size\""<<":"<<"\""<<((((unsigned __int64)FindData.nFileSizeHigh)<<32)|FindData.nFileSizeLow)<<"\"";
				filelist<<"}";
				filelist<<",";




		}
		else//FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
		{
			//printf("<Dir>");
			//info.append("<Dir>");
			char newfather[LEN];
			if(father!="")sprintf(newfather, "%s/%s", father,FindData.cFileName);
			else	sprintf(newfather, "%s",FindData.cFileName);

			DirectoryList(FullPathName,newfather,md5,plist,filelist);
		}



	}
	return 0;
}














// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPFlistDlg 对话框




CPFlistDlg::CPFlistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPFlistDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	ZeroMemory(m_FullPath,sizeof(m_FullPath));
 
}

void CPFlistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPFlistDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CPFlistDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT2, &CPFlistDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDCANCEL, &CPFlistDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CPFlistDlg::OnNMCustomdrawProgress1)
	ON_BN_CLICKED(IDC_BUTTON1, &CPFlistDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPFlistDlg 消息处理程序

BOOL CPFlistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPFlistDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPFlistDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPFlistDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//#include "string"
void CPFlistDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();





	/*
	//获得EDIT

	CEdit* pBoxOne;

	pBoxOne = (CEdit*) GetDlgItem(IDC_EDIT2);

	//付值
	//pBoxOne->SetWindowText( _T("FOO") );

	//取值
	CString str;
	pBoxOne->GetWindowText(str);
	//GetDlgItem(IDC_EDIT2)->SetWindowText( _T("FOO") );
	//也可以
	//取值
	//CString str2;
	//GetDlgItem(IDC_EDIT1)->GetWindowText(str2);


	
	int n = str.GetLength();     // n = 14, len = 18
	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
	//int len2= WideCharToMultiByte(CP_ACP,0,str,-1,NULL,-1,NULL,NULL);//NO!!
	//int len3= WideCharToMultiByte(CP_ACP,0,str,0,NULL,-1,NULL,NULL);//NO!!

	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * pFileName = new char[len+1];   //以字节为单位
	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pFileName,len,NULL,NULL);
	//WideCharToMultiByte(CP_ACP,0,str,str.GetLength() + 1 ,pFileName,len + 1 ,NULL,NULL);//多此一举 有没有那么多 要个+1干啥子！
	pFileName[len+1] = '\0';   //多字节字符以'/0'结束   NO 你的字符写错了 老大！



	std::string info;
	DirectoryList("E:\\备份\\",info);//OK!!!  如果末尾加\\呢？ 也 OK!!
	//DirectoryList("E:\\备份1\\",info);//不存在时  findFirstFile时失败
	//DirectoryList("E:\\备份1.txt",info);//不存在时  findFirstFile时失败
	*/


	//if(m_FullPath==_T("")||m_FullPath==_T(" ")){//这样是不行的！ 即使m_FullPath初始化为0了 这个也不相等！
	if(lstrlen(m_FullPath)==0){//如果使用按钮选择文件路径！
		
		
		CString path;
		GetDlgItem(IDC_EDIT3)->GetWindowText(path);

		if(path==""){//文本框也为空的时候！！
			AfxMessageBox(_T("请输入有效的文件路径"));
			return ;
		}else{
		
		
			lstrcpy(m_FullPath,path);
		
		
		}

	}


	//int n = str.GetLength();     // n = 14, len = 18
	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP,0,m_FullPath,-1,NULL,0,NULL,NULL);
	//int len2= WideCharToMultiByte(CP_ACP,0,str,-1,NULL,-1,NULL,NULL);//NO!!
	//int len3= WideCharToMultiByte(CP_ACP,0,str,0,NULL,-1,NULL,NULL);//NO!!
	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * pDirName = new char[len+1];   //以字节为单位
	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,m_FullPath,-1,pDirName,len,NULL,NULL);
	//WideCharToMultiByte(CP_ACP,0,str,str.GetLength() + 1 ,pFileName,len + 1 ,NULL,NULL);//多此一举 有没有那么多 要个+1干啥子！
	//pFileName[len+1] = '\0';//貌似可以不要！！   //多字节字符以'/0'结束   NO 你的字符写错了 老大！ 





	CString baseUrl,directUrl,comment;
	GetDlgItem(IDC_EDIT2)->GetWindowText(baseUrl);
	GetDlgItem(IDC_EDIT4)->GetWindowText(directUrl);
	GetDlgItem(IDC_EDIT1)->GetWindowText(comment);


	int len2 = WideCharToMultiByte(CP_ACP,0,baseUrl,-1,NULL,0,NULL,NULL);
	char * pbaseUrl = new char[len2+1];   //以字节为单位
	WideCharToMultiByte(CP_ACP,0,baseUrl,-1,pbaseUrl,len2,NULL,NULL);


	int len3 = WideCharToMultiByte(CP_ACP,0,directUrl,-1,NULL,0,NULL,NULL);
	char * pdirectUrl = new char[len3+1];   //以字节为单位
	WideCharToMultiByte(CP_ACP,0,directUrl,-1,pdirectUrl,len3,NULL,NULL);


	int len4 = WideCharToMultiByte(CP_ACP,0,comment,-1,NULL,0,NULL,NULL);
	char * pcomment = new char[len4+1];   //以字节为单位
	WideCharToMultiByte(CP_ACP,0,comment,-1,pcomment,len4,NULL,NULL);






	//设置ofstream可以创建中文文件和识别中文路径！！ 不然会是乱码的！！
	setlocale(LC_ALL, "");
	/*
	std::ofstream of("E:\\包敏.txt");//到对象析构之前 即使close 也不会解除文件的锁定
	//of.open("E:\\备份\\plist.txt");
	of<<"123";
	of<<"爸爸丢人";
	of<<'\t';//用\9为什么不行？！！！ 实际是9 如果用char 指针一个一个取  或者editplus下看 就是这样的
	of<<"11111";
	of<<'\9';//NO 当成字符‘9’了
	of<<"11111";
	of<<'\x09';//\0x09不可以！
	of<<"123";
	of.close();
	*/



	//取得MD5值,即sign 
	//std::string s("hello");
	//MD5 md5 ( s + "Sdg_net2011_SNDA" );  //Sdg_net2011_SNDA前后台定好的密钥
	//std::string sign = md5.toString();
	//md5.



	//MD5 m_md5;

	//std::string basepath=pDirName;
	std::string plist_s=pDirName;
	std::string filelis_st=pDirName;
	if(plist_s[plist_s.length()-1]=='/'||plist_s[plist_s.length()-1]=='\\'){
	
		plist_s.append("plist.txt");
		filelis_st.append("fileList.dat");
	}
	else{
	
		plist_s.append("/plist.txt");
		filelis_st.append("/fileList.dat");
	}


	std::ofstream plist(plist_s.c_str());

	std::ofstream filelist(filelis_st.c_str());

	filelist<<"{";
	filelist<<"\"baseUrl\""<<":"<<"\""<<pbaseUrl<<"\""<<",";
	filelist<<"\"comment\""<<":"<<"\""<<pcomment<<"\""<<",";
	filelist<<"\"directFileUrl\""<<":"<<"\""<<pdirectUrl<<"\""<<",";
	filelist<<"\"directFileType\""<<":"<<"\"plist\""<<",";
	filelist<<"\"fileList\""<<":"<<"[";

	//DirectoryList("E:\\1\\",md5,plist,filelist);
	DirectoryList(pDirName,"",m_md5,plist,filelist);

	filelist.seekp(-1,std::ofstream::end);//ofstream的时候 不能使用seekp啊！！
	filelist<<"]";//修改最后一个filelist的{}, 为{}]
	filelist<<"}";


	plist.close();
	filelist.close();
	



	
	AfxMessageBox(_T("plist.txt和fileList.dat生成成功！\n 存放在所选文件夹下"));

	delete pDirName;
	delete pbaseUrl;
	delete pdirectUrl;
	delete pcomment;

}

void CPFlistDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CPFlistDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CPFlistDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CPFlistDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	//CFileDialog c(false);//true或者false都是  打开文件啊 不是选择目录啊！！
	//c.DoModal();

	BROWSEINFO   bi;                         //BROWSEINFO结构体 
	TCHAR   Buffer[512]= _T(""); 
	//TCHAR   FullPath[512]= _T(" "); 
	bi.hwndOwner   =   m_hWnd;       //m_hWnd你的程序主窗口 
	bi.pidlRoot   =   NULL; 
	bi.pszDisplayName   =   Buffer;   //返回选择的目录名的缓冲区 
	bi.lpszTitle   =   _T("Selection ");   //弹出的窗口的文字提示 
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS   ;   //只返回目录。其他标志看MSDN 
	bi.lpfn   =   NULL;                                                   //回调函数，有时很有用 
	bi.lParam   =   0; 
	bi.iImage   =   0; 
	ITEMIDLIST*   pidl   =   ::SHBrowseForFolder   (&bi);   //显示弹出窗口，ITEMIDLIST很重要 
	if(::SHGetPathFromIDList   (pidl,m_FullPath))           //在ITEMIDLIST中得到目录名的整个路径 
	{ 
		//成功 
		//AfxMessageBox(m_FullPath);

		GetDlgItem(IDC_EDIT3)->SetWindowText(m_FullPath);


	} 
	else 
	{ 
		//失败 
		//AfxMessageBox(FullPath);

	}
}
