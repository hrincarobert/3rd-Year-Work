#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <vector>
using namespace std;

int set_new_priv(LPCTSTR priv) {


    HANDLE hToken;
    LUID luid;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
        
        printf("OpenProcessToken failed = %d \n", GetLastError());
        return 0;
    }

    if (!LookupPrivilegeValue(NULL, priv, &luid)) {
        printf("LookupPrivilegeValue failed = %d \n", GetLastError());
        CloseHandle(hToken);
        return 0;
    }

    TOKEN_PRIVILEGES tokenPrivileges;
    tokenPrivileges.PrivilegeCount = 1;
    tokenPrivileges.Privileges[0].Luid = luid;
    tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
        printf("AdjustTokenPrivileges failed = %d \n", GetLastError());
        CloseHandle(hToken);
        return 0;
    }

    CloseHandle(hToken);
    return 1;
}


int GetFileContor(LPCTSTR filePath) {

    int contor = 0;
    HANDLE hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        char buffer[20000];
        DWORD bytesRead;

        while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
            for (DWORD i = 0; i < bytesRead; i++) {
                if (buffer[i] == '\n') {
                    contor++;
                }
            }
        }

        CloseHandle(hFile);
    }

    return contor;
}

void WordSearch(wstring director) {

    const char* wordToFind = "amdfendrsr.exe";



    int nr = GetFileContor(director.c_str());
   

    wstring filePath = director;


            HANDLE openFileHandle = CreateFile(filePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (openFileHandle != INVALID_HANDLE_VALUE) {



                DWORD fileSize = GetFileSize(openFileHandle, NULL);


                char* buffer = (char*)malloc(fileSize + 1);

                DWORD bytesRead;
                if (!ReadFile(openFileHandle, buffer, fileSize, &bytesRead, NULL)) {
                    printf("Nu s-a putut citi fisierul: %d\n", GetLastError());
                    free(buffer);
                    CloseHandle(openFileHandle);

                }

                buffer[fileSize] = '\0';



                char* found = strstr(buffer, wordToFind);

                if (found != NULL) {

                    printf("S-a gasit in %ls cu %d linii. \n", filePath.c_str(), nr);
                }
                free(buffer);
            }
            CloseHandle(openFileHandle);    
}


DWORD WINAPI WordSearchThread(LPVOID lpParam) {
    wstring filePath = *reinterpret_cast<wstring*>(lpParam);
    WordSearch(filePath);
    return 0;
}


int main()
{
    set_new_priv(SE_SYSTEM_PROFILE_NAME);

    wstring filePaths[] = {
        L"C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\fire.txt",
        L"C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\module_process.txt",
        L"C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\procese.txt"
    };

    const int numThreads = 3;
    HANDLE threads[numThreads];

    for (int i = 0; i < numThreads; i++) {
        threads[i] = CreateThread(NULL, 0, WordSearchThread, &filePaths[i], 0, NULL);
        if (threads[i] == NULL) {
            printf("Nu s-a creat thread-ul %d\n", i);
        }
    }

    WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);

    for (int i = 0; i < numThreads; i++) {
        CloseHandle(threads[i]);
    }
   

    cout << "Pauza de " << 30 << " minute incepe acum..." << endl;

    Sleep(30 * 60 * 1000);

    return 0;
}