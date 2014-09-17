// PoincareMap.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "PoincareMap.h"
#include "afxdialogex.h"

#include "SiDDlg.h"

// CPoincareMap dialog

IMPLEMENT_DYNAMIC(CPoincareMap, CDialogEx)

CPoincareMap::CPoincareMap(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPoincareMap::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPoincareMap::~CPoincareMap()
{
	DestroyWindow();
}

void CPoincareMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PMAPGRAPH, m_stcGraph);
}


BEGIN_MESSAGE_MAP(CPoincareMap, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPoincareMap message handlers


BOOL CPoincareMap::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;
}


void CPoincareMap::OnClose()
{
	CDialogEx::OnClose();

	CSiDDlg* pMainDlg = (CSiDDlg*)AfxGetMainWnd();
	pMainDlg->m_chkPMap.SetCheck(0);
	pMainDlg->DestroyPMapDiagWnd();
}


void CPoincareMap::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//������� ������ ������ �������� 

	CRect rect;
	GetClientRect(&rect);

	//������� �� ����� ��� ���������� ��������� ������� - �� 7 ��������
	int nSpace = 11;
	//������ ������ ������� - 17 ��������
	int nToolbarHeight = 28;

	CRect newRect;
	newRect.top = 2 * nSpace + nToolbarHeight;
	newRect.left = nSpace;
	newRect.bottom = rect.bottom - nSpace;
	newRect.right = rect.right - nSpace;

	m_stcGraph.MoveWindow(&newRect);
	m_stcGraph.Invalidate();
}


void CPoincareMap::OnOK()
{
}


void CPoincareMap::InitializeYAxe()
{
	m_stcGraph.InitializeAxes();
}


void CPoincareMap::UpdateGraph()
{
	m_stcGraph.Invalidate();
}