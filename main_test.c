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
    int codeOfBill;
    order *list;
    int numberDish;
    double total;
    struct bill *next;
};
typedef struct bill *bill;

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

node* insert_food_to_root(node* root, food x){
    if(root == NULL) return createNode(x);
    if(x.code < root->data.code) root->left = insert_food_to_root(root->left, x);
    if(x.code > root->data.code) root->right = insert_food_to_root(root->right, x);
    return root;
}

food createDish(int code, char name[], double cost) { 
    food x;
    x.code = code;
    strcpy(x.name, name);
    x.cost = cost;
    return x;
}

node*search_dish(node*root, int code){
    if(root == NULL) return NULL;
    if(root->data.code == code) return root;
    if(root->data.code > code) return search_dish(root->left,code);
    else return search_dish(root->right,code);
}

node* menu(){
    food menu;
    node*root = NULL;
    menu = createDish(1, "Goi sua", 100000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 2, "Salad ca hoi", 300000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 3, "Goi bap bo", 150000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 4, "Bo Wagyu bit tet", 800000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 5, "Ca hoi nuong me", 400000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 6, "Tom hum xao bo toi", 600000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 7, "Com chien duong chau", 100000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 8, "Ga sot teriyaki", 450000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 9, "Ca ri bo", 300000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 10, "Tiramisu", 30000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 11, "Trai cay ngu vi", 100000);
    root = insert_food_to_root(root,menu);
    menu = createDish( 12, "Pana cotta", 20000);
    root = insert_food_to_root(root,menu);
    return root;
}

void printMenu(node* root){
    if(root == NULL) return;
    printMenu(root->left);
    printf("\t%d:\t%-20s\t%-10.2lf\n", root->data.code, root->data.name, root->data.cost);
    printMenu(root->right);
}

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
    Bill->codeOfBill = ++n;
    Bill->next = NULL;
    Bill->numberDish = 0;
    Bill->total = 0;
    return Bill;
}

void add_order_to_bill(bill head, node*root, order dish ){
	node*found = search_dish(root,dish.code);
	if(found == NULL){
		printf("This dish is not available on the menu!\n");
		return;
	}
	int i; 
	for(i = 0; i < head->numberDish; i++){
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

void order_dish(node*root, bill BILL, order dish){
    int m,n;
    int q=0;
    while(1){
        printf("Enter the dish code(Nhap ma mon an):"); scanf("%d",&m);
        printf("Enter quantity(Nhap so luong):"); scanf("%d",&n);
        if(n < 0) {
            printf("So luong mon an khong phu hop\n");
            continue;
        }
        dish.code = m;        
        dish.number = n;
        if(m == 0 && n == 0) break;
        int check = 0; 
        for(int i = 0; i<BILL->numberDish; i++){// fix i=1-> i=0 đe tranh duyet thieu mon
            if(BILL->list[i].code == m) check = 1; 
        }
        if(!check && BILL->numberDish >= MAX_DISH){
            printf("Max 5 dishes."); continue;
        }
        add_order_to_bill(BILL,root,dish);
    }
}

void print_BILL(bill BILL, node*root){
    printf("-----------BILL-----------\t\n");
    for(int i = 0; i < BILL->numberDish; i++){
        node*found = search_dish(root, BILL->list[i].code);
        if(found != NULL){
            printf("%s x %d = %.2lf\n", found->data.name, BILL->list[i].number, BILL->list[i].number*found->data.cost);
        }
    }
    printf("Total amount payable(Tong so tien phai tra): %.2lf\n", BILL->total);
}

void file(bill BILL, node*root){

}

void find_bill(bill*head,int x, node*root){
    bill temp = *head;
    while(temp != NULL){
        if(temp->codeOfBill == x){
            print_BILL(temp,root);
            return;
        }
        temp = temp->next;
    }
}


int count_bill(bill head){// tong so hoa don 1 ngay
    int count = 0;
    while(head != NULL){
        count++;
        head = head->next;
    }
    return count;
}

double total_revenue(bill head){ // tong doanh thu trong ngay
    double sum = 0;
    while(head != NULL){
        if(head->total >= 2000000)
            sum += head->total * 0.75;
        else
            sum += head->total;
        head = head->next;
    }
    return sum;
}

void most_popular_dish(bill head, node* root){ // mon duoc goi nhieu nhat
    int count[100] = {0};

    while(head != NULL){
        for(int i = 0; i < head->numberDish; i++){
            count[head->list[i].code] += head->list[i].number;
        }
        head = head->next;
    }

    int max = 0, index = -1;
    for(int i = 1; i < 100; i++){
        if(count[i] > max){
            max = count[i];
            index = i;
        }
    }

    if(index != -1){
        node* found = search_dish(root, index);
        if(found != NULL){
            printf("Most popular dish: %s (%d orders)\n", found->data.name, max);
        }
    }
}

void max_bill(bill head){// hoa don gia tri dat nhat
    if(head == NULL) return;

    bill maxBill = head;

    while(head != NULL){
        if(head->total > maxBill->total){
            maxBill = head;
        }
        head = head->next;
    }

    printf("Max bill: %d - %.2lf\n", maxBill->codeOfBill, maxBill->total);
}

void statistics(bill head, node* root){ // ham thong ke tong hop
    printf("\n===== DAILY STATISTICS =====\n");

    printf("Total bills: %d\n", count_bill(head));
    printf("Total revenue: %.2lf\n", total_revenue(head));

    most_popular_dish(head, root);
    max_bill(head);
}

bill create_summary_bill(bill head, node* root){
    bill summary = create_bill();

    while(head != NULL){
        for(int i = 0; i < head->numberDish; i++){
            order dish;
            dish.code = head->list[i].code;
            dish.number = head->list[i].number;

            add_order_to_bill(summary, root, dish);
        }
        head = head->next;
    }

    return summary;
}


void print_summary_bill(bill summary, node* root){
    printf("\n=========== SUMMARY BILL ===========\n");

    for(int i = 0; i < summary->numberDish; i++){
        node* found = search_dish(root, summary->list[i].code);
        if(found != NULL){
            printf("%s x %d = %.2lf\n",
                found->data.name,
                summary->list[i].number,
                summary->list[i].number * found->data.cost);
        }
    }

    double final_total = summary->total;

    if(final_total >= 2000000){
        printf("Discount 25%% applied!\n");
        final_total *= 0.75;
    }

    printf("TOTAL ALL DAY: %.2lf\n", final_total);
}

void write_file(bill head, node* root){
    FILE *f = fopen("bill.txt", "w");
    if(f == NULL){
        printf("Cannot open file!\n");
        return;
    }

    bill temp = head;
    double totalDay = 0;

    while(temp != NULL){
        fprintf(f, "=========== BILL %d ===========\n", temp->codeOfBill);

        for(int i = 0; i < temp->numberDish; i++){
            node* found = search_dish(root, temp->list[i].code);
            if(found != NULL){
                fprintf(f, "%s x %d = %.2lf\n",
                    found->data.name,
                    temp->list[i].number,
                    temp->list[i].number * found->data.cost);
            }
        }

        double original = temp->total;
        double final_total = original;

        fprintf(f, "Original total: %.2lf\n", original);

        if(original >= 2000000){
            double discount = original * 0.25;
            final_total = original * 0.75;
            fprintf(f, "Discount (25%%): -%.2lf\n", discount);
        }

        fprintf(f, "Final total: %.2lf\n\n", final_total);

        totalDay += final_total;
        temp = temp->next;
    }

    // ===== THỐNG KÊ CUỐI NGÀY =====
    fprintf(f, "=========== DAILY SUMMARY ===========\n");
    fprintf(f, "Total bills: %d\n", count_bill(head));
    fprintf(f, "Total revenue: %.2lf\n", totalDay);

    fclose(f);
    printf("Write file successfully!\n");
}


void operation(node*Menu, bill*head){
    int codeDay;
    printf("\nEnter transaction code (Nhap ma giao dich):"); scanf("%d", &codeDay);
    order dish;
    printf("  \t------------------------MENU------------------------\n");
    printMenu(Menu);
    while(1){
        printf("\n\t============ORDERING SYSTEM============\t\n");
		printf( "1. Order food. (Dat mon an.)\n");
		printf( "2. Enter the code to end the day.(Nhap ma de ket thuc ngay.) \n");
		printf( "3. Secondary function.(Chuc nang phu)\n");
		printf( "-------------------------------------\n");
		printf( "Enter your choose (Nhap lua chon cua ban) :");
		int lc; scanf ("%d",&lc);
        printf("\n");
		if(lc == 1){
            bill BILL = create_bill();
			order_dish(Menu,BILL,dish);
            print_BILL(BILL,Menu);
            add_bill_to_list(head,BILL);
		}
		else if(lc == 2){
            int end;
            printf("Please enter the code:"); scanf("%d",&end);

            if(end == codeDay){
                statistics(*head, Menu);

                bill summary = create_summary_bill(*head, Menu);
                print_summary_bill(summary, Menu);

                write_file(*head, Menu);

                printf("End of the day!");
                break;
            }
            else printf("Wrong code!");
        }
		else if(lc == 3){
            printf("1.Find bill from code of bill. ");
            printf("\n2.Add food.");
            int choose;
            printf("\nEnter your choose(Nhap lua chon cua ban):"); scanf("%d",&choose);
            switch(choose){
                case 1:{
                    printf("Input code of bill you want to find: ");
                    int code; scanf("%d",&code);
                    find_bill(head,code,Menu);
                }
                case 2:{}
            }
		}
        else{
            printf("Please enter again!");
        }

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
    //printMenu(Menu);
    operation(Menu,&head);

    return 0;
}
