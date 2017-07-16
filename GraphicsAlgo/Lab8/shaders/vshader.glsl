attribute vec3 coord;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * vec4(coord, 1.0);
}