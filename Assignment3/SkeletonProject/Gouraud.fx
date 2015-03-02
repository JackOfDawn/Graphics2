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
// Effect1
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Effect_Group_1_Effect1_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 matViewProjection : ViewProjection;
float4x4 matWorldIT;
float4x4 matWorld : World;
float4 viewPosition : ViewPosition;
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
> = float4( 1.00, 0.00, 0.00, 1.00 );
float4 materialDiffuse
<
   string UIName = "materialDiffuse";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 1.00, 1.00, 1.00, 1.00 );
float4 materialSpecular
<
   string UIName = "materialSpecular";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 1.00, 1.00, 1.00, 1.00 );
float materialPower
<
   string UIName = "materialPower";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.20 );

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL0;
   float3 View : NORMAL1;
   float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float4 Color : COLOR0;
   float2 texCoord : TEXCOORD;
};

VS_OUTPUT Effect_Group_1_Effect1_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matViewProjection );
   
   // Normalize
   float3 light = normalize(-dirLightDir);
   float3 worldPos = mul(Input.Position, matWorld).xyz;
   float3 view = normalize(viewPosition - worldPos);
   float3 normal = normalize(Input.Normal);
   
   float3 halfway = normalize(light + view);
   
   float4 ambient = materialAmbient;
   float3 diffuse = saturate(dot(normal, light)) * materialDiffuse.rgb;
   float3 specular = pow(saturate(dot(normal, halfway)), materialPower) * materialSpecular;
   //float2 texCoord = Input.texCoord;
   
   //float4 texColor = tex2D(sampTexture, texCoord);
   float4 color = float4(0, 0, 0, 0);
   color.rgb = (saturate(ambient + diffuse) * specular * dirLightColor);
   color.a = materialDiffuse.a;
   
   Output.Color = color;
   Output.texCoord = Input.texCoord;
   
   return( Output );
}




texture texTexture;

texture texTexture_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.jpg";
>;
sampler sampTexture = sampler_state
{
   Texture = (texTexture_Tex);
} = sampler_state {
   Texture = (texTexture);
   };

struct Effect_Group_1_Effect1_Pass_0_Pixel_Shader_VS_OUTPUT 
{
   float3 Position : POSITION0;
   float4 Color : COLOR0;
   float2 texCoord : TEXCOORD;
};

float4 Effect_Group_1_Effect1_Pass_0_Pixel_Shader_ps_main(Effect_Group_1_Effect1_Pass_0_Pixel_Shader_VS_OUTPUT input) : COLOR0
{
   float2 texCoord = input.texCoord;
   float4 texColor = tex2D(sampTexture, texCoord);
   float4 color = saturate(input.Color * texColor);
   return color;
}




//--------------------------------------------------------------//
// Technique Section for Effect Workspace.Effect Group 1.Effect1
//--------------------------------------------------------------//
technique Effect1
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Effect_Group_1_Effect1_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Effect_Group_1_Effect1_Pass_0_Pixel_Shader_ps_main();
   }

}

