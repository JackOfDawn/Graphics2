Jack Storm, Scott Barrett
Graphics 2, Assignment 4
Normal mapping and Environment Reflections

Skybox
To create the skybox we created shader that draws the cube texture to a large
box that is surrounding the scene. The shader for that was fairly straight forward.
We just had to adjust a few flags in the technique of skybox.fx to make it draw correctly.
We had to set Zwriteeneable to false and Cullmode to clockwise so it will show the inside of
the box.

Reflection
The reflection portion of the ReflectionPhong.fx uses the World matrix and the camera position
to find the correct angle of reflection in the vertexShader. In the pixel shader it uses the skybox_texture
and the reflection vector to get the corresponding color. The color is then blended with the specular.

Normal Mapping
To get the TBN vectors we make use of the directX function D3DXComputedTangentFrameEX which
creates them based off of the mesh.

