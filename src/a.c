#include<gl_utils.h>
#include<math.h>
#include<time.h>
#include<assert.h>

void box(int x, int y, int w, int h, u_int32_t c)
{
    for(int i = x; i <= x+w; i++)
    {
        for(int j = y; j <= y+h; j++)
        {
            pixel(i, j, c);
        }
    }
}

#define WIDTH 250
#define HEIGHT 250
#define HW 125
#define HH 125

#define GD 50
int grid[GD][GD] = { [0 ... GD-1] = {[0 ... GD-1] = -1} };

#define TC 8
#define TD 5
int tiles[TC][TD*TD] = {
    {
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
    },{
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
    },{
        0,0,0,0,0,
        0,0,0,0,0,
        1,1,1,1,1,
        0,0,0,0,0,
        0,0,0,0,0,
    },{
        0,0,1,0,0,
        0,0,1,0,0,
        1,1,1,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
    },{
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,1,1,
        0,0,0,0,0,
        0,0,0,0,0,
    },{
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,1,1,1,
        0,0,1,0,0,
        0,0,1,0,0,
    },{
        0,0,0,0,0,
        0,0,0,0,0,
        1,1,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
    },
};
void drawTile(int x, int y, int t)
{
    for(int i = 0; i < TD; i++)
    {
        for(int j = 0; j < TD; j++)
        {
            box(x*(HEIGHT/GD)+i*(WIDTH/GD)/TD, y*(HEIGHT/GD)+j*(HEIGHT/GD)/TD, (WIDTH/GD)/TD, (HEIGHT/GD)/TD, 0x00AA00 * tiles[t][i+(TD-1-j)*TD]);
        }
    }
}

struct tile
{
    int* left;
    int* right;
    int* top;
    int* bottom;
};
struct tile* tileRules = 0;

void assertRules()
{
    struct tile new = {
        .left = 0,
        .right = 0,
        .top = 0,
        .bottom = 0,
    };
    for(int i = 0; i < TC; i++) nec_push(tileRules, new);
    for(int i = 0; i < TC; i++)
    {
        for(int j = 0; j < TC; j++)
        {
            char can = 1;
            for(int s = 0; s < TD; s++)
            {
                if(tiles[i][s] != tiles[j][TD*TD-TD+s])
                {
                    can = 0;
                    break;
                }
            }
            if(can)
            {
                nec_push(tileRules[i].top, j);
                nec_push(tileRules[j].bottom, i);
            }

            can = 1;
            for(int s = 0; s < TD*TD; s+=TD)
            {
                if(tiles[i][s] != tiles[j][s+TD-1])
                {
                    can = 0;
                    break;
                }
            }
            if(can)
            {
                nec_push(tileRules[i].left, j);
                nec_push(tileRules[j].right, i);
            }
        }
    }
}

void place(int i, int j)
{
    if(grid[i][j] != -1) return;
    int** possible = 0;
    if(i - 1 >= 0) if(grid[i-1][j] != -1) nec_push(possible, tileRules[grid[i-1][j]].right);
    if(i + 1 < GD) if(grid[i+1][j] != -1) nec_push(possible, tileRules[grid[i+1][j]].left);
    if(j - 1 >= 0) if(grid[i][j-1] != -1) nec_push(possible, tileRules[grid[i][j-1]].top);
    if(j + 1 < GD) if(grid[i][j+1] != -1) nec_push(possible, tileRules[grid[i][j+1]].bottom);
    int* solutions = 0;
    if(nec_size(possible) > 1)
    {
        for(int s = 0; s < nec_size(possible); s++)
        {
            for(int c = 0; c < nec_size(possible[s]); c++)
            {
                for(int s1 = 0; s1 < nec_size(possible); s1++)
                {
                    for(int c1 = 0; c1 < nec_size(possible[s1]); c1++)
                    {
                        if(s1 == s && c1 == c) continue;
                        if(possible[s][c] == possible[s1][c1])
                        {
                            if(!nec_contains(solutions, possible[s][c]))
                            {
                                nec_push(solutions, possible[s][c]);
                            }
                        }
                    }
                }
            }
        }
    }
    else if(nec_size(possible) == 1) solutions = possible[0];

    if(nec_size(solutions)) grid[i][j] = solutions[rand() % nec_size(solutions)];
    else grid[i][j] = rand() % TC;

    // if(nec_size(possible) > 1) nec_free(solutions);

    if(i - 1 >= 0) place(i-1, j);
    if(i + 1 < GD) place(i+1, j);
    if(j - 1 >= 0) place(i, j-1);
    if(j + 1 < GD) place(i, j+1);
}

int loop()
{
    for(int i = 0; i < GD; i++)
    {
        for(int j = 0; j < GD; j++)
        {
            drawTile(i, j, grid[i][j]);
        }
    }
    if(getKeyDown(GLFW_KEY_ENTER))
    {
        for(int i = 0; i < GD; i++) for(int j = 0; j < GD; j++) grid[i][j] = -1;
        place(0, 0);
    }
    return 0;
}

int main(int argc, char** argv)
{
    assert(WIDTH*HEIGHT % GD == 0);
    createWindow("Window", WIDTH<<1, HEIGHT<<1);
    if(!window) return 1;
    glPointSize(2.0);

    srand(time(0));
    assertRules();
    place(0, 0);
    
    start(loop);
    return 0;
}
