using namespace std;

//int k=1024;
const int k=256;
const int d=39;
//int N=50;//50;
const int N=50;//50;
istream * in;
istream * out;
int nbpts=0, maxpts=d*10000;
ANNpointArray pts;
ANNkd_tree* kdtree;
int tsize = 32;
ANNidxArray * nnseq=new ANNidxArray[128];
ANNdistArray * weseq=new ANNdistArray[128];
//int path[128][k][3];

bool ready=false;


int w1,w2;



bool load(char * filename);
bool disp(ANNpoint p);
bool readn(istream &in, ANNpoint pt, int dim);
/*void fillcost(int i, int j);*/
bool compare(char * name);
float linearcomb(double * p, float * u, int j);

double ps[d];

GLfloat angle = 0.0;

typedef struct {
    int width;
	int height;
	char* title;
 
	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;


int i_cu;
int cho[128];

bool play=false;
bool step=false;
bool show_t=false;