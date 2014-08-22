// PhaseDiagWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "PhaseDiagWnd.h"
#include "afxdialogex.h"


// CPhaseDiagWnd dialog

IMPLEMENT_DYNAMIC(CPhaseDiagWnd, CDialogEx)

CPhaseDiagWnd::CPhaseDiagWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPhaseDiagWnd::IDD, pParent)
{

}

CPhaseDiagWnd::~CPhaseDiagWnd()
{
}

void CPhaseDiagWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, m_stcGraph);
}


BEGIN_MESSAGE_MAP(CPhaseDiagWnd, CDialogEx)
END_MESSAGE_MAP()


// CPhaseDiagWnd message handlers
