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
int tiles[TC][9] = {
    { //BLANK 0
        0,0,0,
        0,0,0,
        0,0,0,
    },
    { //UP 1
        0,1,0,
        1,1,1,
        0,0,0,
    },
    { //RIGHT 2
        0,1,0,
        0,1,1,
        0,1,0,
    },
    { //LEFT 3
        0,1,0,
        1,1,0,
        0,1,0,
    },
    { //DOWN 4
        0,0,0,
        1,1,1,
        0,1,0,
    },
    
    {
        0,0,0,
        1,1,1,
        0,0,0,
    },
    {
        0,1,0,
        0,1,0,
        0,1,0,
    },
    {
        0,1,0,
        1,1,1,
        0,1,0,
    },
};
void drawTile(int x, int y, int t)
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            box(x*(HEIGHT/GD)+i*(WIDTH/GD)/3, y*(HEIGHT/GD)+j*(HEIGHT/GD)/3, (WIDTH/GD)/3, (HEIGHT/GD)/3, 0x00AA00 * tiles[t][i+(2-j)*3]);
        }
    }
}

struct tile
{
    int* left;
    int* right;
    int* top;
    int* bottom;
    int id;
};
struct tile* tileRules = 0;

void assertRules()
{
    struct tile new = {
        .left = 0,
        .right = 0,
        .top = 0,
        .bottom = 0,
        .id = 0,
    };
    for(int i = 0; i < TC; i++) nec_push(tileRules, new);
    for(int i = 0; i < TC; i++)
    {
        tileRules[i].id = i;
        //1 - top
        //3 - left
        //5 - right
        //7 - down
        for(int j = 0; j < TC; j++)
        {
            if(tiles[i][1] == tiles[j][7])
            {
                nec_push(tileRules[i].top, j);
            }
            if(tiles[i][3] == tiles[j][5])
            {
                nec_push(tileRules[i].left, j);
            }
            if(tiles[i][5] == tiles[j][3])
            {
                nec_push(tileRules[i].right, j);
            }
            if(tiles[i][7] == tiles[j][1])
            {
                nec_push(tileRules[i].bottom, j);
            }
        }
    }
}

void place(int i, int j)
{
    if(grid[i][j] != -1) return;
    // if(i == 4 && j == 1)
    // {
    //     if(i - 1 >= 0) if(grid[i-1][j] != -1) { printf("LEFT "); for(int t = 0; t < nec_size(tileRules[grid[i-1][j]].right); t++) printf("%d ", tileRules[grid[i-1][j]].right[t]); printf("\n"); }
    //     if(i + 1 < GD) if(grid[i+1][j] != -1) { printf("RIGHT "); for(int t = 0; t < nec_size(tileRules[grid[i+1][j]].left); t++) printf("%d ", tileRules[grid[i+1][j]].left[t]); printf("\n"); }
    //     if(j - 1 >= 0) if(grid[i][j-1] != -1) { printf("BOTTOM "); for(int t = 0; t < nec_size(tileRules[grid[i][j-1]].top); t++) printf("%d ", tileRules[grid[i][j-1]].top[t]); printf("\n"); }
    //     if(j + 1 < GD) if(grid[i][j+1] != -1) { printf("TOP "); for(int t = 0; t < nec_size(tileRules[grid[i][j+1]].bottom); t++) printf("%d ", tileRules[grid[i][j+1]].bottom[t]); printf("\n"); }
    // }
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

    for(int i = 0; i < nec_size(solutions); i++)
    {
        if(i == 0) printf("FOR(%d %d) = ", i, j);
        printf("%d ", solutions[i]);
        if(i == nec_size(solutions)-1) printf("\n");
    }
    if(nec_size(solutions) > 1) grid[i][j] = solutions[rand() % nec_size(solutions)];
    else if(nec_size(solutions) == 1) grid[i][j] = solutions[0];
    else grid[i][j] = rand() % TC;

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
    return 0;
}

int main(int argc, char** argv)
{
    assert(WIDTH*HEIGHT % GD == 0);
    createWindow("Window", WIDTH<<1, HEIGHT<<1);
    if(!window) return 1;
    glPointSize(2.0);

    assertRules();

    // for(int i = 0; i < nec_size(tileRules); i++)
    // {
    //     printf("Left[");
    //     for(int j = 0; j < nec_size(tileRules[i].left); j++) printf(j == nec_size(tileRules[i].left)-1 ? "%d" : "%d, ", tileRules[i].left[j]);
    //     printf("]\nRight[");
    //     for(int j = 0; j < nec_size(tileRules[i].right); j++) printf(j == nec_size(tileRules[i].right)-1 ? "%d" : "%d, ", tileRules[i].right[j]);
    //     printf("]\nTop[");
    //     for(int j = 0; j < nec_size(tileRules[i].top); j++) printf(j == nec_size(tileRules[i].top)-1 ? "%d" : "%d, ", tileRules[i].top[j]);
    //     printf("]\nBottom[");
    //     for(int j = 0; j < nec_size(tileRules[i].bottom); j++) printf(j == nec_size(tileRules[i].bottom)-1 ? "%d" : "%d, ", tileRules[i].bottom[j]);
    //     printf("]\n\n");
    // }

    srand(time(0));

    place(0, 0);
    // for(int i = 0; i < GD; i++)
    // {
    //     for(int j = 0; j < GD; j++)
    //     {
    //         printf("%d ", grid[j][GD-1-i]);
    //     }
    //     printf("\n");
    // }
    
    start(loop);
    return 0;
}
