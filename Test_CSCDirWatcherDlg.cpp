
// Test_CSCDirWatcherDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Test_CSCDirWatcher.h"
#include "Test_CSCDirWatcherDlg.h"
#include "afxdialogex.h"

#include "../../Common/Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CTestCSCDirWatcherDlg 대화 상자



CTestCSCDirWatcherDlg::CTestCSCDirWatcherDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_CSCDIRWATCHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestCSCDirWatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CTestCSCDirWatcherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestCSCDirWatcherDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestCSCDirWatcherDlg::OnBnClickedCancel)
	ON_REGISTERED_MESSAGE(Message_CSCDirWatcher, &CTestCSCDirWatcherDlg::on_message_CSCDirWatcher)
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()


// CTestCSCDirWatcherDlg 메시지 처리기

BOOL CTestCSCDirWatcherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_resize.Create(this);
	m_resize.Add(IDC_LIST, 0, 0, 100, 100);
	m_resize.Add(IDOK, 100, 100, 0, 0);
	m_resize.Add(IDCANCEL, 100, 100, 0, 0);

	init_list();
	std::deque<CString> dq_list;
	get_registry_list(&theApp, _T("setting\\watching folders"), dq_list);
	m_list.add_item(dq_list);

	m_dir_watcher.init(this);
	m_dir_watcher.add(dq_list);

	DragAcceptFiles();

	RestoreWindowPosition(&theApp, this);

	SetTimer(timer_watching_status, 1000, nullptr);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestCSCDirWatcherDlg::init_list()
{
	m_list.set_headings(_T("Watching Folder,400;Status,100;Recent Action,400"));
	m_list.load_column_width(&theApp, _T("folder list"));
	m_list.set_font_size(8);
	m_list.set_header_height(22);
	m_list.set_line_height(20);
	m_list.allow_edit();
	m_list.set_back_alternate_color();

	m_list.set_text_on_empty(_T("변경을 모니터링 할 폴더를 drag&drop 하세요."));
}

void CTestCSCDirWatcherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestCSCDirWatcherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestCSCDirWatcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestCSCDirWatcherDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_dir_watcher.stop(_T("D:\\temp"));
	m_dir_watcher.stop();
	/*
	m_dir_watcher.stop_all();
	//Sleep(2000);
	m_dir_watcher.add(_T("D:\\test"), false); // Start watching the directory again
	//CDialogEx::OnOK();
	*/
}

void CTestCSCDirWatcherDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_list.save_column_width(&theApp, _T("folder list"));

	CDialogEx::OnCancel();
}

LRESULT CTestCSCDirWatcherDlg::on_message_CSCDirWatcher(WPARAM wParam, LPARAM lParam)
{
	CSCDirWatcherMessage* msg = (CSCDirWatcherMessage*)wParam;

	CString event_str;

	if (msg->action == FILE_ACTION_RENAMED_NEW_NAME)
		event_str.Format(_T("%s. from = %s, to = %s"), CSCDirWatcher::action_str(msg->action), msg->path1, msg->path0);
	else
		event_str.Format(_T("%s. path0 = %s"), CSCDirWatcher::action_str(msg->action), msg->path0);
	TRACE(_T("%s\n"), event_str);

	CString event_folder = get_part(msg->path0, fn_folder);

	LVFINDINFO fi;
	//memset(&fi, 0, sizeof(fi));
	fi.flags = LVFI_STRING;
	fi.psz = event_folder;
	//int index = m_list.find(event_folder);// (&fi);
	int index = m_list.FindItem(&fi);
	m_list.set_text(index, col_recent_action, event_str);

	return 0;
}

void CTestCSCDirWatcherDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TCHAR sfile[MAX_PATH];

	int count = DragQueryFile(hDropInfo, -1, sfile, MAX_PATH);

	for (int i = 0; i < count; i++)
	{
		DragQueryFile(hDropInfo, i, sfile, MAX_PATH);
		if (PathIsDirectory(sfile))
		{
			m_list.add_item(sfile);
			m_dir_watcher.add(sfile, true);
			add_registry(&theApp, _T("setting\\watching folders"), sfile);
		}
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

BOOL CTestCSCDirWatcherDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_DELETE :
				if (GetFocus() == &m_list)
				{
					std::deque<int> dq;
					m_list.get_selected_items(&dq);
					for (int i = dq.size() - 1; i >= 0; i--)
					{
						CString folder = m_list.get_text(dq[i], col_folder);
						m_dir_watcher.stop(folder);
						m_list.delete_item(dq[i]);
					}
				}
				break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTestCSCDirWatcherDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == timer_watching_status)
	{
		for (int i = 0; i < m_list.size(); i++)
		{
			CString folder = m_list.get_text(i, 0);
			if (folder.IsEmpty())
				continue;

			if (!PathFileExists(folder))
			{
				m_list.set_text(i, col_status, _T("not exist"));
				m_list.set_text_color(i, -1, Gdiplus::Color::Red);
				continue;
			}
			else if (PathIsDirectory(folder) == false)
			{
				m_list.set_text(i, col_status, _T("not directory"));
				m_list.set_text_color(i, -1, Gdiplus::Color::Red);
				continue;
			}

			m_list.set_text_color(i, -1, m_list.m_theme.cr_text);
			m_dir_watcher.add(folder);
			m_list.set_text(i, 1, _T("watching"), false);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CTestCSCDirWatcherDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}
