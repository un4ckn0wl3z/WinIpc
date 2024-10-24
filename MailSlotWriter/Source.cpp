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
    return TRUE;

}


BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName)
{
    hSlot = CreateMailslot(lpszSlotName,
        0,                             // no maximum message size 
        MAILSLOT_WAIT_FOREVER,         // no time-out for operations 
        (LPSECURITY_ATTRIBUTES)NULL); // default security

    if (hSlot == INVALID_HANDLE_VALUE)
    {
        printf("CreateMailslot failed with %d\n", GetLastError());
        return FALSE;
    }
    else printf("Mailslot created successfully.\n");
    printf("Press any key to exit...\n");
    _getch();
    return TRUE;
}

void main()
{
    MakeSlot(SlotName);
}