// project.cpp : définit le point d'entrée pour l'application console.
//

#include <windows.h>
#include <GL/gl.h>
#include "GL/glut.h"
#include "stdafx.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include "ANN/ann.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "model_obj.h"
#include "model_view.h"
#include "lbfgs.h"
#include "algorithm"
#include "project.h"


using namespace std;


glutWindow win;




void cube (void) {
    glRotatef(angle, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor4f(1.0, 0.0, 0.0, 0.2); //set the color and alpha of the cube
    glutSolidCube(2);
    glColor4f(0.0, 1.0, 0.0, 0.5); //set the color and alpha of the cube
    glutSolidCube(1);
}



ANNpoint next_pose=annAllocPt(d);
	int in_t=0;
	bool flag=false;

	int frm=0;

	

	

void display (void) {

	frm++;
	if(frm>1&&ready)
	{
	frm=0;
	readn(*in,next_pose,d);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cout << "Query pose :";
		disp(next_pose);
		for(int i=0;i<d;i++)
			ps[i]=next_pose[i];
		face_disp(0, ps);
		cout << "\n";
		kdtree->annkSearch(next_pose,k,nnseq[in_t],weseq[in_t],0);
		for(int i=0;i<d;i++)
			ps[i]=pts[nnseq[in_t][0]][i];
		if(flag)
		{
			/*for(int i=0;i<k;i++)*/
			/*fillcost(in_t, i);*/
		}

		in_t++;
		in_t=in_t%128;
		face_disp(1, ps);
		glutSwapBuffers();
	}

   }



double in_query[d], control_pts[d], ne[d];

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	face_disp(0,in_query);
	face_disp(1,control_pts);
	face_disp(2,ne);
	glutSwapBuffers();
}

int path[128][k], length[128][k];
double  cost[128][k];

	int bestindices[N];

	double bestcosts[N];

	int partsortind[k];

void draw2()
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*glLoadIdentity();
	glViewport ( 0, 0, 200, 100 );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluOrtho2D ( 0.0, 1.0, 0.0, 1.0 );
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );*/
	//glClear ( GL_COLOR_BUFFER_BIT );

	glBegin(GL_QUADS);

	double x_=(float)1/tsize,y_=(float)1/k;

	double x,y;

	for(int i=0;i<k;i++)
	{
		y=1-(float)i/k;
		for(int j=0;j<tsize;j++)
		{
			x=(float)j/tsize;
			glColor3f(weseq[j][i],weseq[j][i],weseq[j][i]);
	glVertex2f(x,y);//tmp=e(i+1,j);glColor3f(1.4*tmp,abs(v[_p(i+1,j)]),smoke[_p(i+1,j)]);
    glVertex2f(x+x_,y);//tmp=e(i+1,j+1);glColor3f(1.4*tmp,abs(v[_p(i+1,j+1)]),smoke[_p(i+1,j+1)]);
    glVertex2f(x+x_,y+y_);//tmp=e(i,j+1);glColor3f(1.4*tmp,abs(v[_p(i,j+1)]),smoke[_p(i,j+1)]);
    glVertex2f(x,y+y_);

		}
	}

	int l=length[in_t][i_cu];
	int pr_i=i_cu;
	int pr_c=in_t;
	for (int i=0;i<l;i++)
	{
		glColor3f(0,1,0);//weseq[in_t][i],0);
		x=(float)pr_c/tsize;
		y=1-(float)pr_i/k-(float)1/k;
	glVertex2f(x,y);//tmp=e(i+1,j);glColor3f(1.4*tmp,abs(v[_p(i+1,j)]),smoke[_p(i+1,j)]);
    glVertex2f(x+x_,y);//tmp=e(i+1,j+1);glColor3f(1.4*tmp,abs(v[_p(i+1,j+1)]),smoke[_p(i+1,j+1)]);
    glVertex2f(x+x_,y+y_);//tmp=e(i,j+1);glColor3f(1.4*tmp,abs(v[_p(i,j+1)]),smoke[_p(i,j+1)]);
    glVertex2f(x,y+y_);
	pr_i=path[pr_c][pr_i];
	pr_c=(pr_c-1+tsize)%tsize;
	}

	glEnd();

	glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

glBegin(GL_LINES);
	for(int i=0;i<tsize-1;i++)
	{
		glColor3f(0,1,(float)(abs(in_t-i)%tsize)/tsize);
		x=(float)i/tsize;
		y=1-(float)cho[i]/k-(float)1/k;
		glVertex2f(x,y);
		x=(float)(i+1)/tsize;
		y=1-(float)cho[i+1]/k-(float)1/k;
		glVertex2f(x,y);
	}
	glEnd();

	glBegin(GL_LINES);
	for(int i=0;i<N;i++)
	{
	int l=length[in_t][partsortind[i]];
	int pr_i=partsortind[i];
	int pr_c=in_t;
	if(partsortind[i]==i_cu)
		glColor3f(1,1,0);//weseq[in_t][i],0);
	else
		glColor3f(1.0*l/30,0,0);//weseq[in_t][i],0);
	for (int j=0;j<l;j++)
	{
		//glColor3f(1,0,0);//weseq[in_t][i],0);
		x=(float)pr_c/tsize;
		y=1-(float)pr_i/k-(float)1/k;
	glVertex2f(x,y);//tmp=e(i+1,j);glColor3f(1.4*tmp,abs(v[_p(i+1,j)]),smoke[_p(i+1,j)]);
	pr_i=path[pr_c][pr_i];
	pr_c=(pr_c-1+tsize)%tsize;
	if(pr_c>x*tsize||pr_i==-1)
		glVertex2f(x,y);
	else{
	x=(float)pr_c/tsize;
		y=1-(float)pr_i/k-(float)1/k;
		glVertex2f(x,y);}
	}

	}


	glEnd();

	glutSwapBuffers();

}

/*int path[128][k], length[128][k];
double  cost[128][k];*/

	/*int bestindices[N];

	double bestcosts[N];*/

	//int partsortind[k];


int prevpos(int current)
{
	int prevpos=(current-1)%tsize;
	if(prevpos<0) prevpos+=tsize;
	//return prevpos;
	return (current-1+tsize)%tsize;
}

bool indexcond(int a, int b) { return b-a<2&&b-a>0; }

double pt_cost(double sum_costs_edges, int length) { return sum_costs_edges/(length*length); }


double ds(double *p)
{
	double e=0;
	for(int i=0;i<d;i++)
		e+=(p[i]-next_pose[i])*(p[i]-next_pose[i]);
	return e;
}

double ds(double *p, double * u)
{
	double e=0;
	for(int i=0;i<d;i++)
		e+=(p[i]-u[i])*(p[i]-u[i]);
	return e;
}


int current_path;


double save[d];

double cumul1,cumul2,cumul3;

inline std::ostream& blue(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
              |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& white(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
       FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
    return s;
}


double val[N];//50];

double dist(double * v1, const double * v2, int co)
{
	double r=0;
	for(int i=0;i<co;i++)
	{
		r+=(v1[i]-v2[i])*(v1[i]-v2[i]);
	}
	return sqrt(r);
}

static lbfgsfloatval_t evaluate(
    void *instance,
    const lbfgsfloatval_t *x,
    lbfgsfloatval_t *g,
    const int n,
    const lbfgsfloatval_t step
    )
{
    int i;
    lbfgsfloatval_t fx = 0.0;

    for (i = 0;i < N;i ++) {
		fx+=val[i]*dist(pts[bestindices[i]],x,d);//sqrt((x[i+1]-x[i]));
			//cout << dist(pts[bestindices[i]],x,d) << "\n";//pts[nnseq[in_t][i]],x,d);
			//cout << annDist(d,pts[bestindices[i]],x);
        /*lbfgsfloatval_t t1 = 1.0 - x[i];
        lbfgsfloatval_t t2 = 10.0 * (x[i+1] - x[i] * x[i]);
        g[i+1] = 20.0 * t2;
        g[i] = -2.0 * (x[i] * g[i+1] + t1);
        fx += t1 * t1 + t2 * t2;*/
    }

	//cout <<fx<<"\n";

	double ithcm=0;


	for (i=0;i<d;i++)
	{
		ithcm=0;
		for(int j=0;j<N;j++)
		{
			ithcm+=val[j]*(x[i]-pts[bestindices[j]][i])/(dist(pts[bestindices[j]],x,d)+0.0000001);
		}
		g[i]=ithcm;
	}

    return fx;
}


static int progress(
    void *instance,
    const lbfgsfloatval_t *x,
    const lbfgsfloatval_t *g,
    const lbfgsfloatval_t fx,
    const lbfgsfloatval_t xnorm,
    const lbfgsfloatval_t gnorm,
    const lbfgsfloatval_t step,
    int n,
    int k,
    int ls
    )
{
    //printf("Iteration %d:\n", k);
    //printf("  fx = %f, x[0] = %f\n", fx, x[0]);
    //printf("  xnorm = %f, gnorm = %f, step = %f\n", xnorm, gnorm, step);
    //printf("\n");
    return 0;
}


bool comp(int a, int b)
{
	return pt_cost(cost[in_t][a],length[in_t][a])<pt_cost(cost[in_t][b],length[in_t][b]);
}

void update()
{
	//display();
	if(!play) return; else play=false;
	if(!ready) return;
	readn(*in,next_pose,d);
	for(int i=0;i<d;i++)
	{
		save[i]=next_pose[i];
	//double n=(double)rand()/RAND_MAX/8-0.125;//06;///5-0.1250;
	//next_pose[i]=next_pose[i]+n;
	double n=(double)rand()/RAND_MAX/4-0.025;//06;///5-0.1250;
	next_pose[i]=min(max(next_pose[i]+n,0),1);
	}

	//cout << "Query pose :";
	//disp(next_pose);
	kdtree->annkSearch(next_pose,k,nnseq[in_t],weseq[in_t],0);
	
	
	//char c=getchar();
	cout << nnseq[in_t][0] << "-\n";


	double tmp;
	int prev_n;
	int prev_col=prevpos(in_t);

	for(int i=0;i<k;i++)
	{
		tmp=1000000000000;
		prev_n=-1;
		for(int j=0;j<k;j++)
		{
			if(nnseq[prev_col][j]!=-1&&indexcond(nnseq[prev_col][j],nnseq[in_t][i]))
			{
				if(pt_cost(cost[prev_col][j]+weseq[in_t][i],length[prev_col][j]+1)<tmp)
				{
					//best choice is neighbor j with index nnsq[prev_col]j
					tmp=pt_cost(cost[prev_col][j]+weseq[in_t][i],length[prev_col][j]+1);
					prev_n=j;
				}
			}
		}
		if(prev_n!=-1)
		{
			path[in_t][i]=prev_n;
			cost[in_t][i]=cost[prev_col][prev_n]+weseq[in_t][i];
			length[in_t][i]=length[prev_col][prev_n]+1; if(length[in_t][i]>tsize) length[in_t][i]=tsize;
		}
		else
		{
			path[in_t][i]=-1;
			cost[in_t][i]=weseq[in_t][i];
			length[in_t][i]=1;
		}
	}

	tmp=100000000;
	int best_choice=0;
	for(int i=0;i<k;i++)
	{
		double c;
		if(path[in_t][i]==current_path) c=pt_cost(cost[in_t][i],length[in_t][i]);///200;///1.4;
		else c=pt_cost(cost[in_t][i],length[in_t][i]);
		//if(length[in_t][i]!=0)
		{
		if(c<tmp)
		{
			tmp=c;
			best_choice=i;
		}
		}
		/*else
		{
			if(cost[in_t][i]<tmp)
			{
				tmp=cost[in_t][i];
				best_choice=i;
			}
		}*/
	}


	if(show_t)//c=='o')
	for(int j=0;j<10;j++)
	{
		for(int i=0;i<tsize;i++)
		{
			if(i==in_t)
				cout <<blue << path[i][j] << " ";
			else
			cout << white << path[i][j] << " ";
		}
		cout << "\n";
	}

	current_path=best_choice;


	for(int i=0;i<d;i++)
			ps[i]=pts[nnseq[in_t][best_choice]][i];
	for(int i=0;i<d;i++)
	{
		in_query[i]=next_pose[i];
		control_pts[i]=ps[i];
		ne[i]=pts[nnseq[in_t][0]][i];
	}

	cout << nnseq[in_t][best_choice] << "\n";

	cout << "erreur sur neighbor :" << ds(pts[nnseq[in_t][0]],save) << "\n";
	cout << "erreur sur oln :" << ds(pts[nnseq[in_t][best_choice]],save) << "\n";
	cout << "longueur oln :" << length[in_t][best_choice] << "\n";
	cout << "erreur cumulée sur neighbor :" << cumul1 << "\n";
	cout << "erreur cumulée sur oln :" << cumul2 << "\n";



	for(int i=0;i<k;i++)
		partsortind[i]=i;//nnseq[i];

	nth_element(partsortind,partsortind+N,partsortind+k,comp);

	for(int i=0;i<N;i++)
	{
		bestindices[i]=nnseq[in_t][partsortind[i]];
		bestcosts[i]=pt_cost(cost[in_t][partsortind[i]],length[in_t][partsortind[i]]);
	}


	//int ni=50;
	double wi[N];//50];

	double max=0;

	for(int i=0;i<N;i++)//50;i++)
	{
		if(bestcosts[i]>max)//pt_cost(cost[in_t][i],length[in_t][i])>max)
		{
			max=bestcosts[i];//pt_cost(cost[in_t][i],length[in_t][i]);
		}
	}

	double sum=0;
	for(int i=0;i<N;i++)//50;i++)
	{
			sum+=max-bestcosts[i];//pt_cost(cost[in_t][i],length[in_t][i]);
	}

	if(sum>0.000001)
	{

	for(int i=0;i<N;i++)//50;i++)
	{
			wi[i]=(max-bestcosts[i])/sum;//pt_cost(cost[in_t][i],length[in_t][i]))/sum;//+=max-pt_cost(cost[in_t][i],length[in_t][i]);
	}


	for(int i=0;i<N;i++)//50;i++)
		val[i]=wi[i];


	cumul1+=ds(pts[nnseq[in_t][0]],save);
	cumul2+=ds(pts[nnseq[in_t][best_choice]],save);

	//int N=d;//50;

	//?

	int i, ret = 0;
    lbfgsfloatval_t fx;
    lbfgsfloatval_t *x = lbfgs_malloc(d);
    lbfgs_parameter_t param;


	/*for (i = 0;i < N;i += 2) {
        x[i] = 0.8;
        x[i+1] = 0.8;
    }*/

	for(i=0;i<d;i++)
	{
		x[i]=pts[nnseq[in_t][best_choice]][i];
	}

    lbfgs_parameter_init(&param);


	//int ret = lbfgs(N, m_x, &fx, _evaluate, NULL, this, NULL);
	ret = lbfgs(d, x, &fx, evaluate, progress, NULL, &param);

	cout << ret;

	for(int i=0;i<d;i++)
	{
		ne[i]=x[i];
	}

	cout << "erreur sur l :" << ds(x,save) << "\n";
	cout << "erreur cumulée sur l :" << cumul3 << "\n";
	cumul3+=ds(x,save);


	}

	i_cu=best_choice;
	cho[in_t]=i_cu;

	

	glutSetWindow(w1);
	draw();
	glutSetWindow(w2);
	draw2();

	in_t++;
	in_t%=tsize;
	/*glutSetWindow(w1);
	draw();
	glutSetWindow(w2);
	draw2();*/
	//glutPostRedisplay();


}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (30, (GLfloat)w / (GLfloat)h, 1.0, 1000.0)
;
    glMatrixMode (GL_MODELVIEW);
}

void reshape2 (int w, int h) {
    /*glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (30, (GLfloat)w / (GLfloat)h, 1.0, 1000.0)
;
    glMatrixMode (GL_MODELVIEW);*/

	glLoadIdentity();
	glViewport ( 0, 0, w, h );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluOrtho2D ( 0.0, 1.0, 0.0, 1.0 );
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
}




void initialize () 
{
    glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat) win.width / win.height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
    glMatrixMode(GL_MODELVIEW);
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.1f, 0.0f, 0.5f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
 
    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
}



void mouse(int x, int y)
{
	ps[0]=(float)x/win.width;
	ps[1]=(float)y/win.width;
}


void key(unsigned char ch, int x, int y)
{
	if(ch==' ')
	{
		play=true;
	}
}

int _tmain(int argc, char * argv[])
{

	win.width = 640;
	win.height = 480;
	win.title = "OpenGL/GLUT OBJ Loader.";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;


	static ifstream file;
	file.open("../data/faceAnimationDatabase/faceAnimationDatabase/bla.txt",ios::in);
	if(!file) printf("Error loading file");
	in=&file;

	glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //set thealpha buffer
    glutInitWindowSize (1000, 500);
    glutInitWindowPosition (100, 100);
    w1=glutCreateWindow ("Computer Graphics");
    glutDisplayFunc (draw);
    glutIdleFunc (update);
    glutReshapeFunc (reshape);
	glutMotionFunc(mouse);
	glutKeyboardFunc(key);
	initialize();

	glutInitWindowSize (200, 100);
    glutInitWindowPosition (1000, 100);
	w2=glutCreateWindow ("Visu");
    glutDisplayFunc (draw2);
	glutReshapeFunc (reshape2);
    glutIdleFunc (update);
	glLoadIdentity();
	glViewport ( 0, 0, 200, 100 );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluOrtho2D ( 0.0, 1.0, 0.0, 1.0 );
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );



	glutSetWindow(w1);

	
	load();

	pts=annAllocPts(maxpts,d);
	char * u = new char [200];
	for(int i=1;i<24+1;i++)
	{
		sprintf(u,"../data/faceAnimationDatabase/faceAnimationDatabase/data_%d.txt",i);
	//load("../data/faceAnimationDatabase/faceAnimationDatabase/data_22.txt");
		load(u);
	}
	for(int i=25;i<60+1;i++)
	{
		sprintf(u,"../data/faceAnimationDatabase/faceAnimationDatabase/synthetic/data_%d.txt",i);
	//load("../data/faceAnimationDatabase/faceAnimationDatabase/data_22.txt");
		load(u);
	}

	kdtree=new ANNkd_tree(pts,nbpts,d);
	ready=true;
	
	cout << -2%3;
	/*indices=new ANNidx[nbpts];
	c=new ANNdist[nbpts];
	ANNpoint q=annAllocPt(d);
	q[0]=2;q[1]=0.5;
	kdtree->annkSearch(q,k,indices,c,0);
	for(int i=0;i<k&&i<10;i++)
	{
		disp(pts[indices[i]]);
		cout << " : " << c[i];
		cout << "\n";
	}*/
	for(int i=0;i<tsize;i++)
	{
		nnseq[i]=new ANNidx[k];
		weseq[i]=new ANNdist[k];
	}

	for(int i=0;i<tsize;i++)
		for(int j=0;j<k;j++)
		{
			path[i][j]=-1;
			cost[i][j]=0;
			length[i][j]=0;
			nnseq[i][j]=-1;
		}
	glutMainLoop ();
	//compare("../data/facerobot3b.txt");
	/*ANNpoint next_pose=annAllocPt(d);
	int in_t=0;
	bool flag=false;
	while(readn(cin,next_pose,d))
	{
		cout << "Query pose :";
		disp(next_pose);
		cout << "\n";
		kdtree->annkSearch(next_pose,k,nnseq[in_t],weseq[in_t],0);
		for(int i=0;i<k&&i<10;i++)
	{
		disp(pts[nnseq[in_t][i]]);
		cout << " : " << weseq[in_t][i];
		cout << "\n";
	}

		for(int j=0;j<10;j++)
		{
			cout << "\n";
		for(int i=0;i<in_t+1;i++)
		{
			cout << nnseq[i][j] << "\t";
		}
		}

		if(flag)
		{
			for(int i=0;i<k;i++)
			fillcost(in_t, i);
		}

		in_t++;
		in_t=in_t%128;
		flag=true;
	}*/
	getchar();
	/*delete[] indices;
	delete[] c;*/
	delete[] nnseq;
	delete[] weseq;
	annClose();
	return 0;
}


int stepsize=2;
int cond=5;

int _(int i)
{
	return (i+128)%128;
}

/*void fillcost(int i, int j)
{
	int j1=j-1,j2=j,j3=j+1;
	int count=0;
	double weight;
	double mweight=LONG_MAX;
	for(int n=0;n<k;n++)
	{
		for(int r=0;r<2;r++)
		{
			if(nnseq[_(i-1-r)][n]<=nnseq[i][j]) // monotonous
				if(nnseq[_(i-1-r)][n]>nnseq[i][j]-5) // boundaries on indices
				{
					//weight=annDist(d,pts[nnseq[_(i-1-r)][n]],pts[nnseq[i][j]]);
					weight=weseq[i][j];
					if(weight<mweight)
					{
						mweight=weight;
						path[i][j][0]=nnseq[_(i-1-r)][n];
					}
				}
		}
		/*
		DTW
		if(nnseq[i-1][i]==nnseq[i][j])
		{
			path[i-1][i][1]=nnseq[i][j];
			count++;
		}
		else if(nnseq[i-1][i]==nnseq[i][j]-1)
		{
			path[i-1][i][2]=nnseq[i][j];
			count++;
		}
		else if(nnseq[i-1][i]==nnseq[i][j]+1)
		{
			path[i-1][i][0]=nnseq[i][j];
			count++;
		}*/
/*
	}*/
	/*if(j1>=0)
		path[i-1][j1][2]=annDist(d,pts[nnseq[i-1][j1]],pts[nnseq[i][j]]);
	path[i-1][j2][1]=annDist(d,pts[nnseq[i-1][j2]],pts[nnseq[i][j]]);
	if(j3<k)
		path[i-1][j3][0]=annDist(d,pts[nnseq[i-1][j3]],pts[nnseq[i][j]]);*/
/*}*/

bool readn(istream &in, ANNpoint pt, int dim)
{
	for(int i=0;i<dim;i++)
		if(!(in>>pt[i])) return false;
	for(int i=0;i<10;i++)
		pt[i]=0;
	return true;
}

bool load(char * file_name)
{
	ifstream file;
	file.open(file_name,ios::in);
	if(!file) printf("Error loading file");
	while(nbpts<maxpts&&readn(file,pts[nbpts],d))
	{
		nbpts++;
	}
	return 1;                          
}

bool compare(char * file_name)
{
	static ifstream file;
	file.open(file_name,ios::in);
	if(!file) printf("Error loading file");
	in=&file;

	ANNpoint next_pose=annAllocPt(d);
	int in_t=0;
	bool flag=false;

	while(readn(*in,next_pose,d))
	{
		cout << "Query pose :";
		disp(next_pose);
		cout << "\n";
		kdtree->annkSearch(next_pose,k,nnseq[in_t],weseq[in_t],0);
		/*for(int i=0;i<k&&i<d;i++)
	{
		disp(pts[nnseq[in_t][i]]);
		cout << " : " << weseq[in_t][i];
		cout << "\n";
	}

		for(int j=0;j<10;j++)
		{
			cout << "\n";
		for(int i=0;i<in_t+1;i++)
		{
			cout << nnseq[i][j] << "\t";
		}
		}*/

		getchar();

		if(flag)
		{
			/*for(int i=0;i<k;i++)*/
			/*fillcost(in_t, i);*/
		}

		in_t++;
		in_t=in_t%128;
		flag=true;
	}


	return 1;                          

}

bool disp(ANNpoint p)
{
	cout << "(";
	for(int i=0;i<d;i++)
		cout << p[i] << ",";
	cout << ")";
	return true;
}