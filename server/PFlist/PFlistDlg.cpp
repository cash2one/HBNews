// PFlistDlg.cpp : ʵ���ļ�
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
// ������ȵݹ����Ŀ¼�����е��ļ�
BOOL  DirectoryList(LPCSTR Path)
{
	WIN32_FIND_DATAA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[LEN];
	// ����·��
	char FullPathName[LEN];
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFileA(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		printf("����ʧ��!");
		return 0;
	}
	while(::FindNextFileA(hError, &FindData))
	{
		// ����.��..
		if (strcmp(FindData.cFileName, ".") == 0 
			|| strcmp(FindData.cFileName, "..") == 0 )
		{
			continue;
		}

		// ��������·��
		sprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		FileCount++;
		// ����������ļ�
		printf("\n%d  %s  ", FileCount, FullPathName);

		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			printf("<Dir>");
			DirectoryList(FullPathName);
		}



	}
	return 0;
}


// ������ȵݹ����Ŀ¼�����е��ļ�
BOOL  DirectoryList(LPCSTR Path,std::string &info)
{
	WIN32_FIND_DATAA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[LEN];
	// ����·��
	char FullPathName[LEN];
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFileA(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		//printf("����ʧ��!");
		info.append(FilePathName);
		info.append("����ʧ��!");
		return 0;
	}
	while(::FindNextFileA(hError, &FindData))
	{
		// ����.��..
		if (strcmp(FindData.cFileName, ".") == 0 
			|| strcmp(FindData.cFileName, "..") == 0 )
		{
			continue;
		}

		// ��������·��
		sprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		FileCount++;
		// ����������ļ�
		//printf("\n%d  %s  ", FileCount, FullPathName);
		info.append("\n");
		//info.append(FileCount);//NO ���������֣�  ��������stringstream������
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
filelist.dat�ṹ��

{
	"baseUrl":"http://localhost/zendworkspace/newVersionCenter",
	"comment":"����һ��СС��fileList�ļ���",
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

plist�ṹ��

*7-4.rar	D0D15AE748529985776176253D2E11F4	1417316







*/

// ������ȵݹ����Ŀ¼�����е��ļ�
BOOL  DirectoryList(LPCSTR Path,MD5 &md5,std::ofstream &plist,std::ofstream &filelist)
{
	WIN32_FIND_DATAA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[LEN];
	// ����·��
	char FullPathName[LEN];
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFileA(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		//printf("����ʧ��!");
		//info.append(FilePathName);
		//info.append("����ʧ��!");
		AfxMessageBox(_T("����ʧ��!"));
		return 0;
	}
	while(::FindNextFileA(hError, &FindData))
	{
		// ����.��..
		if (strcmp(FindData.cFileName, ".") == 0 
			|| strcmp(FindData.cFileName, "..") == 0 )
		{
			continue;
		}

		// ��������·��
		sprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		FileCount++;
		// ����������ļ�
		//printf("\n%d  %s  ", FileCount, FullPathName);
		//info.append("\n");
		//info.append(FileCount);//NO ���������֣�  ��������stringstream������
		//info.append("  ");
		//info.append(FullPathName);


		//����plist��ÿһ�
		std::string md5str=FileDigest(std::string(FullPathName),md5);
		plist<<"*";
		plist<<FindData.cFileName;
		//of<<'\9';���� \0x09���� \t��\x09�ǿ��Եģ�
		plist<<'\x09';//09 /t
		plist<<md5str;
		plist<<'\x09';//09 /t
		plist<<((((unsigned __int64)FindData.nFileSizeHigh)<<32)|FindData.nFileSizeLow);
		//of<<'\x0d';//0d  ����������һ��od ��ô���������������
		plist<<'\x0a';//�������� '\x0a'  ������ OD��OA!!!!


		//����filelist�е�ÿһ��
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











// father����\\ ������ƴurl���ǲ�ˬ����  ���Ƕ���/���ˣ�����

//DirΪ��ǰ���ļ���ȫ·���� fatherΪ��Ը��ļ�������
// ������ȵݹ����Ŀ¼�����е��ļ�
BOOL  DirectoryList(LPCSTR Dir,LPCSTR father,MD5 &md5,std::ofstream &plist,std::ofstream &filelist)
{
	WIN32_FIND_DATAA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[LEN];
	// ����·��
	char FullPathName[LEN];
	strcpy(FilePathName, Dir);
	strcat(FilePathName, "/*.*");
	hError = FindFirstFileA(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		//printf("����ʧ��!");
		//info.append(FilePathName);
		//info.append("����ʧ��!");
		AfxMessageBox(_T("����ʧ��!"));
		return 0;
	}
	while(::FindNextFileA(hError, &FindData))
	{
		// ����.��..
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




		// ��������·��
		sprintf(FullPathName, "%s/%s", Dir,FindData.cFileName);

		if(!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
				
				//FileCount++;
				// ����������ļ�
				//printf("\n%d  %s  ", FileCount, FullPathName);
				//info.append("\n");
				//info.append(FileCount);//NO ���������֣�  ��������stringstream������
				//info.append("  ");
				//info.append(FullPathName);






				//����plist��ÿһ�
				std::string md5str=FileDigest(std::string(FullPathName),md5);
				plist<<"*";
				if(father!="")plist<<father<<"/"<<FindData.cFileName;
				else plist<<FindData.cFileName;

				//of<<'\9';���� \0x09���� \t��\x09�ǿ��Եģ�
				plist<<'\x09';//09 /t
				plist<<md5str;
				plist<<'\x09';//09 /t
				plist<<((((unsigned __int64)FindData.nFileSizeHigh)<<32)|FindData.nFileSizeLow);
				//of<<'\x0d';//0d  ����������һ��od ��ô���������������
				plist<<'\x0a';//�������� '\x0a'  ������ OD��OA!!!!







				//����filelist�е�ÿһ��
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














// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CPFlistDlg �Ի���




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


// CPFlistDlg ��Ϣ�������

BOOL CPFlistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPFlistDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	//���EDIT

	CEdit* pBoxOne;

	pBoxOne = (CEdit*) GetDlgItem(IDC_EDIT2);

	//��ֵ
	//pBoxOne->SetWindowText( _T("FOO") );

	//ȡֵ
	CString str;
	pBoxOne->GetWindowText(str);
	//GetDlgItem(IDC_EDIT2)->SetWindowText( _T("FOO") );
	//Ҳ����
	//ȡֵ
	//CString str2;
	//GetDlgItem(IDC_EDIT1)->GetWindowText(str2);


	
	int n = str.GetLength();     // n = 14, len = 18
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
	//int len2= WideCharToMultiByte(CP_ACP,0,str,-1,NULL,-1,NULL,NULL);//NO!!
	//int len3= WideCharToMultiByte(CP_ACP,0,str,0,NULL,-1,NULL,NULL);//NO!!

	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	char * pFileName = new char[len+1];   //���ֽ�Ϊ��λ
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pFileName,len,NULL,NULL);
	//WideCharToMultiByte(CP_ACP,0,str,str.GetLength() + 1 ,pFileName,len + 1 ,NULL,NULL);//���һ�� ��û����ô�� Ҫ��+1��ɶ�ӣ�
	pFileName[len+1] = '\0';   //���ֽ��ַ���'/0'����   NO ����ַ�д���� �ϴ�



	std::string info;
	DirectoryList("E:\\����\\",info);//OK!!!  ���ĩβ��\\�أ� Ҳ OK!!
	//DirectoryList("E:\\����1\\",info);//������ʱ  findFirstFileʱʧ��
	//DirectoryList("E:\\����1.txt",info);//������ʱ  findFirstFileʱʧ��
	*/


	//if(m_FullPath==_T("")||m_FullPath==_T(" ")){//�����ǲ��еģ� ��ʹm_FullPath��ʼ��Ϊ0�� ���Ҳ����ȣ�
	if(lstrlen(m_FullPath)==0){//���ʹ�ð�ťѡ���ļ�·����
		
		
		CString path;
		GetDlgItem(IDC_EDIT3)->GetWindowText(path);

		if(path==""){//�ı���ҲΪ�յ�ʱ�򣡣�
			AfxMessageBox(_T("��������Ч���ļ�·��"));
			return ;
		}else{
		
		
			lstrcpy(m_FullPath,path);
		
		
		}

	}


	//int n = str.GetLength();     // n = 14, len = 18
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP,0,m_FullPath,-1,NULL,0,NULL,NULL);
	//int len2= WideCharToMultiByte(CP_ACP,0,str,-1,NULL,-1,NULL,NULL);//NO!!
	//int len3= WideCharToMultiByte(CP_ACP,0,str,0,NULL,-1,NULL,NULL);//NO!!
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	char * pDirName = new char[len+1];   //���ֽ�Ϊ��λ
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP,0,m_FullPath,-1,pDirName,len,NULL,NULL);
	//WideCharToMultiByte(CP_ACP,0,str,str.GetLength() + 1 ,pFileName,len + 1 ,NULL,NULL);//���һ�� ��û����ô�� Ҫ��+1��ɶ�ӣ�
	//pFileName[len+1] = '\0';//ò�ƿ��Բ�Ҫ����   //���ֽ��ַ���'/0'����   NO ����ַ�д���� �ϴ� 





	CString baseUrl,directUrl,comment;
	GetDlgItem(IDC_EDIT2)->GetWindowText(baseUrl);
	GetDlgItem(IDC_EDIT4)->GetWindowText(directUrl);
	GetDlgItem(IDC_EDIT1)->GetWindowText(comment);


	int len2 = WideCharToMultiByte(CP_ACP,0,baseUrl,-1,NULL,0,NULL,NULL);
	char * pbaseUrl = new char[len2+1];   //���ֽ�Ϊ��λ
	WideCharToMultiByte(CP_ACP,0,baseUrl,-1,pbaseUrl,len2,NULL,NULL);


	int len3 = WideCharToMultiByte(CP_ACP,0,directUrl,-1,NULL,0,NULL,NULL);
	char * pdirectUrl = new char[len3+1];   //���ֽ�Ϊ��λ
	WideCharToMultiByte(CP_ACP,0,directUrl,-1,pdirectUrl,len3,NULL,NULL);


	int len4 = WideCharToMultiByte(CP_ACP,0,comment,-1,NULL,0,NULL,NULL);
	char * pcomment = new char[len4+1];   //���ֽ�Ϊ��λ
	WideCharToMultiByte(CP_ACP,0,comment,-1,pcomment,len4,NULL,NULL);






	//����ofstream���Դ��������ļ���ʶ������·������ ��Ȼ��������ģ���
	setlocale(LC_ALL, "");
	/*
	std::ofstream of("E:\\����.txt");//����������֮ǰ ��ʹclose Ҳ�������ļ�������
	//of.open("E:\\����\\plist.txt");
	of<<"123";
	of<<"�ְֶ���";
	of<<'\t';//��\9Ϊʲô���У������� ʵ����9 �����char ָ��һ��һ��ȡ  ����editplus�¿� ����������
	of<<"11111";
	of<<'\9';//NO �����ַ���9����
	of<<"11111";
	of<<'\x09';//\0x09�����ԣ�
	of<<"123";
	of.close();
	*/



	//ȡ��MD5ֵ,��sign 
	//std::string s("hello");
	//MD5 md5 ( s + "Sdg_net2011_SNDA" );  //Sdg_net2011_SNDAǰ��̨���õ���Կ
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

	filelist.seekp(-1,std::ofstream::end);//ofstream��ʱ�� ����ʹ��seekp������
	filelist<<"]";//�޸����һ��filelist��{}, Ϊ{}]
	filelist<<"}";


	plist.close();
	filelist.close();
	



	
	AfxMessageBox(_T("plist.txt��fileList.dat���ɳɹ���\n �������ѡ�ļ�����"));

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

	//CFileDialog c(false);//true����false����  ���ļ��� ����ѡ��Ŀ¼������
	//c.DoModal();

	BROWSEINFO   bi;                         //BROWSEINFO�ṹ�� 
	TCHAR   Buffer[512]= _T(""); 
	//TCHAR   FullPath[512]= _T(" "); 
	bi.hwndOwner   =   m_hWnd;       //m_hWnd��ĳ��������� 
	bi.pidlRoot   =   NULL; 
	bi.pszDisplayName   =   Buffer;   //����ѡ���Ŀ¼���Ļ����� 
	bi.lpszTitle   =   _T("Selection ");   //�����Ĵ��ڵ�������ʾ 
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS   ;   //ֻ����Ŀ¼��������־��MSDN 
	bi.lpfn   =   NULL;                                                   //�ص���������ʱ������ 
	bi.lParam   =   0; 
	bi.iImage   =   0; 
	ITEMIDLIST*   pidl   =   ::SHBrowseForFolder   (&bi);   //��ʾ�������ڣ�ITEMIDLIST����Ҫ 
	if(::SHGetPathFromIDList   (pidl,m_FullPath))           //��ITEMIDLIST�еõ�Ŀ¼��������·�� 
	{ 
		//�ɹ� 
		//AfxMessageBox(m_FullPath);

		GetDlgItem(IDC_EDIT3)->SetWindowText(m_FullPath);


	} 
	else 
	{ 
		//ʧ�� 
		//AfxMessageBox(FullPath);

	}
}
