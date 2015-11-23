// PFlistDlg.h : 头文件
//

#pragma once
#include "md52.h"


// CPFlistDlg 对话框
class CPFlistDlg : public CDialog
{
// 构造
public:
	CPFlistDlg(CWnd* pParent = NULL);	// 标准构造函数





	TCHAR   m_FullPath[512]; 
	MD5 m_md5;




// 对话框数据
	enum { IDD = IDD_PFLIST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
};
