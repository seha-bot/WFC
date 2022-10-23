#include<gl_utils.h>
#include<math.h>

#define WIDTH 250
#define HEIGHT 250
#define HW 125
#define HH 125


int loop()
{
    return 0;
}

int main(int argc, char** argv)
{
    createWindow("Window", WIDTH<<1, HEIGHT<<1);
    if(!window) return 1;
    glPointSize(2.0);
    
    start(loop);
    return 0;
}
