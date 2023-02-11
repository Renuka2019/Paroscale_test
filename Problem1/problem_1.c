#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include<pthread.h>
#include <unistd.h>


int no_of_sub_directory_in_directory;
int no_of_files_in_each_directory;

int total_no_directory_in_linux;
int total_no_of_file_in_linux;


DIR* _get_root(void) 
{
    DIR *d = NULL, *prev = NULL;
    char *path = (char *)malloc(strlen(".") + 1);
    char *pdir = "/..";

    strcpy(path, ".");

    do 
    {
        if (prev) 
        {
            closedir(prev);
        }
        prev = d;

        path = (char*)realloc(path, strlen(path) + strlen(pdir) + 1);
        strcat(path, pdir);

        d = opendir(path);
    } while (d);

    free(path);

    return prev;
}


void _ls(const char *dir)
{	 
	char *newpath;
    char  *filepath;
    char *separator = "/"; 
    struct dirent *d;

	newpath =(char*) malloc(1000);
	
	if(!newpath )
	{
		puts("unable to allocate memory");
		exit(EXIT_FAILURE);
	}
	memset(newpath,0,sizeof(newpath));
 
    filepath = (char*)malloc(1000);

	if(!filepath)
	{
		puts("unable to allocate memory");
		exit(EXIT_FAILURE);
	}

	
	memset(filepath,0,sizeof(filepath));
	
	strcpy(filepath,dir);
    
   
    strcpy(newpath,dir);
 
    strcat(filepath, "/");  

                                                
  
    if(access(filepath,R_OK)!=0)
    {
        return;
    }


	DIR *dh = opendir(filepath);


	if(!dh)
	{
		if (errno = ENOENT)
		{
            return;
           
			perror("Directory doesn't exist");
		}
		else
		{
			
			perror("Unable to read directory");
		}
    }

	
	
 if(dh)
 {
    d = readdir(dh);
do{
	if(d->d_type == DT_DIR)
	{

        no_of_sub_directory_in_directory++;
        total_no_directory_in_linux++;

		if ( d->d_name[0] == '.')
			continue;
       		
            strcat(newpath, separator);
		
        	strcat(newpath, d->d_name);
    	
		    _ls(newpath);
		
			memset(newpath,0,1000);
		    strcpy(newpath,dir);  
           
	}
	else
	{
        
		no_of_files_in_each_directory++;
        total_no_of_file_in_linux++;
       
	}
   

}while((d = readdir(dh)) != NULL);

 closedir(dh);	

free(newpath);
newpath = NULL;
free(filepath);
filepath = NULL;    
 }

}


void *thread_entry_function(void *args)
{
    char* str = (char*) args;
    _ls(str);
    
}



int main(int argc, const char *argv[])
{
     DIR   *root = _get_root();
    struct dirent   *sub;
    pthread_t thread_id;
    

    char* str = (char*)malloc(100);
     memset(str,0,100);
    

    while ((sub = readdir(root))) 
    {
        
        
         strcpy(str,"/");

       	if(sub->d_name[0] == '.')
            continue;
        strcat(str,sub->d_name);  
        pthread_create(&thread_id,NULL,thread_entry_function,(void*)str);
        pthread_join(thread_id,NULL);

      
         printf("no of subdirectory in %s = %d\n",str,no_of_sub_directory_in_directory);
         printf("no of  files in directory %s = %d\n",str,no_of_files_in_each_directory);
         printf("-----------------------------------------------------------------------\n\n");
         no_of_sub_directory_in_directory = 0;
         no_of_sub_directory_in_directory = 0;


        memset(str,0,100);
       
    }

    printf("no of dirctory in linux directory = %d\n",total_no_directory_in_linux);
    printf("no of files in linux dirctory = %d\n",total_no_of_file_in_linux);

    closedir(root);

	return 0;

}
