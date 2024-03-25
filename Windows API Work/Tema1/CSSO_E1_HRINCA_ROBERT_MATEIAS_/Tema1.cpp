#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
using namespace std;

bool CreeareDirector(const wchar_t* path) {

    DWORD ok = CreateDirectory(path, NULL);

    if (ok) {

        printf("Directorul %s a fost creat cu succes\n", path);
        return true;
    }
    else {
        printf("Directorul %s nu a fost creat cu succes  %d\n", path , GetLastError());
        return false;
    }
}

void WordSearch(wstring cuvant, wstring director, wstring outputFilePath) {


    WIN32_FIND_DATA fileAtribute;
    HANDLE fileHandler = FindFirstFile((director + L"\\\\*").c_str(), &fileAtribute);
    
    char charBuffer[256];

    
    size_t len = wcstombs(charBuffer, cuvant.c_str(), sizeof(charBuffer));

    charBuffer[len] = '\0'; 

    const char* wordToFind = charBuffer;




    if (fileHandler == INVALID_HANDLE_VALUE) {
        printf("Nu s-au gasit fisiere in director:  %d\n", GetLastError());
        return;
    }

    do {
        if ((fileAtribute.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
            printf("Este director, cautam in continuare\n");
        }
        else {
 
            wstring filePath = director + L"\\\\" + fileAtribute.cFileName;


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

                    HANDLE myFileHandle = CreateFile(outputFilePath.c_str(), FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

                    if (myFileHandle == INVALID_HANDLE_VALUE) {

                        printf("Nu s-au putut deschide fisierul:  %d\n", GetLastError());

                    }

                    DWORD sizePath, sizePozitie;

                    DWORD position = (DWORD)(found - buffer);
                    
                    wchar_t positionStr[20]; 
                    swprintf(positionStr, 20, L"%u", position);
                    wcscat_s(positionStr, L"\r\n");

                    

                    WriteFile(myFileHandle, filePath.c_str(), (DWORD)(wcslen(filePath.c_str()) * sizeof(wchar_t)), &sizePath, NULL);
                    WriteFile(myFileHandle, positionStr, (DWORD)(wcslen(positionStr) * sizeof(wchar_t)), &sizePath, NULL);
                   
                    CloseHandle(myFileHandle);
                   
       

                }
                free(buffer);
                

                

               
            }
            CloseHandle(openFileHandle);

        }
    } while (FindNextFile(fileHandler, &fileAtribute) != 0);

    FindClose(fileHandler);
}




int main() {
   
    const wchar_t* path1 = L"C:\\Facultate";
    const wchar_t* path2 = L"C:\\Facultate\\CSSO";
    const wchar_t* path3 = L"C:\\Facultate\\CSSO\\Laboratoare";
    const wchar_t* path4 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week1";
    const wchar_t* path5 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate";

  
    if (CreeareDirector(path1) &&
        CreeareDirector(path2) &&
        CreeareDirector(path3) &&
        CreeareDirector(path4) &&
        CreeareDirector(path5)) {
        printf("Toate directoarele au fost create.\n");
    }
    else {
        printf("Toate directoarele nu au fost create.\n");
    }        

       
    const int bufferSize = 256;
    WCHAR buffer[bufferSize];
    
    printf("Introduceti cuvantul: ");
    wcin.getline(buffer, bufferSize);
    wstring cuvant(buffer);


    const int bufferSizeDirector = 256;
    WCHAR bufferDirector[bufferSizeDirector];

    printf("Introduceti path-ul catre director: ");
    wcin.getline(bufferDirector, bufferSizeDirector);
    wstring director(bufferDirector);
 
  
    LPCWSTR outputPath = L"C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt";
    HANDLE myFileHandle = CreateFile(outputPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (myFileHandle == INVALID_HANDLE_VALUE)
    {
        printf("Am o eroare la crearea fisierului :  %d\n", GetLastError());
        return 0;
    }
    CloseHandle(myFileHandle);

    WordSearch(cuvant, director, outputPath);

    printf("Verifica sumar.txt!");
  

    return 0;
}
