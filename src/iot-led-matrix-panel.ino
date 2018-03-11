
/*
An Internet connected LED Matrix Panel using a Particle Photon.


Special thanks to Adafruit for the libraries and sample code.


Written by Cory Guynn
InternetOfLEGO.com
2017


*/



#include <SparkJson.h>
#include "Adafruit_mfGFX.h"   // Core graphics library
#include "RGBmatrixPanel.h" // Hardware-specific library
#include "math.h"

// NOTE: Version 4 of the RGBMatrix Shield only works with Photon and Electron (not Core)
#define RGBSHIELDVERSION		4

/** Define RGB matrix panel GPIO pins **/
#if (RGBSHIELDVERSION == 4)		// Newest shield with SD socket onboard
	#warning "new shield"
	#define CLK	D6
	#define OE	D7
	#define LAT	TX
	#define A  	A0
	#define B  	A1
	#define C  	A2
	#define D	RX
#else
	#warning "old shield"
	#define CLK	D6
	#define OE 	D7
	#define LAT	A4
	#define A  	A0
	#define B  	A1
	#define C  	A2
	#define D	A3
#endif
/****************************************/

// Setup Devices
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

// Global Variables
String textHeader = "";
int colorHeader[3];
String colorHeaderString = "";
String textBody = "";
int colorBody[3];
String colorBodyString = "";
String textScroll = "";
int colorScroll[3];
String colorScrollString = "";
String textFooter = "";
int colorFooter[3];
String colorFooterString = "";
int programSpeed = 75;
// Scroll State Variables
int scrollTextX   = matrix.width();
int scrollTextMin = 0;

void setDisplayDefaults(){
    // set default colors
    String header = String("{\"r\":23,\"g\":3,\"b\":2,\"a\":1}");
    String body = String("{\"r\":123,\"g\":123,\"b\":255,\"a\":1}");
    String scroll = String("{\"r\":64,\"g\":69,\"b\":82,\"a\":1}");
    String footer = String("{\"r\":38,\"g\":217,\"b\":59,\"a\":1}");
    setColorHeader(header);
    setColorBody(body);
    setColorScroll(scroll);
    setColorFooter(footer);
}



void displayHeader(String str){
	matrix.setTextColor(matrix.Color888(colorHeader[0],colorHeader[1],colorHeader[2]));
	matrix.setCursor(0, 0);
	matrix.setTextWrap(true); // will override other text on screen, be careful
	matrix.print(str);
}

void displayBody(String str){
	matrix.setTextColor(matrix.Color888(colorBody[0],colorBody[1],colorBody[2]));
	matrix.setCursor(0, 8);
	matrix.setTextWrap(false);
	matrix.print(str);
}




void displayScroll(String str){
    matrix.fillRect(0, 16, 64, 8, 0); // clear row by creating a black rectangle
    matrix.setTextWrap(false);
    matrix.setTextColor(matrix.Color888(colorScroll[0],colorScroll[1],colorScroll[2]));
    matrix.setCursor(scrollTextX, 16);
    matrix.print(textScroll);

    // Move text left (w/wrap), increase hue
    if((--scrollTextX) < scrollTextMin) scrollTextX = matrix.width();

    // Update display
    matrix.swapBuffers(false);

}

void displayFooter(String str){
	matrix.setTextColor(matrix.Color888(colorFooter[0],colorFooter[1],colorFooter[2]));
	matrix.setCursor(0, 24);
	matrix.setTextWrap(false);
	matrix.print(str);
}


// API Function Handlers
// These functions primarily set the state of a given variable. T
int setTextHeader(String str){
    // clear row by creating a black rectangle
    //matrix.fillRect(0, 0, 64, 8, matrix.Color333(0, 0, 0)); // 1 row
    matrix.fillRect(0, 0, 64, 16, matrix.Color333(0, 0, 0)); // 2 rows
	textHeader = str;
	return 1;
}
int setTextBody(String str){
    // clear row by creating a black rectangle
    matrix.fillRect(0, 8, 64, 8, matrix.Color333(0, 0, 0));
	textBody = str;
	return 1;
}
int setTextScroll(String str){
    // note, text is cleared in the displayScroll function since its part of the loop
    Serial.print("scrolling text set");
    Serial.print(str);
	textScroll = str;
    scrollTextMin = sizeof(str) * -12;
	return 1;
}
int setTextFooter(String str){
    matrix.fillRect(0, 24, 64, 8, matrix.Color333(0, 0, 0)); // clear row by creating a black rectangle
	textFooter = str;
	return 1;
}
int clearScreen(String str){
    // clear all
    textHeader = "";
    textBody = "";
    textScroll = "";
    textFooter = "";
	matrix.fillScreen(0);
	return 1;
}

int setColorHeader(String str){
	Serial.print("setColorHeader str");
	Serial.print(str);
	// Convert String to Char array
	char char_array[64];

	// Copy it over
	str.toCharArray(char_array, 64);
	StaticJsonBuffer<200> jsonBuffer;

	// Parse object
	JsonObject& json = jsonBuffer.parseObject(char_array);

    // JsonObject& json = stringToJSON(str);
    if(!json.containsKey("r")){
			return -1;
	}

	// Assign values
	colorHeader[0] = json["r"];
	colorHeader[1] = json["g"];
	colorHeader[2] = json["b"];
	colorHeaderString = str;
	//colorFooter[3] = json["a"];

	Serial.println("colorHeader[] = ");
    Serial.print(String(colorHeader[0])+", ");
    Serial.print(String(colorHeader[1])+", ");
    Serial.print(String(colorHeader[2]));
	return 1;
}

int setColorBody(String str){
	Serial.print("setColorBody str");
	Serial.print(str);
	// Convert String to Char array
	char char_array[64];

	// Copy it over
	str.toCharArray(char_array, 64);
	StaticJsonBuffer<200> jsonBuffer;

	// Parse object
	JsonObject& json = jsonBuffer.parseObject(char_array);

    // JsonObject& json = stringToJSON(str);
    if(!json.containsKey("r")){
			return -1;
	}

	// Assign values
	colorBody[0] = json["r"];
	colorBody[1] = json["g"];
	colorBody[2] = json["b"];
	colorBodyString = str;
	//colorFooter[3] = json["a"];

	Serial.println("colorBody[] = ");
    Serial.print(String(colorBody[0])+", ");
    Serial.print(String(colorBody[1])+", ");
    Serial.print(String(colorBody[2]));
	return 1;
}

int setColorScroll(String str){
	Serial.print("setColorScroll str");
	Serial.print(str);

    // Convert String to Char array
	char char_array[64];

	// Copy it over
	str.toCharArray(char_array, 64);
	StaticJsonBuffer<200> jsonBuffer;

	// Parse object
	JsonObject& json = jsonBuffer.parseObject(char_array);

    // JsonObject& json = stringToJSON(str);
    if(!json.containsKey("r")){
			return -1;
	}

	// Assign values
	colorScroll[0] = json["r"];
	colorScroll[1] = json["g"];
	colorScroll[2] = json["b"];
	colorScrollString = str;
	//colorFooter[3] = json["a"];

	Serial.println("colorScroll[] = ");
    Serial.print(String(colorScroll[0])+", ");
    Serial.print(String(colorScroll[1])+", ");
    Serial.print(String(colorScroll[2]));
	return 1;
}

int setColorFooter(String str){
	Serial.print("setColorFooter str");
	Serial.print(str);

    // Convert String to Char array
	char char_array[64];

	// Copy it over
	str.toCharArray(char_array, 64);
	StaticJsonBuffer<200> jsonBuffer;

	// Parse object
	JsonObject& json = jsonBuffer.parseObject(char_array);

    // JsonObject& json = stringToJSON(str);
    if(!json.containsKey("r")){
			return -1;
	}

	// Assign values
	colorFooter[0] = json["r"];
	colorFooter[1] = json["g"];
	colorFooter[2] = json["b"];
	//colorFooterString = "["+String(colorFooter[0])+","+String(colorFooter[1])+","+String(colorFooter[2])+"]";
	colorFooterString = str;
	//colorFooter[3] = json["a"];

	Serial.println("colorFooter[] = ");
    Serial.print(String(colorFooter[0])+", ");
    Serial.print(String(colorFooter[1])+", ");
    Serial.print(String(colorFooter[2]));
	return 1;
}

int setProgramSpeed(String str){
	Serial.print("setSpeed str");
	Serial.print(str);
	programSpeed = str.toInt();
	if (programSpeed < 0){
	    programSpeed = 0;
	}
	return 1;
}



const char* int_array_to_string(int int_array[], int size_of_array) {
  String returnstring = "";
  for (int i = 0; i < size_of_array; i++)
    returnstring += String(int_array[i]);
  return returnstring;
}


const char* int_array_to_char_array(int int_array[], int size_of_array){
    char output[size_of_array];
    int i;

    for (i = 0 ; i < 5 ; ++i)
    {
        output[i] = int_array[i];
    }
    return output;
}

// Program Setup
void setup() {

	// establish access to WiFi properties. (This is not required for connectivity)
  WiFi.on();

  Serial.begin(9600);
  Serial.print("System Booting");

	// API Functions
	// Text Layout
	Particle.function("textheader", setTextHeader); // string of text
	Particle.function("textbody", setTextBody); // string of text
	Particle.function("textscroll", setTextScroll); // string of text
	Particle.function("textfooter", setTextFooter); // string of text

	// Text Options
	Particle.function("clearscreen", clearScreen); // string boolean
	Particle.function("colorheader", setColorHeader); // string json {"color":[1,2,3]}
	Particle.function("colorbody", setColorBody); // string json {"color":[1,2,3]}
	Particle.function("colorscroll", setColorScroll); // string json {"color":[1,2,3]}
	Particle.function("colorfooter", setColorFooter); // string json {"color":[1,2,3]}

	// Program Options
	Particle.function("programspeed", setProgramSpeed); // string integer

	// Program Variables
	Particle.variable("colorheader", colorHeaderString);
	Particle.variable("colorbody", colorBodyString);
	Particle.variable("colorscroll", colorScrollString);
	Particle.variable("colorfooter", colorFooterString);
	Particle.variable("textheader", textHeader);
	Particle.variable("textbody", textBody);
	Particle.variable("textscroll", textScroll);
	Particle.variable("textfooter", textFooter);
	Particle.variable("programspeed", programSpeed);


	// Image Layout
	/* Feature Enhancement
	Particle.function("image", setImage);
	Particle.function("imagetoggle", setImageToggle);
	Particle.function("imagerotation", setImageRotation);
	Particle.function("imagerotationspeed", setImageRotation);
	*/

	// Initialize LED Matrix Panel and Display Tests
  matrix.begin();

  Serial.print("Testing matrix panel");
  // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7));
  delay(500);

  // fix the screen with green
  matrix.fillRect(0, 0, 63, 63, matrix.Color333(0, 7, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, 63, 63, matrix.Color333(7, 7, 0));
  delay(500);

  // draw an 'X' in red
  matrix.drawLine(0, 0, 63, 63, matrix.Color333(7, 0, 0));
  matrix.drawLine(31, 0, 0, 31, matrix.Color333(7, 0, 0));
  delay(500);

  // draw a blue circle
  matrix.drawCircle(10, 10, 10, matrix.Color333(0, 0, 7));
  delay(500);

  // fill a violet circle
  matrix.fillCircle(21, 21, 10, matrix.Color333(7, 0, 7));
  delay(500);

  // fill the screen with 'black'
  matrix.fillScreen(0);

  matrix.print(WiFi.SSID());
  matrix.print(WiFi.localIP());
  delay(3000);


  matrix.fillScreen(0);

  // Initialize Defaults
  setDisplayDefaults();

}



// Program Loop
void loop() {
  //if(scrollEnabled) scrollLoop();
	displayHeader(textHeader);
	displayBody(textBody);
	displayScroll(textScroll);
	displayFooter(textFooter);

	delay(programSpeed);

    // Clear background
   // matrix.fillScreen(0);

}
