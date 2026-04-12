#pragma once

//namespace se::cs {
	class DialogCSSESettings : public CDialogEx {
		DECLARE_DYNAMIC(DialogCSSESettings)

	public:
		DialogCSSESettings(CWnd* pParent = nullptr);   // standard constructor
		virtual ~DialogCSSESettings();

	virtual BOOL OnInitDialog();

	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);

		// Dialog Data
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CSSE_SETTINGS };
#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		void BuildPropertyGrid();
		void ApplyThemeToPropertyGrid();
		void SyncThemePropertyValues(CMFCPropertyGridProperty* property);
		bool IsThemeProperty(const CMFCPropertyGridProperty* property) const;

		DECLARE_MESSAGE_MAP()
	public:
		CMFCPropertyGridCtrl m_PropertyGrid;

		CMFCPropertyGridProperty* m_PropertyEnabled = nullptr;
		CMFCPropertyGridProperty* m_GroupTheme = nullptr;
		CMFCPropertyGridProperty* m_PropertyThemePreset = nullptr;
		CMFCPropertyGridProperty* m_PropertyThemeEnabled = nullptr;
		CMFCPropertyGridProperty* m_PropertyThemeDarkTitleBar = nullptr;

		// Guard against re-entrant AFX_WM_PROPERTY_CHANGED notifications fired
		// when SyncThemePropertyValues programmatically updates color properties.
		bool m_bSyncingTheme = false;
	};
//}
