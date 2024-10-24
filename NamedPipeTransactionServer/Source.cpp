#include<windows.h>
#include<iostream>

using namespace std;

#define BUFSIZE 512

HANDLE hPipe;
LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\pipe_tx");

DWORD WINAPI InstanceThread(LPVOID lpvParam)
{
    HANDLE hPipe = HANDLE(lpvParam);
    if (hPipe == NULL)
    {
        cout << endl << "The pipe handle invalid";
        return EXIT_FAILURE;
    }
    else
    {
        //Use ReadFile api to read the content 
        DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
        BOOL fSuccess = FALSE;
        TCHAR buff[BUFSIZE] = { 0, };
        CHAR buffRes[BUFSIZE] = { 0, };

        while (1)
        {
            fSuccess = ReadFile(
                hPipe,        // handle to pipe 
                buffRes,    // buffer to receive data 
                BUFSIZE, // size of buffer 
                &cbBytesRead, // number of bytes read 
                NULL);        // not overlapped I/O 

            if (!fSuccess || cbBytesRead == 0)
            {
                if (GetLastError() == ERROR_BROKEN_PIPE)
                {
                    cout << endl << "InstanceThread: client disconnected.\n";
                }
                else
                {
                    cout << endl << "InstanceThread ReadFile failed, GLE=" << GetLastError();
                }
                return EXIT_FAILURE;
            }

            //Print the message from client 
            cout << endl << "Message from client:" << buffRes;


            //Prepare the response and use WriteFile to send the reply.
            wcscpy_s(buff, L"Your request recieved and we are working on it..");
            cbReplyBytes = wcslen(buff);
            fSuccess = WriteFile(
                hPipe,        // handle to pipe 
                buff,     // buffer to write from 
                cbReplyBytes * sizeof(TCHAR), // number of bytes to write 
                &cbWritten,   // number of bytes written 
                NULL);        // not overlapped I/O 
            if (!fSuccess || cbReplyBytes * sizeof(TCHAR) != cbWritten)
            {
                cout << endl << "InstanceThread WriteFile failed, GLE=" << GetLastError();
                return EXIT_FAILURE;
            }
        }

    }
    return EXIT_SUCCESS;
}

int main()
{
    hPipe = CreateNamedPipe(
        lpszPipename,             // pipe name 
        PIPE_ACCESS_DUPLEX,       // read/write access 
        PIPE_TYPE_MESSAGE |       // message type pipe 
        PIPE_READMODE_MESSAGE |   // message-read mode 
        PIPE_WAIT,                // blocking mode 
        PIPE_UNLIMITED_INSTANCES, // max. instances  
        BUFSIZE,                  // output buffer size 
        BUFSIZE,                  // input buffer size 
        0,                        // client time-out 
        NULL);                    // default security attribute 

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        cout << endl << "Unable to create the pipe..";
        return EXIT_FAILURE;
    }

    while (1)
    {
        BOOL bConnected = ConnectNamedPipe(hPipe, NULL);
        if (!bConnected)
        {
            cout << endl << "Failed to connect to client";
            return EXIT_FAILURE;
        }
        else
        {
            //Create a thread to handle this client and response.
            HANDLE thClient = CreateThread(NULL, 0, InstanceThread, (LPVOID)hPipe, 0, NULL);
            if (thClient == NULL)
            {
                cout << endl << "Not able to create the client thread";
                return EXIT_FAILURE;
            }
        }
    }

}