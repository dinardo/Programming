var divMenu = document.getElementById("menu");
var start = 0;
var interval = setInterval(
    function()
    {
	var date = new Date();
	divMenu.innerHTML = "[" + start + "] " + date;
	start++;
	if (start > 15 )
	{
	    clearInterval(interval);
	    divMenu.innerHTML = "Bye bye !";
	}
    },
    1000
);
