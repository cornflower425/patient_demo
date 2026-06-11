#define _CRT_SECURE_NO_WARNINGS
#define _POSIX_C_SOURCE 200809L
#pragma execution_character_set("utf-8")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EXPRESS 1000
#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 20
#define MAX_ID_LEN 20
#define MAX_ADDRESS_LEN 100
#define MAX_TIME_LEN 30

// next_serial used for express ID generation (initialized when loading data)
static int next_serial = 0;

// Express struct
typedef struct {
    char id[MAX_ID_LEN];              // express ID
    char recipient_name[MAX_NAME_LEN]; // recipient name
    char phone[MAX_PHONE_LEN];         // recipient phone
    char sender[MAX_NAME_LEN];         // sender
    char address[MAX_ADDRESS_LEN];     // address
    char receive_time[MAX_TIME_LEN];   // receive time
    int is_received;                   // received flag (0-not received, 1-received)
    char pickup_time[MAX_TIME_LEN];    // pickup time
} Express;

// global express database
Express express_db[MAX_EXPRESS];
int express_count = 0;

// get current time (safe version)
void get_current_time(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        return;
    }
    
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    
    if (timeinfo == NULL) {
        snprintf(buffer, size, "%s", "Unknown");
        return;
    }
    
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// generate express ID (safe version)
void generate_express_id(char *id, size_t size) {
    if (id == NULL || size < 10) {
        return;
    }
    
    snprintf(id, size, "EXP%06d", ++next_serial);
}

// read a line from stdin and strip trailing newline (safe version)
void read_line(char *buffer, int size) {
    if (buffer == NULL || size <= 0) {
        return;
    }
    
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

// safe string copy function
void safe_strcpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return;
    }
    
    snprintf(dest, dest_size, "%s", src);
}

// print express details
void print_express(const Express *express) {
    if (express == NULL) {
        return;
    }
    
    printf("Express ID: %s\n", express->id);
    printf("Recipient: %s\n", express->recipient_name);
    printf("Phone: %s\n", express->phone);
    printf("Sender: %s\n", express->sender);
    printf("Address: %s\n", express->address);
    printf("Receive Time: %s\n", express->receive_time);
    
    const char *status = express->is_received ? "Picked up" : "Not picked up";
    printf("Status: %s\n", status);
    
    if (express->is_received) {
        printf("Pickup Time: %s\n", express->pickup_time);
    }
}

// validate input string
int is_empty_input(const char *str) {
    if (str == NULL || strlen(str) == 0) {
        return 1;
    }
    return 0;
}

// add express
void add_express(void) {
    if (express_count >= MAX_EXPRESS) {
        printf("Express database is full!\n");
        return;
    }

    Express new_express;
    memset(&new_express, 0, sizeof(Express));
    
    printf("\n========== Add Express ==========\n");
    
    generate_express_id(new_express.id, sizeof(new_express.id));
    printf("Express ID: %s\n", new_express.id);
    
    printf("Recipient Name: ");
    read_line(new_express.recipient_name, (int)sizeof(new_express.recipient_name));
    
    if (is_empty_input(new_express.recipient_name)) {
        printf("Recipient name cannot be empty!\n");
        return;
    }

    printf("Recipient Phone: ");
    read_line(new_express.phone, (int)sizeof(new_express.phone));
    
    if (is_empty_input(new_express.phone)) {
        printf("Recipient phone cannot be empty!\n");
        return;
    }

    printf("Sender: ");
    read_line(new_express.sender, (int)sizeof(new_express.sender));
    
    if (is_empty_input(new_express.sender)) {
        printf("Sender cannot be empty!\n");
        return;
    }

    printf("Address: ");
    read_line(new_express.address, (int)sizeof(new_express.address));
    
    if (is_empty_input(new_express.address)) {
        printf("Address cannot be empty!\n");
        return;
    }
    
    get_current_time(new_express.receive_time, sizeof(new_express.receive_time));
    new_express.is_received = 0;
    safe_strcpy(new_express.pickup_time, "Not picked up", sizeof(new_express.pickup_time));
    
    express_db[express_count++] = new_express;
    printf("Express added successfully!\n");
}

// query express by id
void query_by_id(void) {
    char id[MAX_ID_LEN];
    printf("Enter Express ID: ");
    read_line(id, (int)sizeof(id));
    
    if (is_empty_input(id)) {
        printf("Express ID cannot be empty!\n");
        return;
    }
    
    for (int i = 0; i < express_count; i++) {
        if (strcmp(express_db[i].id, id) == 0) {
            printf("\n");
            print_express(&express_db[i]);
            return;
        }
    }
    printf("Express not found!\n");
}

// query express by name
void query_by_name(void) {
    char name[MAX_NAME_LEN];
    printf("Enter Recipient Name: ");
    read_line(name, (int)sizeof(name));
    
    if (is_empty_input(name)) {
        printf("Recipient name cannot be empty!\n");
        return;
    }
    
    int found = 0;
    for (int i = 0; i < express_count; i++) {
        if (strcmp(express_db[i].recipient_name, name) == 0) {
            printf("\n");
            print_express(&express_db[i]);
            printf("---\n");
            found = 1;
        }
    }
    if (!found) {
        printf("No express found for this recipient!\n");
    }
}

// list all express
void list_all_express(void) {
    if (express_count == 0) {
        printf("No express records!\n");
        return;
    }

    printf("\n%-12s %-12s %-15s %-15s %-15s\n", "Express ID", "Recipient", "Phone", "Status", "Receive Time");
    printf("================================================================================\n");
    
    for (int i = 0; i < express_count; i++) {
        const char *status = express_db[i].is_received ? "Picked up" : "Not picked up";
        printf("%-12s %-12s %-15s %-15s %-15s\n",
               express_db[i].id,
               express_db[i].recipient_name,
               express_db[i].phone,
               status,
               express_db[i].receive_time);
    }
}

// query express
void query_express(void) {
    printf("\n========== Query Express ==========\n");
    printf("1. Query by Express ID\n");
    printf("2. Query by Recipient Name\n");
    printf("3. List All Express\n");
    printf("Please select: ");
    
    int choice = 0;
    char buf[32];
    read_line(buf, (int)sizeof(buf));
    if (sscanf(buf, "%d", &choice) != 1) {
        choice = 0;
    }
    
    switch (choice) {
        case 1:
            query_by_id();
            break;
        case 2:
            query_by_name();
            break;
        case 3:
            list_all_express();
            break;
        default:
            printf("Invalid choice!\n");
    }
}

// mark express as received
void mark_as_received(void) {
    printf("\n========== Mark as Picked Up ==========\n");
    char id[MAX_ID_LEN];
    printf("Enter Express ID: ");
    read_line(id, (int)sizeof(id));
    
    if (is_empty_input(id)) {
        printf("Express ID cannot be empty!\n");
        return;
    }
    
    for (int i = 0; i < express_count; i++) {
        if (strcmp(express_db[i].id, id) == 0) {
            if (express_db[i].is_received) {
                printf("This express has already been picked up!\n");
                return;
            }
            express_db[i].is_received = 1;
            get_current_time(express_db[i].pickup_time, sizeof(express_db[i].pickup_time));
            printf("Marked as picked up!\n");
            printf("Recipient: %s\n", express_db[i].recipient_name);
            printf("Pickup Time: %s\n", express_db[i].pickup_time);
            return;
        }
    }
    printf("Express not found!\n");
}

// delete express
void delete_express(void) {
    printf("\n========== Delete Express ==========\n");
    char id[MAX_ID_LEN];
    printf("Enter Express ID to delete: ");
    read_line(id, (int)sizeof(id));
    
    if (is_empty_input(id)) {
        printf("Express ID cannot be empty!\n");
        return;
    }
    
    for (int i = 0; i < express_count; i++) {
        if (strcmp(express_db[i].id, id) == 0) {
            if (!express_db[i].is_received) {
                printf("Only picked-up express can be deleted!\n");
                return;
            }
            
            // shift elements forward
            for (int j = i; j < express_count - 1; j++) {
                express_db[j] = express_db[j + 1];
            }
            express_count--;
            printf("Express deleted successfully!\n");
            return;
        }
    }
    printf("Express not found!\n");
}

// statistics
void statistics(void) {
    printf("\n========== Express Statistics ==========\n");
    
    int total = express_count;
    int received = 0;
    
    for (int i = 0; i < express_count; i++) {
        if (express_db[i].is_received) {
            received++;
        }
    }
    
    printf("Total Express: %d\n", total);
    printf("Picked up: %d\n", received);
    printf("Not picked up: %d\n", total - received);
    printf("Pickup Rate: %.2f%%\n", total > 0 ? (received * 100.0 / total) : 0.0);
}

// save data to file
void save_to_file(void) {
    FILE *fp = fopen("express_data.dat", "wb");
    if (fp == NULL) {
        printf("Cannot open file!\n");
        return;
    }
    
    if (fwrite(&express_count, sizeof(int), 1, fp) != 1) {
        printf("Failed to write data!\n");
        fclose(fp);
        return;
    }
    
    if (fwrite(express_db, sizeof(Express), (size_t)express_count, fp) != (size_t)express_count) {
        printf("Failed to write data!\n");
        fclose(fp);
        return;
    }
    
    fclose(fp);
    printf("Data saved successfully!\n");
}

// load data from file
void load_from_file(void) {
    FILE *fp = fopen("express_data.dat", "rb");
    if (fp == NULL) {
        return;
    }
    
    // read and validate express count
    if (fread(&express_count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        express_count = 0;
        return;
    }
    
    if (express_count < 0 || express_count > MAX_EXPRESS) {
        // invalid data, reset
        express_count = 0;
        fclose(fp);
        return;
    }
    
    if (fread(express_db, sizeof(Express), (size_t)express_count, fp) != (size_t)express_count) {
        // read failed, reset
        express_count = 0;
        fclose(fp);
        return;
    }
    
    // initialize next_serial to avoid duplicate IDs
    next_serial = 0;
    for (int i = 0; i < express_count; i++) {
        // express ID format: EXP######, try to parse serial number
        int num = 0;
        if (sscanf(express_db[i].id, "EXP%d", &num) == 1) {
            if (num > next_serial) {
                next_serial = num;
            }
        }
    }
    
    fclose(fp);
}

// display menu
void display_menu(void) {
    printf("\n");
    printf("================ Express Management System ================\n");
    printf(" 1. Add Express\n");
    printf(" 2. Query Express\n");
    printf(" 3. Mark as Picked Up\n");
    printf(" 4. Delete Express\n");
    printf(" 5. Statistics\n");
    printf(" 6. Save Data\n");
    printf(" 0. Exit\n");
    printf("Please select (0-6): ");
}

// main function
int main(void) {
    load_from_file();
    
    int choice;
    
    while (1) {
        display_menu();
        char buf[32];
        read_line(buf, (int)sizeof(buf));
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
                printf("Thank you for using, goodbye!\n");
                save_to_file();
                return 0;
            default:
                printf("Invalid choice, please try again!\n");
        }
    }
    
    return 0;
}
