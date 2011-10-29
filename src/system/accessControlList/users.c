#include "system/accessControlList/users.h"
#include "system/mm.h"
#include "system/common.h"
#include "library/string.h"
#include "library/stdio.h"
#include "library/stdlib.h"

#define INVALID_PASSWD  1
#define PASSWD_CHANGED  0
#define MAX_USERS       128

static void parseUserLine(char* str, struct User* user, char* def_group);
static int updateUsersFile(struct User* user, int delete);

static struct User* registered_users[MAX_USERS];
static int registered_users_num = 0;

void users_init(void) {

}

int get_users_num(void) {

    //return registered_users_num;

    FILE* fp = fopen("/users", "r");
    int users = 0;
    char c;

    while((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            users++;
        }
    }
    fclose(fp);
    return users;
}

struct User* get_user_by_id(int id) {

    //return registered_users_[id];

    FILE* fp = fopen("/users", "r");
    disableInterrupts();
    struct User* user = kalloc(sizeof(struct User));
    enableInterrupts();

    char str[200];
    char def_group[100];
    do {
        if (fscanf(fp, "%s\n", str) == 0) {
            fclose(fp);
            //kfree(user);
            return NULL;
        }
        //printf("str %s\n",str);
        //printf("scanf devuelve %d\n",as);
        parseUserLine(str, user, def_group);
        //printf("uid %d\n", user->id);
        //printf("gid %d\n", user->gid);
        //printf("name %s\n", user->name);
        //printf("pass %s\n", user->passwd);
        //printf("group %s\n", def_group);
    } while(user->id != id);

    fclose(fp);
    return user;
}

void parseUserLine(char* str, struct User* user, char* def_group) {

        char* nextColumn = strchr(str, ':');
        char* buf = str;
        char aux[200];

        strncpy(user->name, buf, nextColumn - buf);
        user->name[nextColumn - buf] = '\0';
        buf = nextColumn + 1;

        //printf("%s\n",str);

        nextColumn = strchr(buf, ':');
        buf = nextColumn + 1;
        //printf("%s\n",str);

        nextColumn = strchr(buf, ':');
        strncpy(aux, buf, nextColumn - buf);
        aux[nextColumn - buf] = '\0';

        buf = nextColumn + 1;

        user->id = atoi(aux);
        //printf("uid %d\n",atoi(aux));

        nextColumn = strchr(buf, ':');
        strncpy(aux, buf, nextColumn - buf);
        aux[nextColumn - buf] = '\0';

        buf = nextColumn + 1;

        user->gid = atoi(aux);
        //printf("gid %d\n", atoi(aux));

        nextColumn = strchr(buf, ':');
        strncpy(aux, buf, nextColumn - buf);
        aux[nextColumn - buf] = '\0';

        strcpy(user->passwd, aux);
        //printf("%s\n",aux);

        buf = nextColumn + 1;

        strcpy(def_group, aux);
}

struct User* get_user_by_name(char* name) {

    //for (int i = 0; i < registered_users_num; i++) {

        //if (strcmp(registered_users[i]->name, name) == 0) {
            //return registered_users[i];
        //}
    //}
    //return NULL;


    FILE* fp = fopen("/users", "r");
    disableInterrupts();
    struct User* user = kalloc(sizeof(struct User));
    enableInterrupts();

    char str[200];
    char def_group[100];
    do {
        if (fscanf(fp, "%s\n", str) == 0) {
            fclose(fp);
            //kfree(user);
            return NULL;
        }

        //printf("str %s\n",str);
        //printf("scanf devuelve %d\n",as);
        parseUserLine(str, user, def_group);
        //printf("uid %d\n", user->id);
        //printf("gid %d\n", user->gid);
        //printf("name %s\n", user->name);
        //printf("pass %s\n", user->passwd);
        //printf("group %s\n", def_group);
    
    } while(strcmp(user->name, name) != 0);

    fclose(fp);
    return user;
}

char* get_username(int id) {

    //return registered_users[id]->name;
    return get_user_by_id(id)->name;
}

int change_passwd(int uid, char* old_passwd, char* new_passwd) {

    struct User* user = get_user_by_id(uid);

    if (strcmp(user->passwd, old_passwd) == 0) {
        strcpy(user->passwd, new_passwd);
        updateUsersFile(user, 0);
        return PASSWD_CHANGED;
    } else {
        return INVALID_PASSWD;
    }
}

int updateUsersFile(struct User* user, int delete) {
    FILE* fp = fopen("/users", "r");
    char line[200][100];
    char aux[100];
    int length, found = 0, i = 0;

    while(fscanf(fp, "%s\n", line[i++]) != 0);
        
    fclose(fp);
    fp = fopen("/users", "w");

    for (int j = 0; j < i - 1; j++) {
        
        length = strchr(line[j], ':') - line[j];
        strncpy(aux, line[j], length);
        aux[length] = '\0';

        if (strcmp(user->name, aux) == 0) {
            if (!delete) { 
                fprintf(fp, "%s:x:%d:%d:%s:%s\n", user->name, user->id, user->gid, user->passwd, "users");
                found = 1;
            }
        } else {
            fprintf(fp, "%s\n", line[j]);
        }
    }
    
    if (!found && !delete) {
        fprintf(fp, "%s:x:%d:%d:%s:%s\n", user->name, user->id, user->gid, user->passwd, "users");
    }

    fclose(fp);
    return 0;
}

int create_user(char* name, char* passwd) {

    //if (registered_users_num == MAX_USERS) {
        //return -1;
    //}

    //struct User* new_user = kalloc(sizeof(struct User));

    //new_user->id = registered_users_num;
    //strcpy(new_user->name, name);
    //strcpy(new_user->passwd, passwd);

    //registered_users[registered_users_num] = new_user;
    //registered_users_num++;

    //return new_user->id;
    
    FILE* fp = fopen("/users", "r+");
    char line[200];
    char group[100];
    struct User* users[MAX_USERS];
    int ids[MAX_USERS] = {0};
    int i = 0, id;

    while(fscanf(fp, "%s\n", line) != 0) {
        disableInterrupts();
        users[i] = kalloc(sizeof(struct User));
        enableInterrupts();

        parseUserLine(line, users[i], group);
        ids[users[i]->id] = 1;
        i++;
    }
    
    disableInterrupts();
    users[i] = kalloc(sizeof(struct User));
    enableInterrupts();


    for (int j = 0; j < MAX_USERS; j++) {
        if (ids[j] != 1) {
            id = j;
            break;
        }
    }

    strcpy(users[i]->name, name);
    strcpy(users[i]->passwd, passwd);
    users[i]->gid = 1;
    users[i]->id = id;
 
    updateUsersFile(users[i], 0);

    return id;
}

int delete_user(char* name) {

    //TODO make this right!
    //struct User* user = get_user_by_name(name);
    //if (user == NULL) {
        //return -1;
    //}

    //registered_users[user->id] = NULL;
    //registered_users_num--;

    //delete_group_member(user->gid, user->id);
    //kfree(user);
    //return 0;
    
    struct User* user = get_user_by_name(name);
    if (user == NULL) {
        return -1;
    }

    updateUsersFile(user, 1);
    return 0;
}



