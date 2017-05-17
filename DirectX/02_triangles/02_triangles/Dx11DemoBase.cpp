/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    DX11DemoBase Class - Base class for all demos to derive from.
*/


#include"Dx11DemoBase.h"
#include<D3Dcompiler.h>


Dx11DemoBase::Dx11DemoBase( ) : driverType_( D3D_DRIVER_TYPE_NULL ), featureLevel_( D3D_FEATURE_LEVEL_11_0 ),
                                d3dDevice_( 0 ), d3dContext_( 0 ), swapChain_( 0 ), backBufferTarget_( 0 )
{

}


Dx11DemoBase::~Dx11DemoBase( )
{
    Shutdown( );
}


bool Dx11DemoBase::Initialize( HINSTANCE hInstance, HWND hwnd ) // инициализация работы с директом
{
    hInstance_ = hInstance;
    hwnd_ = hwnd;

    RECT dimensions;
    GetClientRect( hwnd, &dimensions );

    unsigned int width = dimensions.right - dimensions.left;
    unsigned int height = dimensions.bottom - dimensions.top; // получаем ширину и высоту окна

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
    };

    unsigned int totalDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };// стандартные параметры для инициализации directx 

    unsigned int totalFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.Windowed = true;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0; // инициализация swapchain

    unsigned int creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT result;
    unsigned int driver = 0;

    for( driver = 0; driver < totalDriverTypes; ++driver )
    {
        result = D3D11CreateDeviceAndSwapChain( 0, driverTypes[driver], 0, creationFlags,
                                                featureLevels, totalFeatureLevels,
                                                D3D11_SDK_VERSION, &swapChainDesc, &swapChain_,
                                                &d3dDevice_, &featureLevel_, &d3dContext_ );//создание device & swapchain

        if( SUCCEEDED( result ) )
        {
            driverType_ = driverTypes[driver];
            break;
        }
    }

    if( FAILED( result ) )
    {
        return false;
    }

    ID3D11Texture2D* backBufferTexture;

    result = swapChain_->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBufferTexture ); //даем "задний" кадр

    if( FAILED( result ) )
    {
        return false;
    }

    result = d3dDevice_->CreateRenderTargetView( backBufferTexture, 0, &backBufferTarget_ );

    if( backBufferTexture )
        backBufferTexture->Release( );

    if( FAILED( result ) )
    {
        return false;
    }

    d3dContext_->OMSetRenderTargets( 1, &backBufferTarget_, 0 );

    D3D11_VIEWPORT viewport;//инициализация вьюпорта
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    d3dContext_->RSSetViewports( 1, &viewport );//его создание

    return LoadContent( );
}

bool Dx11DemoBase::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob ** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG; // проверка на аргументы

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{//почитать в будущем про это
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob); //функия компиляции шейдера на лету
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return false;
	}

	*blob = shaderBlob;

	return true;
}



bool Dx11DemoBase::LoadContent( )
{
    // Override with demo specifics, if any...
    return true;
}


void Dx11DemoBase::UnloadContent( )
{
    // Override with demo specifics, if any...
}


void Dx11DemoBase::Shutdown( ) // освобождение демо
{
    UnloadContent( );

    if( backBufferTarget_ ) backBufferTarget_->Release( );
    if( swapChain_ ) swapChain_->Release( );
    if( d3dContext_ ) d3dContext_->Release( );
    if( d3dDevice_ ) d3dDevice_->Release( );    

    backBufferTarget_ = 0;
    swapChain_ = 0;
    d3dContext_ = 0;
    d3dDevice_ = 0;
}

