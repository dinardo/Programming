function CallServer()
{
    this.myObject;
    this.server_response;

    this.createXMLHTTPRequest = createXMLHTTPRequest;
    this.sendDataToServer     = sendDataToServer;
    this.displayAnswer        = displayAnswer;
    this.launch               = launch;
}

function createXMLHTTPRequest()
{
    this.myObject = null;

    if (window.XMLHttpRequest)
    {
	this.myObject = new XMLHttpRequest();
    }
    else
    {
	alert("Your browser doesn't support XMLHttprequest");
    }
}

function launch()
{
    var ss = document.getElementById("inputData").value;
    this.sendDataToServer("search="+ss);
    this.displayAnswer();
}

function sendDataToServer(dataToSend)
{
    var myObject = this.myObject;

    // On Linux
    // myObject.open("POST","http://192.168.168.11/cgi-bin/serverSide/scripts/job.pl",false);
    // On Mac OS
    myObject.open("POST","http://localhost/cgi-bin/job.pl",false);
    myObject.setRequestHeader("Content-type","application/x-www-form-urlencoded");
    myObject.send(dataToSend);

    if(myObject.readyState == 4)
    {
	this.server_response = myObject.responseText;
    }
    else
    {
	alert("No response from the server!");
    }
}

function displayAnswer()
{
    var t = this.server_response;
    document.getElementById("outputText").innerHTML = t;
}

var callServer = new CallServer();
callServer.createXMLHTTPRequest();
