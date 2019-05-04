#include "stdafx.h"
#include "CClientSocket.h"


CClientSocket::CClientSocket()
{
}


CClientSocket::~CClientSocket()
{
}


void CClientSocket::SetDialog(CClientDlg* aDialog)
{
	// TODO: 在此处添加实现代码.
	Dialog = aDialog;
}


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此处添加实现代码.
	CSocket::OnReceive(nErrorCode);
	Dialog->ReceiveText();
}
