#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EXPRESS 1000
#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 20
#define MAX_ID_LEN 20
#define MAX_ADDRESS_LEN 100

// next_serial used for express ID generation (initialized when loading data)
static int next_serial = 0;

// Express struct
typedef struct {
    char id[MAX_ID_LEN];              // express ID
    char recipient_name[MAX_NAME_LEN]; // recipient name
    char phone[MAX_PHONE_LEN];         // recipient phone
    char sender[MAX_NAME_LEN];         // sender
    char address[MAX_ADDRESS_LEN];     // address
    char receive_time[30];             // receive time
    int is_received;                   // received flag (0-not received, 1-received)
    char pickup_time[30];              // pickup time
} Express;

// global express database
Express express_db[MAX_EXPRESS];
int express_count = 0;

// get current time
void get_current_time(char *buffer) {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// generate express ID
void generate_express_id(char *id) {
    sprintf(id, "EXP%06d", ++next_serial);
}

// read a line from stdin and strip trailing newline
void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

// add express
void add_express() {
    if (express_count >= MAX_EXPRESS) {
        printf("Express database full!\n");
        return;
    }

    Express new_express;
    
    printf("\n========== Add Express ==========\n");
    
    generate_express_id(new_express.id);
    printf("Express ID: %s\n", new_express.id);
    
    printf("Recipient name: ");
    read_line(new_express.recipient_name, MAX_NAME_LEN);

    printf("Recipient phone: ");
    read_line(new_express.phone, MAX_PHONE_LEN);

    printf("Sender: ");
    read_line(new_express.sender, MAX_NAME_LEN);

    printf("Address: ");
    read_line(new_express.address, MAX_ADDRESS_LEN);
    
    get_current_time(new_express.receive_time);
    new_express.is_received = 0;
    strcpy(new_express.pickup_time, "Not received");
    
    express_db[express_count++] = new_express;
    printf("Express added successfully!\n");
}

// query express
void query_express() {
    printf("\n========== Query Express ==========\n");
    printf("1. Query by Express ID\n");
    printf("2. Query by Recipient name\n");
    printf("3. List all expresses\n");
    printf("Select: ");
    
    int choice = 0;
    char buf[32];
    read_line(buf, sizeof(buf));
    if (sscanf(buf, "%d", &choice) != 1) choice = 0;
    
    if (choice == 1) {
        char id[MAX_ID_LEN];
        printf("Enter express ID: ");
        read_line(id, MAX_ID_LEN);
        
        for (int i = 0; i < express_count; i++) {
            if (strcmp(express_db[i].id, id) == 0) {
                printf("\nExpress ID: %s\n", express_db[i].id);
                printf("Recipient: %s\n", express_db[i].recipient_name);
                printf("Phone: %s\n", express_db[i].phone);
                printf("Sender: %s\n", express_db[i].sender);
                printf("Address: %s\n", express_db[i].address);
                printf("Receive time: %s\n", express_db[i].receive_time);
                const char *status = express_db[i].is_received ? "Received" : "Not received";
                printf("Status: %s\n", status);
                if (express_db[i].is_received) {
                    printf("Pickup time: %s\n", express_db[i].pickup_time);
                }
                return;
            }
        }
        printf("Express not found!\n");
        
    } else if (choice == 2) {
        char name[MAX_NAME_LEN];
        printf("Enter recipient name: ");
        read_line(name, MAX_NAME_LEN);
        
        int found = 0;
        for (int i = 0; i < express_count; i++) {
            if (strcmp(express_db[i].recipient_name, name) == 0) {
                printf("\nExpress ID: %s\n", express_db[i].id);
                printf("Recipient: %s\n", express_db[i].recipient_name);
                printf("Phone: %s\n", express_db[i].phone);
                printf("Sender: %s\n", express_db[i].sender);
                printf("Address: %s\n", express_db[i].address);
                printf("Receive time: %s\n", express_db[i].receive_time);
                printf("Status: %s\n", express_db[i].is_received ? "Received" : "Not received");
                printf("---\n");
                found = 1;
            }
        }
        if (!found) printf("No expresses found for that recipient!\n");
        
    } else if (choice == 3) {
        if (express_count == 0) {
            printf("No expresses!\n");
            return;
        }

        printf("\n%-12s %-12s %-15s %-15s %-15s\n", "ExpressID", "Recipient", "Phone", "Status", "ReceiveTime");
        printf("================================================================================\n");
        for (int i = 0; i < express_count; i++) {
            const char *status = express_db[i].is_received ? "Received" : "Not received";
            printf("%-12s %-12s %-15s %-15s %-15s\n",
                   express_db[i].id,
                   express_db[i].recipient_name,
                   express_db[i].phone,
                   status,
                   express_db[i].receive_time);
        }
    } else {
        printf("Invalid selection!\n");
    }
}

// 标记快递为已领取
void mark_as_received() {
    printf("\n========== Mark as Received ==========\n");
    char id[MAX_ID_LEN];
    printf("Enter express ID: ");
    read_line(id, MAX_ID_LEN);
    
    for (int i = 0; i < express_count; i++) {
        if (strcmp(express_db[i].id, id) == 0) {
            if (express_db[i].is_received) {
                printf("This express was already received!\n");
                return;
            }
            express_db[i].is_received = 1;
            get_current_time(express_db[i].pickup_time);
            printf("Marked as received!\n");
            printf("Recipient: %s\n", express_db[i].recipient_name);
            printf("Pickup time: %s\n", express_db[i].pickup_time);
            return;
        }
    }
    printf("Express not found!\n");
}

// 删除已领取的快递
void delete_express() {
    printf("\n========== Delete Express ==========\n");
    char id[MAX_ID_LEN];
    printf("Enter express ID to delete: ");
    read_line(id, MAX_ID_LEN);
    
    for (int i = 0; i < express_count; i++) {
        if (strcmp(express_db[i].id, id) == 0) {
            if (!express_db[i].is_received) {
                printf("Can only delete received expresses!\n");
                return;
            }
            
            // 将后面的元素前移
            for (int j = i; j < express_count - 1; j++) {
                express_db[j] = express_db[j + 1];
            }
            express_count--;
            printf("Express deleted!\n");
            return;
        }
    }
    printf("Express not found!\n");
}

// 统计快递信息
void statistics() {
    printf("\n========== Express Statistics ==========\n");
    
    int total = express_count;
    int received = 0;
    
    for (int i = 0; i < express_count; i++) {
        if (express_db[i].is_received) {
            received++;
        }
    }
    
    printf("Total expresses: %d\n", total);
    printf("Received: %d\n", received);
    printf("Not received: %d\n", total - received);
    printf("Pickup rate: %.2f%%\n", total > 0 ? (received * 100.0 / total) : 0);
}

// 保存数据到文件
void save_to_file() {
    FILE *fp = fopen("express_data.dat", "wb");
    if (fp == NULL) {
        printf("Unable to open file!\n");
        return;
    }
    
    fwrite(&express_count, sizeof(int), 1, fp);
    fwrite(express_db, sizeof(Express), express_count, fp);
    fclose(fp);
    printf("Data saved!\n");
}

// 从文件加载数据
void load_from_file() {
    FILE *fp = fopen("express_data.dat", "rb");
    if (fp == NULL) {
        return;
    }
    
    // 读取快递数量并做基本校验
    if (fread(&express_count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        express_count = 0;
        return;
    }
    if (express_count < 0 || express_count > MAX_EXPRESS) {
        // 数据异常，重置
        express_count = 0;
        fclose(fp);
        return;
    }
    if (fread(express_db, sizeof(Express), express_count, fp) != (size_t)express_count) {
        // 读取失败，重置
        express_count = 0;
        fclose(fp);
        return;
    }
    // 初始化 next_serial，确保生成的ID不会重复
    next_serial = 0;
    for (int i = 0; i < express_count; i++) {
        // 快递ID 格式 EXP######，尝试解析序号
        int num = 0;
        if (sscanf(express_db[i].id, "EXP%d", &num) == 1) {
            if (num > next_serial) next_serial = num;
        }
    }
    fclose(fp);
}

// 显示菜单
void display_menu() {
    printf("\n");
    printf("================ Express Management System ================\n");
    printf(" 1. Add express\n");
    printf(" 2. Query express\n");
    printf(" 3. Mark as received\n");
    printf(" 4. Delete express\n");
    printf(" 5. Statistics\n");
    printf(" 6. Save data\n");
    printf(" 0. Exit\n");
    printf("Select (0-6): ");
}

// 主函数
int main() {
    load_from_file();
    
    int choice;
    
    while (1) {
        display_menu();
        char buf[32];
        read_line(buf, sizeof(buf));
        if (sscanf(buf, "%d", &choice) != 1) {
            choice = -1;
        }
        
        switch (choice) {
            case 1:
                add_express();
                break;
            case 2:
                query_express();
                break;
            case 3:
                mark_as_received();
                break;
            case 4:
                delete_express();
                break;
            case 5:
                statistics();
                break;
            case 6:
                save_to_file();
                break;
            case 0:
                printf("Thank you, goodbye!\n");
                save_to_file();
                return 0;
            default:
                printf("选择错误，请重新输入！\n");
        }
    }
    
    return 0;
}
