#include "shell/groupaddmem/groupaddmem.h"
#include "system/accessControlList/groups.h"
#include "system/accessControlList/users.h"
#include "mcurses/mcurses.h"
#include "library/stdio.h"
#include "library/string.h"



void groupaddmem(char* argv) {

    char* firstSpace = strchr(argv,' ');
    char* secondSpace = strchr(firstSpace + 1, ' ');
    char aux[200];
    struct Group* group;
    struct User* user;

    if (firstSpace == NULL || secondSpace == NULL || strcmp(secondSpace + 1, "") == 0) {
        printf("Usage: groupaddmem GROUP MEMBER\n");
        return;
    }
    
    strncpy(aux, firstSpace + 1, secondSpace - firstSpace - 1);
    aux[secondSpace - firstSpace - 1] = '\0';

    group = get_group_by_name(aux);
    user = get_user_by_name(secondSpace + 1);

    if (group == NULL) {
        printf("-Group %s does not exists\n", aux);
        printf("-Please choose another\n");
    }

    if (user == NULL) {
        printf("-User %s does not exists\n", secondSpace + 1);
        printf("-Please choose another\n");
    }

    add_group_member(group->id, user->id);  
}


void manGroupaddmem(void) {
    setBold(1);
    printf("Usage:\n\t groupaddmem ");
    setBold(0);

    printf("GROUP MEMBER\n");
}

