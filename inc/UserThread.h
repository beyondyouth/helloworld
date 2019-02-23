#ifndef USER_THREAD_
#define USER_THREAD_


typedef struct {
    unsigned int user_time;
}S_user;

typedef struct bullet_data
{
    int x;
    int y;
    int d;
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
    int updateUserMap(void);
    int showUserMap(void *tmp);
    int create_bullet_list();
    int insert_bullet_list(int y, int x, int d);
    int move_bullet_list(Display& ins);
    int destroy_bullet_list();
private:
    bullet_list_t _head;
    int _cols;
    int _lines;
};

extern std::map<uint32_t, S_user>usermap;

#endif /* USER_THREAD_ */
