#ifndef COL_COLLIDABLE_H
#define COL_COLLIDABLE_H

namespace col
{
    extern char underRock;
    extern int zombieToPodIndex;

    void OnCollide(char col, int x, int y);
}

#endif