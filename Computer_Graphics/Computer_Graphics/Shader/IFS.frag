#version 330 core

in vec3 fragCoord;
in vec2 texCoord;

out vec4 fragColor;

// IFS
const float MAX_STEPS = 1024;
const float tolerance = 0.009;

// Random Number Gen
uniform int frame;
float rand( vec2 uv, float time)
{
	return fract( sin( time / uv.x * uv.y));
}
// int seed;
// void srand(int s){seed = s;}
// int rand(void)
// {
//     seed = seed * 0x343fd + 0x269ec3;
//     return (seed >> 16)&32767;
// }
// float frand(void)
// {
//     return float(rand() / 32767.0);
// }
// int hash( int n ) 
// { 
//     n = (n << 13)^n; 
//     return n * (n * n * 15731 + 789221) + 1376312589; 
// }

// Shader
uniform vec2 SystemResolution;
uniform float SystemTime;

vec2 IFSFern(in vec2 z, float p)
{
    //float p = frand();

    if( p < 0.01 )
        z = vec2(0.0, 0.16 * z.y);
    else if( p < 0.86 )
        z = mat2(0.85, 0.04, 0.04, 0.85) * z + vec2(0.0, 0.15);
    else if( p < 0.93 )
        z = mat2(0.20, -0.26, 0.23, 0.22) * z + vec2(0.0, 0.16);
    else
        z = mat2(0.15, -0.28, -0.26, 0.24) * z + vec2(0.0, 0.044);

    return z;
}

// float Render(vec2 p)
// {
//     vec2 z = vec2(0.0);
//     for( int i = 0; i < 32; i++ )
//     {
//         z = IFSFern( z );
//     }
//     float d = 1e4;
//     for( int i = 0; i < MAX_STEPS; i++ )
//     {
//         z = IFSFern( z );
// 		d = min( d, dot(p-z,p-z) );
//     }
//     return sqrt(d);
// }

void main()
{
    vec2 uv = vec2(texCoord.x, -texCoord.y);
    // Zoom in on the fern
    uv = uv * 0.8 - vec2(0.0, -0.5);

    vec2 z = vec2(0.0);

    // if( uv.x > 0.76 || uv.x < 0.18)
    // {
    //     fragColor = vec4(0.1059, 0.0196, 0.2392, 1.0);
    //     return;
    // }

    float result = 1.0;
    float p = rand(uv, SystemTime);

    float t = p;
    float col = 0.0;

    for(float i = 0.0; i < tolerance; i += tolerance/MAX_STEPS)
    {
        // p = fract(p / 0.123454);    // Just some random number
        // z = IFSFern(z, p);

        // // if current pixel is near the result (based on tolerance, then plot it)
		// result=(uv.x-z.x)*(uv.x-z.x)+(uv.y-z.y)*(uv.y-z.y);
		 
		// if( result < (tolerance - i) / MAX_STEPS)
        // {
        //     fragColor = vec4(0.0941, 0.5333, 0.0941, 1.0);
        //     return;
		// }
        p = fract(p / 0.123454);    // Just some random number
        z = IFSFern(z, p);

        //col = max(col, exp(-100.0 * dot(uv - z, uv - z)));
        // if current pixel is near the result (based on tolerance, then plot it)

		result=(uv.x-z.x)*(uv.x-z.x)+(uv.y-z.y)*(uv.y-z.y);
		 
		if( result < (tolerance - i) / MAX_STEPS){
			
			 fragColor = vec4(0.0, 1.0, 0.0, 1.0);
			 return;
		}
    }
    //fragColor = vec4(0.5 * col, col, 0.0, 1.0);
    fragColor = vec4(0.1059, 0.0196, 0.2392, 1.0);

}
	
	