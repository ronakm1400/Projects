
import os
import time
import hashlib
import smtplib	
import schedule
from sys import *
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email.message import EmailMessage
from email.mime.multipart import MIMEMultipart
from email.mime.application import MIMEApplication

#Function to get the check sum of a file
	
def HashFile(path , blocksize = 4096):
	
	afile = open(path,'rb')
	hasher = hashlib.md5()
	
	Buffer = afile.read(blocksize)
	while (len(Buffer)>0):
		hasher.update(Buffer)
		Buffer = afile.read(blocksize)
		
	afile.close()
	
	return hasher.hexdigest()

# Function to traverse the whole directory	
def DirTraversal(path):
	
	flag = os.path.isabs(path)
	if (flag == False):
		path = os.path.abspath(path)
		
	Exists = os.path.isdir(path)
	
	duplicatefiles = {}
	
	if Exists:	
		
		for dirName , subDir , fileList in os.walk(path):
			
			for filenm in fileList:
				path = os.path.join(dirName,filenm)
				file_hash = HashFile(path)
				
				if (file_hash in duplicatefiles):
					duplicatefiles[file_hash].append(path)
					
				else:
					duplicatefiles[file_hash] = [path]
		return duplicatefiles
		
	else :
		print("Invalid Path")


#Function to delete the duplicate files.		
def DeleteFiles(delete_dict):
	
	listprocess = []
	
	separator = "-"*100
	filename = "DeletedFilesLog.txt"
	fd = open(filename , "w")
	fd.write(separator + "\n")
	fd.write("Delete Duplicate Files Logger by Ronak Mutha" +"\t" +time.ctime() +"\n")
	fd.write(separator + "\n")
	
	Results = list(filter(lambda X : len(X)>1 , delete_dict.values()))
	
	iCnt = 0
	
	if (len(Results)>0):
		
		fd.write("Files deleted are:")
		
		for result in Results:
			for subresult in result:
				iCnt = iCnt + 1
				if (iCnt>=2):
					os.remove(subresult)
					fd.write("\n%s"%subresult)
			iCnt = 0
			
	else:
		fd.write("No Duplicate Files")
		
	fd.close()							


#function to send mail at the given mail id.
def SendMail(path,mail_id):

	
	try:
		email_sender = 'ronakm1400@gmail.com'
		email_pwd = "Enter_Your_Password"
		email_receiver = mail_id
		
		subject = "Deleted Files Log"
		body = "Starting time of scanning :%s"%time.ctime()
		
		msg = MIMEMultipart()
		msg['From'] = email_sender
		msg['To'] = email_receiver
		msg['Subject'] = subject
		msg['Body'] = body
		
		Files = os.path.join("Assignment13")
		for File in Files:  # add files to the message
		   
		    filename = "DeletedFilesLog.txt"
		    file_path = os.path.join(path,File)
		    attachment = MIMEApplication(open(filename, "r").read(), _subtype="txt")
		    attachment.add_header('Content-Disposition', 'attachment' , filename = "DeletedFilesLog.txt")
		    msg.attach(attachment)

		mailServer = smtplib.SMTP("smtp.gmail.com:587")
		mailServer.ehlo()
		mailServer.starttls()
		mailServer.ehlo()
		mailServer.login(email_sender, email_pwd)
		mailServer.sendmail(email_sender, email_receiver , msg.as_string())
		mailServer.close()
		print("Successfully sent the mail")

	except Exception as error:
		print(str(error))


#Entry point function main.
def main():
	
	print("*** Automation Script by Ronak Mutha ***")

	if (len(argv) != 3):
		print("Invalid number of arguments")
		print("press --h or --H for help")
		print("press --u or --U for usage of script")
		exit()
		
	if (argv[1] == "--h" or argv[1] == "--H"):
		print("Help: this script is used to traverse the directory and delete the duplicate files in directory and send the log file to the attached mail")
		exit()
		
	if (argv[1] == "--u" or argv[1] == "--U"):
		print("Usage: python3 Name_Of_Application Dir_Name Mail_Id")
		exit()
		
	try :
		
		arr = {}
		StartTime = time.time()
		arr = DirTraversal(argv[1])
		DeleteFiles(arr)
		EndTime = time.time()
		SendMail(argv[1],argv[2])
		
	except ValueError as ve:
		print("Invalid Datatype of input:",ve)
		
	except Exception as E:	
		print("Invalid input:",E)

if __name__ == "__main__":
	main()
