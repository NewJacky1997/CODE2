#include<stdio.h>
#include<stdlib.h>
//Define node content 
struct node{
	int x;
	int y;
	int step;
	struct node *left;
	struct node *right;
	struct node *high;
	struct node *low;
};
typedef struct node NODE;

struct record_head{
	NODE *node;
	struct record_head *next;
};
typedef struct record_head RH;

NODE *new_node();
NODE *first_horizontal_node(NODE *before_node);
NODE *first_straight_node(NODE *before_node);
NODE *fill_up_node(NODE *before_node);
void remove_node(NODE *target_node);
void run_node(NODE* head);
NODE *extend_node(NODE *before_node, char key_command);
RH *make_rnode(RH *before_rnode, NODE* save_node);
int where_closest(RH *rhead, int tx, int ty);
void navigate_dijkstra(NODE *head, int tx, int ty);
RH* RH_list(RH *move_use,RH *l_head,RH *r_head,int *best_step,int tx,int ty,NODE **goal_node);
NODE *sup_OR(NODE *head,int direction);
void output_route(NODE *head);

NODE *new_node(){//make the first node 
	NODE *first = malloc(sizeof(NODE));
	first->x = 0;
	first->y = 0;
	first->step = 0;
	first->left = NULL;
	first->right = NULL;
	first->high = NULL;
	first->low = NULL;
	return first;
}

RH *make_rnode(RH *before_rnode, NODE* save_node){//Add new record node 1. Previous record node 2. Node location to be saved 
	RH *rhead = (RH*)malloc(sizeof(RH));
	rhead->node = save_node;
	rhead->next = NULL;
	if(before_rnode!=NULL){before_rnode->next = rhead;}
	return rhead;
}

NODE *first_horizontal_node(NODE *before_node){//Make the first row of nodes
	NODE *next_node = new_node();
	next_node->x = before_node->x + 1;
	next_node->y = before_node->y;
	next_node->left = before_node;
	before_node->right = next_node;
	return next_node;
}

NODE *first_straight_node(NODE *before_node){//Make the first col of nodes
	NODE *next_node = new_node();
	next_node->x = before_node->x;
	next_node->y = before_node->y + 1;
	next_node->low = before_node;
	before_node->high = next_node;
	return next_node;
}

NODE *fill_up_node(NODE *before_node){//Make nodes after the second row
	NODE *next_node = new_node();
	next_node->x = before_node->x + 1;
	next_node->y = before_node->y;
	next_node->left = before_node;
	before_node->right = next_node;
	next_node->low = before_node->low->right;
	before_node->low->right->high = next_node;
	return next_node;
}

void remove_node(NODE *target_node){//delete the specified node 
	if(target_node->high != NULL){target_node->high->low = NULL;}
	if(target_node->low != NULL){target_node->low->high = NULL;}
	if(target_node->left != NULL){target_node->left->right = NULL;}
	if(target_node->right != NULL){target_node->right->left = NULL;}
	printf("remove ( %d , %d ) success\n",target_node->x,target_node->y);
	free(target_node);
}

void run_node(NODE* head){//Use W, A, S, D to move between nodes, R and then delete the direction node by direction 
	char key_command = 'c';
	int tx,ty;
	while(key_command != 'q'){
		key_command = getch();
		if(key_command=='w'){
			if(head->high!=NULL){head = head->high;}
			else{printf("unable to move forward\n");}
		}
		else if(key_command=='a'){
			if(head->left!=NULL){head = head->left;}
			else{printf("unable to move forward\n");}
		}
		else if(key_command=='s'){
			if(head->low!=NULL){head = head->low;}
			else{printf("unable to move forward\n");}
		}
		else if(key_command=='d'){
			if(head->right!=NULL){head = head->right;}
			else{printf("unable to move forward\n");}
		}
		else if(key_command=='r'){//delete node
			printf("choose direction to remove\n");
			key_command = getch();
			if(key_command=='w'){
				if(head->high!=NULL){remove_node(head->high);}
				else{printf("it's already NULL'\n");}
			}
			else if(key_command=='a'){
				if(head->left!=NULL){remove_node(head->left);}
				else{printf("it's already NULL'\n");}
			}
			else if(key_command=='s'){
				if(head->low!=NULL){remove_node(head->low);}
				else{printf("it's already NULL'\n");}
			}
			else if(key_command=='d'){
				if(head->right!=NULL){remove_node(head->right);}
				else{printf("it's already NULL'\n");}
			}
		}
		else if(key_command=='e'){//new node
			printf("nothing\n");
		}
		else if(key_command=='t'){//search for directions
			printf("enter target location x y:");
			scanf("%d %d",&tx,&ty);
			navigate_dijkstra(head,tx,ty);
			
		}
		printf("now location is ( %d , %d )\n",head->x,head->y,head->step);
	}
	printf("break\n");
}

RH* RH_list(RH *move_use,RH *l_head,RH *r_head,int *best_step,int tx,int ty,NODE **goal_node){//Record the best route times, return the last position of the queue, and record the end node
	if(move_use->node != NULL){
		if(move_use->node->step == 0 && l_head->node->step + 1 <= *best_step){
			move_use->node->step = l_head->node->step + 1;
			r_head = make_rnode(r_head,move_use->node);
		} 
		if(move_use->node->x == tx && move_use->node->y == ty && *best_step > move_use->node->step){
			*best_step = move_use->node->step;
			*goal_node = move_use->node;
		}
	}
	return r_head;
}

NODE *sup_OR(NODE *head,int direction){//look_node : head to explore nearby, if you are sure you can go, let the head move there
	NODE *look_node;
	switch(direction){
		case 1:
			look_node = head->high;
			break;
		case 2:
			look_node = head->low;
			break;
		case 3:
			look_node = head->right;
			break;
		case 4:
			look_node = head->left;
			break;
	}
	if(look_node == NULL)
		return head;
	if(look_node->step == head->step-1){
		head = look_node;
		printf("location( %d , %d ) step:%d\n",head->x,head->y,head->step);
		if(head->step == 1){
			printf("FINISH\n");
			return NULL;
		}
	}
	return head;
}

void output_route(NODE *head){//output shortest path
	int direction = 0;
	printf("route:\n");
	printf("location( %d , %d ) step:%d\n",head->x,head->y,head->step);
	while(head != NULL){
		direction++;
		direction = direction > 4 ? 1 : direction;
		head = sup_OR(head,direction);
	}
}

void delete_steps(NODE *goal_node){//delete marker
	RH *head = make_rnode(NULL, goal_node);
	RH *last_node = head;
	RH *temp_node;
	while(head != NULL){
		head->node->step = 0;
		if(head->node->high != NULL){if(head->node->high->step != 0){last_node = make_rnode(last_node, head->node->high);}}
		if(head->node->low != NULL){if(head->node->low->step != 0){last_node = make_rnode(last_node, head->node->low);}}
		if(head->node->left != NULL){if(head->node->left->step != 0){last_node = make_rnode(last_node, head->node->left);}}
		if(head->node->right != NULL){if(head->node->right->step != 0){last_node = make_rnode(last_node, head->node->right);}}
		
		temp_node = head;
		head = head->next;
		
		free(temp_node);
	}
}

void navigate_dijkstra(NODE *head, int tx, int ty){//navigate
	RH *n_head = make_rnode(NULL, head); //Fork in the road queue begins for myself
	RH *l_head = n_head;//start of queue
	RH *r_head = n_head;//end of queue
	RH *move_use, *temp_use;
	NODE *goal_node = NULL;
	move_use = (RH*)malloc(sizeof(RH));
	int best_step = INT_MAX;
	n_head->node->step = 1; 
	while(l_head != NULL){
		move_use->node = l_head->node->high;
		r_head = RH_list(move_use,l_head,r_head,&best_step,tx,ty,&goal_node);
		move_use->node = l_head->node->low;
		r_head = RH_list(move_use,l_head,r_head,&best_step,tx,ty,&goal_node);
		move_use->node = l_head->node->right;
		r_head = RH_list(move_use,l_head,r_head,&best_step,tx,ty,&goal_node);
		move_use->node = l_head->node->left;
		r_head = RH_list(move_use,l_head,r_head,&best_step,tx,ty,&goal_node);

		temp_use = l_head;
		l_head = l_head->next;
		free(temp_use);
	}
	if(best_step == INT_MAX)
		printf("can not reach\n");
	else{
		output_route(goal_node);
		printf("best_step=%d\n",best_step);
	}
	delete_steps(head);	
	return;
}

int main(void){
	NODE *head = new_node();
	NODE *move = head,*move2 = head;
	int sizex,sizey;
	printf("enter matrix size x y : ");
	scanf("%d %d",&sizex,&sizey);
	sizex--;
	sizey--;
	int i = 0;
	for(i = 0; i < sizex; i++){//Make the first row of nodes
		move = first_horizontal_node(move);
	}
	move = head;
	for(i = 0; i < sizey; i++){//Make the first column of nodes
		move = first_straight_node(move);
	}
	for(move2 = head->high ; move2 != NULL ; move2 = move2->high){//Complete the remaining nodes
		for(move = move2, i = 0; i < sizex ; i++){
			move = fill_up_node(move);
		}
	}
	run_node(head);
	
	return 0;
}
