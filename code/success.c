#include<stdio.h>
#include<time.h>
#include<string.h>

#define FSIZE           10     //edit this if u want to have filname of greater length
#define PATHSIZE        100    //edit this if u want to have greater depths in the directory hierarchy
#define BLOCKSIZE 	64     //edit this if u want to have blocksize greater	
#define INODEBLOCK 	7000
#define DATABLOCK  	14000 
#define TOTBLOCKS 	100    //edit this if u want to have more number of data blocks
#define OWNER           "NAIDU"
#define CLEAR           "clear"
#define OS              "Linux"

char shellprompt[PATHSIZE]="IBFS/";          /* This is the Shell Prompt Display Initiation*/
char act[25];
int currentdepth=1;                    /* This is to track the depth form the root of file system*/
int inodenumber,recno;
int totavarec,reccleared;               /*Variables to store the status of data block area*/
int deny;

char *help1="\tCommands Valid in IBFS Shell are:\n\tUse ~ as argument to Bypass a command"
  	    "(Only for Some Commands as specified)\n";
char *helpcd="\n\t cd <directory name> \t,cd < / > \t,cd <..> \t,cd <.> \t,cd <fullpath> -- To Change Directory(cd)";
char *helpmd="\n\t md <directory name> -- To Create a Directory(mkdir)";
char *helpvd="\n\t vd -- To View Contents of Directory(dir)";
char *helpmv="\n\t mv <sourcefile> <destinationfile> -- To Rename a File(mv and ren)";
char *helpmf="\n\t mf <file name> -- To Create a File(cat and copy con)";
char *helprd="\n\t rd <directory name> -- To Remove an Empty Directory(rmdir)";
char *helpvf="\n\t vf <file name> -- To View the Contents of File(cat and type)";
char *helprf="\n\t rf <file name> -- To Remove a File(rm and del)";
char *helpcf="\n\t cf <sourcefile> <destinationfile> -- To Copy Contents fo source to destination(cp and copy)";
char *helpis="\n\t is -- To View the Inode Structure(INNOVATION)";
char *helprs="\n\t rs -- To View the Root Structure or File Descriptor Block(INNOVATION,tree)";
char *helpcl="\n\t cl -- To Clear the Screen Buffer(clear,cls)";
char *helpdt="\n\t dt -- To Display Date(date,time)";
char *helpdf="\n\t df -- To Display Disk Free(INNOVATION,df)";
char *helpdu="\n\t du -- To Display Disk Usage(INNOVATION,du)";
char *helppd="\n\t pd -- To Display the Current Working Directory(pwd)";
char *helpft="\n\t ft -- To Format the Disk(format)";
char *helph ="\n\t h  -- To Display Help on Commands(?,--h,man)";
char *helpat="\n\t at -- To Display About the Version Related Details(ver)";
char *helpqt="\n\t qt -- To Quit or Logout(logout,exit,quit)";
char *helps ="\n\t s <command> -- To Display Help on Individaul Commands(--h,?)";
char *helpe ="\n\t eo -- To Echo Text on to the Screen Buffer(echo)";
char *helpwc="\n\t wc <file name> -- To Calculate the Lines ans Words in a File(wc)";
char *about="\tVersion: 1.0.0.1\n\tAuthor: "
            "Venkat Vidya Venu\n\tname: inode based file system simulation software\n"
            "\tCompany: ssss Pvt Ltd.\n";

char *exmd="\n\t md venki--creates directory venki in the current directory,"
			       "\n\t\tGives error message if a directory lready exists!";
char *excd="\n\t cd . -- retains prompt in the current directory"
		"\n\t cd .. -- Goes in to the Parent Directory"
		"\n\t cd venki -- goes on directory named \"venki\" if it exists in the current directory"
		"\n\t\telse displays error message"
		"\n\t cd / -- goes to the root directory"
		"\n\t cd IBFS/venki/naidu -- goes to specified directory if complete path is specified exists";
char *exrd="\n\t rd venki -- removes directory named venki if it exists in current working directory";
char *exvd="\n\t vd -- lists the contents of current working directory";
char *exmf="\n\t mf venki.txt -- creates a file with the specified name, Gives error message if it exists"; 
char *exvf="\n\t vf venki.txt -- displays the contents of the file if it exists in current directory else error message";
char *exrf="\n\t rf venki.txt -- removes file specified if it exists in current directory else error message";
char *excf="\n\t cp venki.txt dest.txt -- copies the file venki.txt to dest.txt if venki.txt exists else error message"
		"\n\t\t\tdisplays if dest.txt already exists";
char *exmv="\n\t mv venki.txt dest.txt -- renames the file venki.txt to dest.txt if venki.txt exists else error message"
                "\n\t\t\tdisplays if dest.txt already exists";
char *exrs="\n\t rs -- (root structure) displays the contents of the file system with relevant details";
char *exis="\n\t is -- (inode structure) displays the contents of the inode part of the file system";
char *excl="\n\t cl -- clears the screen buffer";
char *exdt="\n\t dt -- displays the current date and time";
char *exdf="\n\t df -- displays the available blocks for data relevant information";
char *exdu="\n\t du -- displays the usage blocks by files with % usage and internal fragmentation";
char *expd="\n\t pd -- displays the current working directory";
char *exft="\n\t ft -- queries for formatting the disk y to format and anything else to skip";
char *exh ="\n\t h -- diplays the available commands in the shell";
char *exat="\n\t at -- gives the information about the authors";
char *exqt="\n\t qt -- quits from the shell";
char *exs ="\n\t s md -- gives the mini man page related to command \"md\"";
char *exwc="\n\t wc venki.txt -- gives the number of lines and words in the file if it exists else error message";

struct sindextable
        {
        int depth;
        char type;
        char name[FSIZE+1];
        char path[PATHSIZE];
        int size;
        int inode;
        struct sindextable *next;
        }*head,*temp;

struct sinodes
        {
        int inumber;
        char type;
        int size;
        char owner[10];
        char ctime[20];
        int lcount;
        unsigned int b1;
        unsigned int b2;
        struct sinodes *next;
        }*head1,*temp1;

void display(struct sindextable *temp);
void displayi(struct sinodes *temp1);
void calculatecurrentdepth();
char* gettime();
void initializetables();
void makedirectory(char *s);
char* ret(char *s,int block,int size);
int blockallocate(char *str);
void makefile(char *s);
void removeentry(char *s,char type);
void changedirectory(char *s);
void viewdirectory();
void viewfile(char *s);
void copyfile(char *s,char *d);
void movefile(char *s,char *d);
void calculatediskusage();
void calculatediskfree();
void wordcount(char *s);
void syntax(char *str);
void finalize();
void formatdisk();

void display(struct sindextable *temp)
        {
	int i;
        printf("\n   Depth    Type\tName\t\t    Path\t\t\t    \tSize          InodeRef\n");
        for(i=0;i<60;i++)
                printf("--");
	printf("\n");
        while(temp!=NULL)
                {
		if(temp->type=='f')
                	printf("    %-10d%-10s%-20s%-20s\t%10d%20d\n",temp->depth,"FILE",temp->name,
                                             temp->path,temp->size,temp->inode);
		else
			printf("    %-10d%-10s%-20s%-20s\t%10s%20d\n",temp->depth,"DIR",temp->name,
                                             temp->path,"--",temp->inode);
                temp=temp->next;
                }printf("\n");
        }

void displayi(struct sinodes *temp1)
	{
	int i;
	printf("\n   Inode     Type     Size        Owner   \tCreationTime\t\t     LinkCount\tDataBlocksAllocated\n");
        for(i=0;i<60;i++)
                printf("--");
	printf("\n");
        while(temp1!=NULL)
                {
		if(temp1->type=='f')
                	printf("    %-10d%-10s%-10d%-10s%-20s\t\t%d\t\t%d\t%d\n",temp1->inumber,"FILE",
                                    temp1->size, temp1->owner,temp1->ctime,temp1->lcount,temp1->b1,temp1->b2);
		else
			printf("    %-10d%-10s%-10s%-10s%-20s\t\t%d\t\t%s\t%s\n",temp1->inumber,"DIR",
                                    "--", temp1->owner,temp1->ctime,temp1->lcount,"--","--");

                temp1=temp1->next;
                }printf("\n");
	}

void calculatecurrentdepth()
        {
        int i=0;
        currentdepth=0;
        for(i=0;shellprompt[i];i++)
                {
                if(shellprompt[i]=='/')
                        currentdepth=currentdepth+1;
                }
        }

char* gettime()
        {
        time_t timer;
        timer=time(NULL);
        char time[30];
        int i;
        strcpy(time,asctime(localtime(&timer)));
        for(i=0;time[i]!='\0';i++)
                if(time[i]==' ') 
			{
			act[i]=',';
			}
                else
                        {
                        act[i]=time[i];
                        }
        act[i]='\0';
        return(act);
        }

void initializetables()
        {
	FILE *itfp;
        int i;
        currentdepth=1;
	if((itfp=fopen("disk.ibfs","r"))!=NULL)
                {
                head=(struct sindextable *)malloc(sizeof(struct sindextable));
                temp=head;
		fscanf(itfp,"%d",&inodenumber);          /* reading the unique inode number*/
		fscanf(itfp,"%d",&recno);
                for(i=0;i<recno;i++) //upto 50 because only 50 entries are allowed in File Descriptor Block
                        {
                        fscanf(itfp,"%d %c %s %s %d %d",&temp->depth,&temp->type,&temp->name,
                        	                        &temp->path,&temp->size,&temp->inode);
                        temp->next=(struct sindextable *)malloc(sizeof(struct sindextable));
                        temp=temp->next;
                        temp->next=NULL;
                        }
		}
        temp=head;
        while(temp->next!=NULL)
                {
                if(temp->next->next==NULL)
                        {
                        temp->next=NULL;
                        break;
                        }
                temp=temp->next;
                }
	fclose(itfp);
	if((itfp=fopen("disk.ibfs","r"))!=NULL) /* for inode block*/
                {
		fseek(itfp,INODEBLOCK,SEEK_SET);
                head1=(struct sinodes *)malloc(sizeof(struct sinodes));
                temp1=head1;
                for(i=0;i<recno;i++) //upto 50 because only 50 entries are allowed in File Descriptor Block
                        {
                        fscanf(itfp,"%d %c %d %s %s %d %d %d",&temp1->inumber,&temp1->type,&temp1->size,
                                            &temp1->owner,&temp1->ctime,&temp1->lcount,
					    &temp1->b1,&temp1->b2);
                        temp1->next=(struct sinodes *)malloc(sizeof(struct sinodes));
                        temp1=temp1->next;
                        temp1->next=NULL;
                        }
                }
        temp1=head1;
        while(temp1->next!=NULL)
                {
                if(temp1->next->next==NULL)
                        {
                        temp1->next=NULL;
                        break;
                        }
                temp1=temp1->next;
                }
	inodenumber++;
	fseek(itfp,DATABLOCK,SEEK_SET);
	fscanf(itfp,"%d%d",&totavarec,&reccleared);
        fclose(itfp);
	}

void makedirectory(char *s)
        {
	int pos;
        temp=head;
        while(temp!=NULL)
                {
                if( (!strcmp(temp->name,s)) && (!strcmp(temp->path,shellprompt)) && (temp->type=='d'))
                        {
                        printf("\n Directory Exists! Try with Different Name\n");
                        return;
                        }
                temp=temp->next;
                }
        temp=head;
        while(temp->next!=NULL)
                temp=temp->next;
        temp->next=(struct sindextable *)malloc(sizeof(struct sindextable));
        temp=temp->next;
        calculatecurrentdepth();
        temp->depth=currentdepth;
        temp->type='d';
        strcpy(temp->name,s);
        strcpy(temp->path,shellprompt);
        temp->size=0;
	temp->inode=inodenumber;
        temp->next=NULL;
	temp1=head1;
	while(temp1->next!=NULL)
		temp1=temp1->next;
	temp1->next=(struct sinodes *)malloc(sizeof(struct sinodes));
	temp1=temp1->next;
	temp1->type='d';
	temp1->inumber=inodenumber++;
	temp1->size=0;
	strcpy(temp1->owner,OWNER);
	strcpy(temp1->ctime,gettime());
	temp1->lcount=0;
	temp1->b1=0;
	temp1->b2=0;
	temp1->next=NULL;
	recno=recno+1;
	finalize();
	}

char* ret(char *s,int block,int size)
        {
        char *str;
        int i,j;
        str=(char *)malloc( (size+1) * sizeof(char));
        if(block==1)
                {
                for(i=0;i<size;i++)
                        str[i]=s[i];
                str[i]='\0';
                return(str);
                }
        else
                {
                for(i=0,j=size;i<size;i++,j++)
                        {
                        if(s[j]=='\0')
                                {
                                str[i]='\0';
                                return(str);
                                }
                        str[i]=s[j];
                        }
                str[i]='\0';
                return(str);
                }
        }

int blockallocate(char *str)
	{
	FILE *fp;
	int i,status,fpos,j;
	char waste[BLOCKSIZE];
        fp=fopen("disk.ibfs","r+w");
        fseek(fp,DATABLOCK,SEEK_SET);
        fscanf(fp,"%d %d",&totavarec,&reccleared);
        if(totavarec==0)
		printf("\nDisk is FULL,Delete Some Files and Continue:\n");
	else
        		if(reccleared==0)
                                {
				fseek(fp,DATABLOCK+10,SEEK_SET);
                                fpos=ftell(fp);
                                fprintf(fp,"%d %s ",1,str);
                                reccleared=1;
                                totavarec-=1;
                                fseek(fp,DATABLOCK,SEEK_SET);
                                fprintf(fp,"%d %d ",totavarec,reccleared);
                                fclose(fp);
                                return(fpos);
                                }
                        else
                                for(i=0,j=0;i<reccleared;i++,j=j+BLOCKSIZE+3)
                                        {
					if(i==0) 
						fseek(fp,DATABLOCK+10,SEEK_SET);
					else if(i==1) 
						{
						j=j+10;
						fseek(fp,DATABLOCK+j,SEEK_SET);
						}
					else     
						fseek(fp,DATABLOCK+j,SEEK_SET);
                                        fpos=ftell(fp);
                                        fscanf(fp,"%d",&status);
                                        if(status==0)
                                                {
						rewind(fp);
						if(i==0)
	                                                fseek(fp,DATABLOCK+10,SEEK_SET);
                                        	else
                                                	fseek(fp,DATABLOCK+j,SEEK_SET);

                                                fprintf(fp,"%d %s ",1,str);
                                                totavarec-=1;
						fseek(fp,DATABLOCK,SEEK_SET);
					        fprintf(fp,"%d %d ",totavarec,reccleared);
						fclose(fp);
                                                return(fpos);
                                                }
                                        else
						{
                                                fscanf(fp,"%s",&waste);
						}
                                        }
			fseek(fp,DATABLOCK + 10 + (reccleared * (BLOCKSIZE+3)),SEEK_SET);
                        fpos=ftell(fp);
                        fprintf(fp,"%d %s ",1,str);
                        totavarec-=1;
                        reccleared+=1;
                                fseek(fp,DATABLOCK,SEEK_SET);
                                fprintf(fp,"%d %d ",totavarec,reccleared);
                        fclose(fp);
                        return(fpos);
        }

void makefile(char *s)
        {
	int pos,i;
	char str[BLOCKSIZE*2];
        temp=head;
        while(temp!=NULL)
                {
                if( (!strcmp(temp->name,s)) && (!strcmp(temp->path,shellprompt)) && (temp->type=='f'))
                        {
                        printf("\n File Exists! Try with Different Name\n");
                        return;
                        }
                temp=temp->next;
                }
        temp=head;
        while(temp->next!=NULL)
                temp=temp->next;
        temp->next=(struct sindextable *)malloc(sizeof(struct sindextable));
        temp=temp->next;
        calculatecurrentdepth();
        temp->depth=currentdepth;
        temp->type='f';
        strcpy(temp->name,s);
        strcpy(temp->path,shellprompt);
        temp->inode=inodenumber;
        temp->next=NULL;
        temp1=head1;
        while(temp1->next!=NULL)
                temp1=temp1->next;
        temp1->next=(struct sinodes *)malloc(sizeof(struct sinodes));
        temp1=temp1->next;
        temp1->type='f';
        temp1->inumber=inodenumber++;
        strcpy(temp1->owner,OWNER);
        strcpy(temp1->ctime,gettime());
	temp1->lcount=0;
        temp1->next=NULL;
        printf("\nEnter the contents of file and ~ to stop\n");
        for(i=0;;i++)
                {
                scanf("%c",&str[i]);
                if(str[i]=='~')
                        break;
                }str[i]='\0';
        printf("\n\tFile Size is %d bytes\n",i);
        if(i>(2*BLOCKSIZE))
                printf("\n\tThe Size of File Exceeded the Limit and part of it is Truncated\n");
	for(i=0;str[i]!='\0';i++)
		{
		if(str[i]=='\n')
			str[i]='`';
		if(str[i]=='\t')
			str[i]='#';
		if(str[i]==' ')
			str[i]='%';
		}
	if(i<=2*BLOCKSIZE)
		{
        	temp->size=i;
		temp1->size=i;
		}
	else
		{
		temp->size=2*BLOCKSIZE;
		temp1->size=2*BLOCKSIZE;
		}
	
        recno=recno+1;

	if(strlen(str)<=BLOCKSIZE)
                {
		temp1->b1=blockallocate(str);
                }
        else
                {
		temp1->b1=blockallocate(ret(str,1,BLOCKSIZE));
		temp1->b2=blockallocate(ret(str,2,BLOCKSIZE));
                }
	finalize();
	}

void removeentry(char *s,char type)
	{
        FILE *fp;
	int inum,len;
	char dir[PATHSIZE];
        struct sinodes *swap1;
        struct sindextable *swap;
	if(type=='d')
		{
		strcpy(dir,shellprompt);
	        strcat(dir,s);
		len=strlen(dir);
        	dir[len]='/';
	        dir[len+1]='\0';
		temp=head;
		while(temp!=NULL)
			{
			if(!strcmp(dir,temp->path))
				{
				printf("\n\tCannot Remove Direcorty! Directory is not empty\n");
                                return;	
				}
			temp=temp->next;
			}	
		}
        temp=head;
        calculatecurrentdepth();
	temp=head;
        while(temp!=NULL)
                {
                if(!strcmp(temp->name,s) && (!strcmp(temp->path,shellprompt)) && (temp->type==type))
                        {
                        if(head==temp)
                                {
				inum=head->inode;
                                head=head->next;
                                goto NEXT;
                                }
                        else
                                {
				inum=temp->inode;
                                swap->next=temp->next;
                                goto NEXT;
                                }
                        }
                else
                        {
                        swap=temp;
                        temp=temp->next;
                        }
                }
	NEXT:			/* removal of inode entries and releasing the blocks*/
	   temp1=head1;
	   while(temp1!=NULL)
                {
                if(temp1->inumber==inum)
                        {
                        if(head1==temp1)
                                {
	                        if(type=='f')
        	                        {
					if(temp1->size<=BLOCKSIZE)
						{
						fp=fopen("disk.ibfs","r+w");
						fseek(fp,temp1->b1,SEEK_SET);
						fprintf(fp,"%d",0);
						totavarec+=1;
						fseek(fp,DATABLOCK,SEEK_SET);
                                                fprintf(fp,"%d %d ",totavarec,reccleared);
						fclose(fp);
						}
					else
						{
						fp=fopen("disk.ibfs","r+w");
                                                fseek(fp,temp1->b1,SEEK_SET);
                                                fprintf(fp,"%d",0);
						fseek(fp,temp1->b2,SEEK_SET);
                                                fprintf(fp,"%d",0);
						totavarec+=2;
						fseek(fp,DATABLOCK,SEEK_SET);
                                                fprintf(fp,"%d %d ",totavarec,reccleared);
                                                fclose(fp);
						}	
                	                }
                                head1=head1->next;
				recno=recno-1;
				finalize();	/*think over it this may cause seg fault*/
                                return;
                                }
                        else
                                {
                                if(type=='f')
                                        {
                                        if(temp1->size<=BLOCKSIZE)
                                                {
                                                fp=fopen("disk.ibfs","r+w");
                                                fseek(fp,temp1->b1,SEEK_SET);
                                                fprintf(fp,"%d",0);
						fseek(fp,DATABLOCK,SEEK_SET);
                                                fprintf(fp,"%d %d ",totavarec,reccleared);
						totavarec+=1;
                                                fclose(fp);
                                                }
                                        else
                                                {
                                                fp=fopen("disk.ibfs","r+w");
                                                fseek(fp,temp1->b1,SEEK_SET);
                                                fprintf(fp,"%d",0);
                                                fseek(fp,temp1->b2,SEEK_SET);
                                                fprintf(fp,"%d",0);
						fseek(fp,DATABLOCK,SEEK_SET);
                                                fprintf(fp,"%d %d ",totavarec,reccleared);
						totavarec+=2;
                                                fclose(fp);
                                                }
                                        }
                                swap1->next=temp1->next;
				recno=recno-1;
				finalize();
                                return;
                                }
                        }
                else
                        {
                        swap1=temp1;
                        temp1=temp1->next;
                        }
                }
        if(type=='d')
                printf("\n No Directory with the given Name Exists\n");
        else
                printf("\n No File with the given Name Exists\n");
	
        }

void changedirectory(char *s)
        {
	int len,z;
        char ven[PATHSIZE],zen[PATHSIZE];
	if(!strcmp(s,"/"))
		{
		strcpy(shellprompt,"IBFS/");
		return;
		}
	if(!strcmp(s,"."))
		return;
	if(!strcmp(s,".."))
		{
		if(!strcmp(shellprompt,"IBFS/"))
			{
			printf("\n\t!Already in the TopMost Part of Root Directory Cannot Traverse Back\n");
			return;
			}
		else
			{
			strcpy(zen,shellprompt);
			len=strlen(zen);
			for(z=len;zen[z]!='/';z--);
			for(z=z-1;zen[z]!='/';z--);
				zen[z]='/';
			zen[z+1]='\0';
			strcpy(shellprompt,zen);
			return;
			}
		}
        temp=head;
        while(temp!=NULL)
                {
                if(temp->type == 'd')
                        {
                        if( !strcmp(temp->name,s) && !strcmp(temp->path,shellprompt) )
                                {
                                strcat(shellprompt,temp->name);
                                strcat(shellprompt,"/");
                                currentdepth=temp->depth;
                                return;
                                }
                        else
                                {
                                strcpy(ven,temp->path);
                                strcat(ven,temp->name);
                                if(!strcmp(ven,s))
                                        {
                                        strcpy(shellprompt,ven);
                                        strcat(shellprompt,"/");
                                        currentdepth=temp->depth;
                                        return;
                                        }
                                }
                        }
                temp=temp->next;
                }
        printf("\n No Directory with the Given Name Exists\n");
	return;
        }

void viewdirectory()
        {
	int i=0;
        temp=head;
	temp1=head1;
        printf("\n      Type      \tName\t     Owner     InodeNumber\t  CreationTime\t\t Size\tLinkCount\t\n");
	for(i=0;i<60;i++)
		printf("--");
        printf("\n\t.\n\t..");
        while(temp!=NULL)
                {
		while(temp1!=NULL)
		    {
	            if( !strcmp(temp->path,shellprompt) && temp1->inumber==temp->inode)
		       if(temp1->type=='f')
		          printf("\n%10s\t%10s\t%10s\t%5d\t     %5s\t%5d\t%5d","FILE",temp->name,temp1->owner,temp->inode,
			  	   temp1->ctime,temp1->size,temp1->lcount);
   		       else
               	          printf("\n%10s\t%10s\t%10s\t%5d\t     %5s\t%5s\t%5d","DIR",temp->name,temp1->owner,temp->inode,
						temp1->ctime,"--",temp1->lcount);
                    temp1=temp1->next;
		    }
		temp=temp->next;
		temp1=head1;
                }
        printf("\n");
        }

void viewfile(char *s)
	{
	int inoden,i;
	temp=head;
	char data[BLOCKSIZE];
	FILE *fp;
	while(temp!=NULL)
		{
		if(!strcmp(temp->name,s) && !strcmp(temp->path,shellprompt) && (temp->type=='f'))
			{
			inoden=temp->inode;
			break;		
			}
		temp=temp->next;
		}
	temp1=head1;
	while(temp1!=NULL)
		{
		if(temp1->inumber==inoden)
			{
			fp=fopen("disk.ibfs","r");
			if(temp1->size<=BLOCKSIZE)	
				{	
				fseek(fp,temp1->b1+1,SEEK_SET);
				fscanf(fp,"%s",&data);
				for(i=0;data[i]!='\0';i++)
                			{
			                if(data[i]=='`')
			                        data[i]='\n';
      				        if(data[i]=='#')
                		        	data[i]='\t';
		                	if(data[i]=='%')
                        			data[i]=' ';
        		        	}
				printf("%s\n",data);
				fclose(fp);
				return;
				}
			else
				{
				fseek(fp,temp1->b1 + 1,SEEK_SET);
				fscanf(fp,"%s",&data);
                                for(i=0;data[i]!='\0';i++)
                                        {
                                        if(data[i]=='`')
                                                data[i]='\n';
                                        if(data[i]=='#')
                                                data[i]='\t';
                                        if(data[i]=='%')
                                                data[i]=' ';
                                        }
				printf("%s",data);
				fseek(fp,temp1->b2 + 1,SEEK_SET);
				fscanf(fp,"%s",&data);
                                for(i=0;data[i]!='\0';i++)
                                        {
                                        if(data[i]=='`')
                                                data[i]='\n';
                                        if(data[i]=='#')
                                                data[i]='\t';
                                        if(data[i]=='%')
                                                data[i]=' ';
                                        }
				printf("%s\n",data);
				fclose(fp);
				return;
				}
			}
		temp1=temp1->next;
		}
	printf("\n\tFILE doesnot Exists!\n");
	return;
	}

void copyfile(char *s,char *d)
        {
	int inoden,cpsize;
	FILE *fp;
	char data[BLOCKSIZE],cpdata[BLOCKSIZE];
        temp=head;
        while(temp!=NULL)
                {
                if(!strcmp(temp->name,s) && !strcmp(temp->path,shellprompt) && (temp->type=='f'))
                        {
                        goto ACCEPTED; 
                        }
                temp=temp->next;
		if(temp==NULL)
			{
			printf("\nCopy Denied! Source File doesnot exists\n");
			return;
			}
                }
	ACCEPTED:
	temp=head;
        while(temp!=NULL)
                {
                if(!strcmp(temp->name,d) && !strcmp(temp->path,shellprompt) && (temp->type=='f'))
                        {
                        printf("\nCopy Denied! File already exists\n");
                        return;
                        }
                temp=temp->next;
                }
	temp=head;
        while(temp!=NULL)
                {
                if(!strcmp(temp->name,s) && !strcmp(temp->path,shellprompt) && (temp->type=='f'))
                        {
                        inoden=temp->inode;
                        break;
                        }
                temp=temp->next;
                }
        temp1=head1;
        while(temp1!=NULL)
                {
                if(temp1->inumber==inoden)
                        {
                        fp=fopen("disk.ibfs","r");
			cpsize=temp1->size;
                        if(temp1->size<=BLOCKSIZE)
                                {
                                fseek(fp,temp1->b1+1,SEEK_SET);
                                fscanf(fp,"%s",&data);
                                fclose(fp);
				break;
                                }
                        else
                                {
                                fseek(fp,temp1->b1 + 1,SEEK_SET);
                                fscanf(fp,"%s",&data);
                                fseek(fp,temp1->b2 + 1,SEEK_SET);
                                fscanf(fp,"%s",&cpdata);
                                fclose(fp);
				break;
                                }
                        }
		temp1=temp1->next;
		}
	temp=head;
        while(temp->next!=NULL)
                temp=temp->next;
        temp->next=(struct sindextable *)malloc(sizeof(struct sindextable));
        temp=temp->next;
        calculatecurrentdepth();
        temp->depth=currentdepth;
        temp->type='f';
        strcpy(temp->name,d);
        strcpy(temp->path,shellprompt);
        temp->inode=inodenumber;
        temp->next=NULL;
        temp1=head1;
        while(temp1->next!=NULL)
                temp1=temp1->next;
        temp1->next=(struct sinodes *)malloc(sizeof(struct sinodes));
        temp1=temp1->next;
        temp1->type='f';
        temp1->inumber=inodenumber++;
        strcpy(temp1->owner,OWNER);
        strcpy(temp1->ctime,gettime());
        temp1->lcount=0;
        temp1->next=NULL;
        temp->size=cpsize;
        temp1->size=cpsize;
        recno=recno+1;
        if(cpsize<=BLOCKSIZE)
                {
                temp1->b1=blockallocate(data);
                }
        else
                {
                temp1->b1=blockallocate(data);
                temp1->b2=blockallocate(cpdata);
                }
	deny=1;
        finalize();
        }

void movefile(char *s,char *d)
	{
	deny=0;
	copyfile(s,d);
	if(deny==1)
		removeentry(s,'f');
	}

void calculatediskusage()
	{
	float a=0.00f;
	temp=head;
	printf("\n\t\tNAME\t\tSIZE\tINTERNELFRAGMENTATION\t  USAGE %%\t FRAGMENTATTION%%\n");
	while(temp!=NULL)
		{
		if(temp->type=='f')
			{
			a=temp->size;
			if(temp->size<=BLOCKSIZE)
				{
				a/=BLOCKSIZE;
				printf("\n\t  %10s\t\t%d\t\t%d\t\t  %.1f%%\t\t  %.1f%%",temp->name,temp->size, 
				                 BLOCKSIZE - (temp->size),a*100,100-a*100);
				}
			else
				{
				a/=(2*BLOCKSIZE);
				printf("\n\t  %10s\t\t%d\t\t%d\t\t  %.1f%%\t\t  %.1f%%",temp->name,temp->size,
                                                 (2*BLOCKSIZE) - (temp->size),a*100,100-a*100);
				}
			}
		temp=temp->next;
		}printf("\n");
	}

void calculatediskfree()
	{
	float totalsize;
	FILE *itfp;
	itfp=fopen("disk.ibfs","r");
	fseek(itfp,DATABLOCK,SEEK_SET);
        fscanf(itfp,"%d%d",&totavarec,&reccleared);
        fclose(itfp);
	totalsize=TOTBLOCKS-totavarec;
	totalsize/=TOTBLOCKS;
	printf("\n\tFILE SYSTEM\t\t%d-BLOCKS\t USED\t AVAILABLE \tUSE%%\t\n",BLOCKSIZE);
	printf("\n\t   IBFS    \t\t    %d   \t  %d \t     %d    \t %.1f%% \n",TOTBLOCKS,TOTBLOCKS-totavarec,totavarec,
									   totalsize*100);
	}

void wordcount(char *s)
	{
	printf("\n\t:) Will be available in the next release\n");
	}

void syntax(char *str)
	{
	printf("\n\tGenerating \"MINI :)\" Man Page for \"%s\" Command\n\t",str);
             if(!strcmp(str,"md") || !strcmp(str,"MD") || !strcmp(str,"Md") || !strcmp(str,"mD")) 
			{printf(helpmd);printf("\n\n\tEX:");printf(exmd);}
        else if(!strcmp(str,"rd") || !strcmp(str,"RD") || !strcmp(str,"rD") || !strcmp(str,"Rd"))  
			{printf(helprd);printf("\n\n\tEX:");printf(exrd);}
        else if(!strcmp(str,"cd") || !strcmp(str,"CD") || !strcmp(str,"Cd") || !strcmp(str,"cD")) 
			{printf(helpcd);printf("\n\n\tEX:");printf(excd);}
        else if(!strcmp(str,"vd") || !strcmp(str,"VD") || !strcmp(str,"Vd") || !strcmp(str,"vD"))
			{printf(helpvd);printf("\n\n\tEX:");printf(exvd);}
        else if(!strcmp(str,"mf") || !strcmp(str,"MF") || !strcmp(str,"Mf") || !strcmp(str,"mF"))
			{printf(helpmf);printf("\n\n\tEX:");printf(exmf);}
        else if(!strcmp(str,"rf") || !strcmp(str,"RF") || !strcmp(str,"Rf") || !strcmp(str,"rF"))
			{printf(helprf);printf("\n\n\tEX:");printf(exrf);}
        else if(!strcmp(str,"vf") || !strcmp(str,"VF") || !strcmp(str,"Vf") || !strcmp(str,"vF"))
			{printf(helpvf);printf("\n\n\tEX:");printf(exvf);}
        else if(!strcmp(str,"cf") || !strcmp(str,"CF") || !strcmp(str,"Cf") || !strcmp(str,"cF"))
			{printf(helpcf);printf("\n\n\tEX:");printf(excf);}
        else if(!strcmp(str,"mv") || !strcmp(str,"MV") || !strcmp(str,"Mv") || !strcmp(str,"mV"))  
			{printf(helpmv);printf("\n\n\tEX:");printf(exmv);}
        else if(!strcmp(str,"rs") || !strcmp(str,"RS") || !strcmp(str,"Rs") || !strcmp(str,"rS"))  
			{printf(helprs);printf("\n\n\tEX:");printf(exrs);}
        else if(!strcmp(str,"is") || !strcmp(str,"IS") || !strcmp(str,"Is") || !strcmp(str,"iS"))
			{printf(helpis);printf("\n\n\tEX:");printf(exis);}
        else if(!strcmp(str,"cl") || !strcmp(str,"CL") || !strcmp(str,"Cl") || !strcmp(str,"cL"))  
			{printf(helpcl);printf("\n\n\tEX:");printf(excl);}
        else if(!strcmp(str,"wc") || !strcmp(str,"WC") || !strcmp(str,"Wc") || !strcmp(str,"wC"))
			{printf(helpwc);printf("\n\n\tEX:");printf(exwc);}
        else if(!strcmp(str,"dt") || !strcmp(str,"DT") || !strcmp(str,"Dt") || !strcmp(str,"dT"))  
			{printf(helpdt);printf("\n\n\tEX:");printf(exdt);}
        else if(!strcmp(str,"df") || !strcmp(str,"DF") || !strcmp(str,"Df") || !strcmp(str,"dF"))  
			{printf(helpdf);printf("\n\n\tEX:");printf(exdf);}
        else if(!strcmp(str,"du") || !strcmp(str,"DU") || !strcmp(str,"Du") || !strcmp(str,"dU")) 
			{printf(helpdu);printf("\n\n\tEX:");printf(exdu);}
        else if(!strcmp(str,"pd") || !strcmp(str,"PD") || !strcmp(str,"Pd") || !strcmp(str,"pD"))  
			{printf(helppd);printf("\n\n\tEX:");printf(expd);}
        else if(!strcmp(str,"ft") || !strcmp(str,"FT") || !strcmp(str,"Ft") || !strcmp(str,"fT"))  
			{printf(helpft);printf("\n\n\tEX:");printf(exft);}
        else if(!strcmp(str,"h") || !strcmp(str,"H"))    
			{printf(helph); printf("\n\n\tEX:");printf(exh); }
        else if(!strcmp(str,"s") || !strcmp(str,"S"))
			{printf(helps); printf("\n\n\tEX:");printf(exs); }
        else if(!strcmp(str,"at") || !strcmp(str,"AT") || !strcmp(str,"At") || !strcmp(str,"aT"))  
			{printf(helpat);printf("\n\n\tEX:");printf(exat);}
        else if(!strcmp(str,"qt") || !strcmp(str,"QT") || !strcmp(str,"Qt") || !strcmp(str,"qT"))
			{printf(helpqt);printf("\n\n\tEX:");printf(exqt);}
	printf("\n");
	return;
	}

void finalize()
        {
	FILE *itfp;
	int i;
        itfp=fopen("disk.ibfs","r+w");
        temp=head;
	fprintf(itfp,"%d ",inodenumber);
	fprintf(itfp,"%d ",recno);
        while(temp!=NULL)
                {
                fprintf(itfp,"%d %c %s %s %d %d ",temp->depth,temp->type,temp->name,
                               temp->path,temp->size,temp->inode);
                temp=temp->next;
                }
	fseek(itfp,INODEBLOCK,SEEK_SET);
	temp1=head1;
	while(temp1!=NULL)
		{
		fprintf(itfp,"%d %c %d %s %s %d %d %d ",temp1->inumber,temp1->type,temp1->size,temp1->owner,
							     temp1->ctime,temp1->lcount,
                                                             temp1->b1,temp1->b2);
		temp1=temp1->next;
		}
        fseek(itfp,DATABLOCK,SEEK_SET);
        fprintf(itfp,"%d %d ",totavarec,reccleared);
	free(head);
	free(head1);
	initializetables();
        fclose(itfp);
	}

void formatdisk()
	{
        FILE *itfp;
	char garb[30];
	printf("\n\tFormat the Disk! Data will be Lost! Do U want to continue y(format)/n(quit) ?\t");
	scanf("%s",&garb);
	if(!strcmp(garb,"y"))
		{
	        itfp=fopen("disk.ibfs","w");
        	fprintf(itfp,"%d ",101);
	        fprintf(itfp,"%d ",1);
        	fprintf(itfp,"%d %c %s %s %d %d ",1,'d',"venki","IBFS/",0,101);
		rewind(itfp);
	        fseek(itfp,INODEBLOCK,SEEK_SET);
        	fprintf(itfp,"%d %c %d %s %s %d %d %d",101,'d',0,"NAIDU",gettime(),0,0,0);
		rewind(itfp);
		fclose(itfp);
		itfp=fopen("disk.ibfs","r+w");
	        fseek(itfp,DATABLOCK,SEEK_SET);
        	fprintf(itfp,"%d %d",TOTBLOCKS,0);
	        fclose(itfp);
		strcpy(shellprompt,"IBFS/");
		printf("\n\tDisk Formatted Successfully! Written with Default Directory venki\n");
		free(head);
                free(head1);
       	        initializetables();
		}
	else
		printf("\n\tDisk not Formatted! To format select y option\n");
	return;
	}

int main()
        {
        int l=0,i=0,set=0;
        char str[6],str1[20],str2[20];
        system(CLEAR);
        printf("\n.\n..\n...\n....\n.....\nHave a Lot of Fun!\nType \"h\" or \"H\" to know the "
                        "commands of this IBFS shell\n");
        printf("\n CURRENT DATE AND TIME:  %s\n",gettime());
        initializetables();
        while(1)
                {
                PREV:
                if(set==0)
                        {
                        printf("NAIDU:");
                        printf(shellprompt);
                        printf("#");
                        }
                set=0;
                fflush(stdin);
                for(i=0;;i++)
                        {
                        str[i]=getchar();
                        if(str[i]==' ' || str[i]=='\n')
                                break;
                        }str[i+1]='\0';
                if(str[0]=='\n')
                        goto PREV;
                else str[i]='\0';
                if( !strcmp(str,"md") || !strcmp(str,"MD") || !strcmp(str,"Md") || !strcmp(str,"mD") ||
                    !strcmp(str,"rd") || !strcmp(str,"RD") || !strcmp(str,"rD") || !strcmp(str,"Rd") ||
                    !strcmp(str,"cd") || !strcmp(str,"CD") || !strcmp(str,"Cd") || !strcmp(str,"cD") ||
                    !strcmp(str,"mf") || !strcmp(str,"MF") || !strcmp(str,"Mf") || !strcmp(str,"mF") ||
                    !strcmp(str,"rf") || !strcmp(str,"RF") || !strcmp(str,"Rf") || !strcmp(str,"rF") ||
                    !strcmp(str,"vf") || !strcmp(str,"VF") || !strcmp(str,"Vf") || !strcmp(str,"vF") ||
                    !strcmp(str,"wc") || !strcmp(str,"WC") || !strcmp(str,"Wc") || !strcmp(str,"wC") ||
                    !strcmp(str,"s" ) || !strcmp(str,"S"))
                        {
                        scanf("%s",str1);
                        set=1;
                        if(!strcmp(str1,"~"))
                                {
                                printf("\n%s Command Bypassed\n",str);
                                continue;
                                }
                        }
                else if(!strcmp(str,"cf") || !strcmp(str,"CF") || !strcmp(str,"Cf") || !strcmp(str,"cF") ||
                        !strcmp(str,"mv") || !strcmp(str,"MV") || !strcmp(str,"Mv") || !strcmp(str,"mV") )
                        {
                        scanf("%s",str1);
                        set=1;
                        if( (!strcmp(str1,"~")) )
                                {
                                printf("\n %s Command Bypassed\n",str);
                                continue;
                                }
                        else
                                scanf("%s",str2);
                        if( (!strcmp(str2,"~")) )
                                {
                                printf("\n %s Command Bypassed\n",str);
                                continue;
                                }
                        }
                else
                        {}
                if(!strcmp(str,"md") || !strcmp(str,"MD") || !strcmp(str,"Md") || !strcmp(str,"mD"))
				{
				if(strlen(str1)>FSIZE)
                                	printf("\n\tThe Size of File Name is allowable to be %d "
                                        	"and it it truncated to %d\n",FSIZE,FSIZE);
                                str1[FSIZE]='\0';
                                makedirectory(str1);
				}
                else if(!strcmp(str,"rd") || !strcmp(str,"RD") || !strcmp(str,"rD") || !strcmp(str,"Rd"))
                                removeentry(str1,'d');
                else if(!strcmp(str,"cd") || !strcmp(str,"CD") || !strcmp(str,"Cd") || !strcmp(str,"cD"))
                                changedirectory(str1);
                else if(!strcmp(str,"vd") || !strcmp(str,"VD") || !strcmp(str,"Vd") || !strcmp(str,"vD"))
                                viewdirectory();
                else if(!strcmp(str,"mf") || !strcmp(str,"MF") || !strcmp(str,"Mf") || !strcmp(str,"mF"))
				{
				if(strlen(str1)>FSIZE)
                                	printf("\n\tThe Size of File Name is allowable to be %d "
						"and it it truncated to %d\n",FSIZE,FSIZE);
                                str1[FSIZE]='\0';
                                makefile(str1);
				}
                else if(!strcmp(str,"rf") || !strcmp(str,"RF") || !strcmp(str,"Rf") || !strcmp(str,"rF"))
                                removeentry(str1,'f');
                else if(!strcmp(str,"vf") || !strcmp(str,"VF") || !strcmp(str,"Vf") || !strcmp(str,"vF"))
                                viewfile(str1);
                else if(!strcmp(str,"cf") || !strcmp(str,"CF") || !strcmp(str,"Cf") || !strcmp(str,"cF"))
                                copyfile(str1,str2);
                else if(!strcmp(str,"mv") || !strcmp(str,"MV") || !strcmp(str,"Mv") || !strcmp(str,"mV"))
                                movefile(str1,str2);
                else if(!strcmp(str,"rs") || !strcmp(str,"RS") || !strcmp(str,"Rs") || !strcmp(str,"rS"))
                                display(head);
                else if(!strcmp(str,"is") || !strcmp(str,"IS") || !strcmp(str,"Is") || !strcmp(str,"iS"))
                                displayi(head1);
                else if(!strcmp(str,"cl") || !strcmp(str,"CL") || !strcmp(str,"Cl") || !strcmp(str,"cL"))
                                {
                                system(CLEAR);
                                printf("\n CURRENT DATE AND TIME:  %s\n",gettime());
                                }
                else if(!strcmp(str,"wc") || !strcmp(str,"WC") || !strcmp(str,"Wc") || !strcmp(str,"wC"))
                                wordcount(str1);
                else if(!strcmp(str,"dt") || !strcmp(str,"DT") || !strcmp(str,"Dt") || !strcmp(str,"dT"))
                                system("date");
                else if(!strcmp(str,"df") || !strcmp(str,"DF") || !strcmp(str,"Df") || !strcmp(str,"dF"))
                                calculatediskfree();
                else if(!strcmp(str,"du") || !strcmp(str,"DU") || !strcmp(str,"Du") || !strcmp(str,"dU"))
                                calculatediskusage();
                else if(!strcmp(str,"pd") || !strcmp(str,"PD") || !strcmp(str,"Pd") || !strcmp(str,"pD"))
                                printf("\n\t%s is the Current Working Directory\n",shellprompt);
                else if(!strcmp(str,"ft") || !strcmp(str,"FT") || !strcmp(str,"Ft") || !strcmp(str,"fT"))
                                {
                                formatdisk();
                                set=1;
                                }
                else if(!strcmp(str,"h") || !strcmp(str,"H"))
                                {
                                printf(help1);
                        printf("\nDirectory Related Commands:");
                                printf(helpmd);printf(helprd);printf(helpcd);printf(helpvd);
                        printf("\nFile Related Commands:");
                                printf(helpmf);printf(helpvf);printf(helprf);printf(helpcf);printf(helpmv);printf(helpwc);
                        printf("\nDisk Related Commands:");
                                printf(helpdu);printf(helpdf);printf(helpft);
                        printf("\nFile System Structure Related Commands:");
                                printf(helprs);printf(helpis);
                        printf("\nMiscellanous Commands:");
                                printf(helpcl);printf(helpdt);printf(helppd);
                        printf("\nCommands to Know Author and Quit:");
                                printf(helpat);printf(helpqt);
                        printf("\nCommands for Help:");
                                printf(helph);printf(helps);printf("\n******These Commands are Case Insensitive****\n");
                                }
                else if(!strcmp(str,"s") || !strcmp(str,"S"))
                                syntax(str1);
                else if(!strcmp(str,"at") || !strcmp(str,"AT") || !strcmp(str,"At") || !strcmp(str,"aT"))
                                printf(about);
                else if(!strcmp(str,"qt") || !strcmp(str,"QT") || !strcmp(str,"Qt") || !strcmp(str,"qT"))
                                        {
                                        printf("\nThank U\n");
                                        exit(0);
                                        }
                else
                        printf("\tIBFS SHELL: Command not found\n");
                goto PREV;
                }
        }
