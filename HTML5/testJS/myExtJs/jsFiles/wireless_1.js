var nodeName   = "http://localhost/";
var scriptArea = "cgi-bin/";
var scriptName = "wireless_1.py";
var theURL     = nodeName + scriptArea + scriptName;

Ext.onReady
(
    function()
    {
	Ext.create(
            'Ext.Button', 
	    {
  		text    : 'Get data',
  		renderTo: Ext.getBody(),
  		handler : function() 
	      	{

		    /* === Added new feature === */
		    Ext.Ajax.request
		    (
			{
			    url    : theURL,
			    success: function(response)
			    {
			   	var text = response.responseText;
                                var out  = document.getElementById("output");
				out.innerHTML = text;
			    }
			}
		    );
		    /* === Added new feature === */

  	      	}
  	    }
	);
    }
);
