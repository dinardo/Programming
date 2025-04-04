Strumenti per il trattamento dati client-side al tempo di HTML5

JavaScript: tutte le strutture dati sono “array associativi”

JavaScript / HTML5, in generale i browser, adottano la tecnica del non-blocking IO (i.e. IO non sincrono, a differenza del blocking IO: “in computer science, asynchronous I/O, or non-blocking I/O is a form of input/output processing that permits other processing to continue before the transmission has finished”

On my MAC:
- sudo apachectl start
- sudo apachectl stop
- sudo apachectl restart

Framework to program in javaScript: Ext Js
Server: either Apache (i.e. httpd) or Node.Js (Node.Js permette di programmare qualunque cosa in javaScript)

Il “form” senza “action” va evitato ! Il “form” passa tutta l’informazione contenuta al suo interno al file definito proprio con “action”

PHP viene eseguito dal lato server, perché solo il server lo sa eseguire
I browser sanno interpretare: HTML5, CSS, javaScript

Metodi di query di HTTP5 http://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol
- GET: dati passati ne url?chiave1=valore1&chiave2=valore2
- POST: dati passati nello header
- PUT: ...

Al posto di usare shared memory (la cui memoria non viene disallocata se il processo che l’ha creata muore), e meglio usare mmap (file in memoria)

Fare presentazioni in html (S5): http://www.mi.infn.it/~prelz/html5/js/node/s5/primer.html

Mio client
```
<Directory "/home/*/public_html">
	AllowOverride FileInfo AuthConfig Limit Indexes
	Options MultiViews Indexes SymLinksIfOwnerMatch IncludesNoExec
	Require method GET POST OPTIONS
</Directory>
```

Mio cgi
```
<IfModule alias_module>
 	ScriptAlias /cgi-bin/ "/var/www/cgi-bin/"
</IfModule>

<Directory "/var/www/cgi-bin">
	AllowOverride None
	Options FollowSymLinks
	Require all granted
</Directory>
```

To learn:
1. Usare i “form”: slide #2 del corso
2. Realizzare scambio dati client-server asincrono senza l’uso di framework
3. Attraversabilita` directory “serverSide”
4. Implementare wifi_3
5. Implementare wifi_4/5 usando javaScript server-side
6. Proteggere i dati durante il loro trasferimento / accesso a pagine (HTTPS)
