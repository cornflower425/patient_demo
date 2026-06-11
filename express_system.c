#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EXPRESS 1000
#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 20
#define MAX_ID_LEN 20
#define MAX_ADDRESS_LEN 100

// 快递信息结构体
typedef struct {
    char id[MAX_ID_LEN];              // 快递ID
    char recipient_name[MAX_NAME_LEN]; // 收件人姓名
    char phone[MAX_PHONE_LEN];         // 收件人电话
    char sender[MAX_NAME_LEN];         // 寄件人
    char address[MAX_ADDRESS_LEN];     // 收货地址
    char receive_time[30];             // 收货时间
    int is_received;                   // 是否已领取 (0-未领取, 1-已领取)
    char pickup_time[30];              // 领取时间
} Express;

// 全局快递数据库
Express express_db[MAX_EXPRESS];
int express_count = 0;

// 获取当前时间
void get_current_time(char *buffer) {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// 生成快递ID
void generate_express_id(char *id) {
    static int serial = 0;
    sprintf(id, "EXP%06d", ++serial);
}

// 添加快递
void add_express() {
    if (express_count >= MAX_EXPRESS) {
        printf("快递库已满！\n");
        return;
    }

    Express new_express;
    
    printf("\n========== 添加快递 ==========\n");
    
    generate_express_id(new_express.id);
    printf("快递ID: %s\n", new_express.id);
    
    printf("收件人姓名: ");
    scanf("%s", new_express.recipient_name);
    
    printf("收件人电话: ");
    scanf("%s", new_express.phone);
    
    printf("寄件人: ");
    scanf("%s", new_express.sender);
    
    printf("收货地址: ");
    scanf("%s", new_express.address);
    
    get_current_time(new_express.receive_time);
    new_express.is_received = 0;
    strcpy(new_express.pickup_time, "未领取");
    
    express_db[express_count++] = new_express;
    printf("✓ 快递添加成功！\n");
}

// 查询快递
void query_express() {
    printf("\n========== 查询快递 ==========\n");
    printf("1. 按快递ID查询\n");
    printf("2. 按收件人姓名查询\n");
    printf("3. 查询所有快递\n");
    printf("请选择: ");
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1) {
        char id[MAX_ID_LEN];
        printf("请输入快递ID: ");
        scanf("%s", id);
        
        for (int i = 0; i < express_count; i++) {
            if (strcmp(express_db[i].id, id) == 0) {
                printf("\n快递ID: %s\n", express_db[i].id);
                printf("收件人: %s\n", express_db[i].recipient_name);
                printf("电话: %s\n", express_db[i].phone);
                printf("寄件人: %s\n", express_db[i].sender);
                printf("地址: %s\n", express_db[i].address);
                printf("收货时间: %s\n", express_db[i].receive_time);
                printf("领取状态: %s\n", express_db[i].is_received ? "已领取" : "未领取");
                if (express_db[i].is_received) {
                    printf("领取时间: %s\n", express_db[i].pickup_time);
                }
                return;
            }
        }
        printf("未找到该快递！\n");
        
    } else if (choice == 2) {
        char name[MAX_NAME_LEN];
        printf("请输入收件人姓名: ");
        scanf("%s", name);
        
        int found = 0;
        for (int i = 0; i < express_count; i++) {
            if (strcmp(express_db[i].recipient_name, name) == 0) {
                printf("\n快递ID: %s\n", express_db[i].id);
                printf("收件人: %s\n", express_db[i].recipient_name);
                printf("电话: %s\n", express_db[i].phone);
                printf("寄件人: %s\n", express_db[i].sender);
                printf("地址: %s\n", express_db[i].address);
                printf("收货时间: %s\n", express_db[i].receive_time);
                printf("领取状态: %s\n", express_db[i].is_received ? "已领取" : "未领取");
                printf("---\n");
                found = 1;
            }
        }
        if (!found) printf("未找到该收件人的快递！\n");
        
    } else if (choice == 3) {
        if (express_count == 0) {
            printf("暂无快递！\n");
            return;
        }
        
        printf("\n%-12s %-12s %-15s %-15s %-15s\n", "快递ID", "收件人", "电话", "状态", "收货时间");
        printf("================================================================================\n");
        for (int i = 0; i < express_count; i++) {
            printf("%-12s %-12s %-15s %-15s %-15s\n",
                   express_db[i].id,
                   express_db[i].recipient_name,
                   express_db[i].phone,
                   express_db[i].is_received ? "已领取" : "未领取",
                   express_db[i].receive_time);
        }
    } else {
        printf("选择错误！\n");
    }
}

// 标记快递为已领取
void mark_as_received() {
    printf("\n========== 标记领取 ==========\n");
    char id[MAX_ID_LEN];
    printf("请输入快递ID: ");
    scanf("%s", id);
    
    for (int i = 0; i < express_count; i++) {
        if (strcmp(express_db[i].id, id) == 0) {
            if (express_db[i].is_received) {
                printf("该快递已经被领取！\n");
                return;
            }
            express_db[i].is_received = 1;
            get_current_time(express_db[i].pickup_time);
            printf("✓ 已标记为领取！\n");
            printf("领取人: %s\n", express_db[i].recipient_name);
            printf("领取时间: %s\n", express_db[i].pickup_time);
            return;
        }
    }
    printf("未找到该快递！\n");
}

// 删除已领取的快递
void delete_express() {
    printf("\n========== 删除快递 ==========\n");
    char id[MAX_ID_LEN];
    printf("请输入要删除的快递ID: ");
    scanf("%s", id);
    
    for (int i = 0; i < express_count; i++) {
        if (strcmp(express_db[i].id, id) == 0) {
            if (!express_db[i].is_received) {
                printf("只能删除已领取的快递！\n");
                return;
            }
            
            // 将后面的元素前移
            for (int j = i; j < express_count - 1; j++) {
                express_db[j] = express_db[j + 1];
            }
            express_count--;
            printf("✓ 快递已删除！\n");
            return;
        }
    }
    printf("未找到该快递！\n");
}

// 统计快递信息
void statistics() {
    printf("\n========== 快递统计 ==========\n");
    
    int total = express_count;
    int received = 0;
    
    for (int i = 0; i < express_count; i++) {
        if (express_db[i].is_received) {
            received++;
        }
    }
    
    printf("快递总数: %d\n", total);
    printf("已领取: %d\n", received);
    printf("未领取: %d\n", total - received);
    printf("领取率: %.2f%%\n", total > 0 ? (received * 100.0 / total) : 0);
}

// 保存数据到文件
void save_to_file() {
    FILE *fp = fopen("express_data.dat", "wb");
    if (fp == NULL) {
        printf("无法打开文件！\n");
        return;
    }
    
    fwrite(&express_count, sizeof(int), 1, fp);
    fwrite(express_db, sizeof(Express), express_count, fp);
    fclose(fp);
    printf("✓ 数据已保存！\n");
}

// 从文件加载数据
void load_from_file() {
    FILE *fp = fopen("express_data.dat", "rb");
    if (fp == NULL) {
        return;
    }
    
    fread(&express_count, sizeof(int), 1, fp);
    fread(express_db, sizeof(Express), express_count, fp);
    fclose(fp);
}

// 显示菜单
void display_menu() {
    printf("\n");
    printf("╔════════════════════════════════════╗\n");
    printf("║     校园快递代收管理系统           ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║  1. 添加快递                       ║\n");
    printf("║  2. 查询快递                       ║\n");
    printf("║  3. 标记为已领取                   ║\n");
    printf("║  4. 删除快递                       ║\n");
    printf("║  5. 统计信息                       ║\n");
    printf("║  6. 保存数据                       ║\n");
    printf("║  0. 退出系统                       ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("请选择操作 (0-6): ");
}

// 主函数
int main() {
    load_from_file();
    
    int choice;
    
    while (1) {
        display_menu();
        scanf("%d", &choice);
        
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
                printf("感谢使用，再见！\n");
                save_to_file();
                return 0;
            default:
                printf("选择错误，请重新输入！\n");
        }
    }
    
    return 0;
}