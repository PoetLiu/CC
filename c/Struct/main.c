#include <stdio.h>

struct list_head {
	struct	list_head *next, *prev;
};

struct bc_user {
	struct	list_head list;
	int 	status;
	char	*path;
};

struct bc_param {
	int	enable;
	struct	list_head users;
};

struct bc_user user1, user2;
struct bc_param baidu_cloud;


#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))


#define list_for_each_entry(pos, head, member)					\
	for (pos = list_entry((head)->next, typeof(*pos), member);      	\
		prefetch(pos->member.next), &pos->member != (head);        	\
		pos = list_entry(pos->member.next, typeof(*pos), member))

static inline void prefetch(const void *x) {;}
static inline void __list_add(struct list_head *new,
				struct list_head *prev,
				struct list_head *next)
{
	next->prev	= new;
	new->next	= next;
	new->prev	= prev;
	prev->next	= new;
}
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	 __list_add(new, head->prev, head);
}

int main(void)
{

	struct bc_user * user;

	user1.status		= 1;
	user1.path		= "/home/user1";
	user2.status		= 1;
	user2.path		= "/home/user2";
	baidu_cloud.users.prev	= &baidu_cloud.users;
	baidu_cloud.users.next	= &baidu_cloud.users;

	list_add_tail(&user1.list, &baidu_cloud.users);
	list_add_tail(&user2.list, &baidu_cloud.users);
	list_for_each_entry(user, &baidu_cloud.users, list) {
		printf("%d %s\n", user->status, user->path);
	}

	printf("%ld\n", (unsigned long)(&((struct bc_user *)0)->path));
	return 0;
}
