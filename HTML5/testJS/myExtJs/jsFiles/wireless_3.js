Ext.ns('WL');
WL.nodeName   = "http://localhost/";
WL.scriptArea = "cgi-bin/";
WL.scriptName = "wireless_3.pl";
WL.theURL     = WL.nodeName + WL.scriptArea + WL.scriptName;

Ext.require
(
    [
	'Ext.data.*',
	'Ext.grid.*'
    ]
);

Ext.onReady
(
    function()
    {
	Ext.define
	(
	    'ourDataModel',
	    {
		extend  : 'Ext.data.Model',
		fields  :
		[
		    {name: 'TimeStamp', type: 'float' },
		    {name: 'x' 	 , type: 'float' },
		    {name: 'y' 	 , type: 'float' },
		    {name: 'z' 	 , type: 'float' },
		    {name: 'code'     , type: 'string'}
		]
	    }
	);

	var store = Ext.create
  	(
	    'Ext.data.Store', 
	    {
  	        model   : 'ourDataModel',
  	        autoLoad: false,
  	        proxy   :
		{
  	            type  : 'ajax',
  	            url   : WL.theURL,
  	            reader:
		    {
  	            	type  : 'xml',
  	            	record: 'dataBlock'
  	            }
  	        },
		listeners:
		{
	            load: function()
		    {
			loadMask.hide();
		    }
	        }
  	    }
	);
	
	var cols = [
            {
		text	 : "Time Stamp<br>(relative)",
		dataIndex: "TimeStamp",
		sortable : true,
		align	 : 'right'
	    },
  	    {text    : "x"   , flex: 1, dataIndex: 'x'   , align: 'right'},
  	    {text    : "y"   , flex: 1, dataIndex: 'y'   , align: 'right'},
  	    {text    : "z"   , flex: 1, dataIndex: 'z'   , align: 'right'},
  	    {
		text    : "Code", flex: 1, dataIndex: 'code', align: 'right',
		sortable: true,
	    }
        ];
	
	var grid = Ext.create
  	(
	    'Ext.grid.Panel',
	    {
  		store    : store,
  		columns	 : cols
  	    }
	);
	
	var getData = Ext.create
        (
            'Ext.Button', 
	    {
  	    	text   : 'Get data',
		margin : '3 3 3 3',
		handler: function() {store.load()}
		/*
		handler: function()
		{
		    loadMask.show(),
		    setTimeout(function(){store.load()},1000);
		}
		*/
  	    }
	);
	
	var cw = Ext.create
        (
	    'Ext.window.Window',
	    {
		title   : 'Controls',
		height  : 65,
		width   : 128,
		layout  : 'anchor',
		closable: false,
		items   : [
	            getData,
		    {
			xtype  : 'button',
			text   : 'Clear',
			margin : '3 3 3 3',
			handler: function()
			{
			    grid.getStore().removeAll();
			    Ext.Msg.alert(
				'Status', 
				'All data have been cleared.'
			    ).setPosition(200,200,true) ;
			}
		    }
		]
	    }
	).show().setPosition(10,80,true) ;

	var rw = Ext.create
        (
	    'Ext.window.Window',
	    {
		title   	: 'Results',
		height  	: 390,
		width   	: 450,
		layout  	: 'anchor',
		closable	: false,
		collapsible     : true,
		autoScroll      : true,
		items           : [grid]
	    }
	).show().setPosition(150,80,true);

	var loadMask = new Ext.LoadMask
        (
	    {
 		msg   	   : 'Please wait, loading data from server...',
 		target	   : grid,
		alwaysOnTop: true
 	    }
	);
    }
);
