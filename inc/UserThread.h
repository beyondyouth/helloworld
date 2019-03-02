#ifndef USER_THREAD_
#define USER_THREAD_


typedef struct {
    unsigned int user_time;
}S_user;

typedef struct bullet_data
{
    int x;
    int y;
    dir d;
}bullet_data_t;

typedef struct bullet_list
{
    bullet_data_t data;
    struct bullet_list *next;
}bullet_list_t;

class UserThread : public Thread
{
public:
    void run();
    int get_char();
    int ageingUserMap(void);
    int showUserMap(Display& InsDisplay, int y, int x);
    int select_loop(void);
    int fight_loop(void);
    int gameover_loop(void);
 
    int move_myself_bullet_list(Display& ins);
    int move_others_bullet_list(Display& ins);
private:
    List *_myself_bullet_list;
    List *_others_bullet_list;
    int _cols;
    int _lines;
};

extern std::map<uint32_t, S_user>usermap;

#endif /* USER_THREAD_ */
