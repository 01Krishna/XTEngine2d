#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoords;

uniform vec4 newColor; 

uniform vec4 uv_Rect;

uniform sampler2D ourTexture;


void main()
{
    vec2 uv = uv_Rect.xy + TexCoords * (uv_Rect.zw - uv_Rect.xy);
    vec4 tex = texture(ourTexture, uv);
    FragColor = vec4(tex.rgb * ourColor.rgb, tex.a);
    //FragColor = vec4(ourColor,1.0);
}
    