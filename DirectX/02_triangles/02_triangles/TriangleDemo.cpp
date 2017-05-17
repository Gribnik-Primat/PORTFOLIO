/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    Triangle Demo - Demo used to draw a 2D triangle to the screen.
*/


#include"TriangleDemo.h"


struct VertexPos // структура для одной точки в пространстве
{
    float pos[3];
};


TriangleDemo::TriangleDemo( ) : solidColorVS_( 0 ), solidColorPS_( 0 ),
                                inputLayout_( 0 ), vertexBuffer_( 0 )
{

}


TriangleDemo::~TriangleDemo( )
{

}


bool TriangleDemo::LoadContent() // загружаем контент в демо сцену
{
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileShader( L"SolidGreenColor.fx", "VS_Main", "vs_4_0", &vsBuffer); // компилируем вершинный шейдер

	if (compileResult == false)
	{
		MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
		return false;
	}

	HRESULT d3dResult;

	d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), 0, &solidColorVS_);//создаем вершинный шейдер

	if (FAILED(d3dResult))
	{
		if (vsBuffer)
			vsBuffer->Release();

		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] = //создание цвета
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	d3dResult = d3dDevice_->CreateInputLayout(solidColorLayout, totalLayoutElements,
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout_);

	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	ID3DBlob* psBuffer = 0;

	compileResult = CompileShader(L"SolidGreenColor.fx", "PS_Main", "ps_4_0", &psBuffer);//компилируем пиксельный шейдер

	if (compileResult == false)
	{
		MessageBox(0, "Error loading pixel shader!", "Compile Error", MB_OK);
		return false;
	}

	d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &solidColorPS_);//создаем пиксельный шейдер

	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	VertexPos vertices[] = //да, у нас пока 2d но третья координата-задел на будущее
	{
		{0.5f,  0.5f, 0.5f},
		{0.5f, -0.5f, 0.5f },
		{-0.5f, -0.5f, 0.5f}
    };

    D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexPos ) * 3;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory( &resourceData, sizeof( resourceData ) );
    resourceData.pSysMem = vertices;

    d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBuffer_ );

    if( FAILED( d3dResult ) )
    {
        return false;
    }

    return true;
}


void TriangleDemo::UnloadContent( ) // освобождение сцены
{
    if( solidColorVS_ ) solidColorVS_->Release( );
    if( solidColorPS_ ) solidColorPS_->Release( );
    if( inputLayout_ ) inputLayout_->Release( );
    if( vertexBuffer_ ) vertexBuffer_->Release( );

    solidColorVS_ = 0;
    solidColorPS_ = 0;
    inputLayout_ = 0;
    vertexBuffer_ = 0;
}


void TriangleDemo::Update( float dt )
{
    // Nothing to update
}


void TriangleDemo::Render( )
{
    if( d3dContext_ == 0 )
        return;

    float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };// цвет для задника
    d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );

    unsigned int stride = sizeof( VertexPos );
    unsigned int offset = 0;

    d3dContext_->IASetInputLayout( inputLayout_ );
    d3dContext_->IASetVertexBuffers( 0, 1, &vertexBuffer_, &stride, &offset );
    d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );//выбираем режим рисования - Triangle_list - три точки-треугольник 

    d3dContext_->VSSetShader( solidColorVS_, 0, 0 );//устанавливаем вершинный и пиксельный шейдер
    d3dContext_->PSSetShader( solidColorPS_, 0, 0 );
    d3dContext_->Draw( 3, 0 );//рисуем наши точки

    swapChain_->Present( 0, 0 );//меняем кадры местами
}