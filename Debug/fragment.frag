

uniform sampler2D sourceTexture;

void main()
{
	int sampleSize = 20;

	vec2 texCoord = gl_TexCoord [0].st;

	vec2 texSize = textureSize (sourceTexture, 0) * 1.0;

	int x;
	int y;

	vec4 result = vec4 (0, 0, 0, 0);

	float factor = 0.005f;

	float sampleSizeLength = length (vec2 (sampleSize, sampleSize));

	for (x = -sampleSize; x <= sampleSize; x++)
	{
		for (y = -sampleSize; y <= sampleSize; y++)
		{
			result += texture2D (sourceTexture, texCoord + vec2 (x / texSize.x, y / texSize.y)) * factor * (1 - length(vec2(x,y)) / sampleSizeLength);
			//result += texture2D (sourceTexture, texCoord) * 0.0001;
		}
	}

	gl_FragColor =  result;
}