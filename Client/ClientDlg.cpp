
// ClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Name = _T("");
	m_SockClient.Create();
	m_SockClient.SetDialog(this);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_EDIT3, m_ServerIP);
	DDX_Control(pDX, IDC_EDIT4, m_ServerPort);
	DDX_Control(pDX, IDC_EDIT5, m_NickName);
	DDX_Control(pDX, IDC_EDIT6, m_Text);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnLogin)
	ON_BN_CLICKED(IDC_BUTTON2, &CClientDlg::OnSendText)
	ON_MESSAGE(WM_SYSTEMTRAY, &CClientDlg::OnSystemtray)
	ON_WM_CLOSE()
	//	ON_WM_CREATE()
	ON_WM_TIMER()
//	ON_WM_CANCELMODE()
ON_WM_RBUTTONDOWN()
ON_COMMAND(ID_32774, &CClientDlg::On32774)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon = m_hIcon;  //上面那句也可以
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, _T("聊天室服务器"));
	NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CClientDlg::OnSystemtray(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case  WM_RBUTTONDOWN://右键起来时弹出快捷菜单
	{
		CMenu menuexit;
		//menu.LoadMenuW(IDR_MENU1);//加载菜单资源
		menuexit.LoadMenu(IDR_MENU1);
		CMenu *pPopup = menuexit.GetSubMenu(0);
		CPoint mypoint;
		GetCursorPos(&mypoint);
		//ClientToScreen(&mypoint);//将客户区坐标转换为屏幕坐标
		SetForegroundWindow();
		PostMessage(WM_NULL, 0, 0);


		//显示右键菜单，由视类窗口拥有。
		pPopup->TrackPopupMenu(TPM_LEFTALIGN, mypoint.x, mypoint.y, this);
	}
	break;
	case  WM_LBUTTONDOWN://左键单击的处理     
	{
		ModifyStyleEx(0, WS_EX_TOPMOST);   //可以改变窗口的显示风格
		ShowWindow(SW_SHOWNORMAL);
	}
	break;
	}
	return 0;
}


void CClientDlg::ReceiveText()
{
	// TODO: 在此处添加实现代码.
	char buffer[2048];
	int len = m_SockClient.Receive(buffer, 2048);
	if (len != -1)
	{
		buffer[len] = '\0';
		m_List.InsertString(-1, buffer);
		::SendMessage(m_List.m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), NULL);
		SetTimer(1, 1000, nullptr);
	}
}


void CClientDlg::OnLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strIP, strPort;
	UINT port;
	m_ServerIP.GetWindowText(strIP);
	m_NickName.GetWindowText(m_Name);
	m_ServerPort.GetWindowText(strPort);
	if (strIP.IsEmpty() || strPort.IsEmpty() || m_Name.IsEmpty())
	{
		MessageBox(_T("请设置服务器信息"), _T("聊天室客户端"));
		return;
	}
	port = atoi(strPort);
	if (m_SockClient.Connect(strIP, port))
	{
		m_NickName.SetReadOnly(1);
		m_ServerPort.SetReadOnly(1);
		m_ServerIP.SetReadOnly(1);
		MessageBox(_T("连接服务器成功！"), _T("聊天室客户端"));
		CString str;
		str.Format("%s----->进入聊天室", m_Name);
		m_SockClient.Send(str.GetBuffer(str.GetLength()), str.GetLength());
	}
	else
		MessageBox(_T("连接服务器失败！"), _T("聊天室客户端"));
}



void CClientDlg::OnSendText()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText, strInfo;
	m_Text.GetWindowText(strText);
	if (!strText.IsEmpty() && !m_Name.IsEmpty())
	{
		strInfo.Format("%s: %s", m_Name, strText);
		m_SockClient.Send(strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());
		m_Text.SetWindowText("");
	}
}


void CClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->ShowWindow(HIDE_WINDOW);
	
	//CDialogEx::OnClose();
}


BOOL CClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此处添加实现代码.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && (pMsg->hwnd == m_Text.m_hWnd) && pMsg->wParam == 13)
	{
		OnSendText();
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && (pMsg->hwnd != GetDlgItem(IDC_BUTTON1)->m_hWnd) && (pMsg->hwnd != GetDlgItem(IDC_BUTTON2)->m_hWnd) && pMsg->wParam == 13)
		pMsg->wParam = 9;
	return CDialog::PreTranslateMessage(pMsg);
}


//int CClientDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  在此添加您专用的创建代码
//	//m_ServerPort.SetWindowText(_T("1080"));
//	return 0;
//}


void CClientDlg::OnTimer(UINT_PTR trayEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (trayEvent == 1)
	{
		if (::GetActiveWindow() == AfxGetMainWnd()->m_hWnd)
		{
			KillTimer(1);
		}
		else
		{
			FlashWindow(TRUE);
		}
	}
	CDialogEx::OnTimer(trayEvent);
}


//void CClientDlg::OnCancelMode()
//{
//	CDialogEx::OnCancelMode();
//
//	// TODO: 在此处添加消息处理程序代码
//}


BOOL CClientDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);
	return CDialogEx::DestroyWindow();
}


void CClientDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->ShowWindow(HIDE_WINDOW);
	//CDialogEx::OnCancel();
}


void CClientDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	EnableWindow(FALSE);


	CMenu menu;
	menu.LoadMenuA(IDR_MENU1);
	CMenu *pPopup = menu.GetSubMenu(0);
	ClientToScreen(&point);//将客户区坐标转换为屏幕坐标

	//显示右键菜单，由视类窗口拥有。
	pPopup->TrackPopupMenu(nFlags, point.x, point.y, this);
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CClientDlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	if (MessageBox("确定要退出应用程序吗？", "聊天室客户端", MB_OKCANCEL) != IDOK)
		return;
	CString strInfo;
	strInfo.Format("%s----->退出聊天室", m_Name);
	m_SockClient.Send(strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());
	DestroyWindow();
}
