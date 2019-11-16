#version 150

// shader for basic draw call testing
// no transformations so coords must be normalized
// color is hardcoded

in vec2 position;

void main(void)
{
  gl_Position = vec4(position.x, position.y, 1.0, 1.0);
}
