#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <vector>
#include <windows.h>
#include <windows.h>
#include <string>
#include <sstream>


using namespace std;

const char* shelvesFileName = "MarketShelves";
const char* valabilityFileName = "MarketValability";
const char* pricesFileName = "ProductPrices";
const char* soldDirectory = "C:\\Users\\Robert\\Desktop\\sold";
const char* errorsFileName = "C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt";
const char* logsFileName = "C:\\Facultate\\CSSO\\Week4\\Reports\\logs.txt";
const char* soldFileName = "C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\sold.txt";

const int MAX_PATH_LEN = 260;
const int MAX_LINE_LEN = 256;


bool replaceValueSold(string filePath, string oldValue, string newValue) {
    HANDLE hFile = CreateFileA(filePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Eroare la deschidere fisier in replaceValueSold!\n");
        return false;
    }

    DWORD bytesRead, bytesWritten;
    char buffer[50000]; 

    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        string content(buffer, bytesRead);
        size_t pos = content.find(oldValue);

        if (pos != string::npos) {
            content.replace(pos, oldValue.length(), newValue);
            SetFilePointer(hFile, -static_cast<LONG>(bytesRead), NULL, FILE_CURRENT); 
            WriteFile(hFile, content.c_str(), static_cast<DWORD>(content.length()), &bytesWritten, NULL);
            CloseHandle(hFile);
            return true;
        }
    }

    CloseHandle(hFile);
    return false;
}

wchar_t* ConvertToWideString(const char* narrowStr) {
    int wideStrLen = MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, nullptr, 0);
    wchar_t* wideStr = new wchar_t[wideStrLen];
    MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, wideStr, wideStrLen);
    return wideStr;
}

LPDWORD OpenAndMapFile(LPCWSTR fileName) {



    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, fileName);
    if (hMapFile == NULL) {
        cout << "EROARE LA DESCHIDERE DIN SOLD MAPPING" << endl;
        CloseHandle(hMapFile);
        return nullptr;
    }

    LPDWORD data = reinterpret_cast<LPDWORD>(MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0));
    if (data == nullptr) {

        CloseHandle(hMapFile);
        return nullptr;
    }
    
    CloseHandle(hMapFile);
    return data;
}

void WriteToFile(const char* fileName, const char* content) {
    HANDLE hFile = CreateFileA(fileName, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        WriteFile(hFile, content, static_cast<DWORD>(strlen(content)), &bytesWritten, NULL);
        CloseHandle(hFile);
    }
}

string ReadFileContent(const wstring& filename) {
    HANDLE hFile = CreateFile(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Eroare la deschidere fisier in ReadFileContent!\n");

        return "";
    }

    const DWORD bufferSize = 50000;
    char buffer[bufferSize];
    string fileContent;

    while (true) {
        DWORD bytesRead;
        if (ReadFile(hFile, buffer, bufferSize - 1, &bytesRead, NULL)) {
            if (bytesRead == 0) {
                break;
            }

            buffer[bytesRead] = '\0';
            fileContent += buffer;
        }
        else {
            printf("Eroare la citire fisier din ReadFileContent!\n");
            fileContent = "";
            break;
        }
    }

    CloseHandle(hFile);
    return fileContent;
}


vector<string> ExtractLines(const string& content) {
    vector<string> lines;
    istringstream contentStream(content);
    string line;

    while (getline(contentStream, line, '\n')) {
        lines.push_back(line);
    }

    return lines;
}

void ProcessSoldFiles(LPDWORD shelves, LPDWORD valability, LPDWORD prices) {

  
    HANDLE hExistingMutex = OpenMutex(SYNCHRONIZE, FALSE, L"LogsMutex");
    
    LPWIN32_FIND_DATAA findFileData = new WIN32_FIND_DATAA;
    HANDLE hFind = FindFirstFileA((soldDirectory + string("\\*.txt")).c_str(), findFileData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        
        do {
            if (!(findFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
              
                char filePath[MAX_PATH_LEN];

                sprintf_s(filePath, MAX_PATH_LEN, "%s\\%s", soldDirectory, findFileData->cFileName);
        



                LPCWSTR filePath_wide = ConvertToWideString(filePath);
                string content = ReadFileContent(filePath_wide);

                vector<string> lines = ExtractLines(content);

                
                size_t lineNumber = 0;

                while (lineNumber < lines.size()) {
                  
                    string currentLine = lines[lineNumber];

                    DWORD shelve_id;

                    sscanf_s(currentLine.c_str(), "%u", &shelve_id);

                


                    if (shelves[shelve_id] != 0xFFFFFFFF) {
                       
                        DWORD id_produs = shelves[shelve_id];                       
                        

                        char logContent[MAX_LINE_LEN];
                        sprintf_s(logContent, MAX_LINE_LEN, "S-a vandut produsul %u de pe raftul %u cu %u zile inainte de a fi donat si cu pretul de %u.\n", id_produs, shelve_id, valability[id_produs], prices[id_produs]);


                        WaitForSingleObject(hExistingMutex, INFINITE);
                        WriteToFile(logsFileName, logContent);
                        ReleaseMutex(hExistingMutex);
                        CloseHandle(hExistingMutex);

                        DWORD new_value = shelve_id + prices[id_produs];

                        char update[MAX_LINE_LEN];
                        sprintf_s(update, MAX_LINE_LEN, "%u\n", new_value);
                        WriteToFile(soldFileName, update);

                        valability[id_produs] = 0xFFFFFFFF;
                        prices[id_produs] = 0xFFFFFFFF;
                        shelves[id_produs] = 0xFFFFFFFF;
                              
                    }
                    else {
                        
                        
                

                        char errorContent[MAX_LINE_LEN];
                        sprintf_s(errorContent, MAX_LINE_LEN, "S-a incercat vanzarea unui produs de pe un raft %u ce nu contine produs.\n",shelve_id);

                        WriteToFile(errorsFileName, errorContent);


                    }

                    lineNumber++;
                    
                }
                delete[] filePath_wide;


            }
        } while (FindNextFileA(hFind, findFileData) != 0);

        FindClose(hFind);
        delete findFileData;
    }
    
}



int main() {

   


    LPDWORD shelves = OpenAndMapFile(L"MarketShelves");
    LPDWORD valability = OpenAndMapFile(L"MarketValability");
    LPDWORD prices = OpenAndMapFile(L"ProductPrices");



    ProcessSoldFiles(shelves, valability, prices);



    return 0;
}
