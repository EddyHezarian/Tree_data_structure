#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std; 

//! data Structures---------------
class Point {
private:
	friend class region;
	friend class Node;
	double x;
	double y;
public:	Point() { this->x = 0; this->y = 0; }
	Point(double valueX , double valueY) { this->x = valueX; this->y = valueY; }

	double getX() { return this->x; }
	double getY() { return this->y; }	
	void SetX(double val) {  this->x = val; }
	void SetY(double val) {  this->y = val; }
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
	region(Point a , Point b) { 
		this->TL = a;
		this->BR = b;	}
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
	// Hold details of Region of this node
	region reg;
	// Contains details of node (if Node is a point)
	Point pos;
	string name;
	// Children of this node
	Node* nw, * ne, * sw, * se;
public:
	
	Node(region R ) {
		this->reg = R;
		this->pos.x = NULL;
		this->pos.y = NULL; 
		this->name = "";
		setChilds();
	}	
	Node(Point a, Point b ) {
		region R = region(a, b);
		this->reg = R;
		this->name = "";
		this->pos.x = NULL;
		this->pos.y = NULL;	
	}Node() {
		
		this->reg.BR.x = NULL ;
		this->reg.BR.y = NULL ;
		this->reg.TL.x = NULL ;
		this->reg.TL.y = NULL ;
		this->name = "";
		this->pos.x = NULL;
		this->pos.y = NULL;	
	}
	Node(string N, Point P) {
		this->name = N;
		this->pos = P;
		this->reg.BR = P;
		this->reg.TL = P;
		nw = NULL;
		ne = NULL;
		sw = NULL;
		se = NULL;
	}
	void setChilds() {
		double Xaxle = this->reg.BR.x / 2;
		double Yaxle = this->reg.BR.y / 2;
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
		return this->reg.BR.x / 2;
	}
	double getAxleY() {
		return this->reg.BR.y / 2;
	}
	int addDot(Node* root ,Node* dot){
		Node* inc = root; 
		double Xaxle = (inc->reg.BR.x + inc->reg.TL.x) /2;
		double Yaxle = (inc->reg.BR.y +inc->reg.TL.y )/2;
		if (inc == NULL) {
			root = inc;
			return 1;
		}
		if (inc)
		{
			if (dot->pos.x < Xaxle && dot->pos.y < Yaxle) {
				// area = nw
				if (inc->nw->pos.x == NULL) {
					inc->nw = dot;
				}
				else {
					Node* P = inc->nw;
					inc->nw = new Node(inc->nw->reg);
					addDot(inc->nw, P);
					addDot(inc->nw, dot);
				}
			}
			else if (dot->pos.x > Xaxle && dot->pos.y < Yaxle) {
				// area = ne
				if (inc->ne->pos.x == NULL) {
					inc->ne = dot;
				}
				else {
					Node* P = inc->ne;
					Point Start = Point(Xaxle, 0);
					Point End = Point(inc->reg.BR.x, Yaxle);
					region reg = region(Start, End);
					inc->ne = new Node(reg);
					addDot(inc->ne, P);
					addDot(inc->ne,dot);
				}
			}
			else if (dot->pos.x < Xaxle && dot->pos.y >Yaxle) {
				// area = sw
				if (inc->sw->pos.x == NULL) {
					inc->sw = dot;
				}
				else {
					Node* P = inc->sw;
					inc->sw = new Node(inc->sw->reg);
					addDot(inc->sw, P);
					addDot(inc->sw, dot);
				}
			}
			else if (dot->pos.x > Xaxle && dot->pos.y > Yaxle) {
				// area = se
				if (inc->se->pos.x == NULL) {
					inc->se = dot;
				}
				else {
					Node* P = inc->se;
					inc->se = new Node(inc->se->reg);
					addDot(inc->se, P);
					addDot(inc->se, dot);
				}
			}
		}
	
	
	}


};


//global WAY
Node* glob = new Node();
class Tree {
private:
	friend class Node;
	Node* root;
public:
	Tree() { this->root = NULL; }
	void setRoot( Node* j) { this->root = j;}
	Node* getRoot() {return this->root ; }
};

//! Tools-------------------------
string filpath = "C:\\Users\\Eddy\\Desktop\\ex.txt";
//! features---------------------- 
void filler() {
	// open file 
	ifstream File;
	File.open(filpath);
	string currentLine; 
	Tree tree = Tree();// instance of tree ;
	int line = 1;
	int dotsNumber = 0;
	double a=0, b=0, c=0, d=0;
	string name; double x = 0, y = 0;
	while (File >> currentLine) {
		if (line>=1 and line<=4) {
			 
			if (line == 1) {
				a = stod(currentLine); line++;
			}
			else if (line == 2) {
				b = stod(currentLine); line++;
			}
			else if(line== 3){
				c = stod(currentLine); line++;
			}
			else if(line== 4){
				d = stod(currentLine);
				line++;
				region reg = region(a, b, c, d);
				Node* r = new Node(reg);
				glob = r;
				
			}
		}
		else if (line>4) {
			
			
			if(line == 5){
				name = currentLine; line++;
			}
			else if (line == 6){
				x = stod(currentLine); line++;
			}
			else if(line == 7){
				y = stod(currentLine); line  = 5;
				Point P = Point(x, y);
				Node* dot = new Node(name, P);
				glob->addDot(glob, dot);
			}
		}
		//else
		//{
		//	//creat dot 
		//	Node* dot = DotDetector(currentLine);
		//	tree.getRoot()->addDot(tree.getRoot(), dot);
		//}
	}

}

//!-------------------------------
int main()
{
	 filler();
   



}
