
// ClientDlg.h: 头文件
//

#pragma once
#include "CClientSocket.h"

// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSystemtray(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	void ReceiveText();
	CListBox m_List;
	NOTIFYICONDATA NotifyIcon;
	afx_msg void OnLogin();
	bool ctime;
	CEdit m_ServerIP;
	CEdit m_ServerPort;
	CEdit m_NickName;
	afx_msg void OnSendText();
	CEdit m_Text;
	CString m_Name;
	CClientSocket m_SockClient;
	afx_msg void OnClose();
	BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR trayEvent);
//	afx_msg void OnCancelMode();
	virtual BOOL DestroyWindow();
	virtual void OnCancel();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void On32774();
};
