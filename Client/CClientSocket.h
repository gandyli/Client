#pragma once
#include "stdafx.h"

class CClientDlg;
class CClientSocket :
	public CSocket
{
public:
	CClientSocket();
	~CClientSocket();
	CClientDlg* Dialog;
	void SetDialog(CClientDlg* aDialog);
	void OnReceive(int nErrorCode);
};

