
// Test_CSCDirWatcherDlg.h: 헤더 파일
//

#pragma once

#include "../../Common/ResizeCtrl.h"
#include "../../Common/file_system/SCDirWatcher/SCDirWatcher.h"
#include "../../Common/CListCtrl/CVtListCtrlEx/VtListCtrlEx.h"

// CTestCSCDirWatcherDlg 대화 상자
class CTestCSCDirWatcherDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestCSCDirWatcherDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	LRESULT			on_message_CSCDirWatcher(WPARAM wParam, LPARAM lParam);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_CSCDIRWATCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	enum TIMER_ID
	{
		timer_watching_status,
	};

	enum LIST_COLUMN_ID
	{
		col_folder = 0,
		col_status,
		col_recent_action,
	};

	CResizeCtrl			m_resize;
	CSCDirWatcher		m_dir_watcher;

	void				init_list();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CVtListCtrlEx m_list;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};
