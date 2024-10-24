#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>

LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");

BOOL WriteToMailslot(HANDLE hSlot, LPCTSTR lpszMessage)
{
    BOOL fResult;
    DWORD cbWritten;
    fResult = WriteFile(
        hSlot,
        lpszMessage,
        (DWORD)(lstrlen(lpszMessage) + 1) * sizeof(TCHAR),
        &cbWritten,
        ((LPOVERLAPPED)NULL)
        );

    if (!fResult)
    {
        printf("WriteFile failed with %d\n", GetLastError());
        return FALSE;
    }

    return TRUE;

}

int main()
{

    HANDLE hFile;

    hFile = CreateFile(
        SlotName,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile failed with %d\n", GetLastError());
        return FALSE;
    }

    WriteToMailslot(hFile, TEXT("1st message for mailslot."));
    WriteToMailslot(hFile, TEXT("2nd message for mailslot."));
    WriteToMailslot(hFile, TEXT("3rd message for mailslot."));


    printf("Press any key to exit...\n");
    _getch();

    CloseHandle(hFile);

    return TRUE;


}