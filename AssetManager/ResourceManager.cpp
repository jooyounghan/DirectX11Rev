
#include "ResourceManager.h"
#include "BaseTextureAsset.h"
#include "StringHelper.h"

#include <format>

using namespace std;

BOOL ResourceManager::EnumResouceNameProc(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName, LONG_PTR lParam)
{
    vector<BaseTextureAsset*>* result = reinterpret_cast<vector<BaseTextureAsset*>*>(lParam);

    SResourceInfo resourceInfo = GetResourceInfo(hModule, lpszType, lpszName);
    if (IS_INTRESOURCE(lpszName))
    {
        const size_t resouceId = reinterpret_cast<size_t>(lpszName);
        result->emplace_back(new BaseTextureAsset(
            format("Resouce_{}", to_string(resouceId)).c_str(),
            resourceInfo.width, resourceInfo.height,
            resourceInfo.resourceData.data()
        ));
    }
    else 
    {
        result->emplace_back(new BaseTextureAsset(
            StringHelper::ConvertWStringToACP(lpszName), resourceInfo.width, resourceInfo.height,
            resourceInfo.resourceData.data()
        ));
    }
    return TRUE;
}


SResourceInfo ResourceManager::GetResourceInfo(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName)
{
    SResourceInfo resourceInfo = { 0, 0, {} };

    HRSRC hResource = FindResource(hModule, lpszName, lpszType);
    if (!hResource) return resourceInfo;

    HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
    if (!hLoadedResource) return resourceInfo;

    void* pResourceData = LockResource(hLoadedResource);
    if (!pResourceData) return resourceInfo;

    DWORD resourceSize = SizeofResource(hModule, hResource);
    if (resourceSize == 0) return resourceInfo;

    BITMAPINFO* pBitmapInfo = (BITMAPINFO*)pResourceData;
    int width = pBitmapInfo->bmiHeader.biWidth;
    int height = pBitmapInfo->bmiHeader.biHeight;
    

    uint8_t* pBits = reinterpret_cast<uint8_t*>(pResourceData) + pBitmapInfo->bmiHeader.biSize;

    constexpr size_t rgbChannelCount = 3;
    constexpr size_t rgbaChannelCount = 4;

    resourceInfo.width = width;
    resourceInfo.height = height;
    resourceInfo.resourceData.resize(width * height * rgbaChannelCount);

    for (int y = 0; y < height; ++y) 
    {
        for (int x = 0; x < width; ++x)
        {
            int index = y * width + x;
            uint8_t* pixel = pBits + index * rgbChannelCount;

            int rgbaIndex = ((height - y - 1) * width + x) * rgbaChannelCount;
            resourceInfo.resourceData[rgbaIndex] = pixel[0];
            resourceInfo.resourceData[rgbaIndex + 1] = pixel[1];
            resourceInfo.resourceData[rgbaIndex + 2] = pixel[2];
            resourceInfo.resourceData[rgbaIndex + 3] = 255;
        }
    }    
    return resourceInfo;
}


ResourceManager::ResourceManager(HMODULE hModule)
    : m_hModule(hModule ? hModule : GetModuleHandle(nullptr)) 
{
}

ResourceManager::~ResourceManager()
{
    for (BaseTextureAsset* loadedBaseTextureAssets : m_loadedBaseTextureAssets)
    {
        delete loadedBaseTextureAssets;
    }
}

std::vector<BaseTextureAsset*> ResourceManager::LoadBitmapResources()
{
    vector<BaseTextureAsset*> results;

    if (!EnumResourceNames(m_hModule, RT_BITMAP, EnumResouceNameProc, reinterpret_cast<LONG_PTR>(&results)))
    {
    }

    m_loadedBaseTextureAssets.insert(m_loadedBaseTextureAssets.end(), results.begin(), results.end());

    return results;
}