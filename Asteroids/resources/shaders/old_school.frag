uniform sampler2D texture;
uniform vec2 resolution;

void main()
{
	float pixelSize = 5.5;
	vec2 pixelCoords = floor(gl_FragCoord.xy / pixelSize) * pixelSize;
	vec2 uv = pixelCoords / resolution;
	vec4 color = texture2D(texture, uv);
	gl_FragColor = color;
}
