//======================================================================
//
//	Tone Mapping Sample
//  by MJP
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================

#include "SampleFramework11/Shaders/PPIncludes.hlsl"

cbuffer PPConstants : register(b1)
{
    float BloomThreshold : packoffset(c0.x);
    float BloomMagnitude : packoffset(c0.y);
    float BloomBlurSigma : packoffset(c0.z);        
    float Tau : packoffset(c0.w);
    float TimeDelta : packoffset(c1.x);
    float ToneMapTechnique : packoffset(c1.y);
    float Exposure : packoffset(c1.z);
    float KeyValue : packoffset(c1.w);
    float AutoExposure : packoffset(c2.x);
    float WhiteLevel : packoffset(c2.y);    
    float ShoulderStrength : packoffset(c2.z);
    float LinearStrength : packoffset(c2.w);
    float LinearAngle : packoffset(c3.x);
    float ToeStrength : packoffset(c3.y);
    float ToeNumerator : packoffset(c3.z);
    float ToeDenominator : packoffset(c3.w);
    float LinearWhite : packoffset(c4.x);
    float LuminanceSaturation : packoffset(c4.y);
    float ToneMapMode : packoffset(c4.z);
    float LumMapMipLevel : packoffset(c4.w);
	float Bias : packoffset(c5.x);
};

// ------------------------------------------------------------------------------------------------
// Helper Functions
// ------------------------------------------------------------------------------------------------

// Approximates luminance from an RGB value
float CalcLuminance(float3 color)
{
    return max(dot(color, float3(0.299f, 0.587f, 0.114f)), 0.0001f);
}

// Retrieves the log-average lumanaince from the texture
float GetAvgLuminance(Texture2D lumTex, float2 texCoord)
{
	return exp(lumTex.SampleLevel(LinearSampler, texCoord, LumMapMipLevel).x);
}

// Logarithmic mapping
float3 ToneMapLogarithmic(float3 color)
{
	float pixelLuminance = CalcLuminance(color);    
    float toneMappedLuminance = log10(1 + pixelLuminance) / log10(1 + WhiteLevel);
	return toneMappedLuminance * pow(color / pixelLuminance, LuminanceSaturation); 
}

// Drago's Logarithmic mapping
float3 ToneMapDragoLogarithmic(float3 color)
{
	float pixelLuminance = CalcLuminance(color);    
    float toneMappedLuminance = log10(1 + pixelLuminance);
	toneMappedLuminance /= log10(1 + WhiteLevel);
	toneMappedLuminance /= log10(2 + 8 * ((pixelLuminance / WhiteLevel) * log10(Bias) / log10(0.5f)));
	return toneMappedLuminance * pow(color / pixelLuminance, LuminanceSaturation); 
}

// Exponential mapping
float3 ToneMapExponential(float3 color)
{
	float pixelLuminance = CalcLuminance(color);    
    float toneMappedLuminance = 1 - exp(-pixelLuminance / WhiteLevel);
	return toneMappedLuminance * pow(color / pixelLuminance, LuminanceSaturation);
}

// Applies Reinhard's basic tone mapping operator
float3 ToneMapReinhard(float3 color) 
{
	float pixelLuminance = CalcLuminance(color);    
    float toneMappedLuminance = pixelLuminance / (pixelLuminance + 1);
	return toneMappedLuminance * pow(color / pixelLuminance, LuminanceSaturation);    
}

// Applies Reinhard's modified tone mapping operator
float3 ToneMapReinhardModified(float3 color) 
{    
    float pixelLuminance = CalcLuminance(color);
	float toneMappedLuminance = pixelLuminance * (1.0f + pixelLuminance / (WhiteLevel * WhiteLevel)) / (1.0f + pixelLuminance);
	return toneMappedLuminance * pow(color / pixelLuminance, LuminanceSaturation);
}

// Applies the filmic curve from John Hable's presentation
float3 ToneMapFilmicALU(float3 color)
{
    color = max(0, color - 0.004f);
    color = (color * (6.2f * color + 0.5f)) / (color * (6.2f * color + 1.7f)+ 0.06f);

    // result has 1/2.2 baked in
    return pow(color, 2.2f);
}

// Function used by the Uncharte2D tone mapping curve
float3 U2Func(float3 x)
{
    float A = ShoulderStrength;
    float B = LinearStrength;
    float C = LinearAngle;
    float D = ToeStrength;
    float E = ToeNumerator;
    float F = ToeDenominator;
    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F)) - E/F;
}

// Applies the Uncharted 2 filmic tone mapping curve
float3 ToneMapFilmicU2(float3 color)
{
    float3 numerator = U2Func(color);        
    float3 denominator = U2Func(LinearWhite);

    return numerator / denominator;
}

// Determines the color based on exposure settings
float3 CalcExposedColor(float3 color, float avgLuminance, float threshold, out float exposure)
{    
    exposure = 0;

    if (AutoExposure >= 1 && AutoExposure <= 2)
    {
        // Use geometric mean        
        avgLuminance = max(avgLuminance, 0.001f);

        float keyValue = 0;
        if (AutoExposure == 1)
            keyValue = KeyValue;
        else if (AutoExposure == 2)
            keyValue = 1.03f - (2.0f / (2 + log10(avgLuminance + 1)));

        float linearExposure = (keyValue / avgLuminance);
        exposure = log2(max(linearExposure, 0.0001f));
    }
    else
    {
        // Use exposure setting
        exposure = Exposure;
    }

    exposure -= threshold;
    return exp2(exposure) * color;
}

// Applies exposure and tone mapping to the specific color, and applies
// the threshold to the exposure value. 
float3 ToneMap(float3 color, float avgLuminance, float threshold, out float exposure)
{
    float pixelLuminance = CalcLuminance(color);
    color = CalcExposedColor(color, avgLuminance, threshold, exposure);

	[branch]
    if (ToneMapTechnique == 0)
    {
        // Do nothing!
    }
	else if (ToneMapTechnique == 1)
    {
		color = ToneMapLogarithmic(color);
    }
    else if (ToneMapTechnique == 2)
    {
        color = ToneMapExponential(color);
    }	
	else if (ToneMapTechnique == 3)
    {
		color = ToneMapDragoLogarithmic(color);
    }
    else if (ToneMapTechnique == 4)
    {
		color = ToneMapReinhard(color);
    }
    else if (ToneMapTechnique == 5)
    {
        color = ToneMapReinhardModified(color);
    }	
    else if (ToneMapTechnique == 6)
    {
        color = ToneMapFilmicALU(color);
    }
    else
    {
        color = ToneMapFilmicU2(color);
    }

    return color;
}

// Calculates the gaussian blur weight for a given distance and sigmas
float CalcGaussianWeight(int sampleDist, float sigma)
{
	float g = 1.0f / sqrt(2.0f * 3.14159 * sigma * sigma);  
	return (g * exp(-(sampleDist * sampleDist) / (2 * sigma * sigma)));
}

// Performs a gaussian blue in one direction
float4 Blur(in PSInput input, float2 texScale, float sigma)
{
    float4 color = 0;
    for (int i = -6; i < 6; i++)
    {   
		float weight = CalcGaussianWeight(i, sigma);
        float2 texCoord = input.TexCoord;
		texCoord += (i / InputSize0) * texScale;
		float4 sample = InputTexture0.Sample(PointSampler, texCoord);
		color += sample * weight;
    }

    return color;
}

// ================================================================================================
// Shader Entry Points
// ================================================================================================

// Uses a lower exposure to produce a value suitable for a bloom pass
float4 Threshold(in PSInput input) : SV_Target
{             
    float3 color = 0;

    color = InputTexture0.Sample(LinearSampler, input.TexCoord).rgb;

    // Tone map it to threshold
    float avgLuminance = GetAvgLuminance(InputTexture1, input.TexCoord);
	float exposure = 0;
    color = ToneMap(color, avgLuminance, BloomThreshold, exposure);
    return float4(color, 1.0f);
}

// Uses hw bilinear filtering for upscaling or downscaling
float4 Scale(in PSInput input) : SV_Target
{
    return InputTexture0.Sample(LinearSampler, input.TexCoord);
}

// Horizontal gaussian blur
float4 BloomBlurH(in PSInput input) : SV_Target
{
    return Blur(input, float2(1, 0), BloomBlurSigma);
}

// Vertical gaussian blur
float4 BloomBlurV(in PSInput input) : SV_Target
{
    return Blur(input, float2(0, 1), BloomBlurSigma);
}

// Applies exposure and tone mapping to the input, and combines it with the
// results of the bloom pass
void Composite(in PSInput input, 
				out float4 outputColor : SV_Target0, 
				out float4 outputExposure : SV_Target1)
{
    // Tone map the primary input
    float avgLuminance = GetAvgLuminance(InputTexture1, input.TexCoord);
    float3 color = InputTexture0.Sample(PointSampler, input.TexCoord).rgb;
    float exposure = 0;
	color = ToneMap(color, avgLuminance, 0, exposure);

    // Sample the bloom
    float3 bloom = InputTexture2.Sample(LinearSampler, input.TexCoord).rgb;
    bloom = bloom * BloomMagnitude;

    // Add in the bloom
	color = color + bloom;

    outputColor = float4(color, 1.0f);
	// outputExposure = (exposure + 10.0f) / 20.0f;
	outputExposure = avgLuminance;
	outputExposure.a = 1.0f;
}

// Creates the luminance map for the scene
float4 LuminanceMap(in PSInput input) : SV_Target
{
    // Sample the input
    float3 color = InputTexture0.Sample(LinearSampler, input.TexCoord).rgb;
   
    // calculate the luminance using a weighted average
    float luminance = CalcLuminance(color);
                
    return float4(luminance, 1.0f, 1.0f, 1.0f);
}

// Slowly adjusts the scene luminance based on the previous scene luminance
float4 AdaptLuminance(in PSInput input) : SV_Target
{
    float lastLum = exp(InputTexture0.Sample(PointSampler, input.TexCoord).x);
    float currentLum = InputTexture1.Sample(PointSampler, input.TexCoord).x;
       
    // Adapt the luminance using Pattanaik's technique    
    float adaptedLum = lastLum + (currentLum - lastLum) * (1 - exp(-TimeDelta * Tau));
    
    return float4(log(adaptedLum), 1.0f, 1.0f, 1.0f);
}