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
    int mv(int y, int x);
    int mv_addch(int y, int x, char ch);
    int mv_addins(int y, int x, int d, char ch);
    void add_print(const char *cmd, ...);
private:
    
};

#endif /* DISPLAY_H_ */
