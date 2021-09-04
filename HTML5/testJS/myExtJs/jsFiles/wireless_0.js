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
  	      	    alert('No server implemented yet!');
  	      	}
  	    }
	); 
    }
);








