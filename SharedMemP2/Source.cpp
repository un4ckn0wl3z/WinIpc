#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <conio.h>

using namespace std;

typedef struct _employee_data
{
    char sName[32];
    char sAddress[32];
    int empCode;
}emp;

#define BUF_SIZE 256
TCHAR szName[] = TEXT("//ShMemFile");

int _tmain()
{
    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        szName);               // name of mapping object

    if (hMapFile == NULL)
    {
        cout << endl << "Cannot Open File Mapping Object:" << GetLastError();
        return EXIT_FAILURE;
    }

    /*All access may be needed in some scenarios like Railway Reservation system.
    Agent1 : Reservation
    Agent2 : Reservation
    ..
    ....
    Agentn : Reservation

    SERVER : All the logic of shared memory containing the data of reserved/unresrved seats
    Once an agent connects, it creates a thread for that agent (client), and then this new thread
    will map the view of shared memory and update the count of seats after reservation and that should
    be reflected to all the other agents*/
    pBuf = (LPTSTR)MapViewOfFile(hMapFile, // handle to map object
        FILE_MAP_ALL_ACCESS,  // read/write permission
        0,
        0,
        BUF_SIZE);

    if (pBuf == NULL)
    {
        cout << endl << "Cannot map the view of the file:" << GetLastError();

        CloseHandle(hMapFile);

        return EXIT_FAILURE;
    }

    //Print the content of the shared memory
    emp* pEmp = new emp;

    strcpy_s(pEmp->sName, ((emp*)pBuf)->sName);
    strcpy_s(pEmp->sAddress, ((emp*)pBuf)->sAddress);
    pEmp->empCode = ((emp*)pBuf)->empCode;

    cout << endl << "EMP CODE:" << pEmp->empCode;
    cout << endl << "EMP NAME:" << pEmp->sName;
    cout << endl << "EMP ADDRESS:" << pEmp->sAddress;

    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);

    return EXIT_SUCCESS;
}