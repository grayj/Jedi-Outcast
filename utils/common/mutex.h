

typedef void *mutex_t;

void MutexLock (mutex_t *m);
void MutexUnlock (mutex_t *m);
mutex_t *MutexAlloc(void);
