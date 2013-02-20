
uniform sampler2D sourceTexture;

void main()
{
	vec2 size = textureSize (sourceTexture, 0);

	int sampleSize = 10;
	//vec2 discreteSize = vec2 (1.0/size.x, 1.0/size.y);
	vec2 discreteSize = vec2 (0.001, 0.002);

	vec2 texCoord = gl_TexCoord [0].st;

	int x;
	int y;

	vec4 result = vec4 (0, 0, 0, 0);

	for (x = -sampleSize; x <= sampleSize; x++)
	{
		for (y = -sampleSize; y <= sampleSize; y++)
		{
			result += texture2D (sourceTexture, texCoord + vec2 (x, y) * 0.001) * 0.2;
		}
	}

	//result = texture2D (sourceTexture, texCoord);
	gl_FragColor = result;
}