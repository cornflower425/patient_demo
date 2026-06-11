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
        printf("快递数据库已满！\n");
        return;
    }

    Express new_express;
    
    printf("\n========== 添加快递 ==========\n");
    
    generate_express_id(new_express.id);
    printf("快递编号: %s\n", new_express.id);
    
    printf("收件人姓名: ");
    read_line(new_express.recipient_name, MAX_NAME_LEN);

    printf("收件人电话: ");
    read_line(new_express.phone, MAX_PHONE_LEN);

    printf("寄件人: ");
    read_line(new_express.sender, MAX_NAME_LEN);

    printf("地址: ");
    read_line(new_express.address, MAX_ADDRESS_LEN);
    
    get_current_time(new_express.receive_time);
    new_express.is_received = 0;
    strcpy(new_express.pickup_time, "未领取");
    
    express_db[express_count++] = new_express;
    printf("快递已添加！\n");
}

// query express
void query_express() {
    printf("\n========== 查询快递 ==========\n");
    printf("1. 按快递编号查询\n");
    printf("2. 按收件人姓名查询\n");
    printf("3. 列出所有快递\n");
    printf("请选择: ");
    
    int choice = 0;
    char buf[32];
    read_line(buf, sizeof(buf));
    if (sscanf(buf, "%d", &choice) != 1) choice = 0;
    
    if (choice == 1) {
        char id[MAX_ID_LEN];
        printf("请输入快递编号: ");
        read_line(id, MAX_ID_LEN);
        
        for (int i = 0; i < express_count; i++) {
            if (strcmp(express_db[i].id, id) == 0) {
                printf("\n快递编号: %s\n", express_db[i].id);
                printf("收件人: %s\n", express_db[i].recipient_name);
                printf("电话: %s\n", express_db[i].phone);
                printf("寄件人: %s\n", express_db[i].sender);
                printf("地址: %s\n", express_db[i].address);
                printf("收货时间: %s\n", express_db[i].receive_time);
                const char *status = express_db[i].is_received ? "已领取" : "未领取";
                printf("状态: %s\n", status);
                if (express_db[i].is_received) {
                    printf("领取时间: %s\n", express_db[i].pickup_time);
                }
                return;
            }
        }
        printf("快递未找到！\n");
        
    } else if (choice == 2) {
        char name[MAX_NAME_LEN];
        printf("请输入收件人姓名: ");
        read_line(name, MAX_NAME_LEN);
        
        int found = 0;
        for (int i = 0; i < express_count; i++) {
            if (strcmp(express_db[i].recipient_name, name) == 0) {
                printf("\n快递编号: %s\n", express_db[i].id);
                printf("收件人: %s\n", express_db[i].recipient_name);
                printf("电话: %s\n", express_db[i].phone);
                printf("寄件人: %s\n", express_db[i].sender);
                printf("地址: %s\n", express_db[i].address);
                printf("收货时间: %s\n", express_db[i].receive_time);
                printf("状态: %s\n", express_db[i].is_received ? "已领取" : "未领取");
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

        printf("\n%-12s %-12s %-15s %-15s %-15s\n", "快递编号", "收件人", "电话", "状态", "收货时间");
        printf("================================================================================\n");
        for (int i = 0; i < express_count; i++) {
            const char *status = express_db[i].is_received ? "已领取" : "未领取";
            printf("%-12s %-12s %-15s %-15s %-15s\n",
                   express_db[i].id,
                   express_db[i].recipient_name,
                   express_db[i].phone,
                   status,
                   express_db[i].receive_time);
        }
    } else {
        printf("选择错误！\n");
    }
}

// 标记快递为已领取
void mark_as_received() {
    printf("\n========== 标记为已领取 ==========\n");
    char id[MAX_ID_LEN];
    printf("请输入快递编号: ");
    read_line(id, MAX_ID_LEN);
    
    for (int i = 0; i < express_count; i++) {
        if (strcmp(express_db[i].id, id) == 0) {
            if (express_db[i].is_received) {
                printf("该快递已被领取！\n");
                return;
            }
            express_db[i].is_received = 1;
            get_current_time(express_db[i].pickup_time);
            printf("已标记为领取！\n");
            printf("收件人: %s\n", express_db[i].recipient_name);
            printf("领取时间: %s\n", express_db[i].pickup_time);
            return;
        }
    }
    printf("快递未找到！\n");
}

// 删除已领取的快递
void delete_express() {
    printf("\n========== 删除快递 ==========\n");
    char id[MAX_ID_LEN];
    printf("请输入要删除的快递编号: ");
    read_line(id, MAX_ID_LEN);
    
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
            printf("快递已删除！\n");
            return;
        }
    }
    printf("快递未找到！\n");
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
    printf("数据已保存！\n");
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
    printf("================ 快递管理系统 ================\n");
    printf(" 1. 添加快递\n");
    printf(" 2. 查询快递\n");
    printf(" 3. 标记为已领取\n");
    printf(" 4. 删除快递\n");
    printf(" 5. 统计信息\n");
    printf(" 6. 保存数据\n");
    printf(" 0. 退出\n");
    printf("请选择 (0-6): ");
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
                printf("感谢使用，再见！\n");
                save_to_file();
                return 0;
            default:
                printf("选择错误，请重新输入！\n");
        }
    }
    
    return 0;
}
