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
const char* depositDirectory = "C:\\Users\\Robert\\Desktop\\deposit";
const char* errorsFileName = "C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt";
const char* logsFileName = "C:\\Facultate\\CSSO\\Week4\\Reports\\logs.txt";

const int MAX_PATH_LEN = 260;
const int MAX_LINE_LEN = 256;


bool CreateErrorsFile(const wchar_t* filePath) {
  
    HANDLE hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Nu s-a creeat errors.txt!\n");
        return false;
    }

    
    CloseHandle(hFile);

    return true;
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
        cout << "EROARE LA DESCHIDERE DIN DEPOSIT MAPPING" << endl;
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

void ProcessDepositFiles(LPDWORD shelves, LPDWORD valability, LPDWORD prices) {

    HANDLE hExistingMutex = OpenMutex(SYNCHRONIZE, FALSE, L"LogsMutex");
    /*HANDLE hExistingMutex_MarketShelvesMutex = OpenMutex(SYNCHRONIZE, FALSE, L"MarketShelvesMutex");
    HANDLE hExistingMutex_MarketValabilityMutex = OpenMutex(SYNCHRONIZE, FALSE, L"MarketValabilityMutex");
    HANDLE hExistingMutex_ProductPricesMutex = OpenMutex(SYNCHRONIZE, FALSE, L"ProductPricesMutex");*/
    LPWIN32_FIND_DATAA findFileData = new WIN32_FIND_DATAA;
    HANDLE hFind = FindFirstFileA((depositDirectory + string("\\*.txt")).c_str(), findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        
        do {
            if (!(findFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
           
                char filePath[MAX_PATH_LEN];
               
                sprintf_s(filePath, MAX_PATH_LEN, "%s\\%s", depositDirectory, findFileData->cFileName);
    

                     LPCWSTR filePath_wide = ConvertToWideString(filePath);
                     string content = ReadFileContent(filePath_wide);
                     
                     vector<string> lines = ExtractLines(content);
              
                     size_t lineNumber = 0;
                     /*0/WaitForSingleObject(hExistingMutex_MarketShelvesMutex, INFINITE);
                     WaitForSingleObject(hExistingMutex_MarketValabilityMutex, INFINITE);
                     WaitForSingleObject(hExistingMutex_ProductPricesMutex, INFINITE);*/

                     while (lineNumber < lines.size()) {
                         
                         string currentLine = lines[lineNumber];

                         DWORD id_produs, expires_in, shelve_id, product_price;

                         sscanf_s(currentLine.c_str(), "%u,%u,%u,%u", &id_produs, &expires_in, &shelve_id, &product_price);

                 

                         if (shelves[shelve_id] != 0xFFFFFFFF) {
                             
                             char errorContent[MAX_LINE_LEN];
                             sprintf_s(errorContent, MAX_LINE_LEN, "S-a incercat adaugarea produsului %u pe raftul %u care este deja ocupat de %u\n", id_produs, shelve_id, shelves[shelve_id]);
                             
                             WriteToFile(errorsFileName, errorContent);
                             
                         }
                         else {

                            

                             shelves[shelve_id] = id_produs;
                             valability[id_produs] = expires_in;
                             prices[id_produs] = product_price;

                             
                        
                             
                             char logContent[MAX_LINE_LEN];
                             sprintf_s(logContent, MAX_LINE_LEN, "Am adaugat pe raftul %u produsul %u ce are o valabilitate de %u zile si un pret de %u.\n", shelve_id, id_produs, expires_in, product_price);

                             WaitForSingleObject(hExistingMutex, INFINITE);

                             WriteToFile(logsFileName, logContent);

                             ReleaseMutex(hExistingMutex);

                             CloseHandle(hExistingMutex);


                         }

                         lineNumber++;
                         
                     }
                     /*ReleaseMutex(hExistingMutex_MarketShelvesMutex);
                     ReleaseMutex(hExistingMutex_MarketValabilityMutex);
                     ReleaseMutex(hExistingMutex_ProductPricesMutex);
                     CloseHandle(hExistingMutex_MarketShelvesMutex);
                     CloseHandle(hExistingMutex_MarketValabilityMutex);
                     CloseHandle(hExistingMutex_ProductPricesMutex);*/
                     delete[] filePath_wide;
                   
                
            }
        } while (FindNextFileA(hFind, findFileData) != 0);

        FindClose(hFind);
        delete findFileData;  
    }
 
}



int main() {

    const wchar_t* errorsFileName = L"C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt";

    if (CreateErrorsFile(errorsFileName)!=0) {
        printf("S-a creeat errors.txt!\n");
    }


    LPDWORD shelves = OpenAndMapFile(L"MarketShelves");
    LPDWORD valability = OpenAndMapFile(L"MarketValability");
    LPDWORD prices = OpenAndMapFile(L"ProductPrices");


     
    ProcessDepositFiles(shelves, valability, prices);


    return 0;
}
