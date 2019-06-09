#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define depth 3
#define c_num 7

int heuristic_function(int arr[][7], int column, int turn) {
	if (column == 3) return 100;
	else return 1;
}

typedef struct tree {
	int value;
	int sequence[depth];
	struct tree *parent;
	struct tree *child[c_num];
}search_tree;

void node_initialize(search_tree *head) {
	head->parent = NULL;
	head->value = -2;
	for (int i = 0; i < c_num; i++) 
		head->child[i] = NULL;

	for (int i = 0; i < depth; i++)
		head->sequence[i] = -1;
}

void insert_node(search_tree *head, int child_num) {
	search_tree *temp = head;
	search_tree *new_node = (search_tree *)malloc(sizeof(search_tree));
	node_initialize(new_node);

	for (int i = 0; i < depth; i++)
		new_node->sequence[i] = head->sequence[i];

	new_node->value = -5;
	temp->child[child_num] = new_node;
	new_node->parent = temp;
}

void branch(search_tree *pre, int num) {
	if (num == 0) return;
	
	num--;
	for (int i = 0; i < c_num; i++) {
		insert_node(pre, i);
		pre->child[i]->sequence[depth - num - 1] = i;
		branch(pre->child[i], num);
	}
}

void print_tree(search_tree *pre, int num) {
	int i, j;
	if (num != 0) {
		for (i = 0; i < c_num; i++) {
			printf("%d ", pre->child[i]->value);
			if (num == 1) {
				for (j = 0; j <= depth - num; j++)
					printf("%d->", pre->child[i]->sequence[j]);
				printf(" ");
			}
		}
		printf("\t\tdepth : %d\n", num);
		num--;
		for (i = 0; i < c_num; i++) {
			print_tree(pre->child[i], num);
		}
	}
}

search_tree *fmax(search_tree *pre){
	int max = pre->child[0]->value;
	search_tree *temp = pre->child[0];

	for (int i = 0; i < c_num; i++) {
		if (max <= pre->child[i]->value) {
			max = pre->child[i]->value;
			temp = pre->child[i];
		}
	}
		
	return temp;
}

search_tree *fmin(search_tree *pre){
	int min = pre->child[0]->value;
	search_tree *temp = pre->child[0];

	for (int i = 0; i < c_num; i++) {
		if (min >= pre->child[i]->value) {
			min = pre->child[i]->value;
			temp = pre->child[i];
		}
	}

	return temp;
}

void find_path(search_tree *pre, int num) {
	int i;
	int temp = num;

	if (pre->child[0] == NULL)
		return;
	else {
		temp--;
		for (i = 0; i < c_num; i++)
			find_path(pre->child[i], temp);
	}

	if (num % 2 == 0)
		pre->value = fmin(pre)->value;
	else if (num % 2 == 1)
		pre->value = fmax(pre)->value;

}

void initialize(int arr[][7]) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++)
			arr[i][j] = 0;
	}
}

void print_arr(int arr[][7]) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			if (arr[i][j] == 1) printf("|O");
			else if (arr[i][j] == 2) printf("|X");
			else printf("| ");
		}
		printf("|\n");
	}
}

int check(int arr[][7]) {
	int i, j, k;
	int count = 1;
	int flag = 0;

	for (i = 0; i <= 5; i++) {
		for (j = 0; j <= 3; j++) {
			for (k = 1; k <= 3; k++)
				if (arr[i][j] == arr[i][j + k] && arr[i][j] != 0) count++;
			if (count == 4) {
				flag = arr[i][j];
				goto print;
			}
			count = 1;

			for (k = 1; k <= 3; k++) {
				if (i <= 2) {
					if (arr[i][j] == arr[i + k][j + k] && arr[i][j] != 0)
						count++;
				}
				else {
					if (arr[i][j] == arr[i - k][j + k] && arr[i][j] != 0)
						count++;
				}
			}
			if (count == 4) {
				flag = arr[i][j];
				goto print;
			}
			count = 1;
		}
	}

	for (j = 0; j <= 6; j++) {
		for (i = 0; i <= 2; i++) {
			for (k = 1; k <= 3; k++)
				if (arr[i][j] == arr[i + k][j] && arr[i][j] != 0) count++;
			if (count == 4) {
				flag = arr[i][j];
				goto print;
			}
			count = 1;
		}
	}

print:
	if (flag == 1)
		printf("You Win\n");
	else if (flag == 2)
		printf("Computer Win\n");

	return flag;
}

int put(int arr[][7], int column, int &turn) {
	for (int i = 5; i >= 0; i--) {
		if (arr[i][column - 1] == 0) {
			arr[i][column - 1] = turn;
			break;
		}
		if (i == 0) {
			printf("Cannot put on that column\n\n");
			return 0;
		}
	}

	if (turn == 1) turn = 2;
	else if (turn == 2) turn = 1;
	return 1;
}

int set_flag(int arr[][7], int column) {
	if (arr[0][column - 1] != 0 || column > 7 || column < 1) return 0;
	else return 1;
}

void set_temp_arr(int arr[][7], int temp_arr[][7]) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++)
			temp_arr[i][j] = arr[i][j];
	}
}

void put_temp_arr(int temp_arr[][7], int *sequence, int turn) {
	int temp_turn = turn;

	for (int i = 0; i < depth; i++) {
		if (sequence[i] >= 0) {
			put(temp_arr, sequence[i], temp_turn);
		}
	}
}

void set_value(search_tree *pre, int arr[][7], int temp_arr[][7], int num, int turn) {
	int i, j;
	if (num != 0) {
		for (i = 0; i < c_num; i++) {
			if (num == 1) {
				set_temp_arr(arr, temp_arr);
				put_temp_arr(temp_arr, pre->child[i]->sequence, turn);
				pre->child[i]->value = heuristic_function(temp_arr, i, turn);
			}
		}
		num--;
		for (i = 0; i < c_num; i++) {
			set_value(pre->child[i], arr, temp_arr, num, turn);
		}
	}
}

int main() {
	srand(time(NULL));
	search_tree *head = (search_tree *)malloc(sizeof(search_tree));
	node_initialize(head);
	branch(head, depth);
	print_tree(head, depth);
	

	int i, next_column, column;
	int arr[6][7], temp_arr[6][7];
	initialize(arr);

	int turn = 1;
	while (1) {
		print_arr(arr);
		if (check(arr) != 0) break;

		do {
			printf("Select Column: ");
			scanf("%d", &column);
			printf("\n");
		} while (set_flag(arr, column) == 0);

		put(arr, column, turn);
		print_arr(arr);
		if (check(arr) != 0) break;

		set_value(head, arr, temp_arr, depth, turn);
		find_path(head, depth);
		print_tree(head, depth);
		for (i = 0; i < c_num; i++) {
			if (head->value == head->child[i]->value)
				next_column = i;
		}
		printf("%dth column\n", next_column + 1);

		do {
			printf("Select Column: ");
			scanf("%d", &column);
			printf("\n");
		} while (set_flag(arr, column) == 0);

		put(arr, column, turn);
		print_arr(arr);
		if (check(arr) != 0) break;


		//put(arr, search(arr), turn);
		system("cls");
	}
	
	/*scanf("%d", &column);
	put(arr, column, turn);
	

	branch(head, depth);
	set_value(head, arr, temp_arr, depth, turn);
	find_path(head, depth);
	

	

	

	printf("%d\n", head->value);
	print_tree(head, depth);
	

	for (i = 0; i < c_num; i++) {
		if (head->value == head->child[i]->value)
			next_column = i;
	}
	printf("%dth column\n", next_column + 1);*/

	printf("finish\n");
	
	return 0;
}