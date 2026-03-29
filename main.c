#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#define MAX_NAME 50
#define MAX_MENU 20
#define MAX_DISH 5

int total_of_dish = 0;

char menuNames[MAX_MENU][50];
double menuPrices[MAX_MENU];
char menuCategory[MAX_MENU][20];


struct food{
    int code;
    char name[MAX_NAME];
    double cost;
    char category[30];
};

struct order{
    int code;
    int number;
    char note[MAX_NAME];
};

struct bill{
    int ID_of_Bill;
    struct order *list;
    int numberDish;
    double total;
    struct bill *next;
};

struct node{
    struct food data;
    struct node *left;
    struct node *right;
};

typedef struct bill *bill;
typedef struct order order;
typedef struct food food;
typedef struct node node;

void SET_COLOR(int color)
{
	WORD wColor;
     

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
}

void xoaBoNhoDem() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int nhapsonguyen() 
{
    int n;
    while (scanf("%d", &n) != 1) {
        printf("Error! Enter again: ");
        xoaBoNhoDem();
    }
    return n;
}

// tạo root kiểu dữ liệu food
node* createNode(food x){
    node* L = malloc(sizeof(node));
    if(L == NULL) return NULL;
    L->data = x;
    L->left = NULL;
    L->right = NULL;
    return L;
}

// tạo thông tin các món ăn
food createDish(int code, char name[], double cost, char category[]) { 
    food x;
    x.code = code;
    strcpy(x.name, name);
    x.cost = cost;
    strcpy(x.category,category);
    return x;
}

// tạo bill để chứa các thông tin thanh toán
bill create_bill(){
    static int n = 0;
    bill Bill = malloc(sizeof(struct bill));
    if(Bill == NULL){
        printf("Cannot write order into bill!");
        return NULL;
    }
    Bill->list = malloc(sizeof(order)*MAX_DISH);
    if(Bill->list == NULL){
        printf("Cannot order!"); return NULL;
    }
    Bill->ID_of_Bill = ++n;
    Bill->next = NULL;
    Bill->numberDish = 0;
    Bill->total = 0;
    return Bill;
}

// nhập các dữ liệu food vào hàm root
node* insert_food_to_root(node* root, food x){
    if(root == NULL) return createNode(x);
    if(x.code < root->data.code) root->left = insert_food_to_root(root->left, x);
    if(x.code > root->data.code) root->right = insert_food_to_root(root->right, x);
    return root;
}

// tìm kiếm món ăn trong root
node*search_dish(node*root, int code){
    if(root == NULL) return NULL;
    if(root->data.code == code) return root;
    if(root->data.code > code) return search_dish(root->left,code);
    else return search_dish(root->right,code);
}

node* addFood(node* root, int code, char name[], double cost, char category[]){
    food menu = createDish(code, name, cost, category);
    root = insert_food_to_root(root, menu);
    if(total_of_dish < MAX_MENU){
        strcpy(menuNames[total_of_dish], name);
        menuPrices[total_of_dish] = cost;
        strcpy(menuCategory[total_of_dish], category);
        total_of_dish++;
    }
    return root;
}

// tạo menu
node* menu(){
    food menu;
    node*root = NULL;
    root = addFood(root, 1, "Goi sua", 100000, "Khai vi");
    root = addFood(root, 2, "Salad ca hoi", 300000, "Khai vi");
    root = addFood(root, 3, "Goi bap bo", 150000, "Khai vi");

    root = addFood(root, 4, "Bo Wagyu bit tet", 800000, "Mon chinh");
    root = addFood(root, 5, "Ca hoi nuong me", 400000, "Mon chinh");
    root = addFood(root, 6, "Tom hum xao bo toi", 600000, "Mon chinh");
    root = addFood(root, 7, "Com chien duong chau", 100000, "Mon chinh");
    root = addFood(root, 8, "Ga sot teriyaki", 450000, "Mon chinh");
    root = addFood(root, 9, "Ca ri bo", 300000, "Mon chinh");

    root = addFood(root, 10, "Tiramisu", 30000, "Trang mieng");
    root = addFood(root, 11, "Trai cay ngu vi", 100000, "Trang mieng");
    root = addFood(root, 12, "Pana cotta", 20000, "Trang mieng");
    return root;
}

// in ra menu
void printMenu(){
    char group[3][20] = {"Khai vi", "Mon chinh", "Trang mieng"};
    printf("\n\t===============MENU===============\n");
    for(int i = 0; i < 3; i++){
        printf("\n[%s]\n",group[i]);
        printf("------------------------------");
        int found = 0;
        for(int j = 0; j < total_of_dish; j++){
            if(strcmp(group[i],menuCategory[j]) == 0){
                printf("\n%d: %s  %.2lf\n", j+1, menuNames[j], menuPrices[j]);
                found = 1;
            }
        }
        if(!found) printf("This category is currently unavailable.");
    }
}

// nhập các món đã order vào bill
void add_order_to_bill(bill head, node*root, order dish ){
	node*found = search_dish(root,dish.code);
	if(found == NULL){
		printf("This dish is not available on the menu!\n");
		return;
	}
	for(int i = 0; i < head->numberDish; i++){
        if(head->list[i].code ==  dish.code){
            head->list[i].number += dish.number;
            head->total += dish.number * found->data.cost;
            return;
        }
    }
    head->list[head->numberDish].code = dish.code;
    head->list[head->numberDish].number = dish.number;
    head->numberDish++;
    head->total += dish.number*found->data.cost;
}

// hàm gọi món
void order_dish(node*root, bill BILL, order dish){
    if(root == NULL){
        printf("Empty menu!"); return;
    }
    int m = 0;
    int n = 0;
    int q=0;
    while(1){
        printf("Enter the dish code:"); m = nhapsonguyen();
        printf("Enter quantity:"); n = nhapsonguyen();
        if(n < 0) continue;
        dish.code = m;        
        dish.number = n;
        if(m == 0 && n == 0) break;
        int check = 0; 
        for(int i = 1; i<BILL->numberDish; i++){
            if(BILL->list[i].code == m) check = 1; 
        }
        if(!check && BILL->numberDish == MAX_DISH){
            printf("Max 5 dishes.\n"); break;
        }
        add_order_to_bill(BILL,root,dish);
    }
}

void change_quantity(int code, node*root, bill BILL, int number){
    if(BILL == NULL){ printf("Empty bill!"); return;}
    int found = -1;
    for(int i =0; i<BILL->numberDish; i++){
        if(BILL->list[i].code == code){
            found = i;
            break;
        }
    }
    if(found == -1) return;
    node *foundDish = search_dish(root, code);
    if(foundDish == NULL) return;
    BILL->total -= (double)BILL->list[found].number*foundDish->data.cost;
    BILL->list[found].number = number;
    BILL->total += (double)number*foundDish->data.cost;
}

// thêm bill vào danh sách 
void add_bill_to_list(bill*head, bill newbill){
    if(*head == NULL){
        *head = newbill;
        return;
    }
    bill temp = *head;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = newbill;
}


// hàm in ra bill
void print_BILL(bill BILL, node*root){
    printf("===============BILL===============\t\n");
    printf("Code of BILL: %d\n",BILL->ID_of_Bill);
    for(int i = 0; i < BILL->numberDish; i++){
        node*found = search_dish(root, BILL->list[i].code);
        if(found != NULL){
            printf("%s x %d = %.2lf\n", found->data.name, BILL->list[i].number, BILL->list[i].number*found->data.cost);
        }
    }
    printf("--------------------------------------\n");
    printf("Total amount payable: %.2lf\n", BILL->total);
}


// hàm tìm bill bằng code
void find_bill(bill*head,int x, node*root){
    bill temp = *head;
    while(temp != NULL){
        if(temp->ID_of_Bill == x){
            print_BILL(temp,root);
            return;
        }
        temp = temp->next;
    }
}

void file(bill BILL, node*root){

}

void customer_mode(node*Menu, bill BILL, order dish, bill*head){
    order_dish(Menu,BILL,dish);
    while(1){
        printf("\n===============================\n");
        printf("Did you want to change your quantity of dish ? (YES/NO)");
        char choice[10];
        xoaBoNhoDem();
        scanf("%s",&choice);
        if(strcmp(choice,"YES") == 0){
            int code, number;
            printf("Enter code of dish you want to change:"); code = nhapsonguyen();
            printf("Enter the quantity you want to change:"); number = nhapsonguyen();
            change_quantity(code, Menu, BILL, number);
        }
        else{break;}
    }
    print_BILL(BILL,Menu);
    add_bill_to_list(head,BILL);
}

// hàm hoạt động
void operation(node*Menu, bill*head){
    int codeDay = 0;
    int lc = 0;
    int end = 0;
    order dish;
    printf("\nEnter transaction code:");
    codeDay = nhapsonguyen();
    while(1){
        printMenu();
        printf("\n\t============ORDERING SYSTEM============\t\n");
		printf( "1. Order food.\n");
		printf( "2. Management mode.\n");
		printf( "3. Secondary function.\n");
		printf( "-------------------------------------\n");
		printf( "Enter your choose :");
		int choose = 0;
		lc = nhapsonguyen();
        printf("\n");
        switch(lc){
		case 1:{
            bill BILL = create_bill();
			// order_dish(Menu,BILL,dish);
            // print_BILL(BILL,Menu);
            // add_bill_to_list(head,BILL);
            customer_mode(Menu, BILL, dish, head);
            break;
        }
		case 2:{
			printf("Please enter the code:"); 
			end = nhapsonguyen();
            if(end == codeDay){
                //ham thống kê và in file
                printf("End of the day!");
                break;
            }
            else printf("Wrong code!");
            break;
        }
		case 3:
            printf("1.Find bill from code of bill. ");
            printf("\n2.Add food.");
            printf("\nEnter your choose:");
            choose = nhapsonguyen();
            switch(choose){
                case 1:{
                    printf("Input code of bill you want to find: ");
                    int code; scanf("%d",&code);
                    find_bill(head,code,Menu);
                }
                case 2:{}
            }
            break;
        default:
            printf("Please enter again!");
        }
        if(end == codeDay) break;
	}
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
    bill head = NULL;
    operation(Menu,&head);

    return 0;
}
