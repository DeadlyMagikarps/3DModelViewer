#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;
uniform vec3 materialColor;

void main()
{
    vec4 texColor = texture(tex, TexCoords);
    vec3 texCol = texColor.rgb * materialColor;

    if (texColor.a < 0.1) {
        FragColor = vec4(materialColor, 1.0);
    } else {
        FragColor = vec4(texCol, 1.0);
    }
}
