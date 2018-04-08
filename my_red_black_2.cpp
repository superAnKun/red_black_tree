#include <stdio.h>
#include <stdlib.h>
typedef struct Node {
	int key, color;
	struct Node *lchild, *rchild;
} Node;

Node __NIL;
#define NIL (&__NIL)
static int init_NIL() {
	NIL->key = 0;
	NIL->color = 1;
	NIL->lchild= NIL->rchild = NIL;
	return 0;
}
static int _______________temp = init_NIL();

Node *getNewNode(int key) {
	Node *node = (Node*)malloc(sizeof(Node));
	node->lchild = node->rchild = NIL;
	node->key = key;
	node->color = 0;
	return node;
}

Node *rotate_right(Node *root) {
	Node *temp = root->lchild;
	root->lchild = temp->rchild;
	temp->rchild = root;
	return temp;
}

Node *rotate_left(Node *root) {
	Node *temp = root->rchild;
	root->rchild = temp->lchild;
	temp->lchild = root;
	return temp;
}

Node *insert_maintain(Node *root, int flag) {
	if (root == NIL) return root;
	if (root->lchild->color == 0 && root->rchild->color == 0) {
		root->color = 0;
		root->lchild->color = root->rchild->color = 1;
		return root;
	}
	if (flag) {
		if (root->lchild->color) return root;
		if (root->lchild->lchild->color && root->lchild->rchild->color) return root;
		if (root->lchild->rchild->color == 0) {
			root->lchild = rotate_left(root->lchild);
		}
		root->color = 0;
		root = rotate_right(root);
	} else {
		if (root->rchild->color) return root;
		if (root->rchild->rchild->color && root->rchild->lchild->color) return root;
		if (root->rchild->lchild->color == 0) {
			root->rchild = rotate_right(root->rchild);
		}
		root->color = 0;
		root = rotate_left(root);
	}
	root->color = 1;
	return root;
}


Node *__insert(Node *root, int key) {
	if (root == NIL) return getNewNode(key);
	if (root->key == key) return root;
	if (root->key > key) {
		root->lchild = __insert(root->lchild, key);
	} else {
		root->rchild = __insert(root->rchild, key);
	}
	root = insert_maintain(root, root->key > key);
	return root;
}

Node *insert(Node *root, int key) {
	root = __insert(root, key);
	root->color = 1;
	return root;
}

Node *processor(Node *root) {
	if (root == NIL) return root;
	Node *temp = root->lchild;
	while (temp->rchild != NIL) {
		temp = temp->rchild;
	}
	return temp;
}

Node *delete_maintain(Node *root) {
	if (root == NIL) return root;
	if (root->lchild->color < 2 && root->rchild->color < 2) return root;
	if (root->rchild->color == 0) {
		root->color = 0;
		root->rchild->color = 1;
		root = rotate_left(root);
		root->lchild = delete_maintain(root->lchild);
		return root;
	} else if (root->rchild->color == 0) {
		root->color = 0;
		root->lchild->color = 1;
		root = rotate_right(root);
		root->rchild = delete_maintain(root->lchild);
		return root;
	}

	if (root->lchild->color >= 2) {
		if (root->rchild->rchild->color && root->rchild->lchild->color) {
			root->color++;
			root->lchild->color--;
			root->rchild->color--;
			return root;
		} else if (root->rchild->rchild->color) {
			root->rchild->color = 0;
			root->rchild->lchild->color = 1;
			root->rchild = rotate_right(root->rchild);
		}
		root->rchild->color = root->color;
		root->color = 1;
		root->rchild->rchild->color = 1;
		root->lchild->color--;
		root = rotate_left(root);
		return root;
	} else {
		if (root->lchild->lchild->color && root->lchild->rchild->color) {
			root->color++;
			root->rchild->color--;
			root->lchild->color--;
			return root;
		} else if (root->lchild->lchild->color) {
			root->lchild->color = 0;
			root->lchild->rchild->color = 1;
			root->lchild = rotate_left(root->lchild);
		}
		root->lchild->color = root->color;
		root->color = 1;
		root->lchild->lchild->color = 1;
		root->rchild->color--;
		root = rotate_right(root);
		return root;
	}
return root;
}

Node *__delete_node (Node *root, int key) {
	if (root == NIL) return root;
	if (root->key > key) {
		root->lchild = __delete_node(root->lchild, key);
	} else if (root->key < key) {
		root->rchild = __delete_node(root->rchild, key);
	} else {
		if (root->lchild == NIL && root->rchild == NIL) {
			free(root);
			NIL->color++;
			return NIL;
		} else if (root->lchild == NIL || root->rchild == NIL) {
			Node *temp = root->lchild != NIL ? root->lchild : root->rchild;
			free(root);
			temp->color++;
			return temp;
		} else {
			Node *temp = processor(root);
			root->key = temp->key;
			root->lchild = __delete_node(root->lchild, temp->key);
		}
	}
	root = delete_maintain(root);
	return root;
}

Node *delete_node(Node *root, int key) {
	root = __delete_node(root, key);
	root->color = 1;
	return root;
}

void output(Node *root) {
    if (root == NIL) return ;
    printf("%d(%d, %d, %d)\n",
        root->key,
        root->color, 
        root->lchild->key, 
        root->rchild->key
    );
    output(root->lchild);
    output(root->rchild);
    return ;
}

int main() {
    Node *root = NIL;
    int op, key;
    while (~scanf("%d%d", &op, &key)) {
        switch (op) {
            case 1: root = insert(root, key); break;
            case 2: root = delete_node(root, key); break;
        }
        printf("----------------\n");
        output(root);
        printf("----------------\n");
    }
    return 0;
}
