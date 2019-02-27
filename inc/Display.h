#ifndef DISPLAY_H_
#define DISPLAY_H_


class Display
{
public:
    Display();
    ~Display();
    void clean();
    void refresh();
    void fight_map(void);
    int get_char(void);
    int mv_addch(int y, int x, char ch);
    int mv_addins(int y, int x, int d, char ch);
    int mv_printf(int y, int x, const char *cmd, ...);
private:
    
};

#endif /* DISPLAY_H_ */
