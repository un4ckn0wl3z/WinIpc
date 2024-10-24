#include<Windows.h>
#include<stdio.h>

HANDLE hRead, hWrite; //For pipe operations
HANDLE hWriter, hReader; //For reader and writer threads
HANDLE hEvent;

void Reader()
{
	WaitForSingleObject(hEvent, INFINITE);
	char buff[20];
	DWORD nBytesRead;
	ReadFile(hRead, buff, 19, &nBytesRead, NULL);
	printf("Read the content:%s", buff);
}

void Writer()
{
	//Wite using the WriteFile 
	char buff[20];
	DWORD nBytesWritten = 0;
	strcpy_s(buff, "Hi , Hello world.");
	WriteFile(hWrite, buff, 19, &nBytesWritten, NULL);
	SetEvent(hEvent);
}

int main()
{

	BOOL b = CreatePipe(&hRead, &hWrite, NULL, 0);
	if (!b)
	{
		printf("\nPipe not created..");
		exit(EXIT_FAILURE);
	}

	hEvent = CreateEvent(NULL, TRUE, FALSE, LPCWSTR("Event1"));
	HANDLE th[2];
	th[0] = hWriter = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Writer, NULL, 0, NULL);
	th[1] = hReader = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Reader, NULL, 0, NULL);
	WaitForMultipleObjects(2, th, TRUE, INFINITE);
	return 0;
}