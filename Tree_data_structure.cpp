#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <windows.h>
using namespace std;

//! data Structures---------------
class Point {
private:
	friend class region;
	friend class Node;
	double x;
	double y;
public:	Point() { this->x = 0; this->y = 0; }
	  Point(double valueX, double valueY) { this->x = valueX; this->y = valueY; }

	  double getX() { return this->x; }
	  double getY() { return this->y; }
	  void SetX(double val) { this->x = val; }
	  void SetY(double val) { this->y = val; }
};

class region {
private:
	friend class Node;
	friend class point;
	Point TL, BR;
public:
	region() {
		this->TL.x = 0;
		this->TL.y = 0;
		this->BR.x = 0;
		this->BR.y = 0;
	}
	region(Point a, Point b) {
		this->TL = a;
		this->BR = b;
	}
	region(double a, double b, double c, double d) {
		this->TL.x = a;
		this->TL.y = b;
		this->BR.x = c;
		this->BR.y = d;
	}
	Point getTL() { return this->TL; }
	Point getBR() { return this->BR; }
	void setTL(Point x) { this->TL = x; }
	void setBR(Point x) { this->BR = x; }
};

class Node {
private:
	friend class point;
	friend class point;
	friend class region;
	region reg;
	Point pos;
	//isArea 
	bool isDote;
	string name;
	// Children of this node
	Node* nw, * ne, * sw, * se;
public:

	Node(region R) {
		this->reg = R;
		this->pos.x = NULL;
		this->isDote = false;
		this->pos.y = NULL;
		this->name = "";
		setChilds();
	}
	Node(Point a, Point b) {
		region R = region(a, b);
		this->reg = R;
		this->name = "";
		this->isDote = false;
		this->pos.x = NULL;
		this->pos.y = NULL;
	}
	Node() {
		this->isDote = false;
		this->reg.BR.x = NULL;
		this->reg.BR.y = NULL;
		this->reg.TL.x = NULL;
		this->reg.TL.y = NULL;
		this->name = "";
		this->pos.x = NULL;
		this->pos.y = NULL;
	}
	Node(string N, Point P) {
		this->name = N;
		this->isDote = true;
		this->pos = P;
		this->reg.BR = P;
		this->reg.TL = P;
		nw = NULL;
		ne = NULL;
		sw = NULL;
		se = NULL;
	}
	void setChilds() {
		double Xaxle = (this->reg.BR.x + this->reg.TL.x) / 2;
		double Yaxle = (this->reg.BR.y + this->reg.TL.y) / 2;
		Point nwTL = Point(reg.TL.x, this->reg.TL.y);
		Point nwBR = Point(Xaxle, Yaxle);
		Point neTL = Point(Xaxle, this->reg.TL.y);
		Point neBR = Point(this->reg.BR.x, Yaxle);
		Point swTL = Point(this->reg.TL.x, Yaxle);
		Point swBR = Point(Xaxle, this->reg.BR.y);
		Point seTL = Point(Xaxle, Yaxle);
		Point seBR = Point(this->reg.BR.x, this->reg.BR.y);
		Node* a = new Node(nwTL, nwBR);
		this->nw = a;
		Node* b = new Node(neTL, neBR);
		this->ne = b;
		Node* c = new Node(swTL, swBR);
		this->sw = c;
		Node* d = new Node(seTL, seBR);
		this->se = d;
	}
	double getAxleX() {
		return (this->reg.BR.x + this->reg.TL.x) / 2;
	}
	double getAxleY() {
		return(this->reg.BR.y + this->reg.TL.y) / 2;
	}
	int addDot(Node* root, Node* dot) {
		Node* inc = root;
		double Xaxle = (inc->reg.BR.x + inc->reg.TL.x) / 2;
		double Yaxle = (inc->reg.BR.y + inc->reg.TL.y) / 2;
		if (inc == NULL) {
			root = inc;
			return 1;
		}
		if (inc)
		{
			if (dot->pos.x < Xaxle && dot->pos.y < Yaxle) {
				// area = nw
				if (inc->nw->isDote == false) {
					inc->nw = dot; inc->isDote = true;
				}
				else {
					Node* P = inc->nw;
					Point Start = Point(inc->reg.TL.x, inc->reg.TL.y);
					Point End = Point(Xaxle, Yaxle);
					region reg = region(Start, End);
					inc->nw = new Node(reg);
					addDot(inc->nw, P);
					addDot(inc->nw, dot);
				}
			}
			else if (dot->pos.x > Xaxle && dot->pos.y < Yaxle) {
				// area = ne
				if (inc->ne->isDote == false) {
					inc->ne = dot;
					inc->isDote = true;
				}
				else {
					Node* P = inc->ne;
					Point Start = Point(Xaxle, inc->reg.TL.y);
					Point End = Point(inc->reg.BR.x, Yaxle);
					region reg = region(Start, End);
					inc->ne = new Node(reg);
					addDot(inc->ne, P);
					addDot(inc->ne, dot);
				}
			}
			else if (dot->pos.x < Xaxle && dot->pos.y >Yaxle) {
				// area = sw
				if (inc->sw->isDote == false) {
					inc->sw = dot; inc->isDote = true;
				}
				else {
					Node* P = inc->sw;
					Point Start = Point(inc->reg.TL.x, Yaxle);
					Point End = Point(Xaxle, inc->reg.BR.y);
					region reg = region(Start, End);
					inc->sw = new Node(reg);
					addDot(inc->sw, P);
					addDot(inc->sw, dot);
				}
			}
			else if (dot->pos.x > Xaxle && dot->pos.y > Yaxle) {
				// area = se
				if (inc->se->isDote == false) {
					inc->se = dot; inc->isDote = true;
				}
				else {
					Node* P = inc->se;
					Point Start = Point(Xaxle, Yaxle);
					Point End = Point(inc->reg.BR.x, inc->reg.BR.y);
					region reg = region(Start, End);
					inc->se = new Node(reg);
					addDot(inc->se, P);
					addDot(inc->se, dot);
				}
			}
		}


	}
	bool isEmptyChild() { if (this->nw == NULL)return true; else return false; }
	Node* getNW() {
		return this->nw;
	}
	Node* getNE() {
		return this->ne;
	}
	Node* getSW() {
		return this->sw;
	}
	Node* getSE() {
		return this->se;
	}
	string getName() { return this->name; }
	int getX1() { return this->reg.TL.x; }
	int getY1() { return this->reg.TL.y; }
	int getX2() { return this->reg.BR.x; }
	int getY2() { return this->reg.BR.y; }
	void clearNW() {
		this->nw->name = "";
		this->isDote = false;
		this->nw->reg = region( this->reg.TL , Point(this->getAxleX() , this->getAxleY()));
		this->nw->pos.x = NULL;
		this->nw->pos.y = NULL;
	}
	void clearNE() {
		this->ne->name = ""; this->isDote = false;
		this->ne->reg = region( Point(this->getAxleX() , this->reg.TL.y ), Point(this->reg.BR.x, this->getAxleY()));
		this->ne->pos.x = NULL;
		this->ne->pos.y = NULL;
	}
	void clearSW() {
		this->sw->name = ""; this->isDote = false;
		this->sw->reg = region(Point(this->reg.TL.x,this->getAxleY()) , Point(this->getAxleX() , this->reg.BR.y));
		this->sw->pos.x = NULL;
		this->sw->pos.y = NULL;
	}
	void clearSE() {
		this->se->name = ""; this->isDote = false;
		this->se->reg = region(Point(this->getAxleX(),this->getAxleY() ), this->reg.BR);
		this->se->pos.x = NULL;
		this->se->pos.y = NULL;
	}
	double getPosX() { return this->pos.x; }
	double getPosY() { return this->pos.y; }

	//void clearName() { this->name = ""; this->isDote = false; this-> }
};


//global WAY
Node* glob = new Node();
class Tree {
private:
	friend class Node;
	Node* root;
public:
	Tree() { this->root = NULL; }
	void setRoot(Node* j) { this->root = j; }
	Node* getRoot() { return this->root; }
};

//! Tools-------------------------
string filepath = "C:\\Users\\Eddy\\Desktop\\ex.txt";

int menu() {
	int i; HANDLE col;
	col = GetStdHandle(STD_OUTPUT_HANDLE);
	
	system("Cls");
	SetConsoleTextAttribute(col,4);
	cout << "\n\n\t\tMENU\n";
	SetConsoleTextAttribute(col,2);
	cout << "\n\n\t1>> PRINT ALL TREET\n"; SetConsoleTextAttribute(col,1);
	cout << "\n\n\t1>> PRINT WITH RANGE\n"; SetConsoleTextAttribute(col,9);
	cout << "\n\n\t1>> PRINT\n"; SetConsoleTextAttribute(col, 11);
	cout << "\n\n\t1>> DELETE ALL DOTS\n"; SetConsoleTextAttribute(col, 13);
	cout << "\n\n\t1>> DELETE SINGLE DOT\n"; SetConsoleTextAttribute(col, 12);
	cout << "\n\n\t1>> DELETE ALL LIST\n"; SetConsoleTextAttribute(col,4);
	cout << "\n\n\t1>> PRINT\n";

	cin >> i;
	return i;
}
void scriptWriter(double x1, double y1, double x2, double y2, string name) {

	ofstream input_file("data.html", ios::app);
	// Create the HTML table
	input_file << "\n  <script>\n";
	input_file << "    var canvas = document.getElementById(\"myCanvas\");\n";
	input_file << "    var ctx = canvas.getContext(\"2d\");\n";
	input_file << "    var x1 =" << x1 << "; \n";
	input_file << "    var y1 = " << y1 << ";\n";
	input_file << "    var x2 =" << x2 << ";\n";
	input_file << "    var y2 = " << y2 << ";\n";
	input_file << "    var height = y2 - y1;\n";
	input_file << "    var width = x2 - x1;\n";
	input_file << "    ctx.rect(x1, y1, width, height);\n";
	input_file << "    ctx.stroke();\n";
	input_file << "    ctx.font = \"10px Arial\";\n";
	input_file << "    ctx.fillStyle = \"#ffd500\"; \n";
	input_file << "    ctx.textAlign = \"center\";\n";
	input_file << "    ctx.fillText(\"" << name << "\" ,(x1 + x2) / 2, (y1 + y2) / 2); \n";
	input_file << "  </script>\n";
	input_file.close();
}
void partition(Node* inc) {

	// line TOP 
	double X1 = inc->getX1();
	double Y1 = inc->getY1();
	double X2 = inc->getAxleX();
	double Y2 = inc->getY1();
	double X3 = inc->getX2();
	double Y3 = inc->getY1();
	//line middle
	double X4 = inc->getX1();
	double Y4 = inc->getAxleY();
	double X5 = inc->getAxleX();
	double Y5 = inc->getAxleY();
	double X6 = inc->getX2();
	double Y6 = inc->getAxleY();
	// line bottom 
	double X7 = inc->getX1();
	double Y7 = inc->getY2();
	double X8 = inc->getAxleX();
	double Y8 = inc->getY2();
	double X9 = inc->getX2();
	double Y9 = inc->getY2();

	//scrip for HTML format --> making 4 parted 
	scriptWriter(X1, Y1, X5, Y5, "");
	scriptWriter(X2, Y2, X6, Y6, "");
	scriptWriter(X4, Y4, X8, Y8, "");
	scriptWriter(X5, Y5, X9, Y9, "");
}
void WriteDotInFile(string name, double x, double y) {
	ofstream File; 
	File.open(filepath, ios::app);
	File << endl << name << " " << x << " " << y;
	File.close();
}
void WriteAreaInFile(double x1, double y1, double x2, double y2) {
	ofstream File;
	File.open(filepath, ios::app);
	File << endl << x1 << " " << y1 << " " <<x2<<" "<<x2<<" "<<y2;
	File.close();
}
void UpdateFile(Node* root, int counter =0 ) {

	Node* inc = root;
	if (inc) {
		if (counter == 0) {
			WriteAreaInFile(inc->getX1(), inc->getY1(), inc->getX2(), inc->getY2());
		}
		else if (inc->getName() != "") {
			WriteDotInFile(inc->getName(), inc->getPosX(), inc->getPosY() ); counter++;
		}
		if (!inc->isEmptyChild())  {
			UpdateFile(inc->getNW(), counter);
			UpdateFile(inc->getNE(), counter);
			UpdateFile(inc->getSW(), counter);
			UpdateFile(inc->getSE(), counter);
		}
	}
}

//! features---------------------- 
void filler() { // READING FILL AND FILL THE GLOBAL LIST 
	// open file 
	ifstream File;
	File.open(filepath);
	string currentLine;
	Tree tree = Tree();// instance of tree ;
	int line = 1;
	int dotsNumber = 0;
	double a = 0, b = 0, c = 0, d = 0;
	string name; double x = 0, y = 0;
	while (File >> currentLine) {
		if (line >= 1 and line <= 4) {

			if (line == 1) {
				a = stod(currentLine); line++;
			}
			else if (line == 2) {
				b = stod(currentLine); line++;
			}
			else if (line == 3) {
				c = stod(currentLine); line++;
			}
			else if (line == 4) {
				d = stod(currentLine);
				line++;
				region reg = region(a, b, c, d);
				Node* r = new Node(reg);
				glob = r;

			}
		}
		else if (line > 4) {


			if (line == 5) {
				name = currentLine; line++;
			}
			else if (line == 6) {
				x = stod(currentLine); line++;
			}
			else if (line == 7) {
				y = stod(currentLine); line = 5;
				Point P = Point(x, y);
				Node* dot = new Node(name, P);
				glob->addDot(glob, dot);
			}
		}
	}

}
void ListNavigation(Node* root)
{ // NAVIGATING LIST AND WRITE DATA IN HTML FILE 
	Node* inc = root;
	string name = inc->getName();
	double x1 = inc->getX1();
	double x2 = inc->getX2();
	double y1 = inc->getY1();
	double y2 = inc->getY2();
	string color = "#ffd500";
	if (inc) {
		//statment
		scriptWriter(x1, y1, x2, y2, name);
		if (!inc->isEmptyChild()) {
			partition(inc);
			// recersive 
			ListNavigation(inc->getNW());
			ListNavigation(inc->getNE());
			ListNavigation(inc->getSW());
			ListNavigation(inc->getSE());
		}
	}
}
void htmlProvider() {//COMPELET THE HTML FILE 
	ofstream input_file("data.html");
	input_file << "<!DOCTYPE html>\n";
	input_file << "\n<html>\n";
	input_file << "\n  <head><title>Rectangle with Text</title></head>\n";
	input_file << "\n <body><canvas id = \"myCanvas\" width = \"500\" height = \"500\" ></canvas>\n";
	input_file.close();
	ListNavigation(glob);
	ofstream input_file2("data.html", ios::app);
	input_file2 << "\n</body>\n";
	input_file2 << "\n  </html>\n";
	input_file2.close();
}
void insertDot() { // ADD SINGLE DOT TO TREE 

	string name;
	cout << "Enter Name of Dot \nname>> ";
	cin >> name; 
	double x1,y1;
	cout << "Enter Location numbers \nX1>>";
	cin >> x1; 
cout << "Y1>>";
	cin >> y1; 
	//-------------------------------
	Point loc = Point(x1, y1);
	Node* dot = new Node(name, loc);
	glob->addDot(glob, dot);
	//------------------------------
	ofstream openExFile;
	openExFile.open(filepath , ios::app);
	openExFile << "\n" << name << " " << x1 << " " << y1;
}
void deleteList() { // DELETE GLOBAL LIST AND REMOVE EX FILE 
	glob = NULL; 
	ofstream fileForDelete;
	fileForDelete.open(filepath);
	fileForDelete << "";
	fileForDelete.close();

}
void deleteAllDots(Node* root) {// DELETE ALL DOTS IN TREE  ;
	Node* inc = root;
	if (inc) {
		HANDLE col;
		col = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(col, 2);
		cout << "...\n";
		//finding dots
		if (inc->getNW()) {
			if (inc->getNW()->getName() != "") {
				
				SetConsoleTextAttribute(col, 4);
				cout << "dot ";
				SetConsoleTextAttribute(col, 2);
				cout << inc->getNW()->getName();
				SetConsoleTextAttribute(col, 4);
				cout << " have been Deleted ! "<<endl; inc->clearNW();
			}
		}
		if (inc->getNE()) {
			if (inc->getNE()->getName() != "") {
				
				SetConsoleTextAttribute(col, 4);
				cout << "dot ";
				SetConsoleTextAttribute(col, 2);
				cout << inc->getNE()->getName();
				SetConsoleTextAttribute(col, 4);
				cout << " have been Deleted ! "<<endl; inc->clearNE();
			}
		}

		if(inc->getSW()){
			if (inc->getSW()->getName() != "") {
				SetConsoleTextAttribute(col, 4);
				cout << "dot ";
				SetConsoleTextAttribute(col, 2);
				cout << inc->getSW()->getName();
				SetConsoleTextAttribute(col, 4);
				cout << " have been Deleted ! "<<endl;
				inc->clearSW();
			}
		}
		
		if(inc->getSE()){
			if (inc->getSE()->getName() != "") {
				
				SetConsoleTextAttribute(col, 4);
				cout << "dot ";
				SetConsoleTextAttribute(col, 2);
				cout << inc->getSE()->getName();
				SetConsoleTextAttribute(col, 4);
				cout << " have been Deleted ! "<<endl; inc->clearSE();
			}
		}
		//! recrsive 
		if (!inc->isEmptyChild()) {
			deleteAllDots(inc->getNW());
			deleteAllDots(inc->getNE());
			deleteAllDots(inc->getSW());
			deleteAllDots(inc->getSW());
		}
	}
}
int deleteDot(Node* root ,string dotName  ){ // DELETE SINGLE DOT FROM TREE 
	Node* inc = root;
	if (inc) {
		HANDLE col;
		col = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(col,2);
		cout << "...\n";
		if(inc->getNW()->getName() == dotName){
			inc->clearNW();
			SetConsoleTextAttribute(col, 4);
			cout << "selected location have been Cleared  \n"; return 0;
		}
		else if(inc->getNE()->getName() == dotName){
			SetConsoleTextAttribute(col, 4);
			cout << "selected location have been Cleared  \n";
			inc->clearNE();	return 0;
		}
		else if(inc->getSW()->getName() == dotName){
			SetConsoleTextAttribute(col, 4);
			cout << "selected location have been Cleared  \n";
			inc->clearSW();	
			return 0;
			
		}
		else if(inc->getSE()->getName() == dotName){
			inc->clearSE();	
			SetConsoleTextAttribute(col, 4);
			cout << "selected location have been Cleared  \n"; return 0;
				
		}
		if (!inc->isEmptyChild()) {
			deleteDot(inc->getNW(),dotName);
			deleteDot(inc->getNE(), dotName);
			deleteDot(inc->getSW(), dotName);
			deleteDot(inc->getSW(), dotName);
		}
	}
}
int  searchForDot(Node* root, string name) {
	Node* inc = root; 
	if (inc) {
		cout << "...\n";
		if (inc->getName() == name){ 
			
			cout << "item Founded successfully ..."; 
			cout << "\nx = "; 
			cout << inc->getPosX(); 
			cout << "\nY = "; 
			cout << inc->getPosY();
			system("pause");
			exit(0);
		}
		if (!inc->isEmptyChild()) {
			searchForDot(inc->getNW(), name);
			searchForDot(inc->getNE(), name);
			searchForDot(inc->getSW(), name);
			searchForDot(inc->getSE(), name);
	}
	}
}
//!-------------------------------
int main() {
	filler(); // FILL GLOBAL LIST	 
	int menuOption;
	while (true)
	{
		//colorize here 
		menuOption = menu();

		switch (menuOption)
		{
		case 1: { //* print ALL LIST 
			system("cls");
			cout << "Print";

			searchForDot(glob, "A");
			
		
		}
		case 2: { //? Add CONTACT
			
		}
		case 3: { //* print CONTACT INFO
			
		}
		case 4: { //? add PROPERTY
			
		}
		case 5: { //? add DATA
		
		}
		case 6: { //! delete LIST 
			
		}
		case 7: { //! delete ITEM

		
		}
		case 8: { //* print SEARCHED DATA
			
		}
		case 9: { //! EXIT
		}
		default: {
			system("CLs");
			system("COLOR 04");
			cout << "\n\n\t\toption not found ";
			system("pause"); break;
		}
		}
	}
	
}
