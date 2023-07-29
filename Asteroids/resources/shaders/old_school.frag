uniform sampler2D texture;
uniform vec2 resolution;

void main()
{
	float pixelSize = 5.0;
	vec2 pixelCoords = floor(gl_FragCoord.xy / pixelSize) * pixelSize;
	vec2 uv = pixelCoords / resolution;
	uv.y = 1.0 - uv.y;
	vec4 color = texture2D(texture, uv);
	gl_FragColor = color;
}
