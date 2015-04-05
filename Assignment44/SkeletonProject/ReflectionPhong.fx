//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// Textured Phong
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Textured_Phong_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float3 fvLightPosition
<
   string UIName = "fvLightPosition";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -100.00;
   float UIMax = 100.00;
> = float3( -100.00, 100.00, 100.00 );
float3 fvEyePosition
<
   string UIName = "fvEyePosition";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -100.00;
   float UIMax = 100.00;
> = float3( -100.00, 100.00, -100.00 );
float4x4 matView : View;
float4x4 matViewProjection : ViewProjection;
float4x4 World;
float3 CameraPosition;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL0;
   float3 Tangent :         TANGENT;
   float3 Binormal :        BINORMAL;
};

struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 Texcoord :        TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection :  TEXCOORD2;
   float3 Normal :          TEXCOORD3;
   float3 Tangent :         TEXCOORD4;
   float3 Binormal :        TEXCOORD5;
   float3 reflection :      TEXCOORD6;
};

VS_OUTPUT Textured_Phong_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position         = mul( Input.Position, matViewProjection );
   Output.Texcoord         = Input.Texcoord;
   
   float3 fvObjectPosition = mul( Input.Position, matView );
   
   Output.ViewDirection    = fvEyePosition - fvObjectPosition;
   Output.LightDirection   = fvLightPosition - fvObjectPosition;
   Output.Normal           = mul(Input.Normal, matView );
   Output.Tangent          = mul(Input.Tangent, matView);
   Output.Binormal         = mul(Input.Binormal, matView);

   float4 VertexPosition = mul(Input.Position, World);
   float3 viewDirection    = CameraPosition - VertexPosition;

   Output.reflection = reflect(-normalize(viewDirection), normalize(Input.Normal));
     
   return( Output );
   
}



float4 fvAmbient
<
   string UIName = "fvAmbient";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.12, 0.12, 0.12, 1.00 );
float4 fvSpecular
<
   string UIName = "fvSpecular";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.48, 0.48, 0.48, 1.00 );
float4 fvDiffuse
<
   string UIName = "fvDiffuse";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.67, 0.67, 0.67, 1.00 );
float fSpecularPower
<
   string UIName = "fSpecularPower";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 1.00;
   float UIMax = 100.00;
> = float( 25.75 );
float ka
<
   string UIName = "ka";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 1.00 );
float kd
<
   string UIName = "kd";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 1.00 );
float ks
<
   string UIName = "ks";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 128.00;
> = float( 10.24 );
float kt
<
   string UIName = "kt";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 1.00 );
float bumpiness
<
   string UIName = "bumpiness";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.00 );
float reflectiveness
<
   string UIName = "reflectiveness";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 1.00 );

/*
texture base_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Fieldstone.tga";
>;
*/


texture base_Tex;
sampler2D baseMap = sampler_state
{
   Texture = (base_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

/*
texture bump_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\FieldstoneBumpDOT3.tga";
>;

*/

texture bump_Tex;
sampler2D bumpMap = sampler_state
{
   Texture = (bump_Tex);
};
/*
texture grace_cube_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.dds";
>;
*/

texture skyBox_Tex;
samplerCUBE SkyBoxSampler = sampler_state
{
   Texture = (skyBox_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection:   TEXCOORD2;
   float3 Normal :          TEXCOORD3;
   float3 Tangent :         TEXCOORD4;
   float3 Binormal :        TEXCOORD5;
   float3 reflection :      TEXCOORD6;
};

float4 Textured_Phong_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR0
{
   float3 Lm = normalize(Input.LightDirection);
   //float3 N = normalize(Input.Normal);
   float3 V = normalize(Input.ViewDirection);
   //float3 Rm = normalize(2.0f * (dot(Lm, N)) * N - Lm);
   
   float4 baseColor = tex2D(baseMap, Input.Texcoord) * kt;
   float4 bumpColor = tex2D(bumpMap, Input.Texcoord);
   bumpColor = (bumpColor * 2.0f) - 1.0f;
   
   float3 bumpNormal = (bumpColor.x * Input.Tangent) + (bumpColor.y * Input.Binormal) + (bumpColor.z * Input.Normal);
   float3 N = normalize((bumpNormal * bumpiness) + (Input.Normal * (1 - bumpiness)));
   
   float3 Rm = normalize(2.0f * (dot(Lm, N)) * N - Lm);
   
   float4 ambient = ka * fvAmbient * baseColor;
   float4 diffuse = kd * (dot(Lm, N)) * fvDiffuse * baseColor;
   float4 specular = ks * pow(max(0, dot(Rm, V)), fSpecularPower) * fvSpecular;
   float4 reflectionColor = (texCUBE(SkyBoxSampler, normalize(Input.reflection)) * reflectiveness) + (baseColor * (1 - reflectiveness));
   specular = specular * reflectionColor;
   
   return( saturate((ambient + diffuse + specular)));
}



//--------------------------------------------------------------//
// Technique Section for Textured Phong
//--------------------------------------------------------------//
technique Textured_Phong
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Textured_Phong_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Textured_Phong_Pass_0_Pixel_Shader_ps_main();
   }

}

