#include <ESP8266WiFi.h>
#include <Servo.h>

#define PIN_SERVO 13

const char* ssid = "SUB";
const char* password = "123456789";

WiFiServer server(80);
Servo servo;

bool Login = false;

void setup() 
{
  //pinMode(PIN_LED, OUTPUT);
  //digitalWrite(PIN_LED, LOW);
  servo.attach(PIN_SERVO);
  servo.write(0);
  
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if(!client) return;
  
  Serial.println("새로운 클라이언트");
  client.setTimeout(5000);
  
  while(!client.available()) 
  {
    delay(1);
  }
  
  String request = client.readStringUntil('\r');

  if( request.substring(5,16) != "favicon.ico") //favicon.ico request 배제
  {
    Serial.println("request: ");
    Serial.println(request);
    client.flush();
   }

  if ( (request.indexOf("test") != -1)&& (request.indexOf("test123") != -1) )  
  {
    Login = true; 
  }
  else
  {
    if (Login == true)  
    { 
      if(request.indexOf("FeedingStart") != -1) 
      {
        servo.write(90);
        delay(250);
        servo.write(0);
      }
      else 
      {
        Serial.println("invalid request");
        servo.write(0);
      }
    
      if (request.indexOf("Logout") != -1)
      {
        Login = false;
      }
    }
  }

 
  
 // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  //배경 색 문자 색 사이즈 HTML CSS 설정  
  client.println("<style>");
  //Login 버튼 배경 CSS
  client.println("* {box-sizing: border-box;}");
  client.println("*:focus { outline: none;}");
  if( Login == false ) 
  {
    client.println("body {font-family: Arial;background-color: #3498DB;padding: 50px;}");
  }
  else  
  {
    client.println("body {font-family: Arial;background-color: white;padding: 50px;}"); 
  }
  client.println(".Login {margin: 20px auto;width: 300px;}");
  client.println(".Login-screen {background-color: #FFF;padding: 20px;border-radius: 5px}");
  client.println(".app-title {text-align: center;color: #777;} ");
  client.println(".Login-form {text-align: center;}");
  client.println(".control-group {margin-bottom: 10px;}");
  client.println("input {text-align: center;background-color: #ECF0F1;");
  client.println("border: 2px solid transparent;border-radius: 3px;");
  client.println("font-size: 16px;font-weight: 200;padding: 10px 0;");
  client.println("width: 250px;transition: border .5s;}");
  client.println("input:focus {border: 2px solid #3498DB;box-shadow: none;}");
  client.println(".btn {  border: 2px solid transparent;  background: #3498DB;");
  client.println("  color: #ffffff;  font-size: 16px;  line-height: 25px;  padding: 10px 0;");
  client.println("  text-decoration: none;  text-shadow: none;  border-radius: 3px;");
  client.println("  box-shadow: none;  transition: 0.25s;  display: block;  width: 250px;");
  client.println("  margin: 0 auto;} ");
  client.println(".btn:hover {  background-color: #2980B9;} ");
  client.println(".Login-link {font-size: 12px;color: #444;display: block;margin-top: 12px;}");

  //버튼 HTML CSS 설정
  client.println(".button {");
  client.println("background-color: green;");
  client.println("border: 3px solid red;border-radius: 10px;");
  client.println("color: white;");
  client.println("padding: 15px 32px;");
  client.println("text-align: center;");
  client.println("text-decoration: none;");
  client.println("display: inline-block;");
  client.println("font-size: 30px;");
  client.println("margin: 10px 15px;");
  client.println("cursor: pointer;");
  client.println("}");
  client.println("</style>");
    
  if( Login == false ) 
  {  
    client.println("<form method='GET'>");
    client.println("<div class='Login'><div class='Login-screen'><div class='app-title'>");
    client.println("<h1>RemoteFeeder Login</h1></div> <div class='Login-form'><div class='control-group'>");
    client.println("<input type='text' class='Login-field' value='' placeholder='username' name='ID'>");
    client.println("<label class='Login-field-icon fui-user' for='Login-name'></label>");
    client.println("</div> <div class='control-group'>");
    client.println("<input type='password' class='Login-field' value='' placeholder='password' name='PW'>");
    client.println("<label class='Login-field-icon fui-lock' for='Login-pass'></label></div> ");
    client.println("<input type='submit' value='Log in' class='btn btn-primary btn-large btn-block' >");
    client.println(" <br></div></div></div>");
    client.println("</form>");
  }
  client.println("</head>");
  client.println("<body>");
  if( Login == false ) 
  {
  }
  else  
  {
   client.println("<center>");
   client.println("<div class='app-title'><h1>RemoteFeeder Control</h1></div>");
   client.println("<br>");
   client.println("<a href=\"/FeedingStart\"\" class='button'> Start </button></a>");
   //client.println("<a href=\"/OFF\"\" class='button'> OFF </button></a>");
   //client.println("<a href=\"/BLINK\"\" class='button'>BLINK</button></a>");
   client.println("<a href=\"/Logout\"\" class='button'>Logout</button></a>");
   client.println("</center>");
   client.println("</body>");
   client.println("</html>");
  }

  //int nDegree = servo.read();
  //if (nDegree == 0)
  //{
  //  client.print("CLOSE");
 // }
  //else
  //{
  //  client.print("OPEN");
 // }


  Serial.println("클라이언트 연결 해제");
}
