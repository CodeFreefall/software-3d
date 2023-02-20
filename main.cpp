#include <stdio.h>
#include <iostream>
#include <math.h>

const int BYTES_PER_PIXEL = 4; /// red, green, blue, alpha
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

void generateBitmapImage(unsigned char* image, int height, int width, const char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int stride);
unsigned char* createBitmapInfoHeader(int height, int width);

using color_type = __uint32_t;
using channel_type = __u_short;

struct canvas {
    static const int width = 512;
    static const int height = 512;
    color_type m_data[width*height];
};

void gfx_buffer_draw(canvas* l_canvas, const char* l_filename);

#define GFX_GET_PIXEL_CHANNEL_R(pixel) ((pixel & 0x000000FF) >> (8 * 0))
#define GFX_GET_PIXEL_CHANNEL_G(pixel) ((pixel & 0x0000FF00) >> (8 * 1))
#define GFX_GET_PIXEL_CHANNEL_B(pixel) ((pixel & 0x00FF0000) >> (8 * 2))
#define GFX_GET_PIXEL_CHANNEL_A(pixel) ((pixel & 0xFF000000) >> (8 * 3))

color_type gfx_make_color(channel_type r, channel_type g, channel_type b, channel_type a) {
    return  (r << (8 * 0)) |
            (g << (8 * 1)) |
            (b << (8 * 2)) |
            (a << (8 * 3))
            ;
}

size_t gfx_canvas_index(int x, int y, int w, int h) {
    return (w*y) + x; // row-major?
}

color_type gfx_canvas_get_pixel(canvas* l_canvas, int x, int y) {
    return l_canvas->m_data[gfx_canvas_index(x, y, canvas::width, canvas::height)];
}

void gfx_draw_point(canvas* l_canvas, color_type c, int x, int y) {
    auto index = gfx_canvas_index(x, y, canvas::width, canvas::height);
    if (index < 0 || index > canvas::width*canvas::height) { return; }
    l_canvas->m_data[index] = c;
}

void gfx_canvas_fill(canvas* l_canvas, color_type l_fillColor) {
    for(int y = 0; y < canvas::height; ++y) {
        for(int x = 0; x < canvas::width; ++x) {
            gfx_draw_point(l_canvas, l_fillColor, x, y);
        }
    }
}

void gfx_draw_line(canvas* l_canvas, color_type col, int x1, int y1, int x2, int y2) {
    printf("line_call() - [%d,%d] - [%d,%d]\n", x1, y1, x2, y2);
    if(x1 >= canvas::width  || x1 < 0) { return; }
    if(x2 >= canvas::width  || x2 < 0) { return; }
    if(y1 >= canvas::height || y1 < 0) { return; }
    if(y2 >= canvas::height || y2 < 0) { return; }

    /*
    
    y = k*x + c # Line formula

    y1 = k*x1 + c
    y2 = k*x2 + c

    y1 - k*x1 = c # Found c
    y2                  = k*x2 + c
    y2                  = k*x2 + y1 - k*x1
    y2                  = k(x2 - x1) + y1
    y2 - y1             = k(x2 - x1)
    (y2 - y1)/(x2 - x1) = k

    k = dy / dx

    */

    //if(x1 > x2) { std::swap(x1, x2); }
    //if(y1 > y2) { std::swap(y1, y2); }

    auto dx = x2 - x1;
    auto dy = y2 - y1;

    //if((dx > dy && x1 > x2) || (dy > dx && y1 > y2)) { std::swap(x1, x2); std::swap(y1, y2); }

    float k = (float)dy / (float)dx;
    float c = y1 - (k*(float)x1);

    printf("[%d,%d] -> [%d,%d]\n", x1, y1, x2, y2);
    printf("dx: %d, dy: %d, k: %f, c: %f\n", dx, dy, k, c);

    if(dx == 0) {
        for(int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
            gfx_draw_point(l_canvas, col, x1, y);
        }
        return;
    }

    if(std::abs(dx) >= std::abs(dy)) {
        for(int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
            int y = k*(float)x + c;
            //printf("Plotting [%d,%d]", x, y);
            gfx_draw_point(l_canvas, col, x, y);
        }
    } else {
        for(int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
            int x = ((float)y - c) / k;
            //printf("Plotting [%d,%d]", x, y);
            gfx_draw_point(l_canvas, col, x, y);
        }
    }
}

void gfx_draw_rect(canvas* l_canvas, color_type c, int x1, int y1, int x2, int y2) {
    if(x1 >= canvas::width  || x1 < 0) { return; }
    if(x2 >= canvas::width  || x2 < 0) { return; }
    if(y1 >= canvas::height || y1 < 0) { return; }
    if(y2 >= canvas::height || y2 < 0) { return; }

    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            gfx_draw_point(l_canvas, c, x, y);
        }
    }
}

void gfx_draw_circle(canvas* l_canvas, color_type c, int cx, int cy, int r) {
    auto x1 = cx - r;
    auto x2 = cx + r;
    auto y1 = cy - r;
    auto y2 = cy + r;

    for(int y = y1; y <= y2; ++y) {
        if(0 <= y < canvas::height) {
            for(int x = x1; x <= x2; ++x) {
                if(0 <= x < canvas::width) {
                    auto dx = x - cx;
                    auto dy = y - cy;
                    if(dx*dx + dy*dy <= r*r) {
                        gfx_draw_point(l_canvas, c, x, y);
                    }
                }
            }
        }
    }
}

int main ()
{
    color_type clear_color = gfx_make_color(50, 50, 50, 255);
    color_type red = gfx_make_color(255, 0, 0, 255);
    
    #ifdef RUN_TESTS
        printf("Running tests...\n");
        canvas c;
        color_type rect_color = gfx_make_color(255, 0, 0, 255);
        color_type line_colors[3] = {
            gfx_make_color(255, 0, 0, 255),
            gfx_make_color(0, 255, 0, 255),
            gfx_make_color(0, 0, 255, 255)
        };

        gfx_canvas_fill(&c, clear_color);
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

        gfx_canvas_fill(&c, clear_color);
        int x1 = canvas::width / 2;
        int y1 = canvas::height / 2;
        int initialLength = 250;
        float lenInc = 0;
        for(float a = 0; a < 360; a += 0.2f) {
            int x2 = (float)x1 + ((float)initialLength + ((float)a * lenInc)) * std::cos((float)a);
            int y2 = (float)y1 + ((float)initialLength + ((float)a * lenInc)) * std::sin((float)a);

            auto color = gfx_make_color(
                255 * std::sin(a),
                255 * std::sin(a + 2*M_PI / 3.f),
                255 * std::sin(a + 4*M_PI / 3.f),
                255
            );
            gfx_draw_line(&c, color, x1, y1, x2, y2);

            gfx_draw_point(&c, red, x2, y2);
        }
        gfx_draw_point(&c, red, x1, y1);
        gfx_buffer_draw(&c, "docs/examples/color-wheel.bmp"); // For docs.


        gfx_canvas_fill(&c, clear_color);
        gfx_draw_rect(&c, rect_color, 100, 100, 300, 300);
        gfx_buffer_draw(&c, "docs/examples/rect.bmp"); // For docs.

        gfx_canvas_fill(&c, clear_color);
        gfx_draw_circle(&c, gfx_make_color(255, 0, 0, 255), 250, 250, 200);
        gfx_buffer_draw(&c, "docs/examples/circle.bmp"); // For docs.
    #else
        canvas c;
        color_type gray = gfx_make_color(50, 50, 50, 255);
        gfx_canvas_fill(&c, gray);

        int x1 = canvas::width / 2;
        int y1 = canvas::height / 2;
        int initialLength = 250;
        float lenInc = 0;
        for(float a = 0; a < 360; a += 0.2f) {
            int x2 = (float)x1 + ((float)initialLength + ((float)a * lenInc)) * std::cos((float)a);
            int y2 = (float)y1 + ((float)initialLength + ((float)a * lenInc)) * std::sin((float)a);

            auto color = gfx_make_color(
                255 * std::sin(a),
                255 * std::sin(a + 2*M_PI / 3.f),
                255 * std::sin(a + 4*M_PI / 3.f),
                255
            );
            gfx_draw_line(&c, color, x1, y1, x2, y2);

            gfx_draw_point(&c, red, x2, y2);
        }
        gfx_draw_point(&c, red, x1, y1);

        gfx_draw_line(&c, red, 256, 256, 0, 256);

        // Render here:



        // Final draw / save to file
        gfx_buffer_draw(&c, "out.bmp");
    #endif
}


void gfx_buffer_draw(canvas* l_canvas, const char* l_filename) {
    unsigned char image[canvas::height][canvas::width][BYTES_PER_PIXEL];
    int i, j;
    for (i = 0; i < canvas::height; i++) {
        for (j = 0; j < canvas::width; j++) {
            color_type pixel = gfx_canvas_get_pixel(l_canvas, j, i);
            image[i][j][2] = (unsigned char) GFX_GET_PIXEL_CHANNEL_R(pixel);
            image[i][j][1] = (unsigned char) GFX_GET_PIXEL_CHANNEL_G(pixel);
            image[i][j][0] = (unsigned char) GFX_GET_PIXEL_CHANNEL_B(pixel);
            image[i][j][3] = (unsigned char) GFX_GET_PIXEL_CHANNEL_A(pixel); // Full alpha

            //image[i][j][2] = (unsigned char) ( i * 255 / height );             ///red
            //image[i][j][1] = (unsigned char) ( j * 255 / width );              ///green
            //image[i][j][0] = (unsigned char) ( (i+j) * 255 / (height+width) ); ///blue
        }
    }

    generateBitmapImage((unsigned char*) image, canvas::height, canvas::width, l_filename);
    printf("Image generated!\n");
}

void generateBitmapImage (unsigned char* image, int height, int width, const char* imageFileName)
{
    int widthInBytes = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(height, stride);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    int i;
    for (i = 0; i < height; i++) {
        fwrite(image + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader (int height, int width)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}