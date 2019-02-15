#ifndef MUTEX_H_
#define MUTEX_H_


class Mutex
{
public:
    Mutex();
    virtual ~Mutex();
    virtual int lock();
    virtual int trylock();
    virtual int unlock();
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
