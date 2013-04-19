#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	struct lock lock;
	struct condition new_o;
	struct condition new_h;
	int num_h;
	int num_o;
	int num_w;
};

void
reaction_init(struct reaction *reaction)
{
	lock_init(&(reaction->lock));
	cond_init(&(reaction->new_o));
	cond_init(&(reaction->new_h));
	reaction->num_h = 0;
	reaction->num_o = 0;
	reaction->num_w = 0;
}

void
reaction_h(struct reaction *reaction)
{
	lock_acquire(&(reaction->lock));
	reaction->num_h++;
	cond_signal(&(reaction->new_h), &(reaction->lock));
	cond_wait(&(reaction->new_o), &(reaction->lock));
	lock_release(&(reaction->lock));
}

void
reaction_o(struct reaction *reaction)
{
	lock_acquire(&(reaction->lock));
	reaction->num_o++;
	while ( reaction->num_h < 2 )
		cond_wait(&(reaction->new_h), &(reaction->lock));
	cond_signal(&(reaction->new_o), &(reaction->lock));
	cond_signal(&(reaction->new_o), &(reaction->lock));
	reaction->num_o--;
	reaction->num_h-=2;
	reaction->num_w++;
	printf("Making water: o=%d, h=%d, w=%d\n", reaction->num_o, reaction->num_h, reaction->num_w);
	make_water();
	lock_release(&(reaction->lock));
}
