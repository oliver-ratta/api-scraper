#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


int leaks = 0;


int checktext(char *line) {

    char *patterns[] = {
        "sk-",          // theese are commen api extentions 
        "sk-ant-",
        "AIza",
        "gsk_",
        "pplx-",
        "ghp_",
        "github_pat_",
        "gho_",
        "ghs_",
        "ghu_",
        "glpat-",
        "AKIA",
        "ya29a",
        "azure_",
        "xoxb-",
        "xoxp-",
        "SG.",
        "sk_live_",
        "pk_live_",
        "sk_test_",
        "pk_test_"
    };


    int amount = sizeof(patterns) / sizeof(patterns[0]);
    for (int i = 0; i < amount; i++) {

        if (strstr(line, patterns[i])) {
            return 1;
        }

    }
    return 0;
}

int codefile(char *name) {
    if (strstr(name, ".c")) return 1;
    if (strstr(name, ".cpp")) return 1;
    if (strstr(name, ".h")) return 1;
    if (strstr(name, ".hpp")) return 1;
    if (strstr(name, ".py")) return 1;
    if (strstr(name, ".js")) return 1;
    if (strstr(name, ".ts")) return 1;
    if (strstr(name, ".html")) return 1;
    if (strstr(name, ".css")) return 1;
    if (strstr(name, ".java")) return 1;
    if (strstr(name, ".go")) return 1;
    if (strstr(name, ".rs")) return 1;
    if (strstr(name, ".php")) return 1;
    if (strstr(name, ".sh")) return 1;
    return 0;
}



void scanfile(char *file) {
    FILE *f = fopen(file,"r");
    if (!f)
        return;
    char line[1024];
    int number = 1;
    while(fgets(line,sizeof(line),f)) {
        if(checktext(line)) {
            printf("\nLEAK FOUND\n");
            printf("file: %s\n",file);
            printf("line: %d\n",number);
            printf("%s",line);
            leaks++;
        }
        number++;
    }
    fclose(f);
}


void scan(char *folder) {
    DIR *dir = opendir(folder);
    if(!dir)
        return;
    struct dirent *entry;
    while((entry = readdir(dir))) {
        if(strcmp(entry->d_name,".") == 0 ||
           strcmp(entry->d_name,"..") == 0)
            continue;
        char path[512];
        sprintf(path,"%s/%s",folder,entry->d_name);
        struct stat info;
        stat(path,&info);
        if(S_ISDIR(info.st_mode)) {
            scan(path);
        }

        else if(S_ISREG(info.st_mode)) {
            if(codefile(path)) {
                printf("checking %s\n",path);
                scanfile(path);

            }

        }

    }

    closedir(dir);
}




int main() {
    char folder[256];

    printf("folder: ");
    scanf("%255s",folder);
    scan(folder);
    printf("\n\nRESULTS\n");
    
    if(leaks == 0) {
        printf("No leaks found\n");
    }

    else {
        printf("%d possible leaks found\n",leaks);
    }


    


    return 0;
}
