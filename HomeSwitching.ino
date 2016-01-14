 #include <SPI.h>  
 #include <Ethernet.h> 
 #include <avr/pgmspace.h>

 byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
 IPAddress ip( 192, 168, 1, 75 );
 EthernetServer server(80);
 const int Q = 10; //max 10
 int outA[10] = {2,3,5,6,7,8,9,14,15,16}; // Select the pinout address
 boolean outS[10] = {0,0,0,0,0,0,0,0,0,0};
 int outT[10] = {0,0,0,0,0,0,0,0,0,0};
 boolean rdg = false;
 boolean rdp = false;
 int outp = 0;
 int k;
 int pin;
 unsigned long lastConnectionTime = 0;
 const unsigned long postingInterval = 60L * 1000L;
 
const unsigned char htm[] PROGMEM = {"<html><head> <title>Home Switching</title> \
<meta name=\"description\" content=\"Home Switching\"/>  \
<meta http-equiv=\"refresh\" content=\"600000\"; url=/\"\">  \
<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">  \
<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"default\">  \
<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\"> \
<style type=\"text/css\"> body {background: #555;}  \
h1 {color: #eee;font: 30px Arial, sans-serif; \
-webkit-font-smoothing: antialiased;text-shadow: 0px 1px black; \
text-align: center;margin-bottom: 50px;} \
.slideThree { width: 80px; \
height: 26px;background: #333; \
margin: 20px auto;position: relative; \
-webkit-border-radius: 50px;-moz-border-radius: 50px; \
-ms-border-radius: 50px;-o-border-radius: 50px; border-radius: 50px; \
-webkit-box-shadow: inset 0px 1px 1px rgba(0, 0, 0, 0.5), 0px 1px 0px rgba(255, 255, 255, 0.2); \
-moz-box-shadow: inset 0px 1px 1px rgba(0, 0, 0, 0.5), 0px 1px 0px rgba(255, 255, 255, 0.2); \
box-shadow: inset 0px 1px 1px rgba(0, 0, 0, 0.5), 0px 1px 0px rgba(255, 255, 255, 0.2);} \
.slideThree:after {content: 'OFF';color: rgb(255,0,0); \
position: absolute;right: 10px;z-index: 0;font: 12px/26px Arial, sans-serif; \
font-weight: bold;text-shadow: 1px 1px 0px rgba(255, 255, 255, 0.15);} \
.slideThree:before {content: 'ON';color: #00bf00; \
position: absolute; left: 10px; z-index: 0;font: 12px/26px Arial, sans-serif;font-weight: bold;} \
.slideThree label { display: block;width: 34px; \
height: 20px;cursor: pointer;position: absolute; \
top: 3px;left: 3px;z-index: 1;background: #fcfff4; \
background: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #fcfff4), color-stop(40%, #dfe5d7), color-stop(100%, #b3bead)); \
background: -webkit-linear-gradient(top, #fcfff4 0%, #dfe5d7 40%, #b3bead 100%); \
background: -moz-linear-gradient(top, #fcfff4 0%, #dfe5d7 40%, #b3bead 100%); \
background: -o-linear-gradient(top, #fcfff4 0%, #dfe5d7 40%, #b3bead 100%); \
background: linear-gradient(top, #fcfff4 0%, #dfe5d7 40%, #b3bead 100%); \
-webkit-border-radius: 50px;-moz-border-radius: 50px; \
-ms-border-radius: 50px;-o-border-radius: 50px; \
border-radius: 50px;-webkit-transition: all 0.4s ease; \
-moz-transition: all 0.4s ease;-o-transition: all 0.4s ease; transition: all 0.4s ease; \
-webkit-box-shadow: 0px 2px 5px 0px rgba(0, 0, 0, 0.3); \
-moz-box-shadow: 0px 2px 5px 0px rgba(0, 0, 0, 0.3); \
box-shadow: 0px 2px 5px 0px rgba(0, 0, 0, 0.3);} \
.slideThree input[type=checkbox] {visibility: hidden;} \
.slideThree input[type=checkbox]:checked + label {left: 43px;} \
label{text-align: center;} </style><script language=\"javascript\" type=\"text/javascript\"> \
function altCkb (pos, sts){ \
 if (document.getElementById(\"chk\"+pos).checked != sts) { \
   document.getElementById(\"chk\"+pos).checked = sts; \
 }; }; \ 
function processar(ident){ var mreq; document.body.style.cursor = 'wait'; \
if(window.XMLHttpRequest){ mreq = new XMLHttpRequest(); \ 
}else if(window.ActiveXObject){mreq = new ActiveXObject(\"Microsoft.XMLHTTP\"); \
}else{ alert(\"Seu navegador não tem suporte a AJAX.\"); } \ 
var x = \"L\"; \
if(document.getElementById(\"chk\"+ident).checked){ x = \"H\"; }; \ 
mreq.onreadystatechange = function() { \
if(mreq.readyState == 1){ document.body.style.cursor = 'default'; \
}else if(mreq.readyState == 4){ var req = mreq.responseText; \
 for (var r = 0; r < req.length; r++) { altCkb( r, req.charAt(r)); }; }; \
}; mreq.open(\"GET\",\"hassegawa.no-ip.org?\" + x + ident ,false); \
mreq.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded;charset=iso-8859-1\"); \
mreq.send(null); }</script></head><body> \
<form method=\"get\">  \
<div class=\"view\"><div class=\"header-wrapper\" id=\"DivPrincipal\"> <h1>Home Switching</h1></div>"} ;  

 void setup()  
 {  
   Ethernet.begin(mac, ip); 
   server.begin();
   
   Serial.begin(9600);
   
   for (int y = 0; y < Q; y++){
     pinMode(outA[y], OUTPUT);
   }  
   delay(2);   
   Serial.println(Ethernet.localIP());
 } 

void loop() { 
  
  if (millis() - lastConnectionTime > postingInterval) {
     for (int i=0; i<10; i++)
     {
        if (outT[i] > 1) { 
          outT[i] = (outT[i] - 1);
        } 
        if (outT[i] == 1) { 
          outT[i] = 0;
          digitalWrite(outA[i], LOW);
        } 
     }
     lastConnectionTime = millis();
  }
  
  EthernetClient cl = server.available();
  
  if (cl) {       
    boolean sH = false; //Imprimir cabeçalho
    boolean outp;
     
    while (cl.connected()) { 
      if (cl.available()) { 
               
        char c = cl.read(); 
        
        if(rdg && c == ' '){ rdg = false; rdp = false; }
        if(c == '?') { rdg = true; } 
        if(c == '&') { rdp = true; } 
        
        if(rdg && !rdp){  
          if(c == 'H') { outp = 1; }
          if(c == 'L') { outp = 0; }         
          if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'){ 
            sH = true;
            pin = atoi(&c); 
            outS[pin] = outp;
            digitalWrite(outA[pin], outp);
          }
        }  
        
        if((rdp) && (pin >= 0)){           
          if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'){ 
            int v = atoi(&c); 
            outT[pin] = v * 60;
          }
        }  
        
        if (c == '\n') { 
           if(!sH){
             pntHed(cl); 
             for (int b = 0; b < Q; b++){           
               pChk(cl, b, outS[b]);
            } 
            pntFooter(cl); 
         } else { 
            for (int j = 0; j < Q ; j++) { cl.print(outS[j]); };  
         } 
         delay(2);     
         cl.stop();  
         pin = -1;
         break;      
        } 
      }
    }       
  }  
} 

 void pntHed(EthernetClient cl){
   cl.println("HTTP/1.1 200 OK");  
   cl.println("Content-Type: text/html");  
   cl.println("Connection: close");  
   cl.println(); 
   char c;
   k = 0;
   do {
     c = (char)pgm_read_byte_near(htm + k++);
     cl.print(c);
   } while (c!='\0');
}

void pntFooter(EthernetClient cl)
{
  cl.print("</div></form></body><footer>Hassegawa");
  cl.print("</footer></html>");
}

// switch LED and send back HTML for LED checkbox
void pChk(EthernetClient cl, int x, boolean st)
{  
    cl.println("<div class=\"slideThree\">");
    cl.print("<input type=\"checkbox\" value=\"");
    cl.print(x);
    cl.print("\" id=\"chk");
    cl.print(x);
    cl.print("\" name=\"");
    cl.print("H");
    cl.print("\" onclick=\"processar(\'");
    cl.print(x);
    cl.print("\');\"");
    if (st) { cl.print("checked"); }; 
    cl.print("><label for=\"chk");
    cl.print(x);
    cl.print("\">");
    cl.print(x);
    cl.print("</label></div>");
}
