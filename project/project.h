using namespace std;

/* Some of the main parameters
k : number of nearest neighbors
d : dimension of data,
N : refer to the I best paths 
tsize : window size */

//int k=1024;
const int k=256;
const int d=39;
//int N=50;//50;
const int N=50;
const int tsize = 32;

istream * in;
istream * out;

/* 
nbpts number of points in database
pts database used for motion construction
pts2 database used for motion retrieval, computed using the mapping
*/
int nbpts=0, maxpts=100000;
ANNpointArray pts;
ANNpointArray pts2;
ANNkd_tree* kdtree;
ANNidxArray * nnseq=new ANNidxArray[tsize]; // kxt array of nearest neighbor indices
ANNdistArray * weseq=new ANNdistArray[tsize]; // kxt array of nearest neig
ANNpoint next_pose=annAllocPt(d);

int in_t=0; // current frame in time

// in_query : original query input
// olng_ctrl : olng output
// rec_ctrl : motion reconstruction output
double in_query[d], olng_ctrl[d], rec_ctrl[d];
double ps[d]; // temp control points
int path[tsize][k], length[tsize][k]; // path indices and lengths
double  cost[tsize][k]; // cost of the paths at any point (without length influence)
int bestindices[N]; // used for energy minimization, the set of the ending indices for the top paths
double bestcosts[N]; // same as above but with the corresponding path cost
int partsortind[k]; // permutation vector used to rapidly compute the extracted paths
int current_path; // used to store the index of the currently best path of the OLNG
double coeff_remain=1; // should be above one, used to impose/enforce the chose of the same path over time


bool removeeye=false; // remove control points for the eye everywhere, (both databases, input) not set if low dimensional input overrides it

double save[d]; // same as in_query

// used for error omputation
double cumul1=0,cumul2=0,cumul3=0,cumul4;

// will contain the normalized weights for energy optimization
double val[N];

// boolean map allowing to disable some control points
bool mapping[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int indices_lowin[] = {2, 4, 5, 7, 9, 10, 11, 12, 39, 29, 34, 35, 36}; // list of the non independent control points which could be removed
bool lowdim=false; // to activate partial input

bool ready=false;

// used by opengl to identify the windows
int w1,w2;



GLfloat angle = 0.0;

typedef struct {
	int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

// i_cu is the index of the best olng frame, used in the visual debugging tool
int i_cu;
int cho[tsize]; // set of i_cu values in times

// for every neighbor compute the distance of the corresponding fully dimensional neighbor to the actual input
double query_c_dist[tsize][k];
int sortindices[tsize][k]; // when we want to show the indices sorted by the frame indices to better visualize,
							// it is more efficient to compute a permutation matrix updated once every loop that will be
							// used in the draw2() func
int sortindicesr[tsize][k]; // same as above but gives the inverse information, also used by the visual debugger
int l_pos[tsize]; // Where the least square minimized computed frame should be display among other


/* different bool controls, modified using keyboard */
bool play=false; // press space
bool step=false; // wether it is step by step or continuous, press p
bool show_t=false; // show a console mode debugging tool for OLNG with indices
bool show_truedist=false; // not used in final version
bool randomize_input=false;
double amount=1; // random gain. 1: add up to 12.5% error
bool dispindices=false; // showing indices in console when clicking on a path
bool c_li=false; // whether colors are smoothed or not using bilinear interpolation in the debugging window
bool display_dist_ind=false;//either indices sorted or distance to original (no noise) sort
bool lines=false; // still visual debugging tool, whether to show the 50 best paths


int w1h, w1w; // window size of main one

int w2_h,w2_w; // window size of debug tool
int oldi,oldj; // previous old mouse position


static ifstream file;
char input[][100]={"../data/facerobot3b.txt","../data/faceAnimationDatabase/faceAnimationDatabase/synthesized_1.txt"}; // set of different input
int in_n=0;//1; default selected input

int pose_display_c=0;

double dist(double * v1, const double * v2, int co);
bool load(char * filename);
bool disp(ANNpoint p);
bool readn(istream &in, ANNpoint pt, int dim);
float linearcomb(double * p, float * u, int j);







/*void show_str(char* s, int x, int y)
{
	glColor4f(0.0, 1.0, 0.0, 0.0);
	glRasterPos2f(x, y);
	if (s && strlen(s)) {
		while (*s) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *s);
			s++;
		}
	}
}*/