    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec4 aColor;
    layout (location = 2) in vec2 aTexCoords;


    out vec4 ourColor;
    out vec2 TexCoords;

    uniform mat4 viewProjection;

    void main()
    {
        gl_Position = viewProjection * vec4(aPos, 0.0, 1.0);
        ourColor = aColor;
        TexCoords = aTexCoords;
    }