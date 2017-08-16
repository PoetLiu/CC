#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#define ARG_MAX	131072

static pthread_key_t	key;
static pthread_once_t	init_done	= PTHREAD_ONCE_INIT;
pthread_mutex_t	env_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char **environ;

static void thread_init(void)
{
	pthread_key_create(&key, free);
}

char *mygetenv(const char *name)
{
	int	i, len;
	char	*envbuf;

	pthread_once(&init_done, thread_init);
	pthread_mutex_lock(&env_mutex);
	envbuf	= (char *)pthread_getspecific(key);
	if (envbuf == NULL) {
		envbuf	= malloc(ARG_MAX);
		if (envbuf == NULL){
			pthread_mutex_unlock(&env_mutex);
			return(NULL);
		}
	}

	len	= strlen(name);
	for (i = 0; environ[i] != NULL; i++) {
		if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '=')) {
			strcpy(envbuf, &environ[i][len+1]);
			pthread_mutex_unlock(&env_mutex);
			return (envbuf);
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return(NULL);
}

int main(int argc, char *argv[])
{
	int	i;
	char 	*ret;

	if (argc < 2) {
		printf("usage: %s <environ name>\n", argv[0]);
		return 1;
	}

	for (i = 0; i < argc - 1; i++) {
		if((ret = mygetenv(argv[i+1])) == NULL)
			ret	= "can't be found in the environ!";
		printf("%s:%s\n", argv[i+1], ret);
	}

	for (i = 0; environ[i] != NULL; i++)
		printf("%s\n", *(environ+i));
	return 0;
}
