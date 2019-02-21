#ifndef DISPLAY_H_
#define DISPLAY_H_


class Display
{
public:
    Display();
    ~Display();
    void refresh();
    void draw_map(void);
    int get_char(void);
    int move(int y, int x);
    int moveaddch(int y, int x, char ch);
    int movedelch(int y, int x);
    int addprintf(char *str);
private:
    
};

#endif /* DISPLAY_H_ */
