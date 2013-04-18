#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	struct lock lock;
	struct condition new_o;
	struct condition new_h;
	struct condition r_h;
	struct condition r_w;
	int num_h;
	int num_o;
	int num_w;
	int wip;
};

void
reaction_init(struct reaction *reaction)
{
	lock_init(&(reaction->lock));
	cond_init(&(reaction->new_o));
	cond_init(&(reaction->new_h));
	cond_init(&(reaction->r_h));
	cond_init(&(reaction->r_w));
	reaction->num_h = 0;
	reaction->num_o = 0;
	reaction->num_w = 0;
	reaction->wip   = 0;
}

void
reaction_h(struct reaction *reaction)
{
	lock_acquire(&(reaction->lock));
	reaction->num_h++;
	cond_signal(&(reaction->new_h), &(reaction->lock));
	cond_wait(&(reaction->new_o), &(reaction->lock));
	printf("enough o molecules: %d\n", reaction->num_o);
	reaction->num_h--;
	cond_signal(&(reaction->r_h), &(reaction->lock));
	lock_release(&(reaction->lock));
}

void
reaction_o(struct reaction *reaction)
{
	lock_acquire(&(reaction->lock));
	reaction->num_o++;
	if (reaction->wip == 1)
		cond_wait(&(reaction->r_w), &(reaction->lock));
	while ( reaction->num_h < 2 )
		cond_wait(&(reaction->new_h), &(reaction->lock));
	reaction->wip = 1;	
	printf("enough h molecules: %d\n", reaction->num_h);
	cond_signal(&(reaction->new_o), &(reaction->lock));
	cond_wait(&(reaction->r_h), &(reaction->lock));
	cond_signal(&(reaction->new_o), &(reaction->lock));
	cond_wait(&(reaction->r_h), &(reaction->lock));
	reaction->num_o--;
	reaction->num_w++;
	printf("Making water: %d\n", reaction->num_w);
	reaction->wip = 0;
	lock_release(&(reaction->lock));
	make_water();
}
