#include <iostream>
#include "gfx.h"

int main () 
{
    color_type clear_color = gfx_make_color(50, 50, 50, 255);
    color_type red = gfx_make_color(255, 0, 0, 255);
    color_type green = gfx_make_color(0, 255, 0, 255);
    color_type blue = gfx_make_color(0, 0, 255, 255);

    canvas c;
    color_type gray = gfx_make_color(50, 50, 50, 255);
    int frames = 1;

    for(int d = 0; d < frames; ++d) {
        gfx_canvas_fill(&c, gray);

        int additive = d*16;

        gfx_draw_triangle_2d_debug(&c, red, 200 + additive, 150, 350, 400, 400, 200);
        gfx_draw_triangle_2d_debug(&c, red, 100 + additive, 50, 100, 300, 275, 400);
        gfx_draw_triangle_2d_debug(&c, red, 300 + additive, 25, 400, 25, 400, 175);

        // Final draw / save to file
        gfx_buffer_draw(&c, "out.bmp");
        
        usleep(500000);
    }

    return 0;
}