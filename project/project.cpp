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



bool aze=false;
bool flag=false;
int frm=0;






void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport (0, 0, w1w/4, w1h);
	face_disp(0,in_query);
	glViewport (w1w/4, 0, w1w/4, w1h);
	face_disp(1, next_pose);
	glViewport (2*w1w/4, 0, w1w/4, w1h);
	face_disp(2,olng_ctrl);
	glViewport (3*w1w/4, 0, w1w/4, w1h);
	face_disp(3, rec_ctrl );
	glutSwapBuffers();
}



int sortdisp(int j, int i)
{
	return display_dist_ind?sortindices[j][i]:i;
}

int sortdisp2(int j, int i)
{
	return display_dist_ind?sortindicesr[j][i]:i;
}


void draw2()
{
	if(!ready)return;
	glClear(GL_COLOR_BUFFER_BIT);


	glBegin(GL_QUADS);

	double x_=(float)1/tsize,y_=(float)1/k;

	double x,y;

	//double * * gd;if(show_truedist)gd=query_c_dist;else gd=weseq;
	double (* gd)[k]=query_c_dist;

	for(int i2=0;i2<k-1;i2++)
	{
		y=1-(float)i2/k;
		for(int j=0;j<tsize-1;j++)
		{
			x=(float)j/tsize;
			//glColor3f(weseq[j][i],weseq[j][i],weseq[j][i]);
			//double dist_from_or=dist(pts[nnseq[j][i]],query_c[j],d);
			int i=sortdisp(j,i2);
			glColor3f(gd[j][i],gd[j][i],gd[j][i]);
			glVertex2f(x,y);if(c_li)glColor3f(gd[j+1][i],gd[j+1][i],gd[j+1][i]);//glColor3f(gd[j+1][i],gd[j+1][i],gd[j+1][i]);
			glVertex2f(x+x_,y);if(c_li)glColor3f(gd[j+1][i+1],gd[j+1][i+1],gd[j+1][i+1]);//glColor3f(gd[j+1][i+1],gd[j+1][i+1],gd[j+1][i+1]);
			glVertex2f(x+x_,y-y_);if(c_li)glColor3f(gd[j][i+1],gd[j][i+1],gd[j][i+1]);//glColor3f(gd[j][i+1],gd[j][i+1],gd[j][i+1]);
			glVertex2f(x,y-y_);

		}
	}

	int in_tpr=(in_t-1+tsize)%tsize;

	int l=length[in_tpr][i_cu];
	int pr_i=i_cu;
	int pr_c=in_tpr;
	for (int i=0;i<l;i++)
	{
		glColor3f(0,1,0);
		x=(float)pr_c/tsize;
		y=1-(float)sortdisp2(pr_c,pr_i)/k-(float)1/k;
		glVertex2f(x,y);
		glVertex2f(x+x_,y);
		glVertex2f(x+x_,y+y_);
		glVertex2f(x,y+y_);
		pr_i=path[pr_c][pr_i];
		pr_c=(pr_c-1+tsize)%tsize;
	}

	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1);
	glBegin(GL_LINES);
	for(int i=0;i<tsize-1;i++)
	{
		glColor3f(0,1,(float)(abs(in_t-i)%tsize)/tsize);
		x=(float)(i+0.5)/tsize;
		y=1-(float)sortdisp2(i,cho[i])/k-(float)1/k;
		glVertex2f(x,y);
		x=(float)(i+0.5+1)/tsize;
		y=1-(float)sortdisp2(i+1,cho[i+1])/k-(float)1/k;
		glVertex2f(x,y);
		glColor3f(1,0.5,(float)(abs(in_t-i)%tsize)/tsize);
		x=(float)(i+0.5)/tsize;
		y=1-(float)sortdisp2(i,l_pos[i])/k-(float)1/k;
		glVertex2f(x,y);
		x=(float)(i+0.5+1)/tsize;
		y=1-(float)sortdisp2(i+1,l_pos[i+1])/k-(float)1/k;
		glVertex2f(x,y);

	}
	glEnd();

	glBegin(GL_LINES);
	for(int i=0;i<N;i++)
	{
		int l=length[in_tpr][partsortind[i]];
		int pr_i=partsortind[i];
		int pr_c=in_tpr;
		if(partsortind[i]==i_cu)
			glColor3f(1,1,0);
		else
			glColor4f(1.0*l/30*5,0,0,1.0*l/50/10*(lines?10:1));
		for (int j=0;j<l;j++)
		{
			x=(float)(pr_c+0.5)/tsize;
			y=1-(float)sortdisp2(pr_c,pr_i)/k-(float)1/k;
			glVertex2f(x,y);
			pr_i=path[pr_c][pr_i];
			pr_c=(pr_c-1+tsize)%tsize;
			if(pr_c>x*tsize||pr_i==-1)
				glVertex2f(x,y);
			else{
				x=(float)(pr_c+0.5)/tsize;
				y=1-(float)sortdisp2(pr_c,pr_i)/k-(float)1/k;
				glVertex2f(x,y);}
		}

	}

	glEnd();

	glBegin(GL_LINES);

	l=length[oldi][oldj];
	pr_i=oldj;
	pr_c=oldi;
	glColor3f(0,1,1);
	for(int i=0;i<l&&pr_i!=-1;i++)
	{

		x=(float)(pr_c+0.5)/tsize;
		y=1-(float)sortdisp2(pr_c,pr_i)/k-(float)1/k;
		glVertex2f(x,y);

		pr_i=path[pr_c][pr_i];
		pr_c=(pr_c-1+tsize)%tsize;
		if(pr_c>x*tsize||pr_i==-1)
			glVertex2f(x,y);
		else{
			x=(float)(pr_c+0.5)/tsize;
			y=1-(float)sortdisp2(pr_c,pr_i)/k-(float)1/k;
			glVertex2f(x,y);}
	}


	glEnd();

	glutSwapBuffers();

}



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
		fx+=val[i]*dist(pts[bestindices[i]],x,d);
	}


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

int dich_b(double x, double * t, int a, int b);//, int i)

int dichotomy(double x, double * t, int i)
{
	int r=0;
	if(x>t[i/2])
		r= dich_b(x,t,i/2+1,i-1);
	else
		r= dich_b(x,t,0,i/2);
	return r;
}

int dich_b(double x, double * t, int a, int b)//, int i)
{
	int r=0;
	if(a>=b)
	{
		r= a;
	}
	else
	{
		if(x>t[(a+b)/2])
			r=dich_b(x,t,(a+b)/2+1,b);
		else
			r=dich_b(x,t,a,(a+b)/2);
	}
	return r;
}

bool comparewe(int a,int b)
{
	return nnseq[in_t][a]>nnseq[in_t][b];
}

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart)/1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart-CounterStart)/PCFreq;
}


ofstream errf;


void minimize()
{
}


// modify input (next_pose) by adding noise
// or disable control points according to the map

void noise_modinput()
{
	for(int i=0;i<d;i++)
	{
		save[i]=next_pose[i];
		double n=(double)rand()/RAND_MAX/8*amount-0.025;//06;///5-0.1250;
		if(randomize_input)
			next_pose[i]=min(max(next_pose[i]+n,0),1);
		next_pose[i]=mapping[i]?next_pose[i]:0;
	}

}

void update()
{

	
	if(!play) return; else if(!aze) play=false;
	if(!ready) return;
	readn(*in,next_pose,d);
	noise_modinput();


	StartCounter(); // Accurate timer to compute performance


	// Kd tree search ( on pts2 )
	kdtree->annkSearch(next_pose,k,nnseq[in_t],weseq[in_t],0);


	// We can compute the real distance between actual corresponding database points and the non changed input save
	for(int i=0;i<k;i++)
		query_c_dist[in_t][i]=dist(pts[nnseq[in_t][i]],save,d);


	// Display kd tree search performance
	cout << "Kd Tree Search : " << GetCounter() << "\n";

	
	// Compute and store the permutation matrix of the neighbors in order to be
	// able to later show them in the visual debugging tool by their
	// frame indices or distance
	for(int i=0;i<k;i++) sortindices[in_t][i]=i;
	sort(sortindices[in_t],sortindices[in_t]+k-1,comparewe);

	for(int i=0;i<k;i++) sortindicesr[in_t][sortindices[in_t][i]]=i;

	
	// Display best nearest neighbor index
	cout << nnseq[in_t][0] << "-\n";


	StartCounter();

	/*
	OLNG update

	*/

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

	// Compute the best frame according to the OLNG

	tmp=100000000;
	int best_choice=0;
	for(int i=0;i<k;i++)
	{
		double c;
		if(path[in_t][i]==current_path) c=pt_cost(cost[in_t][i],length[in_t][i])*coeff_remain;
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


	// Whether to display the
	// console debugging array

	if(show_t)
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

		cout << "Update OLNG : " << GetCounter() << "\n"; //GetTickCount()-teff << "\n";

		for(int i=0;i<d;i++)
			ps[i]=pts[nnseq[in_t][best_choice]][i];

		// we store the different control points to show the blendshapes later
		// in the drawing functions
		for(int i=0;i<d;i++)
		{
			in_query[i]=save[i];
			olng_ctrl[i]=ps[i];
			rec_ctrl[i]=pts[nnseq[in_t][0]][i];
		}


		StartCounter();

		for(int i=0;i<k;i++)
			partsortind[i]=i;//nnseq[i];

		// instead of sorting the array to extract the top elements, we just do a partial search
		// we leave data unchanged but instead create a permutation vector that we use
		// to compute bestindices and bestcosts

		nth_element(partsortind,partsortind+N,partsortind+k,comp);

		for(int i=0;i<N;i++)
		{
			bestindices[i]=nnseq[in_t][partsortind[i]];
			bestcosts[i]=pt_cost(cost[in_t][partsortind[i]],length[in_t][partsortind[i]]);
		}

		/* Energy minimization */

		// local normalized weights
		double wi[N];

		double max=0;

		// Compute the max cost of the paths

		for(int i=0;i<N;i++)
		{
			if(bestcosts[i]>max)
			{
				max=bestcosts[i];
			}
		}

		double sum=0;
		for(int i=0;i<N;i++)
		{
			sum+=max-bestcosts[i];
		}

		if(sum>0.000001)
		{

			for(int i=0;i<N;i++)
			{
				wi[i]=(max-bestcosts[i])/sum;
			}
		}

		// we store the weights to use them in the evaluate
		// part of the minimization func
		for(int i=0;i<N;i++)
			val[i]=wi[i];


		int i, ret = 0;
		lbfgsfloatval_t fx;
		lbfgsfloatval_t *x = lbfgs_malloc(d);
		lbfgs_parameter_t param;


		// The optimization problen is initialized with the best OLNG frame

		for(i=0;i<d;i++)
		{
			x[i]=pts[nnseq[in_t][best_choice]][i];
		}

		lbfgs_parameter_init(&param);


		ret = lbfgs(d, x, &fx, evaluate, progress, NULL, &param);

		cout << ret;
		cout << " return value\n"; // Whether there was an error, can occur smtime

		for(int i=0;i<d;i++)
		{
			rec_ctrl[i]=x[i];
		}

		cout << "Energy minimization : " << GetCounter() << "\n"; //GetTickCount()-teff << "\n";

		l_pos[in_t]=dichotomy(dist(rec_ctrl,save,d),query_c_dist[in_t],k);
		for(int i=0;i<k;i++)
			query_c_dist[in_t][i]=log(1+query_c_dist[in_t][i]);


		cumul1+=ds(pts[nnseq[in_t][0]],save);
		cumul2+=ds(pts[nnseq[in_t][best_choice]],save);
		cumul4+=ds(next_pose,save);

		cout << "Best frame using OLNG :";
		cout << nnseq[in_t][best_choice] << "\n";

		cout << "-----------------------";
		cout << "\n";
		cout << "longueur oln :" << length[in_t][best_choice] << "\n";
		cout << "erreur bruit :" << ds(next_pose,save) << "\n";
		cout << "erreur sur neighbor :" << ds(pts[nnseq[in_t][0]],save) << "\n";
		cout << "erreur sur oln :" << ds(pts[nnseq[in_t][best_choice]],save) << "\n";
		cout << "erreur sur l :" << ds(x,save) << "\n";
		//cout << "longueur oln :" << length[in_t][best_choice] << "\n";
		cout << "erreur cumulée sur bruit :" << cumul4 << "\n";
		cout << "erreur cumulée sur neighbor :" << cumul1 << "\n";
		cout << "erreur cumulée sur oln :" << cumul2 << "\n";
		cout << "erreur cumulée sur l :" << cumul3 << "\n";
		cout << "-----------------------";
		cout << "\n";
		errf << length[in_t][best_choice] << "," << ds(next_pose,save) << "," << ds(pts[nnseq[in_t][0]],save) << "," << ds(pts[nnseq[in_t][best_choice]],save) << "," << ds(x,save) << "\n";
		cumul3+=ds(x,save);


		

		i_cu=best_choice;
		cho[in_t]=i_cu;



		in_t++;
		in_t%=tsize;
		glutSetWindow(w1);
		draw();
		glutSetWindow(w2);
		draw2();



}

void reshape (int w, int h) {
	w1h=h;
	w1w=w;
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (30, (GLfloat)w / (GLfloat)h/4, 1.0, 1000.0)
		;
	glMatrixMode (GL_MODELVIEW);
}

void reshape2 (int w, int h) {


	w2_h=h;
	w2_w=w;


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




void mouse2(int x, int y)
{
	int i=(float)x/w2_w*tsize;
	int j=sortdisp(i,(float)y/w2_h*k);//(float)y/w2_h*k;
	if(i!=oldi||j!=oldj)
	{

		cout << i << " " << j << "\n";
		cout << "index : " << nnseq[i][j] << "\n";
		cout << "poids : " << weseq[i][j] << "\n";
		cout << "longueur : " << length[i][j] << "\n";
		cout << "coût edges : " << cost[i][j] << "\n";
		cout << "coût path : " << pt_cost(cost[i][j],length[i][j]) << "\n";
	}
	oldi=i;
	oldj=j;//oldj=j;
	draw2();
}

void key(unsigned char ch, int x, int y)
{
	if(ch==' ')
	{
		play=true;
	}
	if(ch=='a')
	{
		randomize_input=!randomize_input;
	}
	if(ch=='t')
		show_truedist=!show_truedist;
	if(ch=='y')
		show_t=!show_t;
	if(ch=='d')
		dispindices=!dispindices;
	if(ch=='r')
	{
		file.close();
		file.open(input[in_n],ios::in);
		
		if(!file) printf("Error loading file");
		in=&file;
		in_t=0;
		for(int i=0;i<tsize;i++)
			for(int j=0;j<k;j++)
			{
				cho[i]=0;
				path[i][j]=-1;
				cost[i][j]=0;
				length[i][j]=0;
				nnseq[i][j]=-1;
				weseq[i][j]=0;
				query_c_dist[i][j]=0;
			}

			cumul1=0;
			cumul2=0;
			cumul3=0;

			glutPostWindowRedisplay(w1);
			glutPostWindowRedisplay(w2);

	}
	if(ch=='q')
	{
		amount-=0.1;
		cout << "noise g :"<<amount;
	}
	if(ch=='w')
	{
		amount+=0.1;
		cout << "noise g :"<<amount;
	}
	if(ch=='i')
	{
		in_n++;
		in_n=in_n%2;
		cout << input[in_n] << "\n";
	}
	if(ch=='l')
	{
		c_li=!c_li;
		glutPostWindowRedisplay(w2);
	}
	if(ch=='o')
	{
		display_dist_ind=!display_dist_ind;
		glutPostWindowRedisplay(w2);
	}
	if(ch=='c')
	{
		lines=!lines;
		glutPostWindowRedisplay(w2);
	}
	if(ch=='h')
	{
		cout << "      space - play \n     ";
		cout << " a - turn random error on/off \n     ";
		cout << " q/w - adjust error gain \n     ";
		cout << " l - display bilinear interpolation \n     ";
		cout << " o - switch neighbor sorting \n     ";
		cout << " r - reset and reload input \n     ";
		cout << " i - change input src \n     ";
		cout << " p - continusou play \n     ";
		cout << " c - display top paths \n";
	}
	if(ch=='k')
	{
		// display different blendshapes
		int c;
		//c=rand()*d/RAND_MAX;
		pose_display_c++;
		pose_display_c%=d;
		for(int i=0;i<d;i++)
			olng_ctrl[i]=i==pose_display_c?1:0;
		draw();
	}
	if(ch=='p')
	{
		aze=!aze;
		play=aze;
	}

}

int _tmain(int argc, char * argv[])
{

	errf.open("error.csv");
	errf << "Longueur OLNG,Noise,Error Nearest Neighbor, Error OLN, Minimizatio\n";

	if(lowdim)
		for(int i=0;i<13;i++)
			mapping[indices_lowin[i]-1]=0;

	win.width = 640;
	win.height = 480;
	win.title = "";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;


	//static ifstream file;
	file.open("../data/facerobot3b.txt",ios::in);
	if(!file) printf("Error loading file");
	in=&file;

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //set thealpha buffer
	glutInitWindowSize (1300, 400);//(1000, 500);
	glutInitWindowPosition (100, 100);
	w1=glutCreateWindow ("Computer Graphics Bachelor project demo");
	glutDisplayFunc (draw);
	glutIdleFunc (update);
	glutReshapeFunc (reshape);
	glutMotionFunc(mouse);
	glutKeyboardFunc(key);
	initialize();

	glutInitWindowSize (1300,300);//(400,200);//(200, 100);
	glutInitWindowPosition (100,550);//(1200, 100);
	w2=glutCreateWindow ("Debugging tool");
	glutDisplayFunc (draw2);
	glutReshapeFunc (reshape2);
	glutMotionFunc (mouse2);
	glutKeyboardFunc(key);
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
	pts2=annAllocPts(maxpts,d);
	char * u = new char [200];
	for(int i=1;i<24+1;i++)
	{
		sprintf(u,"../data/faceAnimationDatabase/faceAnimationDatabase/data_%d.txt",i);
		load(u);
	}
	for(int i=25;i<60+1;i++)
	{
		sprintf(u,"../data/faceAnimationDatabase/faceAnimationDatabase/synthetic/data_%d.txt",i);
		load(u);
	}

	kdtree=new ANNkd_tree(pts2,nbpts,d);
	ready=true;

	
	
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
		
		getchar();
		/*delete[] indices;
		delete[] c;*/
		delete[] nnseq;
		delete[] weseq;
		annClose();
		return 0;
}



int _(int i)
{
	return (i+128)%128;
}


// Read next vector
bool readn(istream &in, ANNpoint pt, int dim)
{
	for(int i=0;i<dim;i++)
		if(!(in>>pt[i])) return false;
	if(!lowdim&&removeeye)
	for(int i=0;i<10;i++)
		pt[i]=0;
	return true;
}


// Load (append) a file within the database
bool load(char * file_name)
{
	ifstream file;
	file.open(file_name,ios::in);
	if(!file) printf("Error loading file");
	while(nbpts<maxpts&&readn(file,pts[nbpts],d))
	{
		for(int i=0;i<d;i++)
			pts2[nbpts][i]=mapping[i]?pts[nbpts][i]:0;
		nbpts++;
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