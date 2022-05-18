#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D rainbowTexture;
uniform vec3 lightColor;

void main()
{
    vec4 texColor = texture(rainbowTexture, TexCoords);
    if (texColor.a < 0.1)
        discard;
    FragColor = texColor * vec4(lightColor, 1.0f);
}