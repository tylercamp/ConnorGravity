

uniform sampler2D sourceTexture;

void main()
{
	int sampleSize = 5;

	vec2 texCoord = gl_TexCoord [0].st;

	int x;
	int y;

	vec4 result = vec4 (0, 0, 0, 0);


	for (x = -sampleSize; x <= sampleSize; x++)
	{
		for (y = -sampleSize; y <= sampleSize; y++)
		{
			result += texture2D (sourceTexture, texCoord + vec2 (x, y) * 0.002) * 0.05 * (1 - length(vec2(x,y)) / length (vec2 (sampleSize, sampleSize)));
			//result += texture2D (sourceTexture, texCoord) * 0.0001;
		}
	}

	gl_FragColor =  result;
}