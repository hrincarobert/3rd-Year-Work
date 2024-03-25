#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
using namespace std;

bool CreeareDirector1(const wchar_t* path) {
    if (CreateDirectory(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        return true;
    }
    else {
        printf("Eroare la a creea directorul %s. Cod eroare: %d\n", path, GetLastError());
        return false;
    }
}

bool get_subKeys_maxSubKey_lastWriteTime(HKEY hKey, LPCWSTR outputPath) {
   


    if (RegOpenKeyEx(hKey, NULL, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {


        DWORD maxSubKeyLen;
        LSTATUS err_maxSubKeyLen =  RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, &maxSubKeyLen, NULL, NULL, NULL, NULL, NULL, NULL);
    
        if (err_maxSubKeyLen != ERROR_SUCCESS)
        {
            printf("Eroare obtinere maxSubKeyLen: %d\n", GetLastError);
        }
       

        DWORD subKeys;
        LSTATUS err_subKeys = RegQueryInfoKey(hKey, NULL, NULL, NULL, &subKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        
        if (err_subKeys != ERROR_SUCCESS)
        {
            printf("Eroare obtinere subKeys: %d\n", GetLastError);
        }


        FILETIME lastWriteTime;
        LSTATUS err_time = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &lastWriteTime);

        if (err_maxSubKeyLen != ERROR_SUCCESS)
        {
            printf("Eroare obtinere lastWriteTime: %d\n", GetLastError);
        }

        SYSTEMTIME st;
        FileTimeToSystemTime(&lastWriteTime, &st);
        wchar_t lastWriteTimeStr[50];
        swprintf_s(lastWriteTimeStr, 50, L"%04d/%02d/%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

        
        HANDLE fileHandle = CreateFile(outputPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (fileHandle != INVALID_HANDLE_VALUE) {


            DWORD subKeyIndex = 0;
            wchar_t subKeyName[MAX_PATH];
            wstring data;
            DWORD maxSubKeyLen1 = MAX_PATH;
          
            DWORD contor = subKeys;

            while (contor != 0) {
                LSTATUS ok =  RegEnumKeyExW(hKey, subKeyIndex, subKeyName, &maxSubKeyLen1, NULL, NULL, NULL, NULL);
                if (ok != ERROR_SUCCESS)
                {
                    printf("Eroare obtinere nume subcheie. : %d\n", GetLastError);
                    
                }
                wstring data_subKey;
                data_subKey = subKeyName;
                data_subKey += L" ";
                DWORD bytesWritten;
                if (!WriteFile(fileHandle, data_subKey.c_str(), (DWORD)(data_subKey.size() * sizeof(wchar_t)), &bytesWritten, NULL)) {
                    printf("Eroare scriere subcheie in fisier. : %d\n", GetLastError);
               }
                contor--;
                subKeyIndex = subKeyIndex + 1;
                maxSubKeyLen1 = MAX_PATH;

            }


            data += to_wstring(subKeys) + L"\n";
            data += L"lpcMaxSubKeyLen: " + to_wstring(maxSubKeyLen) + L"\n";
            data += L"lpftLastWriteTime: " + wstring(lastWriteTimeStr) + L"\n";

            DWORD bytesWritten;
            if (!WriteFile(fileHandle, data.c_str(), (DWORD)(data.size() * sizeof(wchar_t)), &bytesWritten, NULL)) {
                printf("Eroare scriere MaxSubKeyLen si LastWriteTime in fisier. : %d\n", GetLastError);
            }
            CloseHandle(fileHandle);
        }
        else {
            printf("Eroare deschidere fisier: %d\n", GetLastError);
        }
           
        

        RegCloseKey(hKey);
    }
    else {
        printf("Eroare la deschiderea cheii: %d\n", GetLastError());
    }

    return true;
}



bool write_in_sumar(LPCWSTR path1, LPCWSTR path2, LPCWSTR path3)
{
    LPCWSTR outputPath_sumar = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\sumar.txt";
    HANDLE myFileHandle_sumar = CreateFile(outputPath_sumar, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (myFileHandle_sumar == INVALID_HANDLE_VALUE)
    {
        printf("Am o eroare la crearea fisierului :  %d\n", GetLastError());
        return false;
    }

    DWORD sizePath1, size1, sizePath2, size2, sizePath3, size3;
    wstring path1Str(path1);
    wstring path2Str(path2);
    wstring path3Str(path3);

    WriteFile(myFileHandle_sumar, path1Str.c_str(), static_cast<DWORD>(path1Str.size() * sizeof(wchar_t)), &sizePath1, NULL);

    WIN32_FILE_ATTRIBUTE_DATA fileData1;
    if (GetFileAttributesEx(path1, GetFileExInfoStandard, &fileData1)) {
        ULONGLONG fileSize = (ULONGLONG)fileData1.nFileSizeHigh << 32 | fileData1.nFileSizeLow;
        wstring fileSizeStr = to_wstring(fileSize) + L" bytes";

        WriteFile(myFileHandle_sumar, fileSizeStr.c_str(), static_cast<DWORD>(fileSizeStr.size() * sizeof(wchar_t)), &size1, NULL);
    }
    else {
        printf("Am o eroare la scrierea in sumar.txt: %d\n", GetLastError());
        CloseHandle(myFileHandle_sumar);
        return false;
    }

    const wchar_t* positionStr = L"\r\n";
    DWORD nextLine;
    WriteFile(myFileHandle_sumar, positionStr, static_cast<DWORD>(wcslen(positionStr) * sizeof(wchar_t)), &nextLine, NULL);

    WriteFile(myFileHandle_sumar, path2Str.c_str(), static_cast<DWORD>(path2Str.size() * sizeof(wchar_t)), &sizePath2, NULL);

    if (GetFileAttributesEx(path2, GetFileExInfoStandard, &fileData1)) {
        ULONGLONG fileSize = (ULONGLONG)fileData1.nFileSizeHigh << 32 | fileData1.nFileSizeLow;
        wstring fileSizeStr = to_wstring(fileSize) + L" bytes";

        WriteFile(myFileHandle_sumar, fileSizeStr.c_str(), static_cast<DWORD>(fileSizeStr.size() * sizeof(wchar_t)), &size2, NULL);
    }
    else {
        printf("Am o eroare la scrierea in sumar.txt: %d\n", GetLastError());
        CloseHandle(myFileHandle_sumar);
        return false;
    }
    WriteFile(myFileHandle_sumar, positionStr, static_cast<DWORD>(wcslen(positionStr) * sizeof(wchar_t)), &nextLine, NULL);

    WriteFile(myFileHandle_sumar, path3Str.c_str(), static_cast<DWORD>(path3Str.size() * sizeof(wchar_t)), &sizePath3, NULL);
    if (GetFileAttributesEx(path3, GetFileExInfoStandard, &fileData1)) {
        ULONGLONG fileSize = (ULONGLONG)fileData1.nFileSizeHigh << 32 | fileData1.nFileSizeLow;
        wstring fileSizeStr = to_wstring(fileSize) + L" bytes";

        WriteFile(myFileHandle_sumar, fileSizeStr.c_str(), static_cast<DWORD>(fileSizeStr.size() * sizeof(wchar_t)), &size3, NULL);
    }
    else {
        printf("Am o eroare la scrierea in sumar.txt: %d\n", GetLastError());
        CloseHandle(myFileHandle_sumar);
        return false;
    }

    CloseHandle(myFileHandle_sumar);

    return true;
}


bool create_files_in_InstalledSoft( wstring subKey, wstring outputPath) {
    HKEY hKey;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        printf("Eroare la folosirea cheii in create_files_in_InstalledSoft.\n");
        return false;
    }
 
    DWORD index = 0;
    WCHAR valueName[256];
    DWORD valueNameSize = MAX_PATH;

    while (RegEnumKeyExW(hKey, index, valueName, &valueNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
     
            wstring outputFilePath = outputPath + L"\\\\" + valueName + L".txt";
            HANDLE hFile = CreateFileW(outputFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (hFile == INVALID_HANDLE_VALUE) {
                printf("Am o eroare la creearea fisierului pentru cheie soft. :  %d\n", GetLastError());
                return 0;
                break;
            }
            CloseHandle(hFile);
            index++;
            valueNameSize = MAX_PATH;
        
    }
    
    RegCloseKey(hKey);
    return true;
}



int main() {

    const wchar_t* path1 = L"C:\\Facultate";
    const wchar_t* path2 = L"C:\\Facultate\\CSSO";
    const wchar_t* path3 = L"C:\\Facultate\\CSSO\\Laboratoare";
    const wchar_t* path4 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2";
    const wchar_t* path5 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\InstalledSoftware";
    const wchar_t* path6 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate";


    if (CreeareDirector1(path1) &&
        CreeareDirector1(path2) &&
        CreeareDirector1(path3) &&
        CreeareDirector1(path4) &&
        CreeareDirector1(path5) &&
        CreeareDirector1(path6)) {
        printf("Toate directoarele au fost create.\n");
    }
    else {
        printf("Toate directoarele nu au fost create.\n");
    }




    LPCWSTR outputPath_HKLM = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\HKLM.txt";
    LPCWSTR outputPath_HKCC = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\HKCC.txt";
    LPCWSTR outputPath_HKCU = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\HKCU.txt";
   

    //Apel functie aici

    if (get_subKeys_maxSubKey_lastWriteTime(HKEY_LOCAL_MACHINE, outputPath_HKLM) &&
        get_subKeys_maxSubKey_lastWriteTime(HKEY_CURRENT_CONFIG, outputPath_HKCC) &&
        get_subKeys_maxSubKey_lastWriteTime(HKEY_CURRENT_USER, outputPath_HKCU)) {
        printf("Informatiile despre registrii s-au scris.\n");
    }
    else {
        printf("Eroare la scrierea informatiilor despre registrii.\n");
    }

   
    if (write_in_sumar(outputPath_HKLM, outputPath_HKCC , outputPath_HKCU)) {
        printf("Informatiile s-au scris in sumar.txt.\n");

    }
    else {
        printf("Eroare la scrierea informatiilor in sumar.txt.\n");
    }
  

  

    wstring subKey = L"Software";
    wstring outputPath_InstalledSoft = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\InstalledSoftware";
    if (create_files_in_InstalledSoft(subKey, outputPath_InstalledSoft)) {
        printf("S-au creeat cu succes fisiere in InstalledSoftware.\n");
    }
    else {
        printf("Nu s-au creeat cu succes fisiere in InstalledSoftware.\n");
    }
    
    printf("Verifica rezultatele!");


    return 0;
}
