<h1>Basic 3D Software Renderer</h1>

It uses memory and math to draw pretty pictures. No OpenGL or other libraries (within reason).

Things are simply being rendered to a .bmp file, and this was only tested on Linux (Ubuntu 22.04 LTS).

Here's a basic rectangle with a solid clear color in the background:

<img src="docs/examples/rect.bmp" width=50%>

Here's a scuffed example of the lines being drawn:

<img src="docs/examples/lines.bmp" width=50%>

There's something wrong with the Y axis, and the formula I'm using for line rendering.


And now, there are also circles that can be generated:

<img src="docs/examples/circle.bmp" width=50%>