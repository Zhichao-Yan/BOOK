#include <stdlib.h>
#include <pthread.h>

struct foo {
	int             f_count;
	pthread_mutex_t f_lock;
	int             f_id;
	/* ... more stuff here ... */
};
// 是一个动态的分配的对象
// 我们需要让它能够自动释放
// 所以我们给它设置了一个引用计数
// 当引用计数为0的时候，给它释放
// 当引用计数不为0的时候，我们不能释放，确保别人在使用的时候，不能释放
// 但是引用计数的使用，需要保持一致性
// 所以我们同时引入了一个互斥量，必须互斥访问引用计数

// 另外构建函数

struct foo *
foo_alloc(int id) /* allocate the object */
{
	struct foo *fp;

	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id = id;
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return(NULL);
		}
		/* ... continue initialization ... */
	}
	return(fp);
}

void
foo_hold(struct foo *fp) /* add a reference to the object */
{
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}

void
foo_rele(struct foo *fp) /* release a reference to the object */
{
	pthread_mutex_lock(&fp->f_lock);
	if (--fp->f_count == 0) { /* last reference */
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		pthread_mutex_unlock(&fp->f_lock);
	}
}


void* children(void * arg)
{
    foo_hold((struct foo*)arg);
    
    foo_rele((struct foo*)arg);
}

int main()
{
    int id = 2000;
    struct foo *fp =foo_alloc(id);
    pthread_t tid[5];
    for(int i = 0 ;i < 5;++i)
    {
        pthread_create(&tid[i],NULL,children,(void*)fp);
    }
}