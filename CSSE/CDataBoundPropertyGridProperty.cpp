#include "CDataBoundPropertyGridProperty.h"

#include "Settings.h"

CDataBoundPropertyGridProperty::CDataBoundPropertyGridProperty(const CString& strName, bool* bpData, LPCTSTR lpszDescr, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) :
	CDataBoundPropertyGridProperty(strName, COleVariant(*bpData ? VARIANT_TRUE : VARIANT_FALSE, VT_BOOL), lpszDescr, (DWORD_PTR)bpData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{

}

CDataBoundPropertyGridProperty::CDataBoundPropertyGridProperty(const CString& strName, int* bpData, LPCTSTR lpszDescr, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) :
	CDataBoundPropertyGridProperty(strName, COleVariant((long)*bpData), lpszDescr, (DWORD_PTR)bpData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{

}

CDataBoundPropertyGridProperty::CDataBoundPropertyGridProperty(const CString& strName, float *bpData, LPCTSTR lpszDescr, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) :
	CDataBoundPropertyGridProperty(strName, COleVariant(*bpData), lpszDescr, (DWORD_PTR)bpData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{

}

CDataBoundPropertyGridProperty::CDataBoundPropertyGridProperty(const CString& strName, std::string* bpData, LPCTSTR lpszDescr, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) :
	CDataBoundPropertyGridProperty(strName, COleVariant(bpData->c_str()), lpszDescr, (DWORD_PTR)bpData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{

}

CDataBoundPropertyGridProperty::CDataBoundPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) :
	CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{

}

BOOL CDataBoundPropertyGridProperty::OnUpdateValue() {
	if (!CMFCPropertyGridProperty::OnUpdateValue()) {
		return FALSE;
	}

	auto data = (void*)GetData();
	if (data == NULL) {
		return TRUE;
	}

	const auto& value = GetValue();
	switch (value.vt) {
	case VT_BOOL:
		*reinterpret_cast<bool*>(data) = (value.boolVal == VARIANT_TRUE);
		break;
	case VT_I4:
		*reinterpret_cast<int*>(data) = value.intVal;
		break;
	case VT_UI4:
		*reinterpret_cast<unsigned int*>(data) = value.uintVal;
		break;
	case VT_R4:
		*reinterpret_cast<float*>(data) = value.fltVal;
		break;
	case VT_R8:
		*reinterpret_cast<double*>(data) = value.dblVal;
		break;
	case VT_BSTR:
		*reinterpret_cast<std::string*>(data) = (LPCSTR)CString(value.bstrVal);
		break;
	}

	return TRUE;
}