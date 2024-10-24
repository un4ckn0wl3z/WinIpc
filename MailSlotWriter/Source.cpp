#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>

HANDLE hSlot;
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

    printf("message written to slot successfully\n");
    printf("Press any key to exit...\n");
    _getch();
    return TRUE;

}

void main()
{

}