
// MFCQuickEntryDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCQuickEntry.h"
#include "MFCQuickEntryDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCQuickEntryDlg dialog



CMFCQuickEntryDlg::CMFCQuickEntryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCQUICKENTRY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCQuickEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BAND, edtBand);
	DDX_Control(pDX, IDC_LIST_BANDS, lstBands);
	DDX_Control(pDX, IDC_BUTTON_ADD, btnAdd);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, btnRemove);
}

BEGIN_MESSAGE_MAP(CMFCQuickEntryDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMFCQuickEntryDlg::OnButtonAddClick)
	ON_EN_CHANGE(IDC_EDIT_BAND, &CMFCQuickEntryDlg::OnListChange)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CMFCQuickEntryDlg::OnBnClickedButtonRemove)
END_MESSAGE_MAP()


// CMFCQuickEntryDlg message handlers

BOOL CMFCQuickEntryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	presenter = std::make_unique<ListPresenter>(lstModel, *this);
	presenter->Init();

	lstBands.EnableWindow(false);
	edtBand.SetFocus();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCQuickEntryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCQuickEntryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCQuickEntryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCQuickEntryDlg::OnButtonAddClick()
{
	CString strText;
	int len = edtBand.LineLength(edtBand.LineIndex(0));
	edtBand.GetLine(0, strText.GetBuffer(len), len);
	strText.ReleaseBuffer(len);

	std::string item = CStringA(strText);
	IListView::AddItemEvent(item);
}

void CMFCQuickEntryDlg::OnListChange()
{
	int i, nLineCount = edtBand.GetLineCount();
	CString strText, strLine;
	int len = edtBand.LineLength(edtBand.LineIndex(0));
	edtBand.GetLine(0, strText.GetBuffer(len), len);
	strText.ReleaseBuffer(len);

	std::string txt = CStringA(strText);

	IListView::ChangeTextEvent(txt);
}



void CMFCQuickEntryDlg::OnBnClickedButtonRemove()
{
	IListView::RemoveItemEvent();
}

void CMFCQuickEntryDlg::ClearEditLine()
{
	edtBand.SetWindowTextW(L"");
}

void CMFCQuickEntryDlg::AddItemToList(const std::string& item)
{
	CString cs_item(item.c_str(), item.size());
	int index = lstBands.GetCount();
	lstBands.InsertString(index, cs_item);
}

void CMFCQuickEntryDlg::RemoveItemFromList(const std::string& item)
{
}

void CMFCQuickEntryDlg::SetListItems(const std::vector<std::string>& items)
{
	for (const auto& item : items)
	{
		CString cs_item(item.c_str(), item.size());
		lstBands.AddString(cs_item);
	}
}

void CMFCQuickEntryDlg::EnableBtnAdd(bool enabled)
{
	btnAdd.EnableWindow(enabled);
}

void CMFCQuickEntryDlg::EnableBtnRemove(bool enabled)
{
	btnRemove.EnableWindow(enabled);
}

void CMFCQuickEntryDlg::ClearSelectionOnList()
{
	lstBands.SetCurSel(-1); // clear selection
}

bool CMFCQuickEntryDlg::SelectItemOnList(const std::string& item)
{
	CString cs_item(item.c_str(), item.size());

	if (int index_found = lstBands.FindString(0, cs_item); index_found != LB_ERR)
	{
		lstBands.SetCurSel(index_found);
		return true;
	}

	return false;
}

int CMFCQuickEntryDlg::GetIndexOfSelectedItem()
{
	int index = lstBands.GetCurSel();
	return index;
}

void CMFCQuickEntryDlg::RemoveItemFromList(int index)
{
	lstBands.DeleteString(index);
}