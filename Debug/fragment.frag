

uniform sampler2D sourceTexture;

void main()
{
   vec4 sum = vec4(0);
   vec2 texcoord = vec2(gl_TexCoord[0]);
   int j;
   int i;

   for( i= -4 ;i < 4; i++)
   {
        for (j = -3; j < 3; j++)
		{
			sum += texture2D(sourceTexture, texcoord + vec2(j, i)*0.004) * 0.25;
		}
	}

	if (texture2D(sourceTexture, texcoord).r < 0.3)
    {
       gl_FragColor = sum*sum*0.012 + texture2D(sourceTexture, texcoord);
    }
    else
    {
        if (texture2D(sourceTexture, texcoord).r < 0.5)
        {
            gl_FragColor = sum*sum*0.009 + texture2D(sourceTexture, texcoord);
        }
        else
        {
            gl_FragColor = sum*sum*0.0075 + texture2D(sourceTexture, texcoord);
        }
    }

	gl_FragColor = sum;
}

/*
uniform sampler2D sourceTexture;

void main()
{
	int sampleSize = 20;

	vec2 texCoord = gl_TexCoord [0].st;

	int x;
	int y;

	vec4 result = vec4 (0, 0, 0, 0);


	for (x = -sampleSize; x <= sampleSize; x++)
	{
		for (y = -sampleSize; y <= sampleSize; y++)
		{
			result += texture2D (sourceTexture, texCoord + vec2 (x, y) * 0.002) * 0.1 * (1 - length(vec2(x,y)) / length (vec2 (sampleSize, sampleSize)));
			//result += texture2D (sourceTexture, texCoord) * 0.0001;
		}
	}

	gl_FragColor =  result;
}
*/