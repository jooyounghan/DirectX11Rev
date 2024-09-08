#include "PSOManager.h"
#include "DefineUtility.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "PSOObject.h"

#include "ARenderer.h"

#include "PickingIDSolidRenderer.h"
#include "PickingIDWireframeRenderer.h"

#include "BoundingObjectRenderer.h"
#include "MeshObjectRenderer.h"

#include <d3dcompiler.h>

using namespace std;
using namespace Microsoft::WRL;

PSOManager::PSOManager()
{
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

    const UINT SRVFormatCount = 1;
    DXGI_FORMAT SRVFormats[SRVFormatCount] = { DXGI_FORMAT_R8G8B8A8_UNORM };

#pragma region R8G8B8A8_BoundingComponent_Solid
    ComPtr<ID3D11VertexShader>  PositionOnlyPathVS;
    ComPtr<ID3D11InputLayout>   PostionOnlyInputLayout;
    D3D11_INPUT_ELEMENT_DESC OnlyPositionInputElementDesc[1] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    CreateVertexShader(L"./Shaders/PositionOnlyPathVS.hlsl", OnlyPositionInputElementDesc, 1, PositionOnlyPathVS, PostionOnlyInputLayout);

    ComPtr<ID3D11PixelShader>   BoundingComponentPS;
    CreatePixelShader(L"./Shaders/BoundingComponentPS.hlsl", BoundingComponentPS);

    PSOObjects[R8G8B8A8_BoundingComponent_Solid] = make_unique<PSOObject>(
        DeviceContext,
        PostionOnlyInputLayout,
        PositionOnlyPathVS, 2, 0,
        BoundingComponentPS, 1, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    );
#pragma endregion

#pragma region R8G8B8A8_BoundingComponent_Wireframe
    PSOObjects[R8G8B8A8_BoundingComponent_Wireframe] = make_unique<PSOObject>(
        DeviceContext,
        PostionOnlyInputLayout,
        PositionOnlyPathVS, 2, 0,
        BoundingComponentPS, 1, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackWireframeRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    );
#pragma endregion

#pragma region R8G8B8A8_PickingID_Solid
    ComPtr<ID3D11PixelShader> PickingIDPS;
    CreatePixelShader(L"./Shaders/PickingIDPS.hlsl", PickingIDPS);

    PSOObjects[R8G8B8A8_Picking_ID_Solid] = make_unique<PSOObject>(
        DeviceContext,
        PostionOnlyInputLayout,
        PositionOnlyPathVS, 2, 0,
        PickingIDPS, 1, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    );
#pragma endregion

#pragma region R8G8B8A8_Static_Solid
    ComPtr<ID3D11VertexShader> StaticMeshVS;
    ComPtr<ID3D11InputLayout> StaticMeshInputLayout;
    D3D11_INPUT_ELEMENT_DESC StaticMeshElementDescs[5] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    CreateVertexShader(L"./Shaders/StaticVS.hlsl", StaticMeshElementDescs, 5, StaticMeshVS, StaticMeshInputLayout);

    ComPtr<ID3D11PixelShader> StaticMeshPS;
    CreatePixelShader(L"./Shaders/StaticPS.hlsl", StaticMeshPS);

    PSOObjects[R8G8B8A8_Static_Solid] = make_unique<PSOObject>(
        DeviceContext,
        StaticMeshInputLayout,
        StaticMeshVS, 2, 0,
        StaticMeshPS, 0, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    );
#pragma endregion

#pragma region R8G8B8A8_Static_Wireframe
    PSOObjects[R8G8B8A8_Static_Wireframe] = make_unique<PSOObject>(
        DeviceContext,
        StaticMeshInputLayout,
        StaticMeshVS, 2, 0,
        StaticMeshPS, 0, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackWireframeRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    );
#pragma endregion

#pragma region R8G8B8A8_Skeletal_Solid
    ComPtr<ID3D11VertexShader> SkeletalMeshVS;
    ComPtr<ID3D11InputLayout> SkeletalMeshInputLayout;
    
    D3D11_INPUT_ELEMENT_DESC SkeletalMeshInputElementDescs[7] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 6, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    CreateVertexShader(L"./Shaders/SkeletalVS.hlsl", SkeletalMeshInputElementDescs, 7, SkeletalMeshVS, SkeletalMeshInputLayout);

    ComPtr<ID3D11PixelShader> SkeletalMeshPS;
    CreatePixelShader(L"./Shaders/SkeletalPS.hlsl", SkeletalMeshPS);

    PSOObjects[R8G8B8A8_Skeletal_Solid] = make_unique<PSOObject>(
        DeviceContext,
        SkeletalMeshInputLayout,
        SkeletalMeshVS, 2, 0,
        SkeletalMeshPS, 0, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackSolidRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    );
#pragma endregion

#pragma region R8G8B8A8_Skeletal_Wireframe
    PSOObjects[R8G8B8A8_Skeletal_Wireframe] = make_unique<PSOObject>(
        DeviceContext,
        SkeletalMeshInputLayout,
        SkeletalMeshVS, 2, 0,
        SkeletalMeshPS, 0, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        CullBackWireframeRS,
        DepthCompLessDSS, NULL,
        DisabledBS.Get(), SamplerStates
    );
#pragma endregion

    CreateRenderers();
}

PSOManager::~PSOManager()
{
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

void PSOManager::CreateRenderers()
{
    Renderers[R8G8B8A8_BoundingComponent_Solid] = make_unique<BoundingObjectRenderer>(GetPSOObject(R8G8B8A8_BoundingComponent_Solid));
    Renderers[R8G8B8A8_BoundingComponent_Wireframe] = make_unique<BoundingObjectRenderer>(GetPSOObject(R8G8B8A8_BoundingComponent_Wireframe));
    Renderers[R8G8B8A8_Picking_ID_Solid] = make_unique<PickingIDSolidRenderer>(GetPSOObject(R8G8B8A8_Picking_ID_Solid));
    Renderers[R8G8B8A8_Static_Solid] = make_unique<MeshObjectRenderer>(GetPSOObject(R8G8B8A8_Static_Solid));
    Renderers[R8G8B8A8_Static_Wireframe] = make_unique<MeshObjectRenderer>(GetPSOObject(R8G8B8A8_Static_Wireframe));
    Renderers[R8G8B8A8_Skeletal_Solid] = make_unique<MeshObjectRenderer>(GetPSOObject(R8G8B8A8_Skeletal_Solid));
    Renderers[R8G8B8A8_Skeletal_Wireframe] = make_unique<MeshObjectRenderer>(GetPSOObject(R8G8B8A8_Skeletal_Wireframe));
}
