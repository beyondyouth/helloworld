#ifndef USER_THREAD_
#define USER_THREAD_

#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F "
#define STTY_DEF            "stty -raw echo -F "

typedef struct {
    unsigned int user_time;
}S_user;

class UserThread : public Thread
{
public:
    void run();
    int get_char();
    int updateUserMap(void);
    int showUserMap(void);
};

extern std::map<uint32_t, S_user>usermap;

#endif /* USER_THREAD_ */
