#include <iostream>
#include "gfx.h"

int main() {
    printf("Running tests...\n");

    color_type clear_color = gfx_make_color(50, 50, 50, 255);
    color_type red = gfx_make_color(255, 0, 0, 255);
    color_type green = gfx_make_color(0, 255, 0, 255);
    color_type blue = gfx_make_color(0, 0, 255, 255);

    canvas c;
    color_type rect_color = gfx_make_color(255, 0, 0, 255);
    color_type line_colors[3] = {
        gfx_make_color(255, 0, 0, 255),
        gfx_make_color(0, 255, 0, 255),
        gfx_make_color(0, 0, 255, 255)
    };

    gfx_canvas_clear(&c, clear_color);
    gfx_draw_line(&c, line_colors[0], 0, 0, canvas::width - 1, canvas::height - 1);
    gfx_draw_line(&c, line_colors[1], 0, canvas::height / 2, canvas::width - 1, canvas::height / 2);
    gfx_draw_line(&c, line_colors[2], 0, canvas::height - 1, canvas::width - 1, 0);
    gfx_draw_line(&c, gfx_make_color(255, 255, 255, 255), canvas::width / 2, canvas::height - 1, canvas::width / 2, 0);

    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), 0, canvas::height - 1, canvas::width / 2, 0);
    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), 0, canvas::height - 1, canvas::width / 4, 0);
    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), 0, canvas::height - 1, canvas::width / 8, 0);
    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), 0, canvas::height - 1, canvas::width / 16, 0);

    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), canvas::width / 2, 0, canvas::width - 1, canvas::height - 1);          
    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), canvas::width / 2, 0, canvas::width * 0.75f, canvas::height - 1);          
    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), canvas::width / 2, 0, canvas::width * 0.85f, canvas::height - 1);          
    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), canvas::width / 2, 0, canvas::width * 0.95f, canvas::height - 1); 

    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), canvas::width / 2, 0, canvas::width - 1, canvas::height / 2);          
    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), canvas::width / 2, 0, canvas::width - 1, canvas::height / 4);          
    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), canvas::width / 2, 0, canvas::width - 1, canvas::height / 8);          
    gfx_draw_line(&c, gfx_make_color(128, 255, 128, 255), canvas::width / 2, 0, canvas::width - 1, canvas::height / 16);          
    gfx_buffer_draw(&c, "docs/examples/lines.bmp"); // For docs.

    gfx_canvas_clear(&c, clear_color);
    int x1 = canvas::width / 2;
    int y1 = canvas::height / 2;
    int initialLength = 250;
    float lenInc = 0;
    for(float a = 0; a < 90; a += 0.1f) {
        int x2 = (float)x1 + ((float)initialLength + ((float)a * lenInc)) * std::cos((float)a);
        int y2 = (float)y1 + ((float)initialLength + ((float)a * lenInc)) * std::sin((float)a);

        auto color = gfx_angle_to_color(a);
        //auto color = gfx_make_color_hsv(a * 4, a, a);
        gfx_draw_line(&c, color, x1, y1, x2, y2);

        gfx_draw_point(&c, red, x2, y2);
    }
    gfx_draw_point(&c, red, x1, y1);
    gfx_buffer_draw(&c, "docs/examples/color-wheel.bmp"); // For docs.


    gfx_canvas_clear(&c, clear_color);
    int rect_x1 = (float)canvas::width / 4.f;
    int rect_y1 = (float)canvas::height / 2.f;
    int rect_x2 = rect_x1 + 150;
    int rect_y2 = rect_y1 + 150;
    int rect_offset = 75;
    gfx_draw_rect(&c, red,      rect_x1,                    rect_y1, rect_x2, rect_y2);
    gfx_draw_rect(&c, green,    rect_x1 + rect_offset,      rect_y1 - rect_offset, rect_x2 + rect_offset, rect_y2 - rect_offset);
    gfx_draw_rect(&c, blue,     rect_x1 + rect_offset*2,    rect_y1 - rect_offset*2, rect_x2 + rect_offset*2, rect_y2 - rect_offset*2);
    gfx_buffer_draw(&c, "docs/examples/rect.bmp"); // For docs.

    gfx_canvas_clear(&c, clear_color);
    gfx_draw_circle(&c, gfx_make_color(255, 0, 0, 255), canvas::width / 2, canvas::height / 2, 200);
    gfx_buffer_draw(&c, "docs/examples/circle.bmp"); // For docs.

    gfx_canvas_clear(&c, clear_color);
    gfx_draw_triangle_2d_debug(&c, red, 200, 150, 350, 400, 400, 200);
    gfx_draw_triangle_2d_debug(&c, red, 100, 50, 100, 300, 275, 400);
    gfx_draw_triangle_2d_debug(&c, red, 300, 25, 400, 25, 400, 175);
    gfx_buffer_draw(&c, "docs/examples/triangles.bmp"); // For docs.

    gfx_canvas_clear(&c, clear_color);
    gfx_draw_triangle_2d(&c, gfx_make_color(255,0,0,200), 250, 20, 100, 300, 275, 400);
    gfx_draw_triangle_2d(&c, gfx_make_color(0,0,255,200), 300, 250, 400, 25, 75, 275);
    gfx_draw_triangle_2d(&c, gfx_make_color(0,255,0,200), 100, 150, 350, 400, 400, 200);
    gfx_buffer_draw(&c, "docs/examples/alpha-blending.bmp"); // For docs.

    return 0;
}