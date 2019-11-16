#version 150

// shader for basic draw call testing
// no transformations so coords must be normalized
// color is hardcoded

out vec4 frag_color;

void main()
{
  frag_color = vec4(1.0, 1.0, 1.0, 1.0);
}
