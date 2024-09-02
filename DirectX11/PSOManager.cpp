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
    CreatePositionOnlyPSO();
    CreateStatic();
    CreateSkeletal();
    CreateRenderers();
}

PSOManager::~PSOManager()
{
}

void PSOManager::CreatePositionOnlyPSO()
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
    ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

    ComPtr<ID3DBlob> VertexShaderByteCode;
    ComPtr<ID3DBlob> DebugObjectPSByteCode;
    ComPtr<ID3DBlob> PickingIDPSByteCode;
    ComPtr<ID3DBlob> ErrorByteCode;


    UINT CompileFlags = NULL;
#if defined(_DEBUG)
    CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // =======================================================
    //              R8G8B8A8_BoundingComponent_Solid PSO
    // =======================================================
    ComPtr<ID3D11VertexShader>  PositionOnlyPathVS;
    ComPtr<ID3D11PixelShader>   BoundingComponentPS;
    ComPtr<ID3D11PixelShader>   PickingIDPS;
    ComPtr<ID3D11InputLayout>   PostionOnlyInputLayout;

    AssertIfFailed(D3DCompileFromFile(L"./Shaders/PositionOnlyPathVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", CompileFlags, 0, VertexShaderByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf()));
    AssertIfFailed(D3DCompileFromFile(L"./Shaders/DebugObjectPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", CompileFlags, 0, DebugObjectPSByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf()));
    AssertIfFailed(D3DCompileFromFile(L"./Shaders/PickingIDPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", CompileFlags, 0, PickingIDPSByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf()));

    D3D11_INPUT_ELEMENT_DESC InputElementDescs[1] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    Device->CreateVertexShader(VertexShaderByteCode->GetBufferPointer(), VertexShaderByteCode->GetBufferSize(), NULL, PositionOnlyPathVS.GetAddressOf());
    Device->CreatePixelShader(DebugObjectPSByteCode->GetBufferPointer(), DebugObjectPSByteCode->GetBufferSize(), NULL, BoundingComponentPS.GetAddressOf());
    Device->CreatePixelShader(PickingIDPSByteCode->GetBufferPointer(), PickingIDPSByteCode->GetBufferSize(), NULL, PickingIDPS.GetAddressOf());
    Device->CreateInputLayout(InputElementDescs, GetArraySize(InputElementDescs), VertexShaderByteCode->GetBufferPointer(), VertexShaderByteCode->GetBufferSize(), PostionOnlyInputLayout.GetAddressOf());

    DXGI_SAMPLE_DESC SampleDesc;
    if (FAILED(Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &SampleDesc.Quality)))
    {
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;
    }
    else
    {
        SampleDesc.Count = 4;
        SampleDesc.Quality = SampleDesc.Quality - 1;
    }

    ComPtr<ID3D11RasterizerState> PositionOnlySolidRasterizerState;
    D3D11_RASTERIZER_DESC RasterizerStateDesc;
    AutoZeroMemory(RasterizerStateDesc);
    RasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
    RasterizerStateDesc.CullMode = D3D11_CULL_BACK;
    RasterizerStateDesc.FrontCounterClockwise = FALSE;
    RasterizerStateDesc.DepthClipEnable = TRUE;
    RasterizerStateDesc.MultisampleEnable = SampleDesc.Count > 1 ? TRUE : FALSE;
    RasterizerStateDesc.AntialiasedLineEnable = FALSE;
    Device->CreateRasterizerState(&RasterizerStateDesc, PositionOnlySolidRasterizerState.GetAddressOf());

    ComPtr<ID3D11DepthStencilState> PositionOnlyDepthStencilState;
    D3D11_DEPTH_STENCIL_DESC DepthStencilStateDesc;
    AutoZeroMemory(DepthStencilStateDesc);
    DepthStencilStateDesc.DepthEnable = TRUE;
    DepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    DepthStencilStateDesc.StencilEnable = FALSE;
    Device->CreateDepthStencilState(&DepthStencilStateDesc, PositionOnlyDepthStencilState.GetAddressOf());

    ComPtr<ID3D11BlendState> PositionOnlyBlendState;
    D3D11_BLEND_DESC BlendStateDesc;
    AutoZeroMemory(BlendStateDesc);
    BlendStateDesc.AlphaToCoverageEnable = FALSE;
    BlendStateDesc.IndependentBlendEnable = FALSE;
    D3D11_RENDER_TARGET_BLEND_DESC& RenderTargetBlendDesc = BlendStateDesc.RenderTarget[0];
    AutoZeroMemory(RenderTargetBlendDesc);
    RenderTargetBlendDesc.BlendEnable = FALSE;
    RenderTargetBlendDesc.SrcBlend = D3D11_BLEND_ONE;
    RenderTargetBlendDesc.DestBlend = D3D11_BLEND_ZERO;
    RenderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
    RenderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    RenderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    RenderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    RenderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    Device->CreateBlendState(&BlendStateDesc, PositionOnlyBlendState.GetAddressOf());

    const UINT SRVFormatCount = 1;
    DXGI_FORMAT SRVFormats[SRVFormatCount] = { DXGI_FORMAT_R8G8B8A8_UNORM };

    PSOObjects[R8G8B8A8_BoundingComponent_Solid] = make_unique<PSOObject>(
        DeviceContext,
        PostionOnlyInputLayout,
        PositionOnlyPathVS, 2, 0,
        BoundingComponentPS, 1, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        PositionOnlySolidRasterizerState,
        PositionOnlyDepthStencilState, NULL,
        PositionOnlyBlendState.Get()
    );

    // =======================================================
    //            R8G8B8A8_BoundingComponent_Wireframe PSO
    // =======================================================
    RasterizerStateDesc.FillMode = D3D11_FILL_WIREFRAME;
    ComPtr<ID3D11RasterizerState> PositionOnlyWireframeRasterizerState;
    Device->CreateRasterizerState(&RasterizerStateDesc, PositionOnlyWireframeRasterizerState.GetAddressOf());

    PSOObjects[R8G8B8A8_BoundingComponent_Wireframe] = make_unique<PSOObject>(
        DeviceContext,
        PostionOnlyInputLayout,
        PositionOnlyPathVS, 2, 0,
        BoundingComponentPS, 1, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        PositionOnlyWireframeRasterizerState,
        PositionOnlyDepthStencilState, NULL,
        PositionOnlyBlendState.Get()
    );

    // =======================================================
    //            R8G8B8A8_PickingID_Solid PSO
    // =======================================================
    SampleDesc.Count = 1;
    SampleDesc.Quality = 0;

    PSOObjects[R8G8B8A8_Picking_ID_Solid] = make_unique<PSOObject>(
        DeviceContext,
        PostionOnlyInputLayout,
        PositionOnlyPathVS, 2, 0,
        PickingIDPS, 1, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        PositionOnlySolidRasterizerState,
        PositionOnlyDepthStencilState, NULL,
        PositionOnlyBlendState.Get()
    );

    // =======================================================
    //            R8G8B8A8_PickingID_Wireframe PSO
    // =======================================================
    PSOObjects[R8G8B8A8_Picking_ID_Wireframe] = make_unique<PSOObject>(
        DeviceContext,
        PostionOnlyInputLayout,
        PositionOnlyPathVS, 2, 0,
        PickingIDPS, 1, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        PositionOnlyWireframeRasterizerState,
        PositionOnlyDepthStencilState, NULL,
        PositionOnlyBlendState.Get()
    );
}

void PSOManager::CreateStatic()
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
    ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

    ComPtr<ID3DBlob> VertexShaderByteCode;
    ComPtr<ID3DBlob> PixelShaderByteCode;
    ComPtr<ID3DBlob> ErrorByteCode;

    UINT CompileFlags = NULL;
#if defined(_DEBUG)
    CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // =======================================================
    //              R8G8B8A8_Static_Solid PSO
    // =======================================================
    ComPtr<ID3D11VertexShader> StaticMeshVS;
    ComPtr<ID3D11PixelShader> StaticMeshPS;
    ComPtr<ID3D11InputLayout> StaticMeshInputLayout;

    AssertIfFailed(D3DCompileFromFile(L"./Shaders/StaticVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", CompileFlags, 0, VertexShaderByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf()));
    AssertIfFailed(D3DCompileFromFile(L"./Shaders/StaticPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", CompileFlags, 0, PixelShaderByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf()));

    D3D11_INPUT_ELEMENT_DESC InputElementDescs[5] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    Device->CreateVertexShader(VertexShaderByteCode->GetBufferPointer(), VertexShaderByteCode->GetBufferSize(), NULL, StaticMeshVS.GetAddressOf());
    Device->CreatePixelShader(PixelShaderByteCode->GetBufferPointer(), PixelShaderByteCode->GetBufferSize(), NULL, StaticMeshPS.GetAddressOf());
    Device->CreateInputLayout(InputElementDescs, GetArraySize(InputElementDescs), VertexShaderByteCode->GetBufferPointer(), VertexShaderByteCode->GetBufferSize(), StaticMeshInputLayout.GetAddressOf());

    DXGI_SAMPLE_DESC SampleDesc;
    if (FAILED(Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &SampleDesc.Quality)))
    {
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;
    }
    else
    {
        SampleDesc.Count = 4;
        SampleDesc.Quality = SampleDesc.Quality - 1;
    }

    ComPtr<ID3D11RasterizerState> StaticSolidRasterizerState;
    D3D11_RASTERIZER_DESC RasterizerStateDesc;
    AutoZeroMemory(RasterizerStateDesc);
    RasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
    RasterizerStateDesc.CullMode = D3D11_CULL_BACK;
    RasterizerStateDesc.FrontCounterClockwise = FALSE;
    RasterizerStateDesc.DepthClipEnable = TRUE;
    RasterizerStateDesc.MultisampleEnable = SampleDesc.Count > 1 ? TRUE : FALSE;
    RasterizerStateDesc.AntialiasedLineEnable = FALSE;
    Device->CreateRasterizerState(&RasterizerStateDesc, StaticSolidRasterizerState.GetAddressOf());

    ComPtr<ID3D11DepthStencilState> StaticDepthStencilState;
    D3D11_DEPTH_STENCIL_DESC DepthStencilStateDesc;
    AutoZeroMemory(DepthStencilStateDesc);
    DepthStencilStateDesc.DepthEnable = TRUE;
    DepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    DepthStencilStateDesc.StencilEnable = FALSE;
    Device->CreateDepthStencilState(&DepthStencilStateDesc, StaticDepthStencilState.GetAddressOf());

    ComPtr<ID3D11BlendState> StaticBlendState;
    D3D11_BLEND_DESC BlendStateDesc;
    AutoZeroMemory(BlendStateDesc);
    BlendStateDesc.AlphaToCoverageEnable = FALSE;
    BlendStateDesc.IndependentBlendEnable = FALSE;
    D3D11_RENDER_TARGET_BLEND_DESC& RenderTargetBlendDesc = BlendStateDesc.RenderTarget[0];
    AutoZeroMemory(RenderTargetBlendDesc);
    RenderTargetBlendDesc.BlendEnable = FALSE;
    RenderTargetBlendDesc.SrcBlend = D3D11_BLEND_ONE;
    RenderTargetBlendDesc.DestBlend = D3D11_BLEND_ZERO;
    RenderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
    RenderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    RenderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    RenderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    RenderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    Device->CreateBlendState(&BlendStateDesc, StaticBlendState.GetAddressOf());

    const UINT SRVFormatCount = 1;
    DXGI_FORMAT SRVFormats[SRVFormatCount] = { DXGI_FORMAT_R8G8B8A8_UNORM };

    PSOObjects[R8G8B8A8_Static_Solid] = make_unique<PSOObject>(
        DeviceContext,
        StaticMeshInputLayout,
        StaticMeshVS, 2, 0,
        StaticMeshPS, 0, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        StaticSolidRasterizerState,
        StaticDepthStencilState, NULL,
        StaticBlendState.Get()
    );

   
    // =======================================================
    //            R8G8B8A8_Static_Wireframe PSO
    // =======================================================
    RasterizerStateDesc.FillMode = D3D11_FILL_WIREFRAME;
    ComPtr<ID3D11RasterizerState> StaticWireframeRasterizerState;
    Device->CreateRasterizerState(&RasterizerStateDesc, StaticWireframeRasterizerState.GetAddressOf());


    PSOObjects[R8G8B8A8_Static_Wireframe] = make_unique<PSOObject>(
        DeviceContext,
        StaticMeshInputLayout,
        StaticMeshVS, 2, 0,
        StaticMeshPS, 0, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        StaticWireframeRasterizerState,
        StaticDepthStencilState, NULL,
        StaticBlendState.Get()
    );

}

void PSOManager::CreateSkeletal()
{
    ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
    ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

    ComPtr<ID3DBlob> VertexShaderByteCode;
    ComPtr<ID3DBlob> PixelShaderByteCode;
    ComPtr<ID3DBlob> ErrorByteCode;


    UINT CompileFlags = NULL;
#if defined(_DEBUG)
    CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // =======================================================
    //              R8G8B8A8_Skeletal_Solid PSO
    // =======================================================
    ComPtr<ID3D11VertexShader> SkeletalMeshVS;
    ComPtr<ID3D11PixelShader> SkeletalMeshPS;
    ComPtr<ID3D11InputLayout> SkeletalMeshInputLayout;

    AssertIfFailed(D3DCompileFromFile(L"./Shaders/SkeletalVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", CompileFlags, 0, VertexShaderByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf()));
    AssertIfFailed(D3DCompileFromFile(L"./Shaders/SkeletalPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", CompileFlags, 0, PixelShaderByteCode.GetAddressOf(), ErrorByteCode.GetAddressOf()));

    D3D11_INPUT_ELEMENT_DESC InputElementDescs[7] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 6, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    Device->CreateVertexShader(VertexShaderByteCode->GetBufferPointer(), VertexShaderByteCode->GetBufferSize(), NULL, SkeletalMeshVS.GetAddressOf());
    Device->CreatePixelShader(PixelShaderByteCode->GetBufferPointer(), PixelShaderByteCode->GetBufferSize(), NULL, SkeletalMeshPS.GetAddressOf());
    Device->CreateInputLayout(InputElementDescs, GetArraySize(InputElementDescs), VertexShaderByteCode->GetBufferPointer(), VertexShaderByteCode->GetBufferSize(), SkeletalMeshInputLayout.GetAddressOf());


    DXGI_SAMPLE_DESC SampleDesc;
    if (FAILED(Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &SampleDesc.Quality)))
    {
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;
    }
    else
    {
        SampleDesc.Count = 4;
        SampleDesc.Quality = SampleDesc.Quality - 1;
    }

    ComPtr<ID3D11RasterizerState> SkeletalSolidRasterizerState;
    D3D11_RASTERIZER_DESC RasterizerStateDesc;
    AutoZeroMemory(RasterizerStateDesc);
    RasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
    RasterizerStateDesc.CullMode = D3D11_CULL_BACK;
    RasterizerStateDesc.FrontCounterClockwise = FALSE;
    RasterizerStateDesc.DepthClipEnable = TRUE;
    RasterizerStateDesc.MultisampleEnable = SampleDesc.Count > 1 ? TRUE : FALSE;
    RasterizerStateDesc.AntialiasedLineEnable = FALSE;
    Device->CreateRasterizerState(&RasterizerStateDesc, SkeletalSolidRasterizerState.GetAddressOf());

    ComPtr<ID3D11DepthStencilState> SkeletalDepthStencilState;
    D3D11_DEPTH_STENCIL_DESC DepthStencilStateDesc;
    AutoZeroMemory(DepthStencilStateDesc);
    DepthStencilStateDesc.DepthEnable = TRUE;
    DepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    DepthStencilStateDesc.StencilEnable = FALSE;
    Device->CreateDepthStencilState(&DepthStencilStateDesc, SkeletalDepthStencilState.GetAddressOf());

    ComPtr<ID3D11BlendState> SkeletalBlendState;
    D3D11_BLEND_DESC BlendStateDesc;
        AutoZeroMemory(BlendStateDesc);
    BlendStateDesc.AlphaToCoverageEnable = FALSE;
    BlendStateDesc.IndependentBlendEnable = FALSE;
    D3D11_RENDER_TARGET_BLEND_DESC& RenderTargetBlendDesc = BlendStateDesc.RenderTarget[0];
    AutoZeroMemory(RenderTargetBlendDesc);
    RenderTargetBlendDesc.BlendEnable = FALSE;
    RenderTargetBlendDesc.SrcBlend = D3D11_BLEND_ONE;
    RenderTargetBlendDesc.DestBlend = D3D11_BLEND_ZERO;
    RenderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
    RenderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    RenderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    RenderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    RenderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    Device->CreateBlendState(&BlendStateDesc, SkeletalBlendState.GetAddressOf());

    const UINT SRVFormatCount = 1;
    DXGI_FORMAT SRVFormats[SRVFormatCount] = { DXGI_FORMAT_R8G8B8A8_UNORM };

    PSOObjects[R8G8B8A8_Skeletal_Solid] = make_unique<PSOObject>(
        DeviceContext,
        SkeletalMeshInputLayout,
        SkeletalMeshVS, 2, 0,
        SkeletalMeshPS, 0, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        SkeletalSolidRasterizerState,
        SkeletalDepthStencilState, NULL,
        SkeletalBlendState.Get()
    );

    // =======================================================
    //            R8G8B8A8_Skeletal_Wireframe PSO
    // =======================================================
    RasterizerStateDesc.FillMode = D3D11_FILL_WIREFRAME;
    ComPtr<ID3D11RasterizerState> SkeletalWireframeRasterizerState;
    Device->CreateRasterizerState(&RasterizerStateDesc, SkeletalWireframeRasterizerState.GetAddressOf());

    PSOObjects[R8G8B8A8_Skeletal_Wireframe] = make_unique<PSOObject>(
        DeviceContext,
        SkeletalMeshInputLayout,
        SkeletalMeshVS, 2, 0,
        SkeletalMeshPS, 0, 0,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        SRVFormatCount, SRVFormats,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        SampleDesc,
        SkeletalWireframeRasterizerState,
        SkeletalDepthStencilState, NULL,
        SkeletalBlendState.Get()
    );
}

void PSOManager::CreateRenderers()
{
    Renderers[R8G8B8A8_BoundingComponent_Solid] = make_unique<BoundingObjectRenderer>(GetPSOObject(R8G8B8A8_BoundingComponent_Solid));
    Renderers[R8G8B8A8_BoundingComponent_Wireframe] = make_unique<BoundingObjectRenderer>(GetPSOObject(R8G8B8A8_BoundingComponent_Wireframe));
    Renderers[R8G8B8A8_Picking_ID_Solid] = make_unique<PickingIDSolidRenderer>(GetPSOObject(R8G8B8A8_Picking_ID_Solid));
    Renderers[R8G8B8A8_Picking_ID_Wireframe] = make_unique<PickingIDWireframeRenderer>(GetPSOObject(R8G8B8A8_Picking_ID_Wireframe));
    Renderers[R8G8B8A8_Static_Solid] = make_unique<MeshObjectRenderer>(GetPSOObject(R8G8B8A8_Static_Solid));
    Renderers[R8G8B8A8_Static_Wireframe] = make_unique<MeshObjectRenderer>(GetPSOObject(R8G8B8A8_Static_Wireframe));
    Renderers[R8G8B8A8_Skeletal_Solid] = make_unique<MeshObjectRenderer>(GetPSOObject(R8G8B8A8_Skeletal_Solid));
    Renderers[R8G8B8A8_Skeletal_Wireframe] = make_unique<MeshObjectRenderer>(GetPSOObject(R8G8B8A8_Skeletal_Wireframe));
}
