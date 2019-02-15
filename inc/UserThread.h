#ifndef USER_THREAD_
#define USER_THREAD_

#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F "
#define STTY_DEF            "stty -raw echo -F "

class UserThread : public Thread
{
public:
    void run();
    int get_char();
};

#endif /* USER_THREAD_ */