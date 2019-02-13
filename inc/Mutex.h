#ifndef MUTEX_H_
#define MUTEX_H_


class Mutex
{
public:
    Mutex();
    virtual ~Mutex();
    virtual bool lock();
    virtual bool trylock();
    virtual bool unlock();
    virtual pthread_mutex_t getId();

private:
    pthread_mutex_t _mutexId;
};

class MutexLocker
{
public:
	MutexLocker(Mutex* mutex);
	~MutexLocker();

protected:
	Mutex* _mutex;
};
#endif /* MUTEX_H_ */
