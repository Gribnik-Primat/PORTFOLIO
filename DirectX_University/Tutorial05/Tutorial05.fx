//--------------------------------------------------------------------------------------
// File: Tutorial05.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 CameraPosition;

	float4 vLightColor[2];
	float4 vOutputColor;
	float4 vLightRadius[2];
	float4 vLightPos[2];
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
	float3 Norm : NORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
	float3 Norm : TEXCOORD0;
	float4 worldPos : POSITION;
};


PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.worldPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Norm = (float3)normalize(mul(float4(input.Norm, 0), World));

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 finalColor = 1;
	// InIntensity ratios of diffused light
	float Kd[2] = {0.8 , 1.4};
	// InIntensity ratios of specular light
	float Ks[2] = { 1, 1 };
	float ns = 8;
	// Constnant intenstity of light indeoendent from raduius;
	float L_ambient[2] = { 0 , 0 };

	for (int i = 0; i < 2; i++)
	{
		// Vector of normal to surface
		float3 N = input.Norm;
		// Vector point-to-source
		float3 L = (float3) (vLightPos[i] - input.worldPos);
		// Reflectd L
		float3 R = 2 * N * dot(normalize(L), normalize(N)) - L;
		// Vector directed towards the observer
		float3 V = (float3)(CameraPosition - input.worldPos);
		float d = length(L);

		// Check that point in lightning radius
		if (d > vLightRadius[i][0])
		{
			finalColor += saturate(L_ambient[i] * vLightColor[i]);
			continue;
		}

		// Normalization of all vectors
		N = normalize(N);
		L = normalize(L);
		R = normalize(R);
		V = normalize(V);

		float tmp = dot(L, N);

		//Check that light Vector and Normal to surface are co-directed
		if (tmp > 0)
		{
			//Calculating diffused component of light
			float L_diffused = Kd[i] * tmp;

			//Calculating specular component of light
			float tmp2 = abs(dot(R, V));
			float L_specular = (Ks[i] * pow(tmp2, ns));
			/*float3 h = normalize(normalize(CameraPosition - input.worldPos) + L);
			float L_specular = pow(saturate(dot(h, N)), ns);*/
			// Final calculating of light for each lighr source;
			float Light = L_ambient[i] + L_diffused + L_specular;
			finalColor += saturate(Light * vLightColor[i]);
		}
	}
	finalColor.a = 1;
	return finalColor;
}


//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid(PS_INPUT input) : SV_Target
{
	return vOutputColor;
}
