// PFlistDlg.h : ͷ�ļ�
//

#pragma once
#include "md52.h"


// CPFlistDlg �Ի���
class CPFlistDlg : public CDialog
{
// ����
public:
	CPFlistDlg(CWnd* pParent = NULL);	// ��׼���캯��





	TCHAR   m_FullPath[512]; 
	MD5 m_md5;




// �Ի�������
	enum { IDD = IDD_PFLIST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
