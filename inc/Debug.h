#ifndef DEBUG_H_
#define DEBUG_H_

#define DBG

#ifdef DBG
#define NVM_PREFIX		"[NV] : "

#define eprintf(fmt, args...)       \
do {								\
    printf("%s(%d) " fmt, __func__, __LINE__, ##args); \
} while (0);

#define dprintf(fmt, args...)					\
do {								\
    printf("%s(%d) " fmt, __func__, __LINE__, ##args); \
} while (0);
#else

#define eprintf(fmt, args...)					\
do {								\
	syslog(LOG_ERR, "%s(%d) " fmt, __func__, __LINE__, ##args);	\
} while (0)
#define dprintf(fmt, args...) do{}while(0);

#endif	// DBG

#endif /* DEBUG_H_ */