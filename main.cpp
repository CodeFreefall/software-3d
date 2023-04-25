#include <iostream>
#include "gfx.h"

int main () 
{
    color_type clear_color = gfx_make_color(50, 50, 50, 255);
    color_type red = gfx_make_color(255, 0, 0, 200);
    color_type green = gfx_make_color(0, 255, 0, 200);
    color_type blue = gfx_make_color(0, 0, 255, 200);

    canvas c;
    color_type gray = gfx_make_color(50, 50, 50, 255);
    int frames = 1;

    for(int d = 0; d < frames; ++d) {
        gfx_canvas_clear(&c, gray);

        int additive = d*16;


        gfx_draw_triangle_2d(&c, red, 250 + additive, 20, 100, 300, 275, 400); // Left
        gfx_draw_triangle_2d(&c, blue, 300 + additive, 250, 400, 25, 75, 275); // Right
        gfx_draw_triangle_2d(&c, green, 100 + additive, 150, 350, 400, 400, 200); // Middle

        //gfx_draw_point(&c, blue, 200, 350);

        // Final draw / save to file
        gfx_buffer_draw(&c, "out.bmp");
        
        usleep(500000);
    }

    return 0;
}