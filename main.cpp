#include "GL/freeglut.h"
#include "GL/gl.h"
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
class Turtle{
	double x;
	double y;
	double a;
	double d;
	double delta;
	
	void forward(){
		this->x = this->x + this->d * cos(this->a);
		this->y = this->y + this->d * sin(this->a);
	}

	public:
	// Delta in degrees
	Turtle(const double &d=1, const double &delta=90, const double x = 0, const double y = 0):x(x), y(y), a(M_PI/2), d(d), delta(delta*M_PI/180){};
	void f(){
		this->forward();
	}
	void F(){
        glVertex2f(this->x, this->y);
		this->forward();
        glVertex2f(this->x, this->y);
	}
	void plus(){
		this->a+=this->delta;
	}
	void minus(){
		this->a-=this->delta;
	}
};

class TurtleInterpreter{
public:
	std::vector<Turtle> ts;
	TurtleInterpreter(const Turtle &t){
		ts.push_back(t);
	};

	void exec(const std::string &s){
		for(unsigned int i = 0; i<s.length();i++){
			switch(s[i]){
				case 'f':
				ts.back().f();
				break;
				case 'F':
				ts.back().F();
				break;
				case '+':
				ts.back().plus();
				break;
				case '-':
				ts.back().minus();
				break;
				case '[':
				ts.push_back(ts.back());
				break;
				case ']':
				ts.pop_back();
				if(ts.size() == 0){
					std::cerr<<"TurtleInterpreter - more pops than pushes."<<std::endl;
					exit(1);	
				}
				break;
				//default:
				//std::cerr<<"TurtleInterpreter unknown char '"<<s[i]<<"'."<<std::endl;
				//exit(1);
			}

		}
	}

};

void expandExp(const char &init, const std::map<char, std::string> &rules, int n, std::string &out){
	std::vector<char> prev, curr;

	prev.push_back(init);

	for(unsigned int i = 0; i<n;i++){
		for(const char &c:prev){
			if(rules.find(c) != rules.end()){
				for(const char &r:rules.find(c)->second)
					curr.push_back(r);
			}else{
				curr.push_back(c);
			}
		}
		prev = curr;
		curr = std::vector<char>();
	}
	out = std::string(prev.size(), ' ');
		for(unsigned int i = 0; i<prev.size();i++){
			out[i]=prev[i];
		}
}


/* display function - code from:
     http://fly.cc.fer.hr/~unreal/theredbook/chapter01.html
This is the actual usage of the OpenGL library. 
The following code is the same for any platform */
void renderFunction()
{
	//Turtle t(.02, 22.5);
	//Turtle t(.02, 20);
	Turtle t(.0035, 20);
	TurtleInterpreter ti(t);

	std::string out;
	//expandExp('F', {{'F',"FF-[-F+F+F]+[+F-F-F]"}}, 4, out);
	//expandExp('F', {{'F',"F[+F]F[-F][F]"}}, 5, out);
	expandExp('X', {{'F',"FF"},{'X',"F[+X]F[-X]+X"}}, 7, out);
	std::cout<<out<<std::endl;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_LINES);
    	ti.exec(out);

    glEnd();
    glFlush();
}

/* Main method - main entry point of application
the freeglut library does the window creation work for us, 
regardless of the platform. */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1200,800);
    glutInitWindowPosition(100,100);
    glutCreateWindow("OpenGL - First window demo");
    glutDisplayFunc(renderFunction);
    glutMainLoop();    
    return 0;
}
