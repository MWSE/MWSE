#pragma once

class CDataBoundPropertyGridProperty : public CMFCPropertyGridProperty {
public:
	CDataBoundPropertyGridProperty(const CString& strName, bool* bpData, LPCTSTR lpszDescr = NULL, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	CDataBoundPropertyGridProperty(const CString& strName, int* bpData, LPCTSTR lpszDescr = NULL, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	CDataBoundPropertyGridProperty(const CString& strName, float* bpData, LPCTSTR lpszDescr = NULL, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	CDataBoundPropertyGridProperty(const CString& strName, std::string* bpData, LPCTSTR lpszDescr = NULL, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	CDataBoundPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);

	virtual BOOL OnUpdateValue();
};
