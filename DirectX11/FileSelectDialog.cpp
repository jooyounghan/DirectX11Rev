#include "FileSelectDialog.h"

#include <windows.h>
#include <ShObjIdl.h>

FileSelectDialog::FileSelectDialog()
{

}

FileSelectDialog::~FileSelectDialog()
{
}

void FileSelectDialog::DoModal()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
    COINIT_DISABLE_OLE1DDE);
if (SUCCEEDED(hr))
{
    IFileOpenDialog* pFileOpen;

    COMDLG_FILTERSPEC ComDlgFS[3] = { {L"C++ code files", L"*.cpp;*.h;*.rc"},{L"Executable Files", L"*.exe;*.dll"}, {L"All Files",L"*.*"} };

    // Create the FileOpenDialog object.
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    if (SUCCEEDED(hr))
    {
        pFileOpen->SetFileTypes(3, ComDlgFS);

        // Show the Open dialog box.
        hr = pFileOpen->Show(NULL);

        // Get the file name from the dialog box.
        if (SUCCEEDED(hr))
        {
            IShellItem* pItem;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                // Display the file name to the user.
                if (SUCCEEDED(hr))
                {
                    MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
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
