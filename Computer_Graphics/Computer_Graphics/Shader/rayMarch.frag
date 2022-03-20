#version 330 core

// AA
const int AA = 4;

// Ray Marching
const int MAX_STEPS = 128;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0005;
const float FOV = 120.0;

// Fractals
float POWER = 11;
const float BAILOUT = 50.0;
const int ITERATIONS = 20;
const int COLORITERATIONS = 5;

// Shader
const float shadowIntensity = 0.8; //Range from 0 - 1
const float shadowDiffuse = 1.0 - shadowIntensity;
const float diffuseStrength = 1.0;
const float orbitStrength = 0.8;
vec4 orbitTrap = vec4(MAX_DIST);
int curr_Step;

const vec3 O = vec3(0);

in vec3 fragCoord;
in vec2 texCoord;

out vec4 fragColor;

uniform int shape;

uniform float SystemTime;
uniform vec2 SystemResolution;
uniform vec3 Eye;

uniform float zoom;
uniform mat4 rotMat;

uniform samplerCube tChannel0;

// SDF Operators
float smin( float a, float b, float k )  // polynomial smooth min 1 (k=0.1)
{
    if(k == 0) k = 0.1;
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}
float intersectSDF(float distA, float distB)
{
    return max(distA, distB);
}
float unionSDF(float distA, float distB)
{
    return min(distA, distB);
}
float differenceSDF(float distA, float distB)
{
    return max(distA, -distB);
}

// Primitive SDF
float sdTorus(vec3 p, vec2 r) 
{
	float x = length(p.xz) - r.x;
    return length(vec2(x, p.y)) - r.y;
}
float sdSphere(vec3 p, float r)
{
    return length(p) - r;
}
float sdBox(vec3 p, vec3 dim)
{
    vec3 d = abs(p) - dim;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}
float sdTriPrism(vec3 p, vec2 h)
{
    vec3 q = abs(p);
    return max(q.z - h.y, max(q.x * 0.866025 + p.y * 0.5, -p.y) - h.x * 0.5);
}
float sdHexPrism(vec3 p, vec2 h)
{
    const vec3 k = vec3(-0.8660254, 0.5, 0.57735);
    p = abs(p);
    p.xy -= 2.0 * min(dot(k.xy, p.xy), 0.0) * k.xy;
    vec2 d = vec2
    (
        length(p.xy - vec2(clamp(p.x, -k.z * h.x, k.z * h.x), h.x)) * sign(p.y - h.x),
        p.z - h.y
    );
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)); 
}
float sdRhombicDodeca(vec3 p, int fold)
{
    float c = cos(3.1415 / 5.0), s = sqrt(0.75 - c * c);
    vec3 n = vec3(-0.5, -c, s);

    p = abs(p);
    p -= 2.0 * min(0, dot(p, n)) * n;

    for(int i = 0; i < fold; i++)
    {
        p.xy = abs(p.xy);
        p -= 2.0 * min(0, dot(p, n)) * n;
    }

    return p.z - 1;
}


// Fractals SDF
float JuliaEstimator(vec3 pos)
{
    vec4 p = vec4(pos, 0.0);
    vec4 dp = vec4(1, 0, 0, 0);

    for(int i = 0; i < ITERATIONS; i++)
    {
        dp = 2.0 * vec4(p.x * dp.x - dot(p.yzw, dp.yzw), p.x * dp.yzw + dp.x * p.yzw + cross(p.yzw, dp.yzw));
        p = vec4(p.x * p.x - dot(p.yzw, p.yzw), vec3(2.0 * p.x * p.yzw)) + 0.3;

        float p2 = dot(p, p);
        if(i < COLORITERATIONS) orbitTrap = min(orbitTrap, abs(vec4(p.xyz, p2)));
        if(p2 > BAILOUT) break;
    }

    float r = length(p);
    return 0.5 * r * log(r) / length(dp);

}
// const vec3 va = vec3(  0.0,  0.57735,  0.0 );
// const vec3 vb = vec3(  0.0, -1.0,  1.15470 );
// const vec3 vc = vec3(  1.0, -1.0, -0.57735 );
// const vec3 vd = vec3( -1.0, -1.0, -0.57735 );
float SierpenskiEstimator(vec3 z)
{
    float scale = 2.0;
 
    vec3 a1 = vec3(0.0, 0.577, 0.0);
    vec3 a2 = vec3(0.0, -1.0,  1.15470);
    vec3 a3 = vec3(1.0, -1.0, -0.57735);
    vec3 a4 = vec3(-1.0, -1.0, -0.57735);
    vec3 c;

    int n = 0;
    float dist, d;

    while(n < ITERATIONS)
    {
        c = a1; dist = length(z - a1);
        d = length(z - a2); if (d < dist) { c = a2; dist = d; }
		d = length(z - a3); if (d < dist) { c = a3; dist = d; }
		d = length(z - a4); if (d < dist) { c = a4; dist = d; }

        z = scale * z - c * (scale - 1.0);

        n++;
    }

    return length(z) * pow(scale, float(-n));
}
float mandlebulbEstimator(vec3 p)
{
    vec3 z = p;
    float dr = 1.0;
    float r = 0.0;

    for(int i = 0; i < ITERATIONS; i++)
    {
        r = length(z);
        if(r > BAILOUT)
            break;

        float theta = acos(z.z / r);
        float phi = atan(z.y, z.x);
        dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

        float zr = pow(r, POWER);
        theta = theta * POWER;
        phi = phi * POWER;

        z = zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
        z += p;
    }

    return 0.5 * log(r) * r / dr;

}

// Rotate Operations
mat4 RotateX(float theta)
{
    return mat4
    (
        vec4(1, 0, 0, 0),
        vec4(0, cos(theta), -sin(theta), 0),
        vec4(0, sin(theta), cos(theta), 0),
        vec4(0, 0, 0, 1)
    );
}
mat3 RotateY(float theta)
{
    return mat3
    (
        vec3(cos(theta), 0, sin(theta)),
        vec3(0, 1, 0),
        vec3(-sin(theta), 0, cos(theta))
    );
}
mat3 RotateZ(float theta)
{
    return mat3
    (
        vec3(cos(theta), -sin(theta), 0),
        vec3(sin(theta), cos(theta), 0),
        vec3(0, 0, 1)
    );
}

float GetDist(vec3 p) 
{
    vec4 center = vec4(p - vec3(0, 1, 0), 1.0);
    center *= rotMat;
    
    float primitive;

    if (shape == 1)
        primitive = sdRhombicDodeca(center.xyz, 3);
    else if (shape == 2)
        primitive = sdBox(center.xyz, vec3(1)); 
    else if (shape == 3)
        //primitive = sdTriPrism(center.xyz, vec2(1, 2));
        primitive = SierpenskiEstimator(center.xyz);

    return primitive;
}

float RayMarch(vec3 ro, vec3 rd, float side) 
{
	float dO = 0.0;
    
    int i;
    for(i = 0; i < MAX_STEPS; i++) 
    {
    	vec3 p = ro + rd * dO;
        float dS = GetDist(p) * side;
        dO += dS;

        if(dO > MAX_DIST || dS < EPSILON) break;
    }
    curr_Step = i;
    return dO;
}

vec3 GetNormal(vec3 sample)
{
    float distanceToPoint = GetDist(sample);
    vec2 e = vec2(0.04, 0);

    vec3 normal = distanceToPoint - vec3
    (
        GetDist(sample - e.xyy),
        GetDist(sample - e.yxy),
        GetDist(sample - e.yyx)
    ); 

    return normalize(normal);
}

float GetLight(vec3 sample)
{
    vec3 lightPos = vec3(3, 5, -7);

    vec3 light = normalize(lightPos - sample);
    vec3 normal = GetNormal(sample);

    float diff = clamp(dot(normal, light) * diffuseStrength, 0.0, 1.0);

    float distanceToLight = RayMarch(sample + normal * EPSILON * 2.0, light, 1);
    
    // if(distanceToLight < length(lightPos - sample))
    //     diff *= shadowDiffuse;
    
    return diff;
}
float fresnel(vec3 n, vec3 rd) 
{
  return pow(clamp(1.0 - dot(n, -rd), 0.0, 1.0), 5.0);
}

vec3 RayDir(vec2 uv, vec3 p, vec3 l, float z) 
{
    vec3 f = normalize(l-p),
        r = normalize(cross(vec3(0,1,0), f)),
        u = cross(f,r),
        c = p+f*z,
        i = c + uv.x*r + uv.y*u,
        d = normalize(i-p);
    return d;
}

vec3 Render(vec2 uv, out float d, out bool hit)
{
    vec3 color = vec3(0.6196, 0.9333, 0.9451);
    vec3 ro = vec3(0, 1, -7);

    vec3 rd = RayDir(-uv, ro, vec3(0, 1, 0), zoom);

    d = RayMarch(ro, rd, 1);

    const float IOR = 1.52;     // Index of refraction
    const float abb = 0.01;

    if(d < MAX_DIST)
    {
        vec3 p = ro + rd * d;       // 3D Position
        vec3 n = GetNormal(p);      // Normal of surface
        vec3 r = reflect(rd, n); 

        vec3 rdIn = refract(rd, n, 1.0 / IOR);
        vec3 pEnter = p - n * EPSILON * 3.0;

        float dIn = RayMarch(pEnter, rdIn, -1);

        vec3 pExit = pEnter + rdIn * dIn;
        vec3 nExit = -GetNormal(pExit);  

        vec3 reflTex = vec3(0);
        vec3 rdOut = vec3(0);

        // Red 
        rdOut = refract(rdIn, nExit, IOR - abb);
        if(dot(rdOut, rdOut) == 0)
            rdOut = reflect(rdIn, nExit);  
        reflTex.r = texture(tChannel0, rdOut).r;

        // Green
        rdOut = refract(rdIn, nExit, IOR);
        if(dot(rdOut, rdOut) == 0)
            rdOut = reflect(rdIn, nExit);  
        reflTex.g = texture(tChannel0, rdOut).g;

        // Blue
        rdOut = refract(rdIn, nExit, IOR + abb);
        if(dot(rdOut, rdOut) == 0)
            rdOut = reflect(rdIn, nExit);  
        reflTex.b = texture(tChannel0, rdOut).b;

        float diff = GetLight(p);

        color = reflTex * vec3(diff);
        color *= exp( -0.0005 * d * d * d );

        float density = 0.2;
        float opticalDist = exp(-dIn * density);

        vec3 absorbtionCol = vec3(0.8353, 0.2078, 0.9922);

        color *= mix(absorbtionCol, vec3(1), opticalDist);
        color += fresnel(n, rd) * 0.31;

        hit = true;
        return color;
    }
    else
    {
        hit = false;
        return color;
    }
}

void main()
{
    vec3 color = vec3(0);
    vec2 screen = (texCoord) / SystemResolution;

    float d;
    bool hit;

    vec2 aao;
    const float AAINC = 1.0 / float(AA);
    for(aao.x =- 0.5; aao.x < 0.5; aao.x += AAINC)
    {
        for(aao.y =- 0.5; aao.y < 0.5; aao.y += AAINC)
        {
            color += Render(texCoord + aao / SystemResolution.y, d, hit);
        }
    }

    if(!hit) discard;

    color /= float(AA * AA);

    // Post-Process
    //color = smoothstep(0.0, 1.0, color);        // Contrast
    //color = pow(color, vec3(0.4545));         //Gamma Correction

    fragColor = vec4(color, 1.0);
}
