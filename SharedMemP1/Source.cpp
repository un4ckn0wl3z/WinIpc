
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <conio.h>

using namespace std;

#define BUF_SIZE 256

typedef struct _employee_data
{
    char sName[32];
    char sAddress[32];
    int empCode;
}emp;

/*Prefixing the file mapping object names with "Global\" allows
processes to communicate with each other even if they are in
different terminal server sessions. This requires that the first
process must have the SeCreateGlobalPrivilege privilege.*/
TCHAR szName[] = TEXT("//ShMemFile");
void* pMsg = NULL;

int main()
{
    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        BUF_SIZE,                // maximum object size (low-order DWORD)
        szName);                 // name of mapping object

    if (hMapFile == NULL)
    {
        cout << endl << "Unable to initiate the shared memory." << GetLastError();
        return EXIT_FAILURE;
    }

    pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,
        0,
        BUF_SIZE);

    if (pBuf == NULL)
    {
        cout << endl << "Unable to map the shared memory area." << GetLastError();

        CloseHandle(hMapFile);

        return EXIT_FAILURE;
    }

    pMsg = new emp;
    if (pMsg == NULL)
    {
        cout << endl << "Failed to initialize the memory.";
        return EXIT_FAILURE;
    }

    ((emp*)pMsg)->empCode = 1001;
    strcpy_s(((emp*)pMsg)->sName, "un4ckn0wl3z");
    strcpy_s(((emp*)pMsg)->sAddress, "Bang Khen, BANGKOK");

    CopyMemory((PVOID)pBuf, (TCHAR*)pMsg, sizeof(emp));
    _getch();
    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);

    return EXIT_SUCCESS;
}

