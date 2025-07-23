extern void nthread_create(void *fn, void *vargp);
bool initialize_threads(uint32_t threadnums);
bool nthread_run(uint32_t threadnum);
void start(void);

