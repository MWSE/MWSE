#pragma once

class CDataBoundPropertyGridProperty : public CMFCPropertyGridProperty {
public:
	CDataBoundPropertyGridProperty(const CString& strName, bool* bpData, LPCTSTR lpszDescr = NULL, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	CDataBoundPropertyGridProperty(const CString& strName, int* bpData, LPCTSTR lpszDescr = NULL, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	CDataBoundPropertyGridProperty(const CString& strName, float* bpData, LPCTSTR lpszDescr = NULL, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	CDataBoundPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);

	virtual BOOL OnUpdateValue();
};

class CDataBoundPropertyGridStringProperty : public CMFCPropertyGridProperty {
public:
	CDataBoundPropertyGridStringProperty(const CString& strName, std::string* pData, LPCTSTR lpszDescr = NULL);

	virtual BOOL OnUpdateValue();
};

class CDataBoundPropertyGridColorProperty : public CMFCPropertyGridColorProperty {
public:
	CDataBoundPropertyGridColorProperty(const CString& strName, std::array<unsigned char, 3>* pData, LPCTSTR lpszDescr = NULL);

	virtual BOOL OnUpdateValue();
};
