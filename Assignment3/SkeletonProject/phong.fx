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
// Default_DirectX_Effect
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
float4x4 matViewProjection : WorldViewProjection;
float4x4 matWorldIT : WorldInverseTranspose;
float4x4 matWorld : World;
float4 viewPosition : ViewPosition;

struct VS_INPUT 
{
   float4 Position : POSITION0; //Vertex position in object space
   float3 normal : NORMAL; //certex normal in object space
   float2 texCoord : TEXCOORD; // vertex texture coordinates
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0; //pixel position in clip space
   float2 texCoord :TEXCOORD0; //pixel texture coorinates
   float3 normal : TEXCOORD1; //pixel normal vector
   float3 view : TEXCOORD2; //pixel view vector
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matViewProjection );
   
   Output.texCoord = Input.texCoord;
   
   //calulate the normal vector
   Output.normal = mul(matWorldIT, Input.normal);
   //calculate the view vector
   float3 worldPos = mul(Input.Position, matWorld).xyz;
   Output.view = viewPosition - worldPos;   
   
   return( Output );
   
}




float3 dirLightDir
<
   string UIName = "dirLightDir";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 0.00, -1.00, 0.00 );
float4 dirLightColor
<
   string UIName = "dirLightColor";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 1.00, 1.00, 1.00, 1.00 );
float4 materialAmbient
<
   string UIName = "materialAmbient";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.11, 1.00, 0.00, 1.00 );
float4 materialDiffuse
<
   string UIName = "materialDiffuse";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.00, 1.00, 0.90, 1.00 );
float4 materialSpecular
<
   string UIName = "materialSpecular";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 1.00, 0.19, 0.00, 1.00 );
float materialPower
<
   string UIName = "materialPower";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 90.00;
> = float( 1.00 );
texture texTexture;

texture texTexture_Tex
<
   string ResourceName = "EarthClouds.jpg";
   //string ResourceName = "..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\EarthClouds.jpg";
>;
sampler sampTexture = sampler_state
{
   Texture = (texTexture_Tex);
};
/*
sampler sampTexture = sampler_state{
   Texture = (texTexture);
   };*/

struct Default_DirectX_Effect_Pass_0_Pixel_Shader_VS_OUTPUT 
{
   float4 Position : POSITION0; //pixel position in clip space
   float2 texCoord :TEXCOORD0; //pixel texture coorinates
   float3 normal : TEXCOORD1; //pixel normal vector
   float3 view : TEXCOORD2; //pixel view vector
};


float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(Default_DirectX_Effect_Pass_0_Pixel_Shader_VS_OUTPUT Input) : COLOR0
{   
   //normalize all the vectors
   float3 light = normalize(-dirLightDir);
   float3 view = normalize(Input.view);
   float3 normal = normalize(Input.normal);
   
   //calculate halfvector
   float3 halfway = normalize(light + view);
   
   //calculate ambient reflection
   float4 ambient = materialAmbient; 
   //calculate diffuse 
   float3 diffuse = saturate(dot(normal, light)) * materialDiffuse.rgb;
   //calc specular
   float3 specular = pow(saturate(dot(normal, halfway)), materialPower) * materialSpecular;
   //fetch texturee coord
   float2 texCoord = Input.texCoord;
   
   //sample the texture
   float4 texColor = tex2D(sampTexture, texCoord);
   //combine all color components
   float3 color = (saturate(ambient + diffuse) * texColor + specular) * dirLightColor;
   //calculate transp
   float alpha = materialDiffuse.a * texColor.a;
   //return pixels color
   return float4(color, alpha);
   
}




//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

