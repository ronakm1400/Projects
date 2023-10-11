#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

#define EXIT_SUCCESS 0
#define SUCCESS 1

#define FAILURE -1
#define FAILURE_2 -2
#define FAILURE_3 -3
#define FAILURE_4 -4

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048
#define BUFFERSIZE 1024

#define REGULAR_FILE 1
#define SPECIAL_FILE 2

#define START 0
#define CURRENT 1
#define END 2

typedef signed int SINT;

typedef struct superblock
{

	int TotalInodes;
	int FreeInode;
	
}SUPERBLOCK,*PSUPERBLOCK;

typedef struct inode
{

	char FileName[50];
	int InodeNumber;
	int FileSize;
	int ActualFileSize;
	int FileType;
	char *Buffer;
	int LinkCount;
	int ReferenceCount;
	int Permission;
	struct inode *nextPtr;
	
}INODE,*PINODE,**PPINODE;

typedef struct filetable
{

	int ReadOffSet;
	int WriteOffSet;
	int Count;
	int Mode;
	PINODE ptrinode;
	
}FILETABLE,*PFILETABLE;

typedef struct ufdt
{

	PFILETABLE ptrfiletable;

}UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKsObj;
PINODE Head = NULL;

void man(char cName[])
{
	
	if(cName == NULL)
	{
		return;
	}
	
	if (strcmp(cName,"create") == 0)
	{
		printf("\nDescription 	: Used to create new regular file\n");
		printf("Usage 		: create File_Name Permission\n");
	}
	else if(strcmp(cName,"read") == 0)
	{
		printf("\nDescription 	: Used to read data from regular file\n");
		printf("Usage 		: read File_Name No_Of_Bytes_To_Read\n");
	}
	else if(strcmp(cName,"write") == 0)
	{
		printf("\nDescription 	: Used to write data into regular file\n");
		printf("Usage 		: write File_Name\nAfter this enter data that you want to write\n");
	}
	else if(strcmp(cName,"ls") == 0)
	{
		printf("\nDescription 	: Used to list all information of file\n");
		printf("Usage 		: ls\n");
	}
	else if(strcmp(cName,"stat") == 0)
	{
		printf("\nDescription 	: Used to display information about file\n");
		printf("Usage 		: stat File_Name\n");
	}
	else if(strcmp(cName,"fstat") == 0)
	{
		printf("\nDescription 	: Used to display information of file\n");
		printf("Usage 		: fstat File_Descriptor\n");
	}
	else if(strcmp(cName,"truncate") == 0)
	{
		printf("\nDescription 	: Used to remove data from file\n");
		printf("Usage 		: truncate File_Name\n");
	}
	else if(strcmp(cName,"open") == 0)
	{
		printf("\nDescription 	: Used to open an existing file\n");
		printf("Usage 		: open File_Name MODE\n");
	}
	else if(strcmp(cName,"close") == 0)
	{
		printf("\nDescription 	: Used to close an opened file\n");
		printf("Usage 		: close File_Name\n");
	}
	else if(strcmp(cName,"closeall") == 0)
	{
		printf("\nDescription 	: Used to close all opened files\n");
		printf("Usage 		: closeall\n");
	}
	else if(strcmp(cName,"lseek") == 0)
	{
		printf("\nDescription 	: Used to change file offset\n");
		printf("Usage 		: lseek File_Name ChangeInOffSet StartPoint\n");
	}
	else if(strcmp(cName,"rm") == 0)
	{
		printf("\nDescription 	: Used to delete the file\n");
		printf("Usage 		: rm File_Name\n");
	}
	else
	{
		printf("\nError 		: No Manual Entry Available\n");
	}

}/* end of man function */

void DisplayHelp()
{
	
	printf("ls 		: To list out all files in directory file\n");
	printf("clear 		: To clear the console\n");
	printf("open 		: To open an existing file\n");
	printf("close 		: To close the opened file\n");
	printf("closeall 	: To closeall the opened files\n");
	printf("read 		: To read all the contents from the file\n");
	printf("write 		: To write contents into the file\n");
	printf("exit		: To terminate the file system\n");
	printf("stat		: To display information of file using file name\n");
	printf("fstat		: To display information of file using file descriptor\n");
	printf("truncate	: To remove all data from file\n");
	printf("rm		: To delete the file\n");

}/* end of DisplayHelp */

int GetFDFromName(char cName[])
{

	auto SINT i = 0;
	
	while(i < 50)
	{
		if( (UFDTArr[i].ptrfiletable) != NULL)
		{
			if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName),cName) == 0)
			{
				break;
			}
		}
		i++;
	}
	if(i == 50)
	{
		return FAILURE;
	}
	else
	{
		return i;
	}
	

}/* end of GetFDFromName */

PINODE Get_Inode(char cName[])
{
	
	auto PINODE iTemp = Head;
	auto SINT i = 0;
	
	if(cName == NULL)
	{
		return NULL;
	}	
	
	while(iTemp != NULL)
	{
		if(strcmp(cName,iTemp->FileName) == 0)
		{
			break;
		}
		iTemp = iTemp->nextPtr;
	}	
	return iTemp;
}

void CreateDILB()
{

	auto SINT i = 1;
	
	/* Creating new node */
	auto PINODE newNode = NULL;
	auto PINODE iTemp = NULL;
	
	while(i <= MAXINODE)
	{
		newNode = (PINODE)malloc(sizeof(INODE));
		
		newNode->LinkCount = 0;
		newNode->ReferenceCount = 0;
		newNode->FileType = 0;
		newNode->FileSize = 0;
		
		newNode->Buffer = NULL;
		newNode->nextPtr = NULL;
		
		newNode->InodeNumber = i;
		
		if(iTemp == NULL)
		{
			Head = newNode;
			iTemp = Head;
		}	
		else
		{
			iTemp->nextPtr = newNode;
			iTemp = iTemp->nextPtr;
		}
		i++;
		
	}/* end fo while */
	
	printf("DILB : Disk Inode List Block\n");
	printf("Created Successfully!\n");

}/* end of CreateDILB */

void InitializeSuperBlock()
{

	auto SINT i = 0;
	
	while(i < MAXINODE)
	{
		UFDTArr[i].ptrfiletable = NULL;
		i++;
	}
	
	SUPERBLOCKsObj.TotalInodes = MAXINODE;
	SUPERBLOCKsObj.FreeInode = MAXINODE;

}/* end of InitializeSuperBlock */

int CreateFile(char cName[],int permission)
{
	auto SINT i = 0;
	auto PINODE iTemp = Head;
	
	if((cName == NULL) || (permission == 0) || (permission > 3))
	{
		return FAILURE;
	}
	if( (SUPERBLOCKsObj.FreeInode) == 0)
	{
		return FAILURE_2;
	}
	
	(SUPERBLOCKsObj.FreeInode)--;
	
	if(Get_Inode(cName) != NULL)
	{
		return FAILURE_3;
	}
	
	while(iTemp != NULL)
	{
		if( (iTemp->FileType) == 0)
		{
			break;
		}
		iTemp = iTemp->nextPtr;
	}
	
	while(i < 50)
	{
		if(UFDTArr[i].ptrfiletable == NULL)
		{
			break;
		}
		
		i++;
	}
	
	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));	
	
	UFDTArr[i].ptrfiletable->Count = 1;
	UFDTArr[i].ptrfiletable->Mode = permission;
	UFDTArr[i].ptrfiletable->ReadOffSet = 0;
	UFDTArr[i].ptrfiletable->WriteOffSet = 0;
	
	UFDTArr[i].ptrfiletable->ptrinode = iTemp;
	
	strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName,cName);
	UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR_FILE;
	UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
	UFDTArr[i].ptrfiletable->ptrinode->ActualFileSize = 0;
	UFDTArr[i].ptrfiletable->ptrinode->Permission = permission;
	UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);
	
	return i;
		
}/* end of create file */

int rm_File(char cName[])
{
	int fd = 0;
	
	fd = GetFDFromName(cName);
	if(fd == FAILURE)
	{
		return FAILURE;
	}
	
	(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;
	
	if((UFDTArr[fd].ptrfiletable->ptrinode->LinkCount) == 0)
	{
		UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
		free(UFDTArr[fd].ptrfiletable);
	}	
	UFDTArr[fd].ptrfiletable = NULL;
	(SUPERBLOCKsObj.FreeInode)++;
	
}/* end of rm_File */

int ReadFile(int fd,char cArr[],int iSize)
{

	auto SINT read_size = 0;
	
	if((UFDTArr[fd].ptrfiletable) == NULL)
	{
		return FAILURE;
	}
	if( ((UFDTArr[fd].ptrfiletable->Mode) != READ) && ((UFDTArr[fd].ptrfiletable->Mode) != READ + WRITE) )
	{
		return FAILURE_2;
	}
	if( ((UFDTArr[fd].ptrfiletable->ptrinode->Permission) != READ) && ((UFDTArr[fd].ptrfiletable->ptrinode->Permission) != READ + WRITE) )
	{
		return FAILURE_3;
	}
	if( (UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR_FILE )
	{
		return FAILURE_4;
	}
	
	read_size = (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) - (UFDTArr[fd].ptrfiletable->ReadOffSet);
	
	if(read_size < iSize)
	{
		strncpy(cArr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->ReadOffSet),read_size);
		UFDTArr[fd].ptrfiletable->ReadOffSet = UFDTArr[fd].ptrfiletable->ReadOffSet + read_size;
	}
	else
	{
		strncpy(cArr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->ReadOffSet),iSize);
		UFDTArr[fd].ptrfiletable->ReadOffSet = UFDTArr[fd].ptrfiletable->ReadOffSet + iSize;
	}
	return iSize;
	
}/* end of ReadFile */

int WriteFile(int fd,char cArr[],int iSize)
{

	if( ((UFDTArr[fd].ptrfiletable->Mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->Mode) != READ + WRITE) )
	{
		return FAILURE;
	}
	if( ((UFDTArr[fd].ptrfiletable->ptrinode->Permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->Permission) != READ + WRITE) )
	{
		return FAILURE;
	}
	if((UFDTArr[fd].ptrfiletable->WriteOffSet) == MAXFILESIZE)
	{
		return FAILURE_2;
	}
	if( (UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR_FILE )
	{
		return FAILURE_3;
	}
	
	strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->WriteOffSet),cArr,iSize);
	
	(UFDTArr[fd].ptrfiletable->WriteOffSet) = (UFDTArr[fd].ptrfiletable->WriteOffSet) + iSize;
	
	(UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) = (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + iSize;
	
	return iSize;	

}/* end of WriteFile */

int OpenFile(char cName[],int Mode)
{
	auto SINT i = 0;
	PINODE iTemp = NULL;
	
	if( (cName == NULL) || (Mode < 0) )
	{
		return FAILURE;
	}
	
	iTemp = Get_Inode(cName);
	
	if(iTemp == NULL)
	{
	 	return FAILURE_2;
	}
	if( (iTemp->Permission) < Mode)
	{
		return FAILURE_3;
	}
	
	while(i < 50)
	{
		if( (UFDTArr[i].ptrfiletable) == NULL)
		{
			break;
		}
		i++;
	
	}
	
	UFDTArr[i].ptrfiletable	= (PFILETABLE)malloc(sizeof(FILETABLE));
	
	if( (UFDTArr[i].ptrfiletable) == NULL)
	{
		return FAILURE;
	}
	
	UFDTArr[i].ptrfiletable->Count = 1;
	UFDTArr[i].ptrfiletable->Mode = Mode;
	
	if(Mode == READ + WRITE)
	{
		UFDTArr[i].ptrfiletable->ReadOffSet = 0;
		UFDTArr[i].ptrfiletable->WriteOffSet = 0;
	}
	else if(Mode == READ)
	{
		UFDTArr[i].ptrfiletable->ReadOffSet = 0;
	}
	else if(Mode == WRITE)
	{
		UFDTArr[i].ptrfiletable->WriteOffSet = 0;
	}
	UFDTArr[i].ptrfiletable->ptrinode = iTemp;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;
	
	return i;

}/* end of OpenFile */

void CloseFileByFd(int fd)
{

	UFDTArr[fd].ptrfiletable->ReadOffSet = 0;
	UFDTArr[fd].ptrfiletable->WriteOffSet = 0;
	(UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;

}/* end of CloseFileByFd */

int CloseFileByName(char cName[])
{

	auto SINT i = 0;
	i = GetFDFromName(cName);
	
	if(i == FAILURE)
	{
		return FAILURE;
	}
	
	UFDTArr[i].ptrfiletable->ReadOffSet = 0;
	UFDTArr[i].ptrfiletable->WriteOffSet = 0;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
	
	return SUCCESS;

}/* end of CloseFileByName */

void CloseAllFile()
{

	auto SINT i = 0;
	
	while(i < 50)
	{
		if( (UFDTArr[i].ptrfiletable) != NULL)
		{	
			UFDTArr[i].ptrfiletable->ReadOffSet = 0;
			UFDTArr[i].ptrfiletable->WriteOffSet = 0;
			(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
			break;
		}
		i++;
	}


}/* end of CloseAllFile */

int LSeekFile(int fd,int iSize,int From)
{
	if( (fd < 0) || (From > 2) )
	{
		return FAILURE;
	}
	
	if((UFDTArr[fd].ptrfiletable) == NULL)
	{
		return FAILURE;
	}
	
	if( (UFDTArr[fd].ptrfiletable->Mode = READ) || (UFDTArr[fd].ptrfiletable->Mode = READ + WRITE) )
	{
		if(From == CURRENT)
		{
			if( ((UFDTArr[fd].ptrfiletable->ReadOffSet)+iSize) > (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) )
			{
				return FAILURE;
			}
			
			if( ((UFDTArr[fd].ptrfiletable->ReadOffSet)+iSize) < 0 )
			{
				return FAILURE;
			}
			
			(UFDTArr[fd].ptrfiletable->ReadOffSet) = (UFDTArr[fd].ptrfiletable->ReadOffSet) + iSize;
			
		}
		else if(From == START)
		{
			if(iSize > (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize))
			{
				return FAILURE;
			}
			
			if(iSize < 0)
			{
				return FAILURE;
			}
			
			(UFDTArr[fd].ptrfiletable->ReadOffSet) = iSize;	
		}
		else if(From == END)
		{
			if( ((UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + iSize) > MAXFILESIZE)
			{
				return FAILURE;
			}
			
			if( ((UFDTArr[fd].ptrfiletable->ReadOffSet) + iSize) < 0)
			{
				return FAILURE;
			}
			
			(UFDTArr[fd].ptrfiletable->ReadOffSet) = (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + iSize;
		
		}
		
	
	}/* end of 3rd if block */
	
	else if( (UFDTArr[fd].ptrfiletable->Mode) = WRITE)
	{
		if(From == CURRENT)
		{
			if( ((UFDTArr[fd].ptrfiletable->WriteOffSet)+iSize) > MAXFILESIZE )
			{
				return FAILURE;
			}
			
			if( ((UFDTArr[fd].ptrfiletable->WriteOffSet)+iSize) < 0 )
			{
				return FAILURE;
			}
			if( ((UFDTArr[fd].ptrfiletable->WriteOffSet)+iSize) > (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) )
			{
				(UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) = ((UFDTArr[fd].ptrfiletable->WriteOffSet)+iSize);		
			}
				
			(UFDTArr[fd].ptrfiletable->WriteOffSet) = (UFDTArr[fd].ptrfiletable->WriteOffSet) + iSize;
			
		}
		else if(From == START)
		{
			if(iSize > MAXFILESIZE)
			{
				return FAILURE;
			}
			if(iSize < 0)
			{
				return FAILURE;
			}
			if(iSize > (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize))
			{
				 (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) = iSize;
			}
			
			(UFDTArr[fd].ptrfiletable->WriteOffSet) = iSize;	
		}
		else if(From == END)
		{
			if( ((UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + iSize) > MAXFILESIZE)
			{
				return FAILURE;
			}
			
			if( ((UFDTArr[fd].ptrfiletable->WriteOffSet) + iSize) < 0)
			{
				return FAILURE;
			}
			
			(UFDTArr[fd].ptrfiletable->WriteOffSet) = (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + iSize;
		
		}
	
	
	}/* end of else-if */	
	
	
}/* end of LSeekFile */

void Is_File()
{

	auto SINT i = 0;
	
	auto PINODE iTemp = Head;
	
	if( (SUPERBLOCKsObj.FreeInode) == MAXINODE)
	{
		printf("Error : There are no files\n");
		return;
	}
	
	printf("\nFileName\tInode number\tFile Size\tLink Count\n");
	printf("-------------------------------------------------------------------------------------\n");
	
	while(iTemp != NULL)
	{
		if(iTemp->FileType != 0)
		{
			printf("%s\t\t%d\t\t%d\t\t%d\n",iTemp->FileName,iTemp->InodeNumber,iTemp->ActualFileSize,iTemp->LinkCount);
		}
		iTemp = iTemp->nextPtr;
	}
	printf("-------------------------------------------------------------------------------------\n");

}/* end of Is_File */

int fstat_file(int fd)
{

	auto PINODE iTemp = Head;
	auto SINT i = 0;
	
	if(fd < 0)
	{
		return FAILURE;
	}
	if(UFDTArr[fd].ptrfiletable == NULL)
	{
		return FAILURE_2;
	}
	
	iTemp = UFDTArr[fd].ptrfiletable->ptrinode;
	
	printf("\n----------------------Statistical information about file----------------------\n");
	printf("File Name		: %s\n",iTemp->FileName);
	printf("Inode Number		: %d\n",iTemp->InodeNumber);
	printf("File Size		: %d\n",iTemp->FileSize);
	printf("Actual file size	: %d\n",iTemp->ActualFileSize);
	printf("Link Count		: %d\n",iTemp->LinkCount);
	printf("Reference Count		: %d\n",iTemp->ReferenceCount);
	
	if(iTemp->Permission == 1)
	{
		printf("File Permissions : Read Only\n");
	}
	else if(iTemp->Permission == 2)
	{
		printf("File Permissions : Write Only\n");
	}
	else if(iTemp->Permission == 3)
	{
		printf("File Permissions : Read & Write\n");
	}
	printf("---------------------------------------------------------------------------------\n\n");

	return SUCCESS;
	
}/* end of fstat_file */

int stat_file(char cName[])
{

	auto PINODE iTemp = Head;
	auto SINT i = 0;
	
	if(cName == NULL)
	{
		return FAILURE;
	}
	
	while(iTemp != NULL)
	{
		if(strcmp(cName,iTemp->FileName) == 0)
		{
			break;
		}
		iTemp = iTemp->nextPtr;
	}
	
	if(iTemp == NULL)
	{
		return FAILURE_2;
	}
		
	printf("\n----------------------Statistical information about file----------------------\n");
	printf("File Name		: %s\n",iTemp->FileName);
	printf("Inode Number		: %d\n",iTemp->InodeNumber);
	printf("File Size		: %d\n",iTemp->FileSize);
	printf("Actual file size	: %d\n",iTemp->ActualFileSize);
	printf("Link Count		: %d\n",iTemp->LinkCount);
	printf("Reference Count		: %d\n",iTemp->ReferenceCount);
	
	if(iTemp->Permission == 1)
	{
		printf("File Permissions : Read Only\n");
	}
	else if(iTemp->Permission == 2)
	{
		printf("File Permissions : Write Only\n");
	}
	else if(iTemp->Permission == 3)
	{
		printf("File Permissions : Read & Write\n");
	}
	printf("---------------------------------------------------------------------------------\n\n");
	
	return SUCCESS;

}/* end of stat_file */

int truncate_file(char cName[])
{

	auto SINT fd = 0;
	
	fd = GetFDFromName(cName);
	
	if(fd == FAILURE)
	{
		return FAILURE;
	}
	
	memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer,0,BUFFERSIZE);
	UFDTArr[fd].ptrfiletable->ReadOffSet = 0;
	UFDTArr[fd].ptrfiletable->WriteOffSet = 0;
	UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize = 0;	

}/* end of truncate_file */

int main(int argc,char *argv[])
{
	printf("\n--------------------------------------------------------------------------------------------\n");
	printf("\t\tCustomized Virtual FileSystem\n");
	printf("--------------------------------------------------------------------------------------------\n");	
	auto char *cPtr = NULL;
	auto SINT iRet = 0,fd = 0,iCount = 0;
	auto char command[4][80],str[80],cArr[BUFFERSIZE];
	
	// Auxillary data initialization
	InitializeSuperBlock();
	CreateDILB();
	
	// Shell is infinite listening loop
	while(1)
	{
		fflush(stdin);	/* clear all the data if present in the keyboard buffer */
		strcpy(str,"");
		
		printf("\nMarvellous VFS : >");
		
		// terminal vr kiti line cha array lihu shakto tr 80 mhanun 2nd parameter
		fgets(str,80,stdin);	// scanf("%[^'\n']s",str);
		
		// sscanf : accept input from string
		iCount = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);
		
		// strtok
		if(iCount == 1)
		{
			if(strcmp(command[0],"ls") == 0)
			{
				Is_File();
			}
			else if(strcmp(command[0],"closeall") == 0)
			{
				CloseAllFile();
				printf("All files closed successfully\n");
				continue;
			}
			else if(strcmp(command[0],"clear") == 0)
			{
				system("clear");
				continue;
			}
			else if(strcmp(command[0],"help") == 0)
			{
				DisplayHelp();
				continue;
			}
			else if(strcmp(command[0],"exit") == 0)
			{
				printf("Terminating the virtual file system application\n");
				break;
			}
			else
			{
				printf("\nError : Command not found!\n");
				continue;
			}	
		
		}/* end of if */
		
		else if(iCount == 2)
		{
			if(strcmp(command[0],"stat") == 0)
			{
				iRet = stat_file(command[1]);
				if(iRet == FAILURE)
				{
					printf("Error : Incorrect parameters\n");
				}
				if(iRet == FAILURE_2)
				{
					printf("Error : There is no such file\n");
				}
				continue;
			}
			else if(strcmp(command[0],"fstat") == 0)
			{
				iRet = fstat_file(fd);
				if(iRet == FAILURE)
				{
					printf("Error : Incorrect parameters\n");
				}
				if(iRet == FAILURE_2)
				{
					printf("Error : There is no such file\n");
				}
				continue;
			}
			else if(strcmp(command[0],"close") == 0)
			{
				iRet = CloseFileByName(command[1]);
				if(iRet == FAILURE)
				{
					printf("Error : There is no such file\n");
				}
				continue;
			}	
			else if(strcmp(command[0],"rm") == 0)
			{
				iRet = rm_File(command[1]);
				if(iRet == FAILURE)
				{
					printf("Error : There is no such file\n");
				}
				continue;
			}
			else if(strcmp(command[0],"man") == 0)
			{
				man(command[1]);
			}	
			else if(strcmp(command[0],"write") == 0)
			{
				fd = GetFDFromName(command[1]);
				if(fd == FAILURE)
				{
					printf("Error : Incorrect Parameter\n");
					continue;
				}
				printf("Enter the data : \n");
				scanf("%[^'\n']s",cArr);
				
				iRet = strlen(cArr);
				if(iRet == SUCCESS)
				{
					printf("Error : Incorrect Parameter\n");	
					continue;
				}
				
				iRet = WriteFile(fd,cArr,iRet);
				
				if(iRet == FAILURE)
				{
					printf("Error : Permission Denied\n");
				}
				if(iRet == FAILURE_2)
				{
					printf("Error : There is no sufficient memory to write\n");
				}
				if(iRet == FAILURE_3)
				{
					printf("Error : It is not a regular file\n");
				}
				
			}
			else if(strcmp(command[0],"truncate") == 0)
			{
				iRet = truncate_file(command[1]);
				if(iRet == FAILURE)
				{
					printf("Error : Incorrect Parameter\n");
				}
				else
				{
					printf("Error : Command not found\n");
					continue;	
				}
				
			}
				
		}/* end of 2 else-if block */
		else if(iCount == 3)
		{
		
			if(strcmp(command[0],"create") == 0)
			{
				iRet = CreateFile(command[1],atoi(command[2]));
				if(iRet >= 0)
				{
					printf("File is successfully created with file descriptor : %d\n",iRet);
				}
				if(iRet == FAILURE)
				{
					printf("Error : Incorrect Parameters\n");
				}
				if(iRet == FAILURE_2)
				{
					printf("Error : There is no inodes\n");
				}
				if(iRet == FAILURE_3)
				{
					printf("Error : File already exists\n");
				}
				if(iRet == FAILURE_4)
				{
					printf("Error : Memory allocation failure\n");
				}
				continue;
					
			}
			else if(strcmp(command[0],"open") == 0)
			{
				iRet = OpenFile(command[1],atoi(command[2]));
				if(iRet >= SUCCESS)
				{
					printf("File is successfully opened with file descriptor : %d\n",iRet);
				}
				if(iRet == FAILURE)
				{
					printf("Error : Incorrect Parameters\n");
				}
				if(iRet == FAILURE_2)
				{
					printf("Error : File not present\n");
				}
				if(iRet == FAILURE_3)
				{
					printf("Error : Permissions Denied\n");
					continue;
				}

			}
			else if(strcmp(command[0],"read") == 0)
			{
				fd = GetFDFromName(command[1]);
				if(fd == FAILURE)
				{
					printf("Error : Incorrect Parameters\n");
					continue;
				}
				
				cPtr = (char *)malloc(sizeof(atoi(command[2]))+1);
				
				if(cPtr == NULL)
				{
					printf("Error : Memory allocation failure\n");
					continue;
				}
				
				iRet = ReadFile(fd,cPtr,atoi(command[2]));
				
				if(iRet == FAILURE)
				{
					printf("Error : File does not exists\n");
				}
				if(iRet == FAILURE_2)
				{
					printf("Error : Permission Denied\n");
				}
				if(iRet == FAILURE_3)
				{
					printf("Error : Reached at end of file (EOF)\n");
				}
				if(iRet == FAILURE_4)
				{
					printf("Error : It is not a regular file\n");
				}
				if(iRet == SUCCESS)
				{
					printf("File is empty!\n");
				}
				if(iRet > 0)
				{
					write(2,cPtr,iRet);
				}
				continue;
			}
			else
			{
				printf("\nError : Command not found!\n");
				continue;
			}
			
		}/* end of 3 else-if block */	
		
		else if(iCount == 4)
		{
			if(strcmp(command[0],"lseek") == 0)
			{
				fd = GetFDFromName(command[1]);
				if(fd == FAILURE)
				{
					printf("Error : Incorrect Parameter\n");
					continue;
				}
				iRet = LSeekFile(fd,atoi(command[2]),atoi(command[3]));
				
				if(iRet == -1)
				{
					printf("Error : Unable to perform lseek\n");
				}	
			}
			else
			{
				printf("\nError : Command not found\n");
				continue;	
			}
				
		}/* end of  4 else-if block*/
		else
		{
			printf("\nError : Command not found\n");
			continue;	
		}
		
		
	}/* end of while(1) */

	exit(EXIT_SUCCESS);

}/* end of main */
