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
	// TODO: �ڴ˴����ʵ�ִ���.
	Dialog = aDialog;
}


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	CSocket::OnReceive(nErrorCode);
	Dialog->ReceiveText();
}
