#include "FileExplorer.h"

#include <iostream>
#include <Windows.h>
#include <ShObjIdl.h>
#include <print>

FileExplorer::FileExplorer()
{
}

void FileExplorer::open()
{
     HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            hr = pFileOpen->Show(NULL);

            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;

                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(hr))
                    {
                        try
                        {
                            int bufferSize = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, NULL, 0, NULL, NULL);
                            if (bufferSize > 0)
                            {
                                std::string narrowString(bufferSize - 1, 0);
                                WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &narrowString[0], bufferSize, NULL, NULL);
                                m_Path = narrowString;
                            }
                        }
                        catch(...)
                        {
                            std::print("Failed to convert user input to std::string");
                        }

                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
}
