// EFieldDyn.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "EFieldDyn.h"
#include "afxdialogex.h"

#include "SiDDlg.h"


// CEFieldDyn dialog

IMPLEMENT_DYNAMIC(CEFieldDyn, CDialogEx)

CEFieldDyn::CEFieldDyn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEFieldDyn::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEFieldDyn::~CEFieldDyn()
{
	DestroyWindow();
}

void CEFieldDyn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFIELDGRAPH, m_stcGraph);
}


BEGIN_MESSAGE_MAP(CEFieldDyn, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CLOSE()
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// CEFieldDyn message handlers


void CEFieldDyn::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);

	//отбивка от краев для размещения элементов диалога - по 7 пикселей
	int nSpace = 11;
	//высота кнопки тулбара - 17 пикселей
	int nToolbarHeight = 28;

	CRect newRect;
	newRect.top = 2 * nSpace + nToolbarHeight;
	newRect.left = nSpace;
	newRect.bottom = rect.bottom - nSpace;
	newRect.right = rect.right - nSpace;

	m_stcGraph.MoveWindow(&newRect);
	m_stcGraph.Invalidate();
}


void CEFieldDyn::OnOK()
{
	//CDialogEx::OnOK();
}


void CEFieldDyn::SetEnableSaveButton(BOOL set)
{
	//GetDlgItem(IDC_BTN_SAVEPOTSCREEN)->EnableWindow(set);
	//GetDlgItem(IDC_BTN_SAVETXTDATA)->EnableWindow(set);
}


void CEFieldDyn::InitializeYAxe()
{
	m_stcGraph.InitializeYAxe();
}


void CEFieldDyn::UpdateGraph()
{
	m_stcGraph.Invalidate();
}



void CEFieldDyn::OnClose()
{
	CDialogEx::OnClose();

	CSiDDlg* pMainDlg = (CSiDDlg*)AfxGetMainWnd();
	pMainDlg->m_chkEField.SetCheck(0);
	pMainDlg->DestroyEFieldWnd();
}


BOOL CEFieldDyn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon



	return TRUE;
}


//void CEFieldDyn::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: Add your message handler code here
//	// Do not call CDialogEx::OnPaint() for painting messages
//}
