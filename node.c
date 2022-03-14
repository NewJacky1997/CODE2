#include<stdio.h>
#include<stdlib.h>
//定義節點內容 
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

NODE *new_node(){//製作第一個節點 
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

RH *make_rnode(RH *before_rnode, NODE* save_node){//添加新的紀錄節點 1.前一個紀錄節點 2.要存的節點位置 
	RH *rhead = (RH*)malloc(sizeof(RH));
	rhead->node = save_node;
	rhead->next = NULL;
	if(before_rnode!=NULL){before_rnode->next = rhead;}
	return rhead;
}

NODE *first_horizontal_node(NODE *before_node){//製作第一排節點
	NODE *next_node = new_node();
	next_node->x = before_node->x + 1;
	next_node->y = before_node->y;
	next_node->left = before_node;
	before_node->right = next_node;
	return next_node;
}

NODE *first_straight_node(NODE *before_node){//製作第一行節點
	NODE *next_node = new_node();
	next_node->x = before_node->x;
	next_node->y = before_node->y + 1;
	next_node->low = before_node;
	before_node->high = next_node;
	return next_node;
}

NODE *fill_up_node(NODE *before_node){//製作弟二排以後的節點
	NODE *next_node = new_node();
	next_node->x = before_node->x + 1;
	next_node->y = before_node->y;
	next_node->left = before_node;
	before_node->right = next_node;
	next_node->low = before_node->low->right;
	before_node->low->right->high = next_node;
	return next_node;
}

void remove_node(NODE *target_node){//刪除指定節點， 
	if(target_node->high != NULL){target_node->high->low = NULL;}
	if(target_node->low != NULL){target_node->low->high = NULL;}
	if(target_node->left != NULL){target_node->left->right = NULL;}
	if(target_node->right != NULL){target_node->right->left = NULL;}
	printf("remove ( %d , %d ) success\n",target_node->x,target_node->y);
	free(target_node);
}

void run_node(NODE* head){//使用W、A、S、D在節點之間移動、R再按方向刪除該方向節點 
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
		else if(key_command=='r'){//刪除節點 
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
		else if(key_command=='e'){//新增節點 
			/*printf("choose direction to extend\n");
			key_command = getch();
			head = extend_node(head,key_command); 先不要，不知道怎麼找到對面的節點連結*/
			printf("nothing\n");
		}
		else if(key_command=='t'){//搜尋路線
			printf("enter target location x y:");
			scanf("%d %d",&tx,&ty);
			navigate_dijkstra(head,tx,ty);
			
		}
		//printf("now location is ( %d , %d ) step is ( %d )\n",head->x,head->y,head->step);
		printf("now location is ( %d , %d )\n",head->x,head->y,head->step);
	}
	printf("break\n");
}

NODE *extend_node(NODE *before_node, char key_command){//增加新節點(先不要)
	NODE *next_node = new_node();
	int addx=0,addy=0;
	switch(key_command){
		case 'w':
			addy = 1;
			next_node->low = before_node;
			before_node->high = next_node;
			break;
		case 'a':
			addx = -1;
			next_node->right = before_node;
			before_node->left = next_node;
			break;
		case 's':
			addy = -1;
			next_node->high = before_node;
			before_node->low = next_node;
			break;
		case 'd':
			addx = 1;
			next_node->left = before_node;
			before_node->right = next_node;
			break;
	}
	next_node->x = before_node->x + addx;
	next_node->y = before_node->y + addy;
	printf("add ( %d , %d ) succeeded\n",next_node->x,next_node->y);
	return next_node;
}

int where_closest(RH *rhead, int tx, int ty){//回傳最近的方向1左 2右 3上 4下(先不要) 
	int ttx = tx - rhead->node->x;
	int tty = ty - rhead->node->y;
	if(abs(ttx) <= abs(tty))
		return ttx >= 0 ? 2 : 1;
	else
		return tty >= 0 ? 3 : 4;
}

RH* RH_list(RH *move_use,RH *l_head,RH *r_head,int *best_step,int tx,int ty,NODE **goal_node){//記最佳路線次數、回傳佇列最尾端位置，紀錄終點節點 
	if(move_use->node != NULL){
		if(move_use->node->step == 0 && l_head->node->step + 1 <= *best_step){
			move_use->node->step = l_head->node->step + 1;
			r_head = make_rnode(r_head,move_use->node);
		} 
		if(move_use->node->x == tx && move_use->node->y == ty && *best_step > move_use->node->step){
			//printf("move_use->node->x,y = %d,%d move_use->node->step = %d\n",move_use->node->x,move_use->node->y,move_use->node->step);
			*best_step = move_use->node->step;
			*goal_node = move_use->node;
			//*goal_node == NULL ? printf("YES2\n") : printf("NO2\n");
		}
	}
	//printf("%d %d\n",move_use->node->x,move_use->node->y);
	return r_head;
}

NODE *sup_OR(NODE *head,int direction){//look_node : head往附近探，確定可走就讓head移到那 
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

void output_route(NODE *head){//輸出最短路徑 
	int direction = 0;
	printf("route:\n");
	printf("location( %d , %d ) step:%d\n",head->x,head->y,head->step);
	while(head != NULL){
		direction++;
		direction = direction > 4 ? 1 : direction;
		head = sup_OR(head,direction);
	}
}

void delete_steps(NODE *goal_node){//刪除標記 
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

void navigate_dijkstra(NODE *head, int tx, int ty){//Dijkstra搜尋法尋路  
	RH *n_head = make_rnode(NULL, head); //岔路佇列開頭為自己 
	RH *l_head = n_head;//佇列開頭 
	RH *r_head = n_head;//佇列結尾
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
	for(i = 0; i < sizex; i++){//製作第一排node 
		move = first_horizontal_node(move);
	}
	move = head;
	for(i = 0; i < sizey; i++){//製作第一列node 
		move = first_straight_node(move);
	}
	for(move2 = head->high ; move2 != NULL ; move2 = move2->high){//補完剩下的node 
		for(move = move2, i = 0; i < sizex ; i++){
			move = fill_up_node(move);
		}
	}
	/*for(move2 = head ; move2 != NULL ; move2 = move2->high){//歷遍 
		for(move = move2 ; move != NULL ; move = move->right){
			printf("%d %d\n",move->x,move->y);
		}
	}*/
	run_node(head);
	
	return 0;
}
