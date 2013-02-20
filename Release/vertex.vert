


void main()
{
    // Transforming The Vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//gl_FrontColor = vec4 (0.5, 0.5, 0.5, 1.0);
	gl_TexCoord [0] = gl_MultiTexCoord0;
}