#ifndef USER_THREAD_
#define USER_THREAD_

#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F "
#define STTY_DEF            "stty -raw echo -F "

typedef struct {
    unsigned int user_addr;
    unsigned int user_time;
    char user_name[32];
}user;

class UserThread : public Thread
{
public:
    void run();
    int get_char();
};

extern std::map<int, user>mapuser;

#endif /* USER_THREAD_ */
