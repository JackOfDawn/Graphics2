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
float4x4 ViewProjection : ViewProjection;
float4 ViewPosition : ViewPosition;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 Position3D :TEXCOORD0;  
};

//constructs a translation matrix
float4x4 translation(float3 position)
{
   return float4x4(
   float4(1,0,0,0),
   float4(0,1,0,0),
   float4(0,0,1,0),
   float4(position.xyz, 1)
   );
}

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   
   //create a world matric that translates the skybox to the camera position
   float4x4 world = translation(ViewPosition.xyz);
   
   //combine it with the existingview projection matrix
   float4x4 WorldViewProjection = mul(world, ViewProjection);
   
   //transform your vertex into place
   Output.Position = mul(Input.Position, WorldViewProjection);
   
   //Save the original vertex position in model space
   //Since the cube model is centered around the origin, you can use
   //These vertex positions directly as directions for your cube map
   Output.Position3D = Input.Position;
          
   return( Output );
   
}



/*
texture Skybox_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Snow.dds";
>;
*/
sampler SO = sampler_state
{
	Texture = (Skybox_Tex);
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};
/*
sampler Texture = sampler_state
{
   Texture = (Skybox_Tex);
};*/
struct Default_DirectX_Effect_Pass_0_Pixel_Shader_VS_OUTPUT
{
   float4 Position : POSITION0;
   float3 Position3D : TEXCOORD0;
};


float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(Default_DirectX_Effect_Pass_0_Pixel_Shader_VS_OUTPUT input) : COLOR0
{   
   return texCUBE(Texture, input.Position3D);
   
}


//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      CULLMODE = CW;
      ZWRITEENABLE = FALSE;

      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }
}

