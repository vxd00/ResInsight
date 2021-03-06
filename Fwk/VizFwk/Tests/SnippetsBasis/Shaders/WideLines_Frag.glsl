
varying vec2 v_circleFactors;
varying float v_alpha;

vec4 srcFragment();

//--------------------------------------------------------------------------------------------------
/// Fragment Shader - snipLineDrawing
//--------------------------------------------------------------------------------------------------
void main()
{
    const vec3 lightDir = vec3(0, 0, 1);

    // Calculate normal from texture coordinates
    vec3 N;
    N.xy = vec2(v_circleFactors.x, v_circleFactors.y);
    float mag = dot(N.xy, N.xy);
    if (mag > 1) discard;   
    N.z = sqrt(1 - mag);

    float diffuse = max(0.0, dot(lightDir, N));

	vec3 color = srcFragment().rgb;
    gl_FragColor = vec4(color*diffuse, v_alpha);
}


