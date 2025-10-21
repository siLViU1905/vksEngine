#include "FileExplorer.h"

#include <iostream>
#include <Windows.h>
#include <ShObjIdl.h>
#include <print>
#include <thread>

namespace vks_engine
{
    FileExplorer::FileExplorer()
    {
    }

    void FileExplorer::onFileSelected(std::function<void(const std::vector<std::string> &)> function)
    {
        m_FileSelectedCallback = std::move(function);
    }

    void FileExplorer::open()
    {
        std::thread([this]()
        {
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

            if (SUCCEEDED(hr))
            {
                IFileOpenDialog *pFileOpen;

                hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog,
                                      reinterpret_cast<void **>(&pFileOpen));

                if (SUCCEEDED(hr))
                {
                    DWORD dwOptions;
                    hr = pFileOpen->GetOptions(&dwOptions);
                    if (SUCCEEDED(hr))
                        hr = pFileOpen->SetOptions(dwOptions | FOS_ALLOWMULTISELECT);

                    hr = pFileOpen->Show(NULL);

                    if (SUCCEEDED(hr))
                    {
                        IShellItemArray *pItemArray;
                        hr = pFileOpen->GetResults(&pItemArray);
                        if (SUCCEEDED(hr))
                        {
                            DWORD dwCount;
                            hr = pItemArray->GetCount(&dwCount);

                            if (SUCCEEDED(hr))
                            {
                                std::vector<std::string> paths;
                                for (DWORD i = 0; i < dwCount; ++i)
                                {
                                    IShellItem *pItem;
                                    hr = pItemArray->GetItemAt(i, &pItem);
                                    if (SUCCEEDED(hr))
                                    {
                                        PWSTR pszFilePath;
                                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                        if (SUCCEEDED(hr))
                                        {
                                            try
                                            {
                                                int bufferSize = WideCharToMultiByte(
                                                    CP_UTF8, 0, pszFilePath, -1, NULL, 0, NULL, NULL);
                                                if (bufferSize > 0)
                                                {
                                                    std::string pathString(bufferSize - 1, 0);
                                                    WideCharToMultiByte(
                                                        CP_UTF8, 0, pszFilePath, -1, &pathString[0], bufferSize,
                                                        NULL,
                                                        NULL);
                                                    if (!pathString.empty())
                                                        paths.push_back(pathString);
                                                }
                                            } catch (...)
                                            {
                                                std::print("Failed to convert user input to std::string");
                                            }

                                            CoTaskMemFree(pszFilePath);
                                        }
                                        pItem->Release();
                                    }
                                }
                                if (!paths.empty() && m_FileSelectedCallback)
                                    this->m_FileSelectedCallback(paths);
                            }
                            pItemArray->Release();
                        }
                    }
                    pFileOpen->Release();
                }
                CoUninitialize();
            }
        }).detach();
    }
}
