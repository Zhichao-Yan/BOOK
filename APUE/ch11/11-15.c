#include <pthread.h>
struct msg {
	struct msg *m_next;
	/* ... more stuff here ... */
};
struct msg *workq;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
void
process_msg(void)
{
	struct msg *mp;
	for (;;) {
		pthread_mutex_lock(&qlock); // 调用者把互斥量锁住
		while (workq == NULL) // 因为锁住了，所以其他线程想改变workq，没有办法
			pthread_cond_wait(&qready, &qlock); // 调用者把锁住的互斥量传递给函数
		mp = workq;
		workq = mp->m_next;
		pthread_mutex_unlock(&qlock);
		/* now process the message mp */
	}
}
void
enqueue_msg(struct msg *mp)
{
	pthread_mutex_lock(&qlock);
	mp->m_next = workq;
	workq = mp;
	pthread_mutex_unlock(&qlock);
	pthread_cond_signal(&qready);
}
