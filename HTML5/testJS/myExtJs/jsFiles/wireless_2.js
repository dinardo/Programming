Ext.ns('WL');
WL.nodeName   = "http://localhost/";
WL.scriptArea = "cgi-bin/";
WL.scriptName = "wireless_3.py";
WL.theURL     = WL.nodeName + WL.scriptArea + WL.scriptName;

Ext.onReady
(
    function()
    {

	/* === Added new feature === */
	var out = document.getElementById("output");

	var succFunc = function(response)
  	{
 	    out.innerHTML = response.responseText;
  	}

	var failFunc = function(response)
  	{
 	    out.innerHTML = "Some error occurred";
  	}
  	
	var ajaxFunc = function()
  	{
  	    return Ext.Ajax.request
  	    (
  		{
  		    url    : WL.theURL,
  		    success: succFunc,
  		    failure: failFunc	
  		}
  	    );
  	}
	/* === Added new feature === */

	Ext.create(
            'Ext.Button',
            {
		text    : 'Get data',
		renderTo: Ext.getBody(),
		handler : ajaxFunc
            }
        );
    }
);
