/*
  DarkForm.h Version 1.7.3									 
  First Release (Feb 18, 2010)	by Paul Chin							    
  Last Updated: March 12, 2010									 
  Forum Post:													 	
  http://forum.thegamecreators.com/?m=forum_view&t=166159&b=22 
	
  Credits:
  See pdf Darkform tutorial
  
  * Free to modify and use for any purpose *	

  fixed picturebox setPicture bug
  (unable for 2 picturebox to show pics at same time)
  
*/


#ifndef DARKFORM_H
#define DARKFORM_H

#include "DarkGDK.h"

//--------- BOXPOINTS ----------
// Used in Picturebox, but may be used anywhere
typedef struct Boxpoints{
	int xLeft,yTop;	
	int width,height;	
}BOXPOINTS;


//--------- LABEL ----------

class Label{
public:
	Label(int _xLeft, int _yTop, char* _text);
	Label(int _xLeft, int _yTop, int _length, bool _isLED);
	void update();
	void setText(char* _text);
	char* getText();
	void clear();
private:
	int xLeft,yTop;
	char text[128];
	bool isLED;
	int length; //for LED

};
Label::Label(int _xLeft, int _yTop, char* _text){
	xLeft = _xLeft;
	yTop = _yTop;
    isLED = false;
	memset(text,0,128);
	strcpy(text, _text);	
}
Label::Label(int _xLeft, int _yTop,int _length, bool _isLED){
	xLeft = _xLeft;
	yTop = _yTop;
	length = _length;
    isLED = _isLED;
	memset(text,0,128);
}
void Label::setText(char* _text){
	strcpy(text,_text);
}
char* Label::getText(){
	return text;
}
void Label::clear(){
	memset(text,0,sizeof(text));
}
void Label::update(){
	if(isLED){
		dbInk(0,0);
		dbBox(xLeft,yTop,xLeft+length*9,yTop+20);
		dbInk(dbRGB(0,255,0),dbRGB(0,255,0));
		dbSetTextSize(12);
		text[length]='\0'; //limit to label's length
		dbText(xLeft+2,yTop+2,text);
	}else{
		dbInk(0,0); //set back to black
		dbSetTextSize(12);
		dbText(xLeft+2,yTop+2,text);
	}
}


//--------- TEXTBOX ----------

class Textbox {
public:
	Textbox(int _xLeft,int _yTop, int _length);
	Textbox(int _xLeft,int _yTop, int _length, bool _isPassword);
	void update();
	char* getText();
	void setText(char* text); 
	void clear();
private:
	int xLeft,yTop,xRight,yBottom,length;
	bool selected;
	char buffer[256];
	bool isBackspaceKeyDown; 
	bool isPassword;
};

Textbox::Textbox(int _xLeft,int _yTop, int _length){
	xLeft = _xLeft;
	yTop = _yTop;
	length = _length;
	xRight = xLeft + length*9;
	yBottom = _yTop + 20;	
	selected=isBackspaceKeyDown=false;
	memset(buffer,0,256);
	isPassword = false;
}
Textbox::Textbox(int _xLeft,int _yTop, int _length,bool _isPassword){
	xLeft = _xLeft;
	yTop = _yTop;
	length = _length;
	xRight = xLeft + length*9;
	yBottom = _yTop + 20;	
	selected=isBackspaceKeyDown=false;
	memset(buffer,0,256);
	isPassword = _isPassword;
}

void Textbox::update(){
	static bool isBackspaceKeyDown=false;  
	static int counter=0;  //backspace press & hold count
	int mc = dbMouseClick();
	int mx = dbMouseX();
	int my = dbMouseY();
	dbInk(dbRGB(255,255,255),dbRGB(255,255,255));
	dbBox(xLeft,yTop,xRight,yBottom);

	if (mc == 1){  
		if ((mx>xLeft+5 && mx<xRight-5) && (my>yTop && my<yBottom)){
			dbClearEntryBuffer( );
			selected = true;
		}
		else{
			dbClearEntryBuffer( );
			selected = false;	
		}
	}
	if(selected==true){ //selected look		
		dbInk(0,0);
		dbLine(xLeft,yBottom,xLeft,yTop); //left black edge
		dbLine(xLeft,yTop,xRight,yTop);   //top black edge
		dbLine(xRight,yTop,xRight,yBottom); //right black edge
		dbLine(xRight,yBottom,xLeft,yBottom);   //bottom black edge
	}else{
		dbInk(dbRGB(128,128,128),dbRGB(128,128,128));
		dbLine(xLeft,yBottom,xLeft,yTop); //left gray edge
		dbLine(xLeft,yTop,xRight,yTop);   //top gray edge
		dbLine(xRight,yTop,xRight,yBottom); //right gray edge
		dbLine(xRight,yBottom,xLeft,yBottom);   //bottom gray edge
		
		dbInk(dbRGB(255,255,255),dbRGB(255,255,255));
		dbLine(xLeft-1,yBottom+1,xLeft-1,yTop-1); //left white edge
		dbLine(xLeft-1,yTop-1,xRight+1,yTop-1);   //top white edge
		dbLine(xRight+1,yTop-1,xRight+1,yBottom+1); //right white edge
		dbLine(xRight+1,yBottom+1,xLeft-1,yBottom+1);   //bottom white edge
	}
	
	/*if (selected==true && dbGetEntry()>0 && strlen(buffer)<length && dbScanCode()!=14){
		strcat(buffer, dbGetEntry() );
	}*/
	char* input = dbGetEntry();
	if (selected==true && input != NULL && strlen(buffer)<length && dbScanCode()!=14){
                strcat(buffer, input);
    }
    delete[] input;
	
	if(selected==true && strlen(buffer)>0 && dbScanCode()==14){//backspace
		counter++;
		if(isBackspaceKeyDown==false) {		
			//char *str = dbLeft(buffer,dbLen(buffer)-1);	
			//memset(buffer,0,128);
			//strcpy(buffer,str);
			buffer[strlen(buffer) - 1] = '\0'; // Removing a character from the right
			isBackspaceKeyDown = true;	
			counter=0;			
		}
		if(counter>20){ //if backspace press and hold
			memset(buffer,0,128);			
			counter=0;		
			dbClearEntryBuffer( );
		}
	}
	
	if(dbKeyState(14)==0) { //if backspace not pressed
		isBackspaceKeyDown = false;
		counter=0;
	}
	dbInk(0,0);
	dbSetTextSize(12);
	char password[256]={0};
	for(int i=0; i<strlen(buffer); i++){
		strcat(password,"*");
	}
	if(!isPassword) dbText(xLeft+2,yTop+2,buffer);
	else dbText(xLeft+2,yTop+2,password);
	if(selected==true)dbClearEntryBuffer ( );
}

char* Textbox::getText(){
	return buffer;
}

void Textbox::setText(char* text){
	strcpy(buffer,text);
}
void Textbox::clear(){
	memset(buffer,0,256);
}

// -------- BUTTON ----------
class Button{
public:
	Button(int _xLeft,int _yTop, char* _caption);
	void update();	
	int getClickState();
private:
	int xLeft,yTop,xRight,yBottom;
	bool selected;
	char caption[128];
	int isClicked;
	int width;
};

Button::Button(int _xLeft,int _yTop, char* _caption){
	xLeft = _xLeft;
	yTop = _yTop;
	width = strlen(_caption)*11;
	xRight = xLeft + width;
	yBottom = yTop + 20;
	
	isClicked=0;
	memset(caption,0,128);
	strcpy(caption,_caption);
}

void Button::update(){
	int mc = dbMouseClick();
	int mx = dbMouseX();
	int my = dbMouseY();
	dbInk(dbRGB(190,190,190),dbRGB(190,190,190));
	dbBox(xLeft,yTop,xRight,yBottom);
	dbInk(dbRGB(255,255,255),dbRGB(255,255,255));
	dbLine(xLeft-1,yBottom+1,xLeft-1,yTop-1); //left white edge
	dbLine(xLeft-1,yTop-1,xRight+1,yTop-1);   //top white edge
	dbInk(0,0);
	dbLine(xRight+1,yTop-1,xRight+1,yBottom+1); //right black edge
	dbLine(xRight+1,yBottom+1,xLeft-1,yBottom+1);   //bottom black edge
	dbInk(0,0);
	dbCenterText(xLeft+width/2,yTop+2,caption);	
	
	if (mc == 1){  
		if ((mx > xLeft && mx < xRight) && (my > yTop && my < yBottom)){
			dbInk(dbRGB(160,160,160),dbRGB(160,160,160));
			dbBox(xLeft+1,yTop+1,xRight-1,yBottom-1);
			dbInk(0,0);
			dbCenterText(xLeft+width/2,yTop+2,caption);	
			isClicked = 1;				
		}
		else isClicked = 0;		
	}
}

int Button::getClickState(){
	if(isClicked==1 && dbMouseClick()==0){
		isClicked=0;		
		return 1;
	}else return 0;
}

//--------- STATUSBAR ----------
class Statusbar{
public:
	Statusbar();
	void update();	
	void setText(char* text);
private:
	int xLeft,yTop,xRight,yBottom;
	char text[256];
	int width;

};
Statusbar::Statusbar(){
	xLeft=0;
	yTop=dbScreenHeight()-30;
	xRight=dbScreenWidth();
	yBottom=dbScreenHeight();	
	memset(text,0,256);
}
void Statusbar::update(){
	dbInk(dbRGB(128,128,128),dbRGB(128,128,128));
	dbLine(xLeft,yTop-2,xRight,yTop-2);
	dbInk(dbRGB(190,190,190),dbRGB(190,190,190));
	dbBox(xLeft,yTop,xRight,yBottom);
	dbInk(0,0);
	dbSetTextSize(12);
	dbText(xLeft+10,yTop+5,text);
}
void Statusbar::setText(char* _text){
	memset(text,0,256);
	strcpy(text, _text);
}

//--------- TEXTAREA ----------
class Textarea{
public:
	Textarea(int _xLeft,int _yTop,int _width,int _height);
	void update();	
	void appendText(char* text);
	void clear();
private:
	int xLeft,yTop,xRight,yBottom;
	char buffer[100][300];
	int width,height;
	int line;
	int yMinline, yMaxline;
	int xMincolumn, xMaxcolumn;
};
Textarea::Textarea(int _xLeft,int _yTop,int _width,int _height){
	xLeft = _xLeft;
	yTop = _yTop;
	width = _width;
	height = _height;
	xRight = xLeft + width*9;
	yBottom = yTop + height*13 + 10;
	memset(buffer,0,sizeof(buffer));
	yMinline = yTop + 2;
	yMaxline = yBottom - 5;
	xMincolumn = xLeft + 5;
	xMaxcolumn = xRight - 5;
	line = 0;	
}
void Textarea::update(){	
	dbInk(dbRGB(240,240,240),dbRGB(240,240,240));
	dbBox(xLeft,yTop,xRight,yBottom);
	dbInk(0,0);	
	dbSetTextSize(12);
	for(int i=0; i<height; i++){		
		if(strlen(buffer[i])>width){
			char* str = dbLeft(buffer[i],width);
			dbText(xMincolumn,yMinline + i*13, str);
		}else{
			dbText(xMincolumn,yMinline + i*13, buffer[i]);
		}
	}
	
	dbInk(dbRGB(128,128,128),dbRGB(128,128,128));
	dbLine(xLeft,yBottom,xLeft,yTop); //left black edge
	dbLine(xLeft,yTop,xRight,yTop);   //top black edge
	dbLine(xRight,yTop,xRight,yBottom); //right black edge
	dbLine(xRight,yBottom,xLeft,yBottom);   //bottom black edge
	
	dbInk(dbRGB(255,255,255),dbRGB(255,255,255));
	dbLine(xLeft-1,yBottom+1,xLeft-1,yTop-1); //left black edge
	dbLine(xLeft-1,yTop-1,xRight+1,yTop-1);   //top black edge
	dbLine(xRight+1,yTop-1,xRight+1,yBottom+1); //right black edge
	dbLine(xRight+1,yBottom+1,xLeft-1,yBottom+1);   //bottom black edge	
}
//void Textarea::appendText(char* text){	
//	if(line < height&& strlen(text)>0){
//		strcat(buffer[line],text);
//		line++;
//	}else if(line >= height && strlen(text)>0) {
//		line=0;
//		memset(buffer,0,sizeof(buffer));
//		strcat(buffer[line],text);
//		line++;
//	}
//}

void Textarea::appendText(char* text){	
	if(line < height&& strlen(text)>0){
		strcat(buffer[line],text);
		line++;
	}else if(line >= height && strlen(text)>0) {
/*		line=0;
		memset(buffer,0,sizeof(buffer));
		strcat(buffer[line],text);
		line++;
*/
	for(int i=1; i<height; i++){
		strset(buffer[i-1],NULL);
		strcat(buffer[i-1],buffer[i]);
	}
	line--;
	strset(buffer[line],NULL);
	strcat(buffer[line],text);
	line++;
	}
}

void Textarea::clear(){
	memset(buffer,0,sizeof(buffer));
	line=0;
}

//--------- MESSAGEBOX ----------
class Messagebox{
public:
	Messagebox();
	void show(char* _message);
	void update();	
	int getResponse();
	bool getShowStatus();
protected: //so that accessible by Inputbox sub-class
	int xLeft,yTop,xRight,yBottom;
	char message[128];
	int width,height;
	bool isShow;
    bool isOK,isCancel;
	Button *btnOK, *btnCancel;
};
Messagebox::Messagebox(){	
	height = 100;
	memset(message,0,sizeof(message));	
	isOK=isCancel=isShow = false;	
}
void Messagebox::show(char* _message){
	memset(message,0,sizeof(message));
	strcpy(message,_message);
	width = 200 + strlen(message) * 5;
	xLeft = dbScreenWidth()/2 - width/2; //center it
	xRight = xLeft + width;	
	yTop = dbScreenHeight()/2-height/2;	//center it
	yBottom = yTop + height;
	btnOK = new Button(xLeft+width/3-(strlen("OK")*11)/2,yTop+60,"OK");
	btnCancel = new Button(xLeft+(width*2)/3-(strlen("Cancel")*11)/2,yTop+60,"Cancel");
	
	isShow = true;
}
void Messagebox::update(){
	if(isShow){		
		dbInk(dbRGB(190,190,190),dbRGB(190,190,190));
		dbBox(xLeft,yTop,xRight,yBottom);
		dbInk(dbRGB(0,0,200),dbRGB(0,0,200)); 
		dbBox(xLeft,yTop,xRight,yTop+20); //Top blue strip
		dbLine(xLeft,yBottom,xLeft,yTop); //left blue edge
		dbLine(xLeft,yTop,xRight,yTop);   //top blue edge
		dbLine(xRight,yTop,xRight,yBottom); //right blue edge
		dbLine(xRight,yBottom,xLeft,yBottom);   //bottom blue edge
		dbInk(0,0);
		dbCenterText(xLeft+width/2,yTop+30,message);
		btnOK->update();
		if(btnOK->getClickState()==1){
			//MessageBox(0,"OK","caption",0);
		    isOK=true;
		}
		btnCancel->update();		
		if(btnCancel->getClickState()==1) isCancel=true;
	}
}
int Messagebox::getResponse(){	
	if(isOK==true){
		isOK=false;
		isShow = false;	
		return 1;
	}
	if(isCancel){
		isCancel = false;
		isShow = false;	
		return 0;
	}	
	else return -1;
}
bool Messagebox::getShowStatus(){
	return isShow;
}

//--------- CHECKBOX ----------
class Checkbox{
public:
	Checkbox(int _xLeft,int _yTop,char* _text);	
	void update();	
	int getCheckstatus();
	void check();
	void uncheck();
private:
	int xLeft,yTop,xRight,yBottom;
	char text[64];
	int width,height;
	bool isChecked;
    bool isOK,isCancel;
    int length;
	Textbox *txtChkBox;
    Label *lblChkBoxText;
	int isClicked;
	void updateClickState();
};
Checkbox::Checkbox(int _xLeft,int _yTop,char* _text){
	xLeft = _xLeft;
	yTop = _yTop;
	length = 3;
	memset(text,0,sizeof(text));
	strcpy(text,_text);
	txtChkBox = new Textbox(xLeft,yTop,length);
	lblChkBoxText = new Label(xLeft+35,yTop+3,text);
	isClicked=0;
}
void Checkbox::update(){
	txtChkBox->update();
	lblChkBoxText->update();
	int mc = dbMouseClick();
	int mx = dbMouseX();
	int my = dbMouseY();
	int xRight = xLeft + length*9;
	int yBottom = yTop + 20;	
	if((mc==1 && mx>xLeft && mx<xRight) && (my>yTop && my<yBottom))isClicked=1;
	updateClickState();
}
void Checkbox::updateClickState(){
	if(isClicked==1 && dbMouseClick()==0){	
		char tmp[3] = {0};
		strcpy(tmp,txtChkBox->getText());
		if(strcmp(tmp," X")==0)	txtChkBox->clear();
		else if(strcmp(tmp,"")==0) txtChkBox->setText(" X");
		isClicked=0;		
	}
}
int Checkbox::getCheckstatus(){
		char tmp[3] = {0};
		strcpy(tmp,txtChkBox->getText());
		if(strcmp(tmp," X")==0) return 1;
		else return 0;
}
void Checkbox::check(){
	txtChkBox->setText(" X");
}
void Checkbox::uncheck(){
	txtChkBox->clear();
}


//--------- PICTUREBOX ----------
class Picturebox{
public:
	Picturebox(int _xLeft,int _yTop,int _width,int _height);
	void update();	
	void setPicture(char *_filename);
	void clear();
	void showPicture();
	BOXPOINTS getBoxpoints();
	void setPictureID(int _pictureID);
private:
	int xLeft,yTop,xRight,yBottom;	
	int width,height;
	char filename[64];
	int counter;
	BOXPOINTS boxPts;
	int pictureID;
};
Picturebox::Picturebox(int _xLeft,int _yTop,int _width,int _height){
	xLeft = _xLeft;
	yTop = _yTop;
	width = _width;
	height = _height;
	xRight = xLeft + width;
	yBottom = yTop + height;
	memset(filename,0,sizeof(filename));
	counter=0;	

	boxPts.xLeft = xLeft;
	boxPts.yTop = yTop;
	boxPts.width = width;
	boxPts.height = height;

	pictureID = -1;
}
BOXPOINTS Picturebox::getBoxpoints(){
	return boxPts;
}
void Picturebox::update(){
	counter++; if(counter>10000) counter=0;
	dbInk(0,0);
	dbBox(xLeft,yTop,xRight,yBottom);
	if(counter%100==0) showPicture();
}
void Picturebox::setPicture(char* _filename){
	strcpy(filename,_filename);
}
void Picturebox::showPicture(){
	if(strcmp(filename,"")==0) { 
		//MessageBox(0,"setPicture() first","Error",0);
		return;
	}
	if(pictureID==-1) return; //setPicture not called
	if(dbImageExist(pictureID)==1) dbDeleteImage(pictureID);
	dbLoadImage(filename,pictureID); 
	if(dbImageExist(pictureID)==1) {
		if(dbSpriteExist(pictureID+200)==1) dbDeleteSprite(pictureID+200);
		dbSprite(pictureID+200,xLeft,yTop,pictureID);
		//dbStretchSprite(200,(width*100)/maxX,(height*100)/maxY);		
		dbSizeSprite(pictureID+200,width,height);
	}
}
void Picturebox::clear(){
	//memset(filename,0,sizeof(filename));
	if(dbSpriteExist(pictureID+200)) dbDeleteSprite(pictureID+200);
}
void Picturebox::setPictureID(int _pictureID){
	pictureID = _pictureID;
}

//--------- GROUPBOX ----------
class Groupbox{
public:
	Groupbox(int _xLeft,int _yTop,int _width,int _height);
	Groupbox(int _xLeft,int _yTop,int _width,int _height,char* _caption);
	void update();		
private:
	int xLeft,yTop,xRight,yBottom;	
	int width,height;
	char caption[64];
	Label *lblGroupbox;
};
Groupbox::Groupbox(int _xLeft,int _yTop,int _width,int _height,char* _caption){
	xLeft = _xLeft;
	yTop = _yTop;
	width = _width;
	height = _height;
	xRight = xLeft + width;
	yBottom = yTop + height;
	memset(caption,0,sizeof(caption));
	strcpy(caption,_caption);
	if(strcmp(caption,"")!=0) lblGroupbox = new Label(xLeft,yTop-20,caption);	
}
Groupbox::Groupbox(int _xLeft,int _yTop,int _width,int _height){
	xLeft = _xLeft;
	yTop = _yTop;
	width = _width;
	height = _height;
	xRight = xLeft + width;
	yBottom = yTop + height;
	memset(caption,0,sizeof(caption));	
	lblGroupbox=NULL;
}
void Groupbox::update(){
	dbInk(0,0);
	dbLine(xLeft,yTop,xRight,yTop);
	dbLine(xRight,yTop,xRight,yBottom);
	dbLine(xRight,yBottom,xLeft,yBottom);
	dbLine(xLeft,yBottom,xLeft,yTop);
	if(lblGroupbox!=NULL) lblGroupbox->update();
}

//--------- INPUTBOX BOX ----------
class Inputbox:public Messagebox{
public:
	Inputbox();
	char* getText();
	void show(char* _message,int _input_length,int _isPassword);
	void update();
private:
	Textbox *textbox;
	bool isPassword;
};
Inputbox::Inputbox(){
	Messagebox::height = 200;	   
}
void Inputbox::show(char* _message,int _input_length,int _isPassword){	
	memset(message,0,sizeof(message));
	strcpy(message,_message);
	width = 200 + strlen(message) * 5;
	xLeft = dbScreenWidth()/2 - width/2; //center it
	xRight = xLeft + width;	
	yTop = dbScreenHeight()/2-height/2;	//center it
	yBottom = yTop + height;
	btnOK = new Button(xLeft+width/3-(strlen("OK")*11)/2,yTop+160,"OK");
	btnCancel = new Button(xLeft+(width*2)/3-(strlen("Cancel")*11)/2,yTop+160,"Cancel");
	textbox = new Textbox(xLeft+20,yTop+80,_input_length,_isPassword);
	isShow = true;	
}
void Inputbox::update(){	
	if(isShow){		
		dbInk(dbRGB(190,190,190),dbRGB(190,190,190));
		dbBox(xLeft,yTop,xRight,yBottom);
		dbInk(dbRGB(0,0,200),dbRGB(0,0,200)); 
		dbBox(xLeft,yTop,xRight,yTop+20); //Top blue strip
		dbLine(xLeft,yBottom,xLeft,yTop); //left blue edge
		dbLine(xLeft,yTop,xRight,yTop);   //top blue edge
		dbLine(xRight,yTop,xRight,yBottom); //right blue edge
		dbLine(xRight,yBottom,xLeft,yBottom);   //bottom blue edge
		dbInk(0,0);
		dbCenterText(xLeft+width/2,yTop+30,message);
		btnOK->update();
		if(btnOK->getClickState()==1){
			//MessageBox(0,"OK","caption",0);
		    isOK=true;
		}
		textbox->update();
		btnCancel->update();		
		if(btnCancel->getClickState()==1) isCancel=true;
		
	}
}
char* Inputbox::getText(){
	return textbox->getText();
}

//--------- HORIZONTAL SLIDER ----------
class SliderH{
public:
	SliderH(int _xLeft,int _yTop,int _width, float _value, float _max);
	SliderH(int _xLeft,int _yTop,int _width, float _value, float _max,char* _caption);
	float max, value;
	void update();		
private:
	int xLeft,yTop,xRight;	
	int width;
	int isClicked;
	char caption[64];
	Label *lblSliderH;
};
SliderH::SliderH(int _xLeft,int _yTop,int _width, float _value, float _max,char* _caption){
	xLeft = _xLeft;
	yTop = _yTop;
	width = _width;
	value = _value;
	max = _max;
	xRight = xLeft + width;
	memset(caption,0,sizeof(caption));
	strcpy(caption,_caption);
	if(strcmp(caption,"")!=0) lblSliderH = new Label(xLeft,yTop-20,caption);
}
SliderH::SliderH(int _xLeft,int _yTop,int _width, float _value, float _max){
	xLeft = _xLeft;
	yTop = _yTop;
	width = _width;
	xRight = xLeft + width;
	value = _value;
	max = _max;
	memset(caption,0,sizeof(caption));	
}
void SliderH::update(){

	if (value<0) value=0;
	if (value>max) value=max;

	dbInk(dbRGB(255,255,255),dbRGB(255,255,255));
	dbBox(xLeft-1,yTop-1,xRight+1,yTop+2);
	dbBox((xLeft+((value/max)*width))-2,yTop-5,(xLeft+((value/max)*width))+3,yTop+6);

	dbInk(dbRGB(128,128,128),dbRGB(128,128,128));
	dbLine(xLeft,yTop,xRight,yTop);
	dbInk(0,0);
	dbBox((xLeft+((value/max)*width))-1,yTop-4,(xLeft+((value/max)*width))+2,yTop+5);

	if (dbMouseClick() == 1){  
		if ((dbMouseX() > xLeft-1 && dbMouseX() < xRight+1) && (dbMouseY() > yTop-5 && dbMouseY() < yTop+5)){
			dbInk(dbRGB(160,160,160),dbRGB(160,160,160));
			dbBox((xLeft+((value/max)*width))-1,yTop-4,(xLeft+((value/max)*width))+2,yTop+5);
			value = ceil(((dbMouseX()-xLeft)*max)/width);
			isClicked = 1;				
		}
		else isClicked = 0;		
	}

	if(lblSliderH!=NULL) lblSliderH->update();
}

//--------- VERTICAL SLIDER ----------
class SliderV{
public:
	SliderV(int _xLeft,int _yTop,int _height, float _value, float _max);
	SliderV(int _xLeft,int _yTop,int _height, float _value, float _max,char* _caption);
	float max, value;
	void update();		
private:
	int xLeft,yTop,yBottom;	
	int height;
	int isClicked;
	char caption[64];
	Label *lblSliderV;
};
SliderV::SliderV(int _xLeft,int _yTop,int _height, float _value, float _max,char* _caption){
	xLeft = _xLeft;
	yTop = _yTop;
	height = _height;
	value = _value;
	max = _max;
	yBottom = yTop + height;
	memset(caption,0,sizeof(caption));
	strcpy(caption,_caption);
	if(strcmp(caption,"")!=0) lblSliderV = new Label(xLeft,yTop-20,caption);
}
SliderV::SliderV(int _xLeft,int _yTop,int _height, float _value, float _max){
	xLeft = _xLeft;
	yTop = _yTop;
	height = _height;
	yBottom = yTop + height;
	value = _value;
	max = _max;
	memset(caption,0,sizeof(caption));	
}
void SliderV::update(){

	if (value<0) value=0;
	if (value>max) value=max;

	dbInk(dbRGB(255,255,255),dbRGB(255,255,255));
	dbBox(xLeft-1,yTop-1,xLeft+2,yBottom+1);
	dbBox(xLeft-5,yBottom-((value/max)*height)-2,xLeft+6,yBottom-((value/max)*height)+2);

	dbInk(dbRGB(128,128,128),dbRGB(128,128,128));
	dbLine(xLeft,yTop,xLeft,yBottom);
	dbInk(0,0);
	dbBox(xLeft-4,yBottom-((value/max)*height)-1,xLeft+5,yBottom-((value/max)*height)+1);

	if (dbMouseClick() == 1){  
		if ((dbMouseX() > xLeft-5 && dbMouseX() < xLeft+5) && (dbMouseY() > yTop-1 && dbMouseY() < yBottom+1)){
			dbInk(dbRGB(160,160,160),dbRGB(160,160,160));
			dbBox(xLeft-4,yBottom-((value/max)*height)-1,xLeft+5,yBottom-((value/max)*height)+1);
			value = ceil(((yBottom-dbMouseY())*max)/height);
			isClicked = 1;				
		}
		else isClicked = 0;		
	}

	if(lblSliderV!=NULL) lblSliderV->update();
}

//--------- FORM ----------
#define SIZE 32
class Form{
public:
	Form(char* _title);
	Form(char* _title,int _color);
	Form(char* _title,int _color,int _xLeft,int _yTop,int _width,int _height);
	void update();
	void add(Label* _label);
	void add(Textbox* _textbox);
	void add(Button* _button);
	void add(Statusbar* _statusbar);
	void add(Textarea* _textarea);
	void add(Messagebox* _messagebox);
	void add(Checkbox* _checkbox);
	void add(Picturebox* _picturebox);	
	void add(Groupbox* _groupbox);
	void add(Inputbox* _inputbox);
	void add(SliderH* _sliderh);
	void add(SliderV* _sliderv);
private:
	int color;
	char title[128];
	int xLeft,yTop,xRight,yBottom;
	int width,height;
	Label* label[SIZE];
	Textbox* textbox[SIZE];
	Button* button[SIZE];
	Statusbar* statusbar[SIZE];
	Textarea* textarea[SIZE];
	Messagebox* messagebox[SIZE];
	Checkbox* checkbox[SIZE];
	Picturebox* picturebox[SIZE];
	Groupbox* groupbox[SIZE];
	bool isMessageboxShown,isInputboxShown;
	Inputbox* inputbox[SIZE];
	SliderH* sliderh[SIZE];
	SliderV* sliderv[SIZE];
	int pictureboxID;
};
Form::Form(char* _title){
	dbSyncOn();
	dbSyncRate(60);
	color = 220;
	memset(title,0,sizeof(title));
	strcpy(title,_title);	
	dbColorBackdrop (dbRGB(color,color,color));
	dbBackdropOn();
	dbSetWindowTitle(title);
	memset(label,NULL,sizeof(label));
	memset(textbox,NULL,sizeof(textbox));
	memset(button,NULL,sizeof(button));
	memset(statusbar,NULL,sizeof(statusbar));
	memset(textarea,NULL,sizeof(textarea));
	memset(messagebox,NULL,sizeof(messagebox));
	memset(checkbox,NULL,sizeof(checkbox));
	memset(picturebox,NULL,sizeof(picturebox));
	memset(groupbox,NULL,sizeof(groupbox));
	memset(inputbox,NULL,sizeof(inputbox));
	memset(sliderh,NULL,sizeof(sliderh));
	memset(sliderv,NULL,sizeof(sliderv));
	isMessageboxShown = isInputboxShown = false;
	pictureboxID = 88888;
}
Form::Form(char* _title,int _color){
	dbSyncOn();
	dbSyncRate(60);
	color = _color;
	memset(title,0,128);
	strcpy(title,_title);	
	dbColorBackdrop (dbRGB(color,color,color));
	dbBackdropOn();
	dbSetWindowTitle(title);
	memset(label,NULL,sizeof(label));
	memset(textbox,NULL,sizeof(textbox));
	memset(button,NULL,sizeof(button));
	memset(statusbar,NULL,sizeof(statusbar));
	memset(textarea,NULL,sizeof(textarea));
	memset(messagebox,NULL,sizeof(messagebox));
	memset(checkbox,NULL,sizeof(checkbox));
	memset(picturebox,NULL,sizeof(picturebox));
	memset(groupbox,NULL,sizeof(groupbox));
	memset(inputbox,NULL,sizeof(inputbox));
	memset(sliderh,NULL,sizeof(sliderh));
	memset(sliderv,NULL,sizeof(sliderv));
	isMessageboxShown = isInputboxShown = false;
	pictureboxID = 88888;
}
Form::Form(char* _title,int _color,int _xLeft,int _yTop,int _width,int _height){
	//dbSyncOn();
	//dbSyncRate(60);
	color = _color;
	memset(title,0,128);
	strcpy(title,_title);	
	//dbColorBackdrop (dbRGB(color,color,color));
	//dbBackdropOn();
	dbSetWindowTitle(title);
	xLeft = _xLeft;
	yTop = _yTop;
	width = _width;
	height = yTop + _height;
	memset(label,NULL,sizeof(label));
	memset(textbox,NULL,sizeof(textbox));
	memset(button,NULL,sizeof(button));
	memset(statusbar,NULL,sizeof(statusbar));
	memset(textarea,NULL,sizeof(textarea));
	memset(messagebox,NULL,sizeof(messagebox));
	memset(checkbox,NULL,sizeof(checkbox));
	memset(picturebox,NULL,sizeof(picturebox));
	memset(groupbox,NULL,sizeof(groupbox));
	memset(inputbox,NULL,sizeof(inputbox));
	memset(sliderh,NULL,sizeof(sliderh));
	memset(sliderv,NULL,sizeof(sliderv));
	isMessageboxShown = isInputboxShown = false;
	pictureboxID = 88888;
}
void Form::update(){	
	dbBackdropOn(); //so that messagebox can hide (in partial forms)
	dbInk(dbRGB(color,color,color),dbRGB(color,color,color));
	dbBox(xLeft,yTop,width,height);
	for(int i=0; i<SIZE; i++){
		if(label[i]!=NULL && !isMessageboxShown && !isInputboxShown) label[i]->update();
		if(textbox[i]!=NULL && !isMessageboxShown && !isInputboxShown) textbox[i]->update();
		if(button[i]!=NULL && !isMessageboxShown && !isInputboxShown) button[i]->update();
		if(statusbar[i]!=NULL ) statusbar[i]->update();
		if(textarea[i]!=NULL&& !isMessageboxShown && !isInputboxShown) textarea[i]->update();
		if(messagebox[i]!=NULL){
			messagebox[i]->update();
			isMessageboxShown = messagebox[i]->getShowStatus();
		}
		if(checkbox[i]!=NULL && !isMessageboxShown && !isInputboxShown) checkbox[i]->update();
		if(picturebox[i]!=NULL && !isMessageboxShown && !isInputboxShown) {
			picturebox[i]->update();
		}else if(picturebox[i]!=NULL) picturebox[i]->clear(); //in case a picture is loaded
		if(groupbox[i]!=NULL && !isMessageboxShown && !isInputboxShown ) groupbox[i]->update();
		if(sliderh[i]!=NULL && !isMessageboxShown && !isInputboxShown ) sliderh[i]->update();
		if(sliderv[i]!=NULL && !isMessageboxShown && !isInputboxShown ) sliderv[i]->update();
		if(inputbox[i]!=NULL){
			inputbox[i]->update();
			isInputboxShown = inputbox[i]->getShowStatus();
		}
	}	
}
void Form::add(Label* _label) {
	for(int i=0; i<SIZE; i++){
		if(label[i]==NULL){
			label[i] =  _label; 
			break;
		}
	}
}
void Form::add(Textbox* _textbox) {
	for(int i=0; i<SIZE; i++){
	if(textbox[i]==NULL){
		textbox[i] =  _textbox; 
		break;
	}
} 
}
void Form::add(Button* _button) { 
	for(int i=0; i<SIZE; i++){
		if(button[i]==NULL){
			button[i] =  _button; 
			break;
		}
	}
}
void Form::add(Statusbar* _statusbar) {
	for(int i=0; i<SIZE; i++){
		if(statusbar[i]==NULL){
			statusbar[i] =  _statusbar; 
			break;
		}
	} 
}
void Form::add(Textarea* _textarea) { 
	for(int i=0; i<SIZE; i++){
		if(textarea[i]==NULL){
			textarea[i] =  _textarea; 
			break;
		}
	} 
}
void Form::add(Messagebox* _messagebox) { 
	for(int i=0; i<SIZE; i++){
		if(messagebox[i]==NULL){
			messagebox[i] =  _messagebox; 
			break;
		}
	} 
}
void Form::add(Checkbox* _checkbox) { 
	for(int i=0; i<SIZE; i++){
		if(checkbox[i]==NULL){
			checkbox[i] =  _checkbox; 
			break;
		}
	} 
}
void Form::add(Picturebox* _picturebox) { 
	for(int i=0; i<SIZE; i++){
		if(picturebox[i]==NULL){
			picturebox[i] =  _picturebox; 
			picturebox[i]->setPictureID(pictureboxID++);
			break;
		}
	}  
}
void Form::add(Groupbox* _groupbox) {
	for(int i=0; i<SIZE; i++){
		if(groupbox[i]==NULL){
			groupbox[i] =  _groupbox; 
			break;
		}
	}
}
void Form::add(Inputbox* _inputbox) {
	for(int i=0; i<SIZE; i++){
		if(inputbox[i]==NULL){
			inputbox[i] =  _inputbox; 
			break;
		}
	}
}
void Form::add(SliderH* _sliderh) {
	for(int i=0; i<SIZE; i++){
		if(sliderh[i]==NULL){
			sliderh[i] =  _sliderh; 
			break;
		}
	}
}

void Form::add(SliderV* _sliderv) {
	for(int i=0; i<SIZE; i++){
		if(sliderv[i]==NULL){
			sliderv[i] =  _sliderv; 
			break;
		}
	}
}


#endif