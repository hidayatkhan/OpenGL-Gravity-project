#include<cstdlib>
#include<GL\glut.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif
 
struct Particle {
        float x;
        float y;
        float r;
        float vx;
        float vy;
        float m;
        float color[3];
};

 
void timer(int = 0);
void display();
void mouse(int, int, int, int);
void addParticle(float, float, bool = true, float = 0, float = 0);

 
int Mx, My, wX=1360, wY=700 ;
bool PRESSED_LEFT = false , PRESSED_RIGHT = false;
 
vector<Particle> particles;
 
int main(int argc, char **argv)
{
        Particle p;        
        p.x = 0;
        p.y = 0;
        p.vx = p.vy = 0;
        p.m = 100000;
        p.r = 30;
        p.color[0] = 0.5;
        p.color[1] = 0.3;
        p.color[2] = 0.4;
        particles.push_back(p);
 
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(wX, wY);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("Gravity");
 
        glClearColor(1, 1, 1, 0.3);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-wX/2, wX/2, wY/2 , -wY/2 , 0, 1);
       
        glutDisplayFunc(display);
        glutMouseFunc(mouse);
  
        timer();
       
        glutMainLoop();
		
        return 0;
}
 

void display()
{
        glClear(GL_COLOR_BUFFER_BIT);       
 		
        for(unsigned int i = 0; i < particles.size(); i++)
        {
                Particle &p = particles[i];
                glColor3f(p.color[0], p.color[1], p.color[2]);
                glBegin(GL_POLYGON);
                for(float a = 0; a < 2*M_PI; a+=0.1)
                        glVertex2f(p.r*cos(a) + p.x, p.r*sin(a) + p.y);
                glEnd();       
        }
 
        glFlush();
        glutSwapBuffers();
}
 
void mouse(int button, int state, int x, int y)
{
	
        Mx = x - wX/2;
        My = y - wY/2;
	
 
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
                
				PRESSED_LEFT = true;
		else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
                
				PRESSED_RIGHT = true;      
        

}

 
 
void addParticle(float m, float r, bool randColor, float vx, float vy)
{
        Particle p;
        p.x = Mx;
        p.y = My;
        p.vx = vx;
        p.vy = vy;
        p.m = m;
        p.r = r;
        if(randColor)
        {
                p.color[0] = rand()%200 / 200.0;
                p.color[1] = rand()%200 / 200.0;
                p.color[2] = rand()%200 / 200.0;
        }
      
        particles.push_back(p);
 
   
}
 

 



void timer(int)
{
        display();
        if(PRESSED_LEFT)
        {
                addParticle(10, 3);
                PRESSED_LEFT = false;
        }
	    else if(PRESSED_RIGHT)
        {
                addParticle(1000, 6);
                PRESSED_RIGHT = false;
        }
 
        for(unsigned int i = 0; i < particles.size(); i++)
        {
                Particle &p = particles[i];
                bool not_fall = true;
                for(unsigned int j = 0; j < particles.size(); j++)
                {
                        if(j == i || p.m >= 100000)
                                continue;
 
                        const Particle &p1 = particles[j];
 
                        float d = sqrt((p1.x - p.x)*(p1.x - p.x) + (p1.y - p.y)*(p1.y - p.y));
 
                        if(d > p1.r)
                        {
                               p.vx += (0.03* p1.m / (d*d) ) * (p1.x - p.x)/d;
							  
							   p.vy += (0.03* p1.m / (d*d) ) * (p1.y - p.y)/d;
                        }
                        else
                                not_fall = false;
                }
 
                if(not_fall)   
                {
                        p.x += p.vx;
                        p.y += p.vy;
                }
                else
                        particles.erase(particles.begin()+i);
        }
 
        glutTimerFunc(1, timer, 0);
}