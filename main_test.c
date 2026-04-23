#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifdef _WIN32
#include<Windows.h>
#endif
#define MAX_NAME 50
#define MAX_DISH 5
#define MAX_CODE 1000

struct food{
    int code;
    char name[MAX_NAME];
    double cost;
    char category[40];
};

struct order{
    int code;
    int number;
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
#ifdef _WIN32
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
#else
    const char* ansi[] = {
        "\033[0m",  "\033[34m", "\033[32m", "\033[36m",
        "\033[31m", "\033[35m", "\033[33m", "\033[37m",
        "\033[90m", "\033[94m", "\033[92m", "\033[96m",
        "\033[91m", "\033[95m", "\033[93m", "\033[97m"
    };
    if(color >= 0 && color < 16) printf("%s", ansi[color]);
#endif
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

void clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
// tạo root kiểu dữ liệu food
node* createNode(food x)
{
    node* L = malloc(sizeof(node));
    if(L == NULL) return NULL;
    L->data = x;
    L->left = NULL;
    L->right = NULL;
    return L;
}

// tạo thông tin các món ăn
food createDish(int code, char name[], double cost, char category[])
{
    food x;
    x.code = code;
    strcpy(x.name, name);
    x.cost = cost;
    strcpy(x.category,category);
    return x;
}

// tạo bill để chứa các thông tin thanh toán
bill create_bill()
{
    static int n = 0;
    bill Bill = malloc(sizeof(struct bill));
    if(Bill == NULL)
    {
        printf("Cannot write order into bill!\n");
        return NULL;
    }
    Bill->list = malloc(sizeof(order) * MAX_DISH);
    if(Bill->list == NULL)
    {
        printf("Cannot order!\n");
        free(Bill);   // tránh memory leak khi malloc list thất bại
        return NULL;
    }
    Bill->ID_of_Bill = ++n;
    Bill->next = NULL;
    Bill->numberDish = 0;
    Bill->total = 0;
    return Bill;
}

// nhập các dữ liệu food vào hàm root
node* insert_food_to_root(node* root, food x)
{
    if(root == NULL) return createNode(x);
    if(x.code < root->data.code) root->left = insert_food_to_root(root->left, x);
    if(x.code > root->data.code) root->right = insert_food_to_root(root->right, x);
    return root;
}

// tìm kiếm món ăn trong root
node*search_dish(node*root, int code)
{
    if(root == NULL) return NULL;
    if(root->data.code == code) return root;
    if(root->data.code > code) return search_dish(root->left,code);
    else return search_dish(root->right,code);
}

// hàm thêm món vào menu
node* addFood(node* root, int code, char name[], double cost, char category[])
{
    food menu = createDish(code, name, cost, category);
    root = insert_food_to_root(root, menu);
    return root;
}

// Doc menu tu file menu.txt
// Dinh dang moi dong: <code> <ten_mon> <gia> <hang_muc>
// Ten mon va hang muc dung gach duoi thay khoang trang, vi du:
//   1 Goi_sua 100000 Khai_vi
//   4 Bo_Wagyu_bit_tet 800000 Mon_chinh
node* load_menu_from_file(const char* filename)
{
    FILE *f = fopen(filename, "r");
    if(f == NULL)
    {
        printf("[!] Cannot open menu file '%s'! Using default menu.\n", filename);
        return NULL;
    }

    node* root = NULL;
    int code, count = 0;
    double cost;
    char rawName[MAX_NAME], rawCategory[40];
    char name[MAX_NAME], category[40];

    while(fscanf(f, "%d %49s %lf %39s", &code, rawName, &cost, rawCategory) == 4)
    {
        // Doi gach duoi thanh khoang trang
        strcpy(name, rawName);
        for(int i = 0; name[i]; i++) if(name[i] == '_') name[i] = ' ';

        strcpy(category, rawCategory);
        for(int i = 0; category[i]; i++) if(category[i] == '_') category[i] = ' ';

        if(search_dish(root, code) != NULL)
        {
            printf("[!] Duplicate code %d in file, skipped.\n", code);
            continue;
        }
        root = addFood(root, code, name, cost, category);
        count++;
    }

    fclose(f);
    printf("[+] Loaded %d dishes from '%s'.\n", count, filename);
    return root;
}

// tạo menu: uu tien doc tu file, neu khong co file thi dung hardcode
node* menu()
{
    node* root = load_menu_from_file("menu.txt");
    if(root != NULL) return root;

    // Fallback: hardcode
    root = NULL;
    root = addFood(root, 1, "Goi sua", 100000, "Khai vi");
    root = addFood(root, 2, "Salad ca hoi", 400000, "Khai vi");
    root = addFood(root, 3, "Goi bap bo", 150000, "Khai vi");

    root = addFood(root, 4, "Bo Wagyu bit tet", 800000, "Mon chinh");
    root = addFood(root, 5, "Ca hoi nuong me", 400000, "Mon chinh");
    root = addFood(root, 6, "Tom hum xao bo toi", 600000, "Mon chinh");
    root = addFood(root, 7, "Com chien duong chau", 100000, "Mon chinh");
    root = addFood(root, 8, "Ga sot teriyaki", 450000, "Mon chinh");
    root = addFood(root, 9, "Ca ri bo", 400000, "Mon chinh");

    root = addFood(root, 10, "Tiramisu", 40000, "Trang mieng");
    root = addFood(root, 11, "Trai cay ngu vi", 100000, "Trang mieng");
    root = addFood(root, 12, "Pana cotta", 20000, "Trang mieng");
    return root;
}

node*minNode(node* node){
    while( node && node->left) node = node->left;
    return node;
}

// hàm xóa món ra khỏi menu
node* remove_food(node* root, int code)
{
    if(root == NULL) return root;
    if(code < root->data.code) root->left = remove_food(root->left,code);
    else if(code > root->data.code) root->right = remove_food(root->right, code);
    else
	{
        if(!root->left)
		{
            node* temp = root->right;
            free(root);
            return temp;
        }
        else if(!root->right)
		{
            node*temp = root->left;
            free(root);
            return temp;
        }
        node* temp = minNode(root->right);
        root->data = temp->data;
        root->right = remove_food(root->right, temp->data.code);
    }
    return root;

}

// thêm món ăn vào menu
void add_food_menu(node**root)
{
    int code;
    char name[MAX_NAME];
    double cost;
    char category[40];
    printf("Enter dish code: "); code = nhapsonguyen();
    if(search_dish(*root, code) != NULL)
	{
        printf("Code %d already exists!\n", code); return;
    }
    printf("Enter dish name: ");
    xoaBoNhoDem();
    if(fgets(name, MAX_NAME, stdin)) name[strcspn(name, "\n")] = 0;
    printf("Enter cost: ");
    while(scanf("%lf", &cost) != 1 || cost < 0)
    {
        printf("Enter again: ");
        xoaBoNhoDem();
    }
    xoaBoNhoDem(); // xoa buffer sau khi doc so thuc
    printf("Select category:\n");
    printf("1. Khai vi\n");
    printf("2. Mon chinh\n");
    printf("3. Trang mieng\n");
    printf("Your choice: ");
    int choice;
    choice = nhapsonguyen();
    switch(choice)
	{
        case 1: strcpy(category, "Khai vi"); break;
        case 2: strcpy(category, "Mon chinh"); break;
        case 3: strcpy(category, "Trang mieng"); break;
        default: strcpy(category, "Khac");
    }

    *root = addFood(*root, code, name, cost,category);
}


// Hàm phụ dùng để duyệt cây và in các món theo hạng mục yêu cầu
void printByCategory(node* root, char category[], int* found) {
    if (root == NULL) return;

    // Duyệt bên trái
    printByCategory(root->left, category, found);

    // Kiểm tra nếu món này thuộc hạng mục đang cần in
    if (strcmp(root->data.category, category) == 0) {
        printf("%-5d %-25s %.2lf\n", root->data.code, root->data.name, root->data.cost);
        *found = 1; // Đánh dấu là hạng mục này có món
    }

    // Duyệt bên phải
    printByCategory(root->right, category, found);
}

// in ra menu
void printMenu(node* root)
{
    if (root == NULL)
    {
        SET_COLOR(4);
        printf("\n[!] Menu hien dang trong.\n");
        SET_COLOR(15);
        return;
    }
    SET_COLOR(14);
    char group[3][20] = {"Khai vi", "Mon chinh", "Trang mieng"};
    printf("\n\t=============== MENU ===============\n");

    for (int i = 0; i < 3; i++)
    {
        SET_COLOR(11);
        printf("\n[%s]\n", group[i]);
        SET_COLOR(15);
        for (int h = 0; h < 40; h++) printf("-");
        printf("\n%-5s %-25s %-10s\n", "Code", "Name dish", "Price"); // Thêm tiêu đề cho đẹp
        int found = 0;
        // Gọi hàm duyệt cây để in các món thuộc group[i]
        printByCategory(root, group[i], &found);

        if (!found) {
            SET_COLOR(8);
            printf("This category is currently unavailable.\n");
            SET_COLOR(15);
        }
    }
    printf("\n");
    SET_COLOR(14);
    for (int h = 0; h < 40; h++) printf("=");
    SET_COLOR(15);
    printf("\n");
}
// nhập các món đã order vào bill
void add_order_to_bill(bill head, node*root, order dish)
{
    node*found = search_dish(root, dish.code);
    if(found == NULL) return;

    // Nếu món đã có trong bill, cộng thêm số lượng
    for(int i = 0; i < head->numberDish; i++)
    {
        if(head->list[i].code == dish.code)
        {
            head->list[i].number += dish.number;
            head->total += dish.number * found->data.cost;
            return;
        }
    }

    // Giới hạn tối đa 5 loại món khác nhau trong một bill
    if(head->numberDish >= MAX_DISH)
    {
        printf("Max %d different dishes per bill!\n", MAX_DISH);
        return;
    }

    head->list[head->numberDish].code = dish.code;
    head->list[head->numberDish].number = dish.number;
    head->numberDish++;
    head->total += dish.number * found->data.cost;
}

// hàm gọi món
void order_dish(node*root, bill BILL, order dish)
{
    if(root == NULL)
	{
        printf("Empty menu!"); return;
    }
    int m = 0;
    int n = 0;
    while(1)
	{
        printf("Enter the dish code:"); m = nhapsonguyen();
        printf("Enter quantity:"); n = nhapsonguyen();
        if(n < 0 || n>1001){
        	printf("The quantity of dishes is not appropriate!");
        	continue;
		}
        if(m == 0 && n == 0) break;
        node*found = search_dish(root,m);
        if(found == NULL)
        {
            printf("Do not found dish code!"); continue;
        }
        dish.code = m;
        dish.number = n;
        add_order_to_bill(BILL,root,dish);
    }
}

//hàm thay đổi số lượng món trong bill
void change_quantity(int code, node*root, bill BILL, int number)
{
    if(BILL == NULL){ printf("Empty bill!"); return;}
    int found = -1;
    for(int i =0; i<BILL->numberDish; i++)
	{
        if(BILL->list[i].code == code)
		{
            found = i;
            break;
        }
    }
    if(found == -1)
    {
        printf("Dish not found in bill!\n");
        return;
    }
    else
    {
        node *foundDish = search_dish(root, code);
        if(foundDish == NULL) return;
        BILL->total -= (double)BILL->list[found].number*foundDish->data.cost;
        if(number == 0){
            // Xoa mon khoi bill bang cach dich chuyen mang
            for(int j = found; j < BILL->numberDish - 1; j++)
                BILL->list[j] = BILL->list[j + 1];
            BILL->numberDish--;
            printf("Dish has been removed from bill!\n");
            return;
        }
        BILL->list[found].number = number;
        BILL->total += (double)number*foundDish->data.cost;
    }
}

void update_dish_price(node* root, int code)
{
    node* found = search_dish(root, code);
    if (found == NULL)
	{
        printf("\nDish not found!");
        return;
    }

    double new_price;
    printf("\nName of dish: %s", found->data.name);
    printf("\nCurrent price: %.2lf", found->data.cost);
    printf("\nEnter new price: ");

    while (scanf("%lf", &new_price) != 1 || new_price < 0)
	{
        printf("Invalid price! Enter again: ");
        xoaBoNhoDem();
    }
    found->data.cost = new_price;
    printf("\nThe price of %s has been updated.", found->data.name);
}

// thêm bill vào danh sách
void add_bill_to_list(bill*head, bill newbill)
{
    if(*head == NULL)
	{
        *head = newbill;
        return;
    }
    bill temp = *head;
    while(temp->next != NULL)
	{
        temp = temp->next;
    }
    temp->next = newbill;
}

// hàm in ra bill
void print_BILL(bill BILL, node*root)
{
    printf("===============BILL===============\t\n");
    printf("Code of BILL: %d\n",BILL->ID_of_Bill);
    for(int i = 0; i < BILL->numberDish; i++)
	{
        node*found = search_dish(root, BILL->list[i].code);
        if(found != NULL)
		{
            printf("%s x %d = %.2lf\n", found->data.name, BILL->list[i].number, BILL->list[i].number*found->data.cost);
        }
    }
    for(int i= 0; i<40; i++)printf("-");
    printf("\n");
    double final = BILL->total;
    printf("Subtotal: %.2lf\n", BILL->total);
    if(BILL->total >= 2000000)
	{
    	double discount = BILL->total * 0.25;
    	final = BILL->total - discount;
    	printf("Discount (25%%): -%5.2lf\n", discount);
    }
   	printf("Total: %5.2lf\n", final);
   	for(int i=0; i<40; i++) printf("=");
	printf("\n");

}

// hàm tìm bill bằng code
void find_bill(bill*head,int x, node*root)
{
    bill temp = *head;
    if(temp == NULL)
	{
        printf("The bill list is empty!"); return;
    }
    while(temp != NULL)
	{
        if(temp->ID_of_Bill == x)
		{
            print_BILL(temp,root);
            return;
        }
        temp = temp->next;
    }
    if(temp == NULL) printf("The bill is not found!");
}

// tong so hoa don 1 ngay
int count_bill(bill head)
{
    int count = 0;
    while(head != NULL)
	{
        count++;
        head = head->next;
    }
    return count;
}
// tong doanh thu trong ngay
double total_revenue(bill head)
{
    double sum = 0;
    while(head != NULL)
	{
        if(head->total >= 2000000)
            sum += (head->total * 0.75);
        else
            sum += head->total;
        head = head->next;
    }
    return sum;
}
// mon duoc goi nhieu nhat
void most_popular_dish(bill head, node* root)
{
    int count[MAX_CODE] = {0};

    while(head != NULL)
	{
        for(int i = 0; i < head->numberDish; i++)
		{
            count[head->list[i].code] += head->list[i].number;
        }
        head = head->next;
    }

    int max = 0, index = -1;
    for(int i = 1; i < MAX_CODE; i++)
	{
        if(count[i] > max)
		{
            max = count[i];
            index = i;
        }
    }

    if(index != -1)
	{
        node* found = search_dish(root, index);
        if(found != NULL)
		{
            printf("Most popular dish: %s (%d orders)\n", found->data.name, max);
        }
    }
}
// hoa don gia tri dat nhat
void max_bill(bill head)
{
    if(head == NULL) return;

    bill maxBill = head;

    while(head != NULL)
	{
        if(head->total > maxBill->total)
		{
            maxBill = head;
        }
        head = head->next;
    }

    printf("Max bill: %d - %.2lf\n", maxBill->ID_of_Bill, maxBill->total);
}
// ham thong ke tong hop
void statistics(bill head, node* root)
{
    printf("\n========== DAILY STATISTICS ==========\n");

    printf("Total bills: %d\n", count_bill(head));
    printf("Total revenue: %.2lf\n", total_revenue(head));

    most_popular_dish(head, root);
    max_bill(head);
}

bill create_summary_bill(bill head, node* root)
{
    bill summary = create_bill();
    while(head != NULL)
	{
        for(int i = 0; i < head->numberDish; i++)
		{
            order dish;
            dish.code = head->list[i].code;
            dish.number = head->list[i].number;
            add_order_to_bill(summary, root, dish);
        }
        head = head->next;
    }

    return summary;
}


void print_summary_bill(bill summary, node* root)
{
    printf("\n=========== SUMMARY BILL ===========\n");

    for(int i = 0; i < summary->numberDish; i++)
	{
        node* found = search_dish(root, summary->list[i].code);
        if(found != NULL)
		{
            printf("%s x %d = %.2lf\n",
                found->data.name,
                summary->list[i].number,
                summary->list[i].number * found->data.cost);
        }
    }

    double final_total = summary->total;

    if(final_total >= 2000000)
	{
        printf("Discount 25%% applied!\n");
        final_total *= 0.75;
    }

    printf("TOTAL ALL DAY: %.2lf\n", final_total);
}

// giải phóng bộ nhớ của bst
void free_tree(node* n)
{
        if (!n) return;
        free_tree(n->left); free_tree(n->right); free(n);
}

// giải phóng dữ liệu
void free_data(node** root, bill* head)
{
    free_tree(*root); *root = NULL;
    bill curr = *head;
    while(curr)
	{
        bill next = curr->next;
        free(curr->list); free(curr); curr = next;
    }
    *head = NULL;
}

// Ham duyet cay va ghi tung mon vao file (inorder de ra thu tu tang dan)
void write_menu_inorder(node* root, FILE* f)
{
    if(root == NULL) return;
    write_menu_inorder(root->left, f);

    // Doi khoang trang thanh gach duoi de giu dung dinh dang file
    char name[MAX_NAME], category[40];
    strcpy(name, root->data.name);
    for(int i = 0; name[i]; i++) if(name[i] == ' ') name[i] = '_';
    strcpy(category, root->data.category);
    for(int i = 0; category[i]; i++) if(category[i] == ' ') category[i] = '_';

    fprintf(f, "%d %s %.0lf %s\n", root->data.code, name, root->data.cost, category);
    write_menu_inorder(root->right, f);
}

// Xuat toan bo menu ra file menu.txt
void export_menu_to_file(node* root)
{
    if(root == NULL)
    {
        printf("[!] Menu is empty, nothing to export!\n");
        return;
    }
    FILE *f = fopen("menu.txt", "w");
    if(f == NULL)
    {
        printf("[!] Cannot open menu.txt for writing!\n");
        return;
    }
    write_menu_inorder(root, f);
    fclose(f);
    printf("[+] Menu exported to 'menu.txt' successfully!\n");
}

// hàm xuất hóa đơn và doanh thu ra file
void write_file(bill head, node* root)
{
    FILE *f = fopen("bill.txt", "w");
    if(f == NULL)
	{
        printf("Cannot open file!\n");
        return;
    }
    bill temp = head;
    double totalDay = 0;
    while(temp != NULL){
        fprintf(f, "=========== BILL %d ===========\n", temp->ID_of_Bill);

        for(int i = 0; i < temp->numberDish; i++)
		{
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
        fprintf(f,"Original total: %.2lf\n", original);
        if(original >= 2000000)
		{
            double discount = original*0.25;
            final_total = original*0.75;
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

// chế độ khách hàng
void customer_mode(node*Menu, bill BILL, order dish, bill*head)
{
    order_dish(Menu,BILL,dish);
    while(1){
    	printf("\n");
        for(int i = 0; i<40; i++) printf("=");
        printf("\n");
		printf("Did you want to change your quantity of dish ? (1.YES/2.NO)");
        int choice; choice = nhapsonguyen();
        switch(choice){
            case 1:
		    {
                int code, number, found = 0;
                printf("Enter code of dish you want to change:"); code = nhapsonguyen();
                for(int j = 0; j < BILL->numberDish; j++)
			    {
                    if(BILL->list[j].code == code)
				    {
                        found = 1;
                	    printf("Enter the quantity you want to change:"); number = nhapsonguyen();
                        if(number < 0){
                            printf("Please enter again! "); break;
                        }
                        else{
                            change_quantity(code, Menu, BILL, number);
                            printf("Number of dish has been successfully changed!");
                        } break;
				    }
                }
                if(found != 1)
                {
                    printf("Do not found dish code in the bill!\n");
                }
                else continue;
            } break;
            case 2: break;
            default: printf("Please enter again!");
        }
        if(choice == 2) break;
    }
    print_BILL(BILL,Menu);
    add_bill_to_list(head,BILL);
}

// chế độ quản lí
void management_mode(node**Menu, bill*head)
{
    for(int i=0; i<40; i++) printf("=");
    printf("\n1. Menu management.\n");
    printf("2. Admin mode.\n");
    printf("3. Back.\n");
    printf("Enter your choose:");
    int option; option = nhapsonguyen();
    switch(option){
        case 1:{
        	clearScreen();
            for(int i=0; i<40; i++) printf("=");
            printf("\n1. Add/remove food to menu.\n");
            printf("2. Update dish price\n");
            printf("3. Export menu to file\n");
            printf("4. Back.\n");
            printf("Enter your choose:");
            int select; select = nhapsonguyen();
            switch(select){
                case 1:{
                	clearScreen();
                    for(int i=0; i<40; i++) printf("=");
                    printf("\n1. Add food\n");
                    printf("2. Remove food\n");
                    printf("3. Back.\n");
                    printf("Enter your choose:");
                    int choice; choice = nhapsonguyen();
                    switch(choice){
                        case 1: 
                        {
                        	clearScreen();
                        	printMenu(*Menu);
                            for(int i=0; i<40; i++) printf("=");
                            printf("\n");
						    add_food_menu(Menu);
						    break;
                        }
                        case 2:
                        {
                        	clearScreen();
                        	printMenu(*Menu);
                            for(int i=0; i<40; i++) printf("=");
                            printf("\nEnter dish code to remove: ");
                            int dish_code = nhapsonguyen();
                            if(search_dish(*Menu, dish_code) == NULL) printf("\nDish code have not found!");
                            else{
                                *Menu = remove_food(*Menu, dish_code);
                                printf("\nRemove successfully!");
                        }
                        break;
                        }
                        case 3: break;
                        default: printf("Please enter again.\n"); break;
                    }
                    break;
                }
                case 2: {
                	clearScreen();
                	printMenu(*Menu);
                    for(int i=0; i<40; i++) printf("=");
                	printf("\nEnter dish code to update price: ");
                    int c = nhapsonguyen();
                    update_dish_price(*Menu, c);
					break;
				}
                case 3: {
                    export_menu_to_file(*Menu);
                    break;
                }
				case 4: break;
				default: printf("Please enter again.\n"); break;
		    }
            break;
		}
        case 2:{
            for(int i=0; i<40; i++) printf("=");
            printf("\n1.Find bill from code of bill. ");
            printf("\n2.Revenue report.");
            printf("\n3.Back.");
            printf("\nEnter your choose:");
            int choose;
            choose = nhapsonguyen();
            switch(choose){
                case 1:{
                    for(int i=0; i<40; i++) printf("=");
                    printf("\nInput code of bill you want to find: ");
                    int code; code = nhapsonguyen();
                    find_bill(head,code,*Menu);
                    break;
                }
                case 2:{
                    for(int i=0; i<40; i++) printf("=");
                    printf("\n");
                	statistics(*head,*Menu);
				}
				case 3: break;
				default: printf("Please enter again.\n"); break;
            }
            break;
        }
        case 3: break;
        default: break;
    }
}

// hàm hoạt động
void operation(node*Menu, bill*head)
{
    int codeDay = 0;
    int lc = 0;
    int end = 0;
    order dish;
    SET_COLOR(10);
    printf("\nEnter transaction code:");
    codeDay = nhapsonguyen();
    clearScreen();
    SET_COLOR(15);
    printMenu(Menu);
    while(1){
        SET_COLOR(15);
        printf("\n\t============ORDERING SYSTEM============\t\n");
		printf( "1. Order food.\n");
		printf( "2. Management mode.\n");
        printf( "3. Enter code to end day.\n");
		for(int i = 0; i<40; i++) printf( "-");
		printf("\n");
		printf( "Enter your choose :");
		int choose = 0;
		lc = nhapsonguyen();
        printf("\n");
        switch(lc){
		case 1:{
			clearScreen();
            printMenu(Menu);
            bill BILL = create_bill();
            customer_mode(Menu, BILL, dish, head);
            break;
        }
		case 2:{
			clearScreen();
            management_mode(&Menu, head); break;
        }
        case 3:{
            printf("Please enter the code:");
			end = nhapsonguyen();
            if(end == codeDay){
            	bill summary = create_summary_bill(*head, Menu);
                print_summary_bill(summary, Menu);
                write_file(*head,Menu);
                free_data(&Menu, head);
                free(summary->list);
                free(summary);
                printf("End of the day!");
                break;
            }
            else printf("Wrong code!");
            break;
        }
        default:
            printf("Please enter again!");
        }
        if(end == codeDay) break;
	}
}

// Ham dang nhap doc tai khoan tu file accounts.txt
// File co dinh dang moi dong: <username> <password>
// Vi du: admin 1234
int login()
{
    FILE *f = fopen("accounts.txt", "r");
    if(f == NULL)
    {
        printf("Cannot open accounts file!\n");
        return 0;
    }

    char fileUser[50], filePass[50];
    char inputUser[50], inputPass[50];
    int attempts = 3;

    while(attempts > 0)
    {
        printf("\n========== LOGIN ==========\n");
        printf("Username: ");
        if(fgets(inputUser, sizeof(inputUser), stdin))
            inputUser[strcspn(inputUser, "\n")] = 0;
        printf("Password: ");
        if(fgets(inputPass, sizeof(inputPass), stdin))
            inputPass[strcspn(inputPass, "\n")] = 0;

        rewind(f);
        int found = 0;
        while(fscanf(f, "%49s %49s", fileUser, filePass) == 2)
        {
            if(strcmp(inputUser, fileUser) == 0 && strcmp(inputPass, filePass) == 0)
            {
                found = 1;
                break;
            }
        }

        if(found)
        {
            printf("Login successful! Welcome, %s!\n", inputUser);
            fclose(f);
            return 1;
        }
        else
        {
            attempts--;
            if(attempts > 0)
                printf("Wrong username or password! %d attempt(s) left.\n", attempts);
            else
                printf("Too many failed attempts. Exiting...\n");
        }
    }

    fclose(f);
    return 0;
}

int main(){
    printf("  \t_____________________________________________________________________________________________________\n");
    printf("  \t|                         Truong: Dai Hoc Bach Khoa- Dai hoc Da Nang                                |\n");
	printf("  \t|                                Khoa: Cong nghe thong tin                                          |\n");
	printf("  \t|                                PBL1: DO AN LAP TRINH TINH TOAN                                    |\n");
	printf("  \t|                     De tai: XAY DUNG UNG DUNG DAT MON AN VA THANH TOAN DON HANG                   |\n");
	printf("  \t|                                Giao vien huong dan: Tran Ho Thuy Tien                             |\n");
	printf("  \t|                                Sinh vien: Dang Gia Khiem - 25T_DT2                                |\n");
	printf("  \t|                                           Nguyen Minh Hien - 25T_DT2                              |\n");
	printf("  \t|___________________________________________________________________________________________________|\n");

    if(login() == 0) return 0;

    node*Menu = menu();
    bill head = NULL;
    operation(Menu,&head);

    return 0;
}
