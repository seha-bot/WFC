#include<gl_utils.h>
#include<math.h>
#include<time.h>
#include<assert.h>

struct dl
{
    int v;
    struct dl* next;
};
typedef struct dl dl;

dl* sud[9][9] = { [0 ... 9-1] = { [0 ... 9-1] = 0 } };

dl* dl_remove(dl* a, int v)
{
    if(!a) return 0;
    if(a->v == v) return a->next;
    dl* s = a;
    while(a->next)
    {
        if(a->next->v == v)
        {
            a->next = a->next->next;
            return s;
        }
        a = a->next;
    }
    return s;
}

int dl_size(dl* a)
{
    if(!a) return 0;
    int s = 0;
    while(a->next)
    {
        s++;
        a = a->next;
    }
    return s + 1;
}

void affect(int x, int y, int v)
{
    if(!sud[x][y]->next) return;
    sud[x][y]->next = 0;
    sud[x][y]->v = v;

    //1) REMOVE LOCAL GRID ENTROPY
    int sX = (x/3)*3;
    int sY = (y/3)*3;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(sX+i == x && sY+j == y) continue;
            sud[sX+i][sY+j] = dl_remove(sud[sX+i][sY+j], v);
        }
    }

    //2) REMOVE AXIS ENTROPY
    for(int i = 0; i < 9; i++)
    {
        if(i != x) sud[i][y] = dl_remove(sud[i][y], v);
        if(i != y) sud[x][i] = dl_remove(sud[x][i], v);
    }

    //3) FIND LEAST ENTROPY
    int idX = -1;
    int idY = -1;
    int minV = 10;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            int s = dl_size(sud[i][j]);
            if(s < minV && s != 1)
            {
                minV = s;
                idX = i;
                idY = j;
            }
        }
    }

    if(idX == -1)
    {
        printf("ERROR\n");
        return;
        // exit(1);
    }

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(sud[i][j]->next) printf("- ");
            else printf("%d ", sud[i][j]->v);
        }
        printf("\n");
    }
    printf("\n");

    affect(idX, idY, sud[idX][idY]->v);
}

int main(int argc, char** argv)
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            dl* mem = malloc(sizeof(dl) * 9);
            for(int j = 0; j < 9; j++)
            {
                mem[j].v = j+1;
                if(j != 8) mem[j].next = &mem[j+1];
            }
            sud[i][j] = mem;
        }
    }
    affect(0, 0, 1);

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            printf("%d ", sud[i][j]->v);
            // printf("%d ", dl_size(sud[i][j]) + 0);
        }
        printf("\n");
    }

    // affect(0, 1, 1);
    // affect(0, 2, 1);

    // for(int i = 0; i < 9; i++)
    // {
    //     for(int j = 0; j < 9; j++)
    //     {
    //         printf("%d ", sud[j+i*9]);
    //     }
    //     printf("\n");
    // }

    return 0;
}
