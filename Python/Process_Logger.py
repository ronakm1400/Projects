
import psutil
import time
from sys import *
import os
import smtplib
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.message import EmailMessage
from email.mime.application import MIMEApplication
	
def CheckProcess(logdir):
	
	processlist = []
		
	separator = "-"*100
	path = os.path.join(logdir , "ProcessLogger.txt%s"%time.ctime())
	fd = open(path , "w")
	fd.write(separator +"\n")
	fd.write("Process Logger by Ronak Mutha :" "\t" +time.ctime() + "\n")
	fd.write(separator +"\n")
	
	for proc in psutil.process_iter():
		
		try :
			pinfo = proc.as_dict(attrs = ['pid' , 'username' , 'name'])
			vms = proc.memory_info().vms / 1024 * 1024
			pinfo['vms'] = vms
			processlist.append(pinfo)
			
		except (psutil.NoSuchProcess , psutil.AccessDenied , psutil.ZombieProcess):
			pass
						
	for elem in processlist:
		fd.write("%s\n"%elem)	



def SendMail(path,mail_id):

	
	try:
		email_sender = 'ronakm1400@gmail.com'
		email_pwd = 'Enter_Your_Password_Here'
		email_receiver = mail_id
		
		subject = "Process LogFile"
		
		body = "body of mail:"	
			
		msg = MIMEMultipart()
		msg['From'] = email_sender
		msg['To'] = email_receiver
		msg['Subject'] = subject
		msg['Body'] = body
	
		
		Files = os.path.join(path)
		for File in Files:  # add files to the message
		  
		    File = "ProcessLogger.txt%s"%time.ctime() 
		    file_path = os.path.join(path,File)
		    attachment = MIMEApplication(open(file_path, "r").read(), _subtype="txt")
		    attachment.add_header('Content-Disposition', 'attachment' , filename = File)
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


		
def main():

	print("*** Process automation by Ronak Mutha ***")

	if (len(argv) != 3):
		print("Invalid number of arguments")
		print("press --h or --H for help 	")
		print("press --u or --U for usage")
		exit()
		
	if (argv[1] == "--h" or argv[1] == "--H"):
		print("Help : this script is used to display the running process")
		exit()
	
	if (argv[1] == "--u" or argv[1] == "--U"):
		print("Usage : python3 Name_Of_Application DirName Mail_Id")
		exit()

	try:	
		
		CheckProcess(argv[1])
		SendMail(argv[1],argv[2])
		
	except ValueError as E:	
		print("Invalid datatype of input:",E)
		
	except Exception as E:
		print("Invalid input:",E)

if __name__ == "__main__":
	main()
