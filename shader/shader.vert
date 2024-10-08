in vec3 position;
in vec3 normal;
in vec4 color;
uniform vec3 pos;
uniform mat4 projection;
uniform mat4 view;

out vec4 fPosition;
out vec4 fColor;
out vec4 fLightPosition;
out vec3 fNormal;

void main(void)
{
    fPosition = view * vec4(position.x+pos.x, position.y+pos.y, position.z,1.0);
    fLightPosition = view * vec4(0.0,0.0,1.0,1.0);

    fColor = color;
    fNormal = vec3(view * vec4(normal,0.0));

    gl_Position = projection * fPosition;
    /*gl_Position.x *= 1000.0f;*/
    /*gl_Position.y = 0.0;*/
}
