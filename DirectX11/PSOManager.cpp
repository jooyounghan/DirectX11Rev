#include "PSOManager.h"
#include "DefineUtility.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

#include "PSOObject.h"

#include <d3dcompiler.h>

using namespace std;
using namespace Microsoft::WRL;

PSOManager::PSOManager()
{
    App::GPSOManager = this;

    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
    ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

    DXGI_SAMPLE_DESC SampleDesc;
    SampleDesc.Count = 1;
    SampleDesc.Quality = 0;

    ComPtr<ID3D11RasterizerState> CullBackSolidRS;
    ComPtr<ID3D11RasterizerState> CullBackWireframeRS;

    CreateRastersizerState(D3D11_FILL_SOLID, D3D11_CULL_BACK, SampleDesc, CullBackSolidRS);
    CreateRastersizerState(D3D11_FILL_WIREFRAME, D3D11_CULL_BACK, SampleDesc, CullBackWireframeRS);

    ComPtr<ID3D11DepthStencilState> DepthCompLessDSS;
    CreateDepthStencilState(TRUE, D3D11_COMPARISON_LESS, FALSE, DepthCompLessDSS);

    ComPtr<ID3D11BlendState> DisabledBS;
    CreateBlendState(FALSE, FALSE, DisabledBS, 0, FALSE,
        D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
        D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
        D3D11_COLOR_WRITE_ENABLE_ALL
    );

    ComPtr<ID3D11SamplerState> WrapSampler;
    ComPtr<ID3D11SamplerState> ClampSampler;

    CreateSamplerState(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR, WrapSampler);
    CreateSamplerState(D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR, WrapSampler);

    ComPtr<ID3D11SamplerState> WrapComparisonSampler;
    ComPtr<ID3D11SamplerState> ClampComparisonSampler;

    CreateSamplerState(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_LESS, D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, WrapComparisonSampler);
    CreateSamplerState(D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_COMPARISON_LESS, D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, ClampComparisonSampler);

    vector<ComPtr<ID3D11SamplerState>> SamplerStates{ WrapSampler, ClampSampler, WrapComparisonSampler, ClampComparisonSampler };


#pragma region EnvironmentActor_Solid
    DXGI_FORMAT SceneFormat[1] = { DXGI_FORMAT_R8G8B8A8_UNORM };

    vector<D3D11_INPUT_ELEMENT_DESC> EnvironmentActorInputElementDesc =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    ComPtr<ID3D11VertexShader>  EnvironmentActorVS;
    ComPtr<ID3D11PixelShader>   EnvironmentActorPS;
    ComPtr<ID3D11InputLayout>   EnvironmentActorInputLayout;

    CreateVertexShader(L"./Shaders/EnvironmentActorVS.hlsl", EnvironmentActorInputElementDesc.data(), 
        EnvironmentActorInputElementDesc.size(), EnvironmentActorVS, EnvironmentActorInputLayout);
    CreatePixelShader(L"./Shaders/EnvironmentActorPS.hlsl", EnvironmentActorPS);

    PSOObjects.emplace(EPSOType::Environment_Solid, make_unique<PSOObject>(
        DeviceContext,
        EnvironmentActorInputLayout,
        EnvironmentActorVS, 2, 0,
        nullptr, 0, 0,
        nullptr, 0, 0,
        EnvironmentActorPS, 1, 1,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        1, SceneFormat,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    ));
#pragma endregion

#pragma region Forward Shading

#pragma region Forward_Bounding_Wireframe
    vector<DXGI_FORMAT> SceneWithIDFormats = { DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM };

    vector<D3D11_INPUT_ELEMENT_DESC> BoundingObjectInputElementDesc =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    ComPtr<ID3D11VertexShader>  BoundingObjectVS;
    ComPtr<ID3D11PixelShader>   BoundingObjectPS;
    ComPtr<ID3D11InputLayout>   BoundingObjectInputLayout;

    CreateVertexShader(L"./Shaders/BoundingObjectVS.hlsl", BoundingObjectInputElementDesc.data(), 
        BoundingObjectInputElementDesc.size(), BoundingObjectVS, BoundingObjectInputLayout);
    CreatePixelShader(L"./Shaders/BoundingObjectPS.hlsl", BoundingObjectPS);

    PSOObjects.emplace(EPSOType::Forward_Bounding_Wireframe, make_unique<PSOObject>(
        DeviceContext,
        BoundingObjectInputLayout,
        BoundingObjectVS, 2, 0,
        nullptr, 0, 0,
        nullptr, 0, 0,
        BoundingObjectPS, 2, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SceneWithIDFormats.size(), SceneWithIDFormats.data(),
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackWireframeRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    ));
#pragma endregion

#pragma region Foward_Static_Solid
    ComPtr<ID3D11VertexShader> StaticMeshVS;
    ComPtr<ID3D11InputLayout> StaticMeshInputLayout;

    vector<D3D11_INPUT_ELEMENT_DESC> StaticMeshElementDescs =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    CreateVertexShader(L"./Shaders/StaticMeshObjectVS.hlsl", StaticMeshElementDescs.data(), StaticMeshElementDescs.size(),
        StaticMeshVS, StaticMeshInputLayout);

    ComPtr<ID3D11HullShader> MeshObjectHS;
    CreateHullShader(L"./Shaders/MeshObjectHS.hlsl", MeshObjectHS);

    ComPtr<ID3D11DomainShader> MeshObjectDS;
    CreateDomainShader(L"./Shaders/MeshObjectDS.hlsl", MeshObjectDS);

    ComPtr<ID3D11PixelShader> MeshObjectPS;
    CreatePixelShader(L"./Shaders/MeshObjectPS.hlsl", MeshObjectPS);

    PSOObjects.emplace(EPSOType::Foward_Static_Solid, make_unique<PSOObject>(
        DeviceContext,
        StaticMeshInputLayout,
        StaticMeshVS, 2, 0,
        MeshObjectHS, 0, 0,
        MeshObjectDS, 3, 1,
        MeshObjectPS, 3, 10,
        D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
        SceneWithIDFormats.size(), SceneWithIDFormats.data(),
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    ));
#pragma endregion

#pragma region Forward_Skeletal_Solid
    ComPtr<ID3D11VertexShader> SkeletalMeshVS;
    ComPtr<ID3D11InputLayout> SkeletalMeshInputLayout;

    vector<D3D11_INPUT_ELEMENT_DESC> SkeletalMeshInputElementDescs =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    CreateVertexShader(L"./Shaders/SkeletalMeshObjectVS.hlsl", SkeletalMeshInputElementDescs.data(), 
        SkeletalMeshInputElementDescs.size(), SkeletalMeshVS, SkeletalMeshInputLayout);

    PSOObjects.emplace(EPSOType::Forward_Skeletal_Solid, make_unique<PSOObject>(
        DeviceContext,
        SkeletalMeshInputLayout,
        SkeletalMeshVS, 2, 1,
        MeshObjectHS, 0, 0,
        MeshObjectDS, 3, 1,
        MeshObjectPS, 3, 10,
        D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
        SceneWithIDFormats.size(), SceneWithIDFormats.data(),
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    ));
#pragma endregion

#pragma endregion

#pragma region Deffered Shading

#pragma region Deffered_Bounding_Wireframe
    vector<DXGI_FORMAT> GBuffersWithIDFormat = { 
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM, /* Position, BaseColor*/ 
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM, /* Normal, AO_Metallic_Roughness*/
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM /* Normal, ID*/
    };

    ComPtr<ID3D11PixelShader> DefferedBoundingObjectPS;
    CreatePixelShader(L"./Shaders/DefferedBoundingObjectPS.hlsl", DefferedBoundingObjectPS);

    PSOObjects.emplace(EPSOType::Deffered_Bounding_Wireframe, make_unique<PSOObject>(
        DeviceContext,
        BoundingObjectInputLayout,
        BoundingObjectVS, 2, 0,
        nullptr, 0, 0,
        nullptr, 0, 0,
        DefferedBoundingObjectPS, 2, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        GBuffersWithIDFormat.size(), GBuffersWithIDFormat.data(),
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackWireframeRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    ));
#pragma endregion

#pragma region Deffered_Static_Solid
    ComPtr<ID3D11PixelShader> DefferedMeshObjectPS;
    CreatePixelShader(L"./Shaders/DefferedMeshObjectPS.hlsl", DefferedMeshObjectPS);

    PSOObjects.emplace(EPSOType::Deffered_Static_Solid, make_unique<PSOObject>(
        DeviceContext,
        StaticMeshInputLayout,
        StaticMeshVS, 2, 0,
        MeshObjectHS, 0, 0,
        MeshObjectDS, 3, 1,
        DefferedMeshObjectPS, 3, 10,
        D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
        GBuffersWithIDFormat.size(), GBuffersWithIDFormat.data(),
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    ));
#pragma endregion

#pragma region Deffered_Skeletal_Solid
    PSOObjects.emplace(EPSOType::Deffered_Skeletal_Solid, make_unique<PSOObject>(
        DeviceContext,
        SkeletalMeshInputLayout,
        SkeletalMeshVS, 2, 1,
        MeshObjectHS, 0, 0,
        MeshObjectDS, 3, 1,
        DefferedMeshObjectPS, 3, 10,
        D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
        GBuffersWithIDFormat.size(), GBuffersWithIDFormat.data(),
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    ));
#pragma endregion

#pragma endregion

#pragma region Resolver
    vector<DXGI_FORMAT> GBufferResolveSceneFormat = { DXGI_FORMAT_R8G8B8A8_UNORM };

    vector<D3D11_INPUT_ELEMENT_DESC> GBufferResolverElementDesc =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    ComPtr<ID3D11VertexShader>  GBufferResolveVS;
    ComPtr<ID3D11PixelShader>   GBufferResolvePS;
    ComPtr<ID3D11InputLayout>   GBufferResolveInputLayout;

    CreateVertexShader(L"./Shaders/GBufferResolveVS.hlsl", GBufferResolverElementDesc.data(), 
        GBufferResolverElementDesc.size(), GBufferResolveVS, GBufferResolveInputLayout);
    CreatePixelShader(L"./Shaders/GBufferResolvePS.hlsl", GBufferResolvePS);

    PSOObjects.emplace(EPSOType::GBuffer_Resolve, make_unique<PSOObject>(
        DeviceContext,
        GBufferResolveInputLayout,
        GBufferResolveVS, 0, 0,
        nullptr, 0, 0,
        nullptr, 0, 0,
        GBufferResolvePS, 1, 8,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        GBufferResolveSceneFormat.size(), GBufferResolveSceneFormat.data(),
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    ));
#pragma endregion
}

PSOManager::~PSOManager()
{
}

PSOObject* PSOManager::GetPSOObject(EPSOType PsoTypeIn) 
{ 
    if (PSOObjects.find(PsoTypeIn) != PSOObjects.end())
    {
        return PSOObjects[PsoTypeIn].get();
    }
    return nullptr;
}

void PSOManager::CreateVertexShader(
    const wchar_t* HlslFileName, 
    const D3D11_INPUT_ELEMENT_DESC* InputElementDescs, 
    const UINT& NumElements, 
    ComPtr<ID3D11VertexShader>& VertexShader,
    ComPtr<ID3D11InputLayout>& InputLayout
)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

    ComPtr<ID3DBlob> VertexShaderByteCode;
    ComPtr<ID3DBlob> ErrorByteCode;

    UINT CompileFlags = NULL;
#if defined(_DEBUG)
    CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    AssertIfFailed(D3DCompileFromFile(
        HlslFileName,
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, 
        "main", "vs_5_0", CompileFlags, 0,
        VertexShaderByteCode.GetAddressOf(), 
        ErrorByteCode.GetAddressOf())
    );

    Device->CreateVertexShader(
        VertexShaderByteCode->GetBufferPointer(), 
        VertexShaderByteCode->GetBufferSize(), 
        NULL, VertexShader.GetAddressOf()
    );

    Device->CreateInputLayout(
        InputElementDescs, NumElements, 
        VertexShaderByteCode->GetBufferPointer(), 
        VertexShaderByteCode->GetBufferSize(),
        InputLayout.GetAddressOf()
    );
}

void PSOManager::CreateHullShader(const wchar_t* HlslFileName, Microsoft::WRL::ComPtr<ID3D11HullShader>& HullShader)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

    ComPtr<ID3DBlob> HullShaderByteCode;
    ComPtr<ID3DBlob> ErrorByteCode;

    UINT CompileFlags = NULL;
#if defined(_DEBUG)
    CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    AssertIfFailed(D3DCompileFromFile(
        HlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "hs_5_0", CompileFlags, 0,
        HullShaderByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf())
    );

    Device->CreateHullShader(
        HullShaderByteCode->GetBufferPointer(),
        HullShaderByteCode->GetBufferSize(),
        NULL, HullShader.GetAddressOf()
    );
}

void PSOManager::CreateDomainShader(const wchar_t* HlslFileName, Microsoft::WRL::ComPtr<ID3D11DomainShader>& DomainShader)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

    ComPtr<ID3DBlob> DomainShaderByteCode;
    ComPtr<ID3DBlob> ErrorByteCode;

    UINT CompileFlags = NULL;
#if defined(_DEBUG)
    CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    AssertIfFailed(D3DCompileFromFile(
        HlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ds_5_0", CompileFlags, 0,
        DomainShaderByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf())
    );

    Device->CreateDomainShader(
        DomainShaderByteCode->GetBufferPointer(),
        DomainShaderByteCode->GetBufferSize(),
        NULL, DomainShader.GetAddressOf()
    );
}

void PSOManager::CreatePixelShader(
    const wchar_t* HlslFileName, 
    ComPtr<ID3D11PixelShader>& PixelShader
)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
    
    ComPtr<ID3DBlob> PixelShaderByteCode;
    ComPtr<ID3DBlob> ErrorByteCode;

    UINT CompileFlags = NULL;
#if defined(_DEBUG)
    CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    AssertIfFailed(D3DCompileFromFile(
        HlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, 
        "main", "ps_5_0", CompileFlags, 0, 
        PixelShaderByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf())
    );

    Device->CreatePixelShader(
        PixelShaderByteCode->GetBufferPointer(),
        PixelShaderByteCode->GetBufferSize(),
        NULL, PixelShader.GetAddressOf()
    );
}

void PSOManager::CreateRastersizerState(
    const D3D11_FILL_MODE& FillMode, 
    const D3D11_CULL_MODE& CullMode, 
    const DXGI_SAMPLE_DESC& SampleDesc,
    ComPtr<ID3D11RasterizerState>& RasterizerState
)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

    D3D11_RASTERIZER_DESC RasterizerStateDesc;
    AutoZeroMemory(RasterizerStateDesc);
    RasterizerStateDesc.FillMode = FillMode;
    RasterizerStateDesc.CullMode = CullMode;
    RasterizerStateDesc.FrontCounterClockwise = FALSE;
    RasterizerStateDesc.DepthClipEnable = TRUE;
    RasterizerStateDesc.MultisampleEnable = SampleDesc.Count > 1 ? TRUE : FALSE;
    RasterizerStateDesc.AntialiasedLineEnable = FALSE;
    Device->CreateRasterizerState(&RasterizerStateDesc, RasterizerState.GetAddressOf());
}

void PSOManager::CreateDepthStencilState(
    const BOOL& DepthEnable, 
    const D3D11_COMPARISON_FUNC& DepthComparisonFunc, 
    const BOOL& StencilEnable,
    ComPtr<ID3D11DepthStencilState>& DepthStencilState
)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

    D3D11_DEPTH_STENCIL_DESC DepthStencilStateDesc;
    AutoZeroMemory(DepthStencilStateDesc);
    DepthStencilStateDesc.DepthEnable = DepthEnable;
    DepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DepthStencilStateDesc.DepthFunc = DepthComparisonFunc;
    DepthStencilStateDesc.StencilEnable = StencilEnable;
    Device->CreateDepthStencilState(&DepthStencilStateDesc, DepthStencilState.GetAddressOf());
}

void PSOManager::CreateBlendState(
    const BOOL& AlphaToCoverageEnable, 
    const BOOL& IndependentBlendEnable,
    ComPtr<ID3D11BlendState>& BlendState,
    const size_t RenderTargetIndex, 
    const BOOL& BlendEnable, 
    const D3D11_BLEND& SrcBlend, 
    const D3D11_BLEND& DestBlend, 
    const D3D11_BLEND_OP& BlendOp,
    const D3D11_BLEND& SrcBlendAlpha, 
    const D3D11_BLEND& DestBlendAlpha, 
    const D3D11_BLEND_OP& BlendOpAlpha, 
    const D3D11_COLOR_WRITE_ENABLE& RenderTargetWriteMask
)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

    D3D11_BLEND_DESC BlendStateDesc;
    AutoZeroMemory(BlendStateDesc);
    BlendStateDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;
    BlendStateDesc.IndependentBlendEnable = IndependentBlendEnable;
    D3D11_RENDER_TARGET_BLEND_DESC& RenderTargetBlendDesc = BlendStateDesc.RenderTarget[RenderTargetIndex];
    AutoZeroMemory(RenderTargetBlendDesc);
    RenderTargetBlendDesc.BlendEnable = BlendEnable;
    RenderTargetBlendDesc.SrcBlend = SrcBlend;
    RenderTargetBlendDesc.DestBlend = DestBlend;
    RenderTargetBlendDesc.BlendOp = BlendOp;
    RenderTargetBlendDesc.SrcBlendAlpha = SrcBlendAlpha;
    RenderTargetBlendDesc.DestBlendAlpha = DestBlendAlpha;
    RenderTargetBlendDesc.BlendOpAlpha = BlendOpAlpha;
    RenderTargetBlendDesc.RenderTargetWriteMask = RenderTargetWriteMask;

    Device->CreateBlendState(&BlendStateDesc, BlendState.GetAddressOf());
}

template<typename ...Args>
inline void PSOManager::CreateBlendState(
    const BOOL& AlphaToCoverageEnable, 
    const BOOL& IndependentBlendEnable, 
    Microsoft::WRL::ComPtr<ID3D11BlendState>& BlendState, 
    const size_t RenderTargetIndex, 
    const BOOL& BlendEnable, 
    const D3D11_BLEND& SrcBlend, 
    const D3D11_BLEND& DestBlend, 
    const D3D11_BLEND_OP& BlendOp, 
    const D3D11_BLEND& SrcBlendAlpha, 
    const D3D11_BLEND& DestBlendAlpha, 
    const D3D11_BLEND_OP& BlendOpAlpha, 
    const D3D11_COLOR_WRITE_ENABLE& RenderTargetWriteMask, 
    Args ...args
)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

    D3D11_BLEND_DESC BlendStateDesc;
    AutoZeroMemory(BlendStateDesc);
    BlendStateDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;
    BlendStateDesc.IndependentBlendEnable = IndependentBlendEnable;
    D3D11_RENDER_TARGET_BLEND_DESC& RenderTargetBlendDesc = BlendStateDesc.RenderTarget[RenderTargetIndex];
    AutoZeroMemory(RenderTargetBlendDesc);
    RenderTargetBlendDesc.BlendEnable = BlendEnable;
    RenderTargetBlendDesc.SrcBlend = SrcBlend;
    RenderTargetBlendDesc.DestBlend = DestBlend;
    RenderTargetBlendDesc.BlendOp = BlendOp;
    RenderTargetBlendDesc.SrcBlendAlpha = SrcBlendAlpha;
    RenderTargetBlendDesc.DestBlendAlpha = DestBlendAlpha;
    RenderTargetBlendDesc.BlendOpAlpha = BlendOpAlpha;
    RenderTargetBlendDesc.RenderTargetWriteMask = RenderTargetWriteMask;

    return CreateBlendState(BlendStateDesc, args...);
}

template<typename ...Args>
void PSOManager::CreateBlendState(
    D3D11_BLEND_DESC& BlendStateDesc,
    ComPtr<ID3D11BlendState>& BlendState,
    const size_t RenderTargetIndex, 
    const BOOL& BlendEnable, 
    const D3D11_BLEND& SrcBlend, 
    const D3D11_BLEND& DestBlend, 
    const D3D11_BLEND_OP& BlendOp, 
    const D3D11_BLEND& SrcBlendAlpha, 
    const D3D11_BLEND& DestBlendAlpha, 
    const D3D11_BLEND_OP& BlendOpAlpha, 
    const D3D11_COLOR_WRITE_ENABLE& RenderTargetWriteMask, 
    Args ...args
)
{
    D3D11_RENDER_TARGET_BLEND_DESC& RenderTargetBlendDesc = BlendStateDesc.RenderTarget[RenderTargetIndex];
    AutoZeroMemory(RenderTargetBlendDesc);
    RenderTargetBlendDesc.BlendEnable = BlendEnable;
    RenderTargetBlendDesc.SrcBlend = SrcBlend;
    RenderTargetBlendDesc.DestBlend = DestBlend;
    RenderTargetBlendDesc.BlendOp = BlendOp;
    RenderTargetBlendDesc.SrcBlendAlpha = SrcBlendAlpha;
    RenderTargetBlendDesc.DestBlendAlpha = DestBlendAlpha;
    RenderTargetBlendDesc.BlendOpAlpha = BlendOpAlpha;
    RenderTargetBlendDesc.RenderTargetWriteMask = RenderTargetWriteMask;

    return CreateBlendState(BlendStateDesc, BlendState, args...);
}


void PSOManager::CreateBlendState(
    D3D11_BLEND_DESC& BlendStateDesc,
    ComPtr<ID3D11BlendState>& BlendState,
    const size_t RenderTargetIndex, 
    const BOOL& BlendEnable, 
    const D3D11_BLEND& SrcBlend, 
    const D3D11_BLEND& DestBlend, 
    const D3D11_BLEND_OP& BlendOp, 
    const D3D11_BLEND& SrcBlendAlpha, 
    const D3D11_BLEND& DestBlendAlpha, 
    const D3D11_BLEND_OP& BlendOpAlpha, 
    const D3D11_COLOR_WRITE_ENABLE& RenderTargetWriteMask
)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

    D3D11_RENDER_TARGET_BLEND_DESC& RenderTargetBlendDesc = BlendStateDesc.RenderTarget[RenderTargetIndex];
    AutoZeroMemory(RenderTargetBlendDesc);
    RenderTargetBlendDesc.BlendEnable = BlendEnable;
    RenderTargetBlendDesc.SrcBlend = SrcBlend;
    RenderTargetBlendDesc.DestBlend = DestBlend;
    RenderTargetBlendDesc.BlendOp = BlendOp;
    RenderTargetBlendDesc.SrcBlendAlpha = SrcBlendAlpha;
    RenderTargetBlendDesc.DestBlendAlpha = DestBlendAlpha;
    RenderTargetBlendDesc.BlendOpAlpha = BlendOpAlpha;
    RenderTargetBlendDesc.RenderTargetWriteMask = RenderTargetWriteMask;

    Device->CreateBlendState(&BlendStateDesc, BlendState.GetAddressOf());
}

void PSOManager::CreateSamplerState(
    const D3D11_TEXTURE_ADDRESS_MODE& TextureAddressMode, 
    const D3D11_COMPARISON_FUNC& ComparisonFunc,
    const D3D11_FILTER& Filter, 
    Microsoft::WRL::ComPtr<ID3D11SamplerState>& SamplerState
)
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

    D3D11_SAMPLER_DESC SamplerDesc;
    AutoZeroMemory(SamplerDesc);
    SamplerDesc.AddressU = TextureAddressMode;
    SamplerDesc.AddressV = TextureAddressMode;
    SamplerDesc.AddressW = TextureAddressMode;
    SamplerDesc.ComparisonFunc = ComparisonFunc;
    SamplerDesc.Filter = Filter;

    Device->CreateSamplerState(&SamplerDesc, SamplerState.GetAddressOf());
}