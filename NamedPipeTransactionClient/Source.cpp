#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 512

int main(int argc, TCHAR* argv[])
{
    LPTSTR lpszWrite = LPTSTR("Demo Message from the client");
    TCHAR chReadBuf[BUFSIZE] = { 0, };
    BOOL fSuccess;
    DWORD cbRead;
    LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\pipe_tx");

    if (argc > 1)
    {
        lpszWrite = argv[1];
    }

    fSuccess = CallNamedPipe(
        lpszPipename,        // pipe name 
        (LPVOID)lpszWrite,           // message to server 
        (lstrlen(lpszWrite) + 1) * sizeof(TCHAR), // message length 
        chReadBuf,              // buffer to receive reply 
        BUFSIZE * sizeof(TCHAR),  // size of read buffer 
        &cbRead,                // number of bytes read 
        20000);                 // waits for 20 seconds 

    if (fSuccess || GetLastError() == ERROR_MORE_DATA)
    {
        _tprintf(TEXT("%s\n"), chReadBuf);

        // The pipe is closed; no more data can be read. 

        if (!fSuccess)
        {
            printf("\nExtra data in message was lost\n");
        }
    }


    //_getch();

    return 0;
}