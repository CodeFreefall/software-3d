<h1>Basic 3D Software Renderer</h1>

It uses memory and math to draw pretty pictures. No OpenGL or other libraries (within reason).

Things are simply being rendered to a .bmp file, and this was only tested on Linux (Ubuntu 22.04 LTS).

Supported commands:
| Command | Result |
| --- | --- |
| ./run.sh main | Execute the main code |
| ./run.sh debug | Execute the debug code |
| ./run.sh tests | Execute the runtime tests & generate doc images |

Basic features supported so far:

| Description | Image |
| --- | --- |
| Basic Rectangle               | <img src="docs/examples/rect.bmp" width=100%> |
| Lines                         | <img src="docs/examples/lines.bmp" width=100%> |
| Full 360deg range w/ lines    | <img src="docs/examples/color-wheel.bmp" width=100%> |
| Circles                       | <img src="docs/examples/circle.bmp" width=100%> |
| 2D Triangles (partial)        | <img src="docs/examples/triangles.bmp" width=100%> |