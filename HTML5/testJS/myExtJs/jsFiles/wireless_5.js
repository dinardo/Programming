/*======================================
 Author: D. Menasce
 
 First step towards the final goal: 
 an interface to the smart phone
======================================*/

Ext.ns('WL') ; // WS stands for WireLess...

WL.nodeName    = "http://mobile.mib.infn.it/"                     ;
WL.scriptArea  = "cgi-bin/dottorato/esercizi/wireless/perlFiles/" ;
WL.scriptName  = "wireless_4.pl"                                  ;
WL.theURL      = WL.nodeName + WL.scriptArea + WL.scriptName      ;
WL.thisProcess = parseInt(Math.random()*10000)  		  ;
WL.statusIcon  = 'icon-stopped' 	        		  ;
WL.statusText  = 'Start'        	        		  ;

//===============================================		
Ext.require
(
 [
  'Ext.data.*'	     ,
  'Ext.grid.*'	     ,
  'Ext.window.Window'
//  'Ext.widget.chart' ,     
//  'Ext.chart.*'      
 ]
);

//===============================================		
Ext.onReady
(
 function()
 {
  var chartWin     ;
  var contDataColl ;
  var theChart     ;
  var s            ;
  
  var ajaxR = function(theAction)
              {
	       Ext.Ajax.request
	       (
	        {
	         url	: WL.theURL  + 
		          '?action=' +
	        	  theAction  +
			  '&procID=' +
			  WL.thisProcess,
	         success: function(response)
	        	  {
	        	   var rText = response.responseText ;
			   if     ( rText.match(/STATUS: running/))
			   {
			    WL.statusIcon = 'icon-started'       ;
			    serverB.setIconCls(WL.statusIcon)    ;
			    serverB.setText   ('Stop'       )    ;
			    getDataB.setDisabled(false)          ;
			   }
			   else if( rText.match(/STATUS: stopped/))
			   {
			    WL.statusIcon = 'icon-stopped'    	 ;
			    serverB.setIconCls(WL.statusIcon) 	 ;
			    serverB.setText   ('Start'      )    ;
			    getDataB.setDisabled(true)           ;
			   }
	        	  },
	         failure: function(response)
		          {
	        	   var rText = response.responseText ;
                           Ext.Msg.alert('FATAL', 
			                 'The server is not responding');	        	   
	                  }
		}
	       );
	      }	      
  
  var serverB  = Ext.create
                 (
                  'Ext.Button', 
	          {
  	           text   : WL.statusText,
	           margin : '4 4 4 4'	 ,
	           iconCls: WL.statusIcon,
                   height : 30           ,
                   width  : 70           ,
	           handler: function()
	        	    {
	        	     ajaxR(this.text) ;
	        	    } 
  	          }
	         );
		 
  var graphB   = Ext.create
                 (
                  'Ext.Button', 
	          {
  	           text   : 'Timeline',
	           margin : '4 4 4 4'	 ,
	           iconCls: 'icon-chart',
	           handler: function()
	        	    {
	        	     chartWin.show() ;
			     chartWin.setPosition(10,160,true) ;
			     rw.setHeight(75) ;
	        	    } 
  	          }
	         );
		 
  var getDataB = Ext.create
                 (
                  'Ext.Button', 
	          {
  	           text    : 'Get Data',
	           margin  : '4 4 4 4' ,
		   disabled: true      ,
		   anchor  : '75%'     ,
		   border  : 1         ,
	           handler : function()
	         	     {
			      loadMask.show(),
			      setTimeout( 
			                 function()
			                 {
					  store.load() ;
					 }, 
					 1000
					) ;
	         	     } 
  	          }
	         );
		 
  Ext.define
  (
   'ourDataModel',
   {
    extend  : 'Ext.data.Model',
    fields  : [
               {name: 'serial'   , type: 'int'   },
               {name: 'timeStamp', type: 'float' },
	       {name: 'x' 	 , type: 'float' },
	       {name: 'y' 	 , type: 'float' },
	       {name: 'z' 	 , type: 'float' }
	      ]
   }
  );

  var store = Ext.create
  	      (
	       'Ext.data.Store', 
	       {
  	        model   : 'ourDataModel',
  	        autoLoad: false,
  	        proxy   : {
  	            	   type  : 'ajax',
  	            	   url   : WL.nodeName + WL.scriptArea + 'getData_4.pl',
  	            	   reader: {
  	            	   	    type  : 'xml',
  	            	   	    record: 'dataBlock'
  	            	   	   }
  	                  },
	       listeners: {
	                   load: function()
			   	 {
				  loadMask.hide() ;
				  if( store.getCount() == 0 )
				  {
                           	   Ext.Msg.alert(
				                 'WARNING', 
			   	   		 'No records received ' +
						 'from server'
						 ).setPosition(250,200,true);          
				  }
			   	 }
	                  }
  	       }
	      );
	      
  var cols = [
              {
	       text	: "Time Stamp<br>(relative)",
	       dataIndex: "timeStamp"   	    ,
	       sortable : true        		    ,
	       align	: 'right'     
	      },
  	      {text: "x", flex: 1, dataIndex: 'x', align: 'right'},
  	      {text: "y", flex: 1, dataIndex: 'y', align: 'right'},
  	      {text: "z", flex: 1, dataIndex: 'z', align: 'right'}
             ];
	     
  var grid = Ext.create
  	     (
	      'Ext.grid.Panel', 
	      {
  	       store     : store,
  	       columns	 : cols
  	      }
	     );

  var cw = Ext.create
           (
	    'Ext.window.Window',
	    {
	     title   : 'Server Controls',
	     height  : 74	 	,
	     width   : 120	 	,
	     layout  : 'vbox'  	,
	     closable: false     	,
	     items   : [
	                serverB
		       ]
	    }
	   ).show().setPosition(10,80,true) ;

  var rw = Ext.create
           (
	    'Ext.window.Window',
	    {
	     title   	: 'Results',
	     height  	: 390	   ,
	     width   	: 450	   ,
	     layout  	: 'anchor' ,
	     closable	: false    ,
	     collapsible: true     ,
	     autoScroll : true     ,
	     items      : grid     ,
	     tbar       : [
	                   getDataB,
			   graphB  ,
			   {
  	           	    text    : 'Reset',
	           	    margin  : '4 4 4 4' ,
		   	    disabled: false	,
		            border  : 1 	,
		   	    anchor  : '75%'	,
	           	    handler : function()
	         	  	      {
			  	       grid.getStore().removeAll();
	         	  	      } 
			   }
			  ]
	    }
	   ).show().setPosition(140,80,true) ;

  series = [
  	    {
  	     type	 : 'line',
  	     highlight   : {
  		  	    size  : 7,
  		  	    radius: 7
  		  	   },
  	     axis	 : 'left',
  	     xField	 : 'timeStamp',
  	     yField	 : 'x',
             tips	 : {
             		    trackMouse: true,
             		    width     : 150 ,
             		    height    : 45  ,
             		    renderer  : function(storeItem, item) 
	        			{
					 s = 'Time: ' + storeItem.get('timeStamp')            ;
             				 this.setTitle(s)                                     ;
					 s = 'x: '    + storeItem.get('x'        ).toFixed(3) ;
             				 this.update  (s);
             				}
	        	   },
  	     markerConfig: {
  		  	    type	  : 'cross',
  		  	    size	  : 4,
  		  	    radius	  : 4,
  		  	    'stroke-width': 0
  		  	   }
  	    },
  	    {
  	     type	 : 'line',
  	     highlight   : {
  		  	    size  : 7,
  		  	    radius: 7
  		  	   },
  	     axis	 : 'left',
  	     smooth	 : true,
  	     xField	 : 'timeStamp',
  	     yField	 : 'y',
             tips	 : {
             		    trackMouse: true,
             		    width     : 150 ,
             		    height    : 45  ,
             		    renderer  : function(storeItem, item) 
	        			{
					 s = 'Time: ' + storeItem.get('timeStamp')            ;
             				 this.setTitle(s)                                     ;
					 s = 'y: '    + storeItem.get('y'        ).toFixed(3) ;
             				 this.update  (s);
             				}
	        	   },
  	     markerConfig: {
  		  	    type	  : 'circle',
  		  	    size	  : 4,
  		  	    radius	  : 4,
  		  	    'stroke-width': 0
  		  	   }
  	    },
  	    {
  	     type	 : 'line',
  	     highlight   : {
  		  	    size  : 7,
  		  	    radius: 7
  		  	   },
  	     axis	 : 'left',
  	     smooth	 : true,
  	     fill	 : true,
  	     xField	 : 'timeStamp',
  	     yField	 : 'z',
             tips	 : {
             		    trackMouse: true,
             		    width     : 150 ,
             		    height    : 45  ,
             		    renderer  : function(storeItem, item) 
	        			{
					 s = 'Time: ' + storeItem.get('timeStamp')            ;
             				 this.setTitle(s)                                     ;
					 s = 'z: '    + storeItem.get('z'        ).toFixed(3) ;
             				 this.update  (s);
             				}
	        	   },
  	     markerConfig: {
  		  	    type	  : 'circle',
  		  	    size	  : 4,
  		  	    radius	  : 4,
  		  	    'stroke-width': 0
  		  	   }
  	    }
           ] ;
           
  var chartTools = [
                    {
     	             type    : 'up',
     	             callback: function()
     	           	       {
     	           		chartWin.expand() ;
     	           	       }
     	            },
     	            {
     	             type    : 'down',
     	             callback: function()
     	           	       {
     	           		chartWin.collapse() ;
     	           	       }
     	            }
		   ];
		   
  var chartTBar = [
	           {						   	     
	            text   : 'Continuous plot', 		   	     
		    handler: function() 				     
		  	     {						     
		  	      contDataColl = setInterval		     
		  			     (
		  			      function()
		  			      { 			     
		  			       store.load() ;		     
		  			      }, 
		  			      1000
		  			     ) ;
		  	     }
		   },
		   {							     
		    text   : 'Stop data collection',			     
		    handler: function() 				     
		  	     {						     
		  	      clearInterval(contDataColl) ;		     
		  	     }
		   },
		   {							     
		    xtype	: 'button'  ,				     
		    text	: 'Snapshot',				     
		    disabled	: false     ,				     
		    handler	: function()				     
		  		  {					     
		  		   clearInterval(contDataColl) ;	     
		  		   chartWin.child('#chartCmp').save	     
                  		   (				   	     
		  		    {					     
		  		     type: 'image/png'			     
                  		    }				   	     
                  		   );				   	     
		  		  }					     
		   },							     
		  ] ;
		  
  var chartAxes  = [
     	            {							     
     	             title	 : 'x/y/z'	,
     	             type	 : 'Numeric'	,
     	             position	 : 'left'	,
     	             fields	 : ['x','y','z'],
     	             grid	 : {				  
     	           	  	    odd: {			  
     	           	  		  fill  	: '#dedede',
     	           	  		  stroke	: '#ddd'   ,
     	           	  		  'stroke-width': 0.5	    
     	           	  		 }			  
     	           	  	   }		,
     	             minimum	 : -2		,
     	             maximum	 :  15  			  
     	            },
     	            {
     	             title	 : 'Time Stamp' ,
     	             type	 : 'Numeric'	,
     	             position	 : 'bottom'	,
     	             fields	 : 'timeStamp'  ,
     	            }
     	           ] ;
		   				    
  var chartItems = [								  							    
     	            {								  	   
     	             xtype     : 'chart'	    ,	 				   
     	             style     : 'background:#edd'  ,	 				   
     	             itemId    : 'chartCmp'	    ,	 				   
     	             shadow    : true		    ,	 				   
     	             store     : store  	    ,	 				   
     	             animate   : true		    ,	 				   
     	             theme     : 'Category1'	    ,	 				   
		     background: {fill: '#ffffff'}  ,
     	             legend    : {position: 'right'},
     	             axes      : chartAxes          ,
     	             series    : series
     	            }								           
     	           ] ;								           
     	        	    	                         	          
  chartWin = Ext.create
             (
     	      'Ext.window.Window', 
     	      {
     	       title	  : 'Wireless test',
     	       width	  : 580            ,
     	       height	  : 400            ,
     	       minHeight  : 400            ,
     	       minWidth   : 550            ,
     	       maximizable: true           ,	   
     	       closable   : true           ,
     	       tools	  : chartTools	   ,
	       tbar       : chartTBar 	   ,
     	       items	  : chartItems     ,
     	       layout	  : {type: 'fit'}  ,
     	       listeners  : {close: function(){rw.setHeight(390) ;}}
     	      }
     	     );

  var checker = setInterval
              	(
	      	 function() 
	      	 {
		  ajaxR('Check') ; 
	      	 },
	      	 1000
	      	) ;

 var loadMask = new Ext.LoadMask
                    (
		     {
 		      msg   	 : 'Please wait, loading data from server...',
 		      target	 : grid,
		      alwaysOnTop: true
 		     }
		    );

  window.onblur   = function(e)
                    {
		     if( serverB.getText() == 'Stop' )
		     {
		      Ext.Msg.show
		      (
		       {
		        title  : 'The server is still activezzz!',
		        msg    : 'Would you like to stop it and leave the current page?',
		        buttons: Ext.Msg.YESNOCANCEL          ,	
		        icon   : Ext.Msg.QUESTION             ,	
		        fn     : function(btn) 
		        {
		         if (btn === 'yes') 
		         {
		          ajaxR('Stop') ;
		         }
		        }
		       }
		      );
		     }		   
		    }
 } 
);















