######################################################
# Need to set "Allow less secure apps: ON" on google #
######################################################


from smtplib import SMTP, SMTP_SSL
from email.mime.text import MIMEText


serverAddress    = 'smtp.gmail.com'
senderAddress    = 'mauro.dinardo@gmail.com'
recipientAddress = 'mauro.dinardo@cern.ch'
myID             = senderAddress
myPW             = 'gmail299792'


msgMime         = MIMEText('Ciao, come va?')
msgMime['From'] = senderAddress
msgMime['To']   = recipientAddress


###############
server = SMTP(serverAddress)
server.connect(serverAddress, 587)
server.starttls()
server.login(myID, myPW)
###############
#server = SMTP('localhost')
###############

server.sendmail(senderAddress, recipientAddress, msgMime.as_string())
server.quit()


print('Email sent!')
