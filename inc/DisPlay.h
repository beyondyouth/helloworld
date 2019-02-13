#ifndef DISPLAY_H_
#define DISPLAY_H_

class DisPlay
{
public:
    DisPlay();
    ~DisPlay();
    int check_ncurse();
    WINDOW *create_win(int height,int width,int starty,int startx);
    void destroy_win(WINDOW *local_win);

    void create_fightwin();
private:
    WINDOW *fightwin;
};

#endif