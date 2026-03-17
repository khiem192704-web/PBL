#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_NAME 50
#define MAX_DISH 5
struct food{
    int code;
    char name[MAX_NAME];
    double cost;
};
typedef struct food food;

struct order{
    int code;
    int number;
};
typedef struct order order;

struct bill{
    order *list;
    int numberDish;
    double total;
    struct bill *next;
};
typedef struct bill bill;

struct node{
    food data;
    struct node *left;
    struct node *right;
};
typedef struct node node;

node* createNode(food x){
    node* L = malloc(sizeof(node));
    if(L == NULL) return NULL;
    L->data = x;
    L->left = NULL;
    L->right = NULL;
    return L;
}

node* insert(node* root, food x){
    if(root == NULL) return createNode(x);
    if(x.code < root->data.code) root->left = insert(root->left, x);
    if(x.code > root->data.code) root->right = insert(root->right, x);
    return root;
}

food createDish(int code, char name[], double cost) { 
    food x;
    x.code = code;
    strcpy(x.name, name);
    x.cost = cost;
    return x;
}
node* menu(){
    food menu;
    node*root = NULL;
    menu = createDish(1, "Ga ran", 50000);
    root = insert(root,menu);
    menu = createDish( 2, "Lau thai", 300000);
    root = insert(root,menu);
    menu = createDish( 3, "Bo luc lac", 150000);
    root = insert(root,menu);
    return root;
}

void printMenu(node* root){
    if(root == NULL) return;
    printMenu(root->left);
    printf("%d:%s\t%.2lf\n", root->data.code, root->data.name, root->data.cost);
    printMenu(root->right);
}



bill *createBill(){
    bill *bill = malloc(sizeof(bill));
    if(bill == NULL){
        printf("Cannot create bill!");
        return NULL;
    }
    bill->list = malloc(sizeof(order)*MAX_DISH);
    bill->numberDish = 0;
    bill->total = 0;
    bill->next = NULL;
    return bill; 
}

int add_Bill(bill b, order o){

}

int main(){
    printf("  \t\t_____________________________________________________________________________________________________\n");
    printf("  \t\t|                         Truong: Dai Hoc Bach Khoa- Dai hoc Da Nang                                |\n");
	printf("  \t\t|                                Khoa: Cong nghe thong tin                                          |\n");
	printf("  \t\t|                                PBL1: DO AN LAP TRINH TINH TOAN                                    |\n");
	printf("  \t\t|                     De tai: XAY DUNG UNG DUNG DAT MON AN VA THANH TOAN DON HANG                   |\n");
	printf("  \t\t|                                Giao vien huong dan: Tran Ho Thuy Tien                             |\n");
	printf("  \t\t|                                Sinh vien: Dang Gia Khiem - 25T_DT2                                |\n");
	printf("  \t\t|                                           Nguyen Minh Hien - 25T_DT2                              |\n");
	printf("  \t\t|___________________________________________________________________________________________________|\n");
    node*Menu = menu();
    //printMenu(Menu);
    while(1){
        int codeDay;
        printf("\nEnter transaction code:"); scanf("%d", &codeDay);
        int n = 0 ;
        printf("------------------------MENU------------------------\n");
        printMenu(Menu);
    }
    return 0;
}