//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse1 : register( t0 );
Texture2D txDiffuse2 : register( t1 );
SamplerState samLinear : register( s0 );

cbuffer cbChangeOnResize : register( b0 )
{
    matrix Projection;
};

cbuffer cbChangesEveryFrame : register( b1 )
{
    matrix View;
    matrix World;
    float4 vMeshColor;
	float4 cameraposition;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Tex = input.Tex;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	  float4 camposmax = float4 (0.0,15.0,2.0,1.0);
	  float4 camposmin = float4 (0.0, 3.0, 2.0,1.0);
	  float1 maxdist = distance(camposmax , input.Pos);
	  float1 mindist = distance(camposmin, input.Pos);
	  float1 dist = distance(cameraposition, input.Pos);
	  float1 coeff = (maxdist - mindist) / dist;
      return txDiffuse1.Sample(samLinear, input.Tex) * (1.0 - coeff) + txDiffuse2.Sample(samLinear, input.Tex) * (coeff);
}


