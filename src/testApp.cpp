#include "testApp.h"
#include "ofxMacKeys.h"
#include "clipboard.h"

extern "C" {
#include "macGlutfix.h"
}
unsigned char *data = NULL;
bool mouseIsDown = false;
ofVec2f start, end;
ofColor currColour;
string hexString;
string info;
bool showInstructions = true;
bool doMouse = true;
bool doColour = true;
//--------------------------------------------------------------
void testApp::setup(){
	
	char c[512];
	getcwd(c, 512);
	//printf("Cwd: %s\n", c);
	string path = c;
	path += "/../Resources/data/";
	ofSetDataPathRoot(path);
	
	
	tex.allocate(ofGetScreenWidth(),ofGetScreenHeight(), GL_RGBA);
	shader.load("colourConvertor.vert", "colourConvertor.frag");
	ofBackground(0,0,0);
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update() {
	 data = pixelsBelowWindow(0,0,ofGetScreenWidth(),ofGetScreenHeight());
	 if (data!= NULL) tex.loadData(data, ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA);
}

void drawString(string str, ofVec2f pos) {
	glColor4f(0, 0, 0, 1);
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			ofDrawBitmapString(str, pos + ofVec2f(i, j));
		}
	}
	glColor4f(1, 1, 1, 1);
	ofDrawBitmapString(str, pos);
}
//--------------------------------------------------------------
void testApp::draw(){
	shader.begin();
	shader.setUniformTexture("tex", tex, 0);
	if(mouseIsDown) {
		shader.setUniform2f("start", start.x, start.y);
		shader.setUniform2f("end", end.x, end.y);
	} else {
		shader.setUniform2f("start", 0, 0);
		shader.setUniform2f("end", 0, 0);
	}
	tex.draw(0,0, ofGetWidth(), ofGetHeight());
	shader.end();
	
	unsigned char pix[4];
	glReadPixels(	mouseX,
					  ofGetHeight() - mouseY,
					  1,
					  1,
					  GL_RGB,
					  GL_UNSIGNED_BYTE,
					  pix);
	
	currColour = ofColor(pix[0], pix[1], pix[2]);
	
	
	char hex[9];
	
	sprintf(hex, "%x", currColour.getHex());
	
	hexString = hex;
	while(hexString.size()<6) {
		hexString = string("0") + hexString;
	}
	hexString = string("#")+hexString;
	hexString = ofToUpper(hexString);
	
	
	if(doMouse) {
		info = string("(") + ofToString(mouseX) +", " + ofToString(mouseY) + ") ";
	} else {
		info = "";
	}
	
	if(mouseIsDown) {
		//ofLine(start, end);
		int dx = end.x - start.x;
		int dy = end.y - start.y;
		string sizeInfo = ofToString(dx) + " x " + ofToString(dy);
		
		drawString(info, start);
		
		drawString(sizeInfo, end+ofVec2f(5, 5));
		info += ofToString(dx) + " x " + ofToString(dy) + "  ";
	}
	if(doColour) {
		info += hexString;
	}
	drawString(info, ofVec2f(10, ofGetHeight()-5));
	string instructions = "[ h ] hide this menu\n[ c ] toggle colour\n[ m ] toggle mouse position\n[' '] append to clipboard\n[esc] quit (or click without\n      dragging)\n\nShift-drag to do\nhorizontal or vertical lines.";
	if(showInstructions) {
		glColor4f(0, 0, 0, 0.3);
		ofRect(5, 5, 241, 130);
		drawString(instructions, ofVec2f(10, 20));
	}
}

string infoLog = "";
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='c' || key=='C') {
		doColour ^= true;
	} else if(key=='m') {
		doMouse ^= true;
	} else if(key==' ') {
		infoLog += info + "\n";
		copyToClipboard(infoLog);
	} else if(key=='h') {
		showInstructions ^= true;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

void snapShift() {
	if(ofxMacShiftKeyDown()) {
		int dx = end.x - start.x;
		int dy = end.y - start.y;
		if(ABS(dx)>ABS(dy)) {
			end.y = start.y;
		} else {
			end.x = start.x;
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	end = ofVec2f(x,y);
	snapShift();
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	start = ofVec2f(x, y);
	end = start;
	mouseIsDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	end = ofVec2f(x, y);
	snapShift();
	
	mouseIsDown = false;
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	if(ABS(dx)>0 || ABS(dy)>0) {
		string dims = ofToString(dx) + ", " + ofToString(dy);
		copyToClipboard(dims);
		
	}
	if(start==end) {
		std::exit(0);
	}
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}