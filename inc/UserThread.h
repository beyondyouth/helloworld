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
    int create_myself_bullet_list();
    int insert_myself_bullet_list(int y, int x, dir d);
    int move_myself_bullet_list(Display& ins);
    int destroy_myself_bullet_list();

    int create_others_bullet_list();
    int insert_others_bullet_list(int y, int x, dir d);
    int move_others_bullet_list(Display& ins);
    int destroy_others_bullet_list();
private:
    bullet_list_t _myself_bullet_list_head;
    bullet_list_t _others_bullet_list_head;
    int _cols;
    int _lines;
};

extern std::map<uint32_t, S_user>usermap;

#endif /* USER_THREAD_ */
