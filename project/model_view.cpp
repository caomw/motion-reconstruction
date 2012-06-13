
#include "model_view.h"

using namespace std;
float * vertices, * normals;

float ** heads_v, ** heads_n;

const int d=39;

ModelOBJ object,heads[d];

// function which loads the blendshapes
// then compute the normals / vertices, and preprocess them to easily display composite
// blendshapes in the draw part

void load()
{
	object.import("../data/blendshapes/blendshapes/Neutral.obj");
	heads[1].import("../data/blendshapes/blendshapes/01_Blink_R_AU45.obj");
	heads[0].import("../data/blendshapes/blendshapes/00_Blink_L_AU45.obj");
	heads[2].import("../data/blendshapes/blendshapes/02_BrowU_CAU1_I.obj");
	heads[3].import("../data/blendshapes/blendshapes/03_BrowU_LAU1_2.obj");
	heads[4].import("../data/blendshapes/blendshapes/04_BrowU_RAU1_2.obj");
	heads[5].import("../data/blendshapes/blendshapes/05_Eyelid_L_Squint.obj");
	heads[6].import("../data/blendshapes/blendshapes/06_Eyelid_R_Squint.obj");
	heads[7].import("../data/blendshapes/blendshapes/07_Frown_L_LAU4.obj");
	heads[8].import("../data/blendshapes/blendshapes/08_Frown_R_RAU4.obj");
	heads[9].import("../data/blendshapes/blendshapes/09_LidT_L_LAU7.obj");
	heads[10].import("../data/blendshapes/blendshapes/10_LidT_R_RAU7.obj");
	heads[11].import("../data/blendshapes/blendshapes/11_CorD_L_LAU15.obj");
	heads[12].import("../data/blendshapes/blendshapes/12_CorD_R_RAU15.obj");
	heads[13].import("../data/blendshapes/blendshapes/13_JawO_CAU2627.obj");
	heads[14].import("../data/blendshapes/blendshapes/14_JawSd_L_LAU30.obj");
	heads[15].import("../data/blendshapes/blendshapes/15_JawSd_R_RAU30.obj");
	heads[16].import("../data/blendshapes/blendshapes/16_JawTh_F_FAU29.obj");
	heads[17].import("../data/blendshapes/blendshapes/17_JawUp_CAU17.obj");
	heads[18].import("../data/blendshapes/blendshapes/18_Kiss_C_CAU18.obj");
	heads[19].import("../data/blendshapes/blendshapes/19_LLipD_CAU16.obj");
	heads[20].import("../data/blendshapes/blendshapes/20_LLipD_L_LAU16.obj");
	heads[21].import("../data/blendshapes/blendshapes/21_LLipD_R_RAU16.obj");
	heads[22].import("../data/blendshapes/blendshapes/22_LipBite.obj");
	heads[23].import("../data/blendshapes/blendshapes/23_LipP_D_DAU24.obj");
	heads[24].import("../data/blendshapes/blendshapes/24_LipP_U_UAU24.obj");
	heads[25].import("../data/blendshapes/blendshapes/25_LipSk_U_UAU28.obj");
	heads[26].import("../data/blendshapes/blendshapes/26_LipSt_L_LAU20.obj");
	heads[27].import("../data/blendshapes/blendshapes/27_LipSt_R_RAU20.obj");
	heads[28].import("../data/blendshapes/blendshapes/32_Smile_R_RAU12.obj");
	heads[29].import("../data/blendshapes/blendshapes/29_OO.obj");
	heads[30].import("../data/blendshapes/blendshapes/30_Puff_CAU34.obj");
	heads[31].import("../data/blendshapes/blendshapes/31_Smile_L_LAU12.obj");
	heads[32].import("../data/blendshapes/blendshapes/32_Smile_R_RAU12.obj");
	heads[33].import("../data/blendshapes/blendshapes/33_Smirk_L_LAU13.obj");
	heads[34].import("../data/blendshapes/blendshapes/34_Smirk_R_RAU13.obj");
	heads[35].import("../data/blendshapes/blendshapes/35_Suck_CAU35.obj");
	heads[36].import("../data/blendshapes/blendshapes/36_ULipU_CAU1025.obj");
	heads[37].import("../data/blendshapes/blendshapes/37_ULipU_L_LAU1025.obj");
	heads[38].import("../data/blendshapes/blendshapes/38_ULipU_R_RAU1025.obj");


	heads_v=new float*[d];
	heads_n=new float*[d];

	const int * v, * o; v=object.getIndexBuffer();

	for(int j=0;j<d;j++)
	{
		heads_v[j]=new float[object.getNumberOfVertices()*3];
		heads_n[j]=new float[object.getNumberOfVertices()*3];
		for(int i=0;i<object.getMesh(0).triangleCount*3;i++)
		{
			heads_v[j][v[i]*3]=heads[j].getVertexBuffer()[v[i]].position[0]-object.getVertexBuffer()[v[i]].position[0];
			heads_v[j][v[i]*3+1]=heads[j].getVertexBuffer()[v[i]].position[1]-object.getVertexBuffer()[v[i]].position[1];
			heads_v[j][v[i]*3+2]=heads[j].getVertexBuffer()[v[i]].position[2]-object.getVertexBuffer()[v[i]].position[2];
			heads_n[j][v[i]*3]=heads[j].getVertexBuffer()[v[i]].normal[0];
			heads_n[j][v[i]*3+1]=heads[j].getVertexBuffer()[v[i]].normal[1];
			heads_n[j][v[i]*3+2]=heads[j].getVertexBuffer()[v[i]].normal[2];
		}
	}


	// Computing the smallset marker pos using blendshape and barycentric coordinates
	// May be used to convert the whole database and input prior to any
	// proces. step

	ifstream file;
	file.open("../data/smallset.fsm",ios::in);
	if(!file) printf("Error loading file");
	bool keep=true;
	int indices_b[3];
	double coeff_b[3];
	while(keep)
	{
		for(int i=0;i<3;i++)
			if(!(file>>indices_b[i])) keep=false;
		for(int i=0;i<3;i++)
			if(!(file>>coeff_b[i])) keep=false;
		double coord[3]={0,0,0};
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				coord[j]+=object.getVertexBuffer()[indices_b[i]].position[j]*coeff_b[i];//object.getIndexBuffer()[indices_b[i]]].position[j]*coeff_b[j];
			}
		}
		cout <<"x: "<<coord[0]<<"y:"<<coord[1]<<"z:"<<coord[2]<<"\n";

	}


	vertices=new float[object.getNumberOfVertices()*3];
	normals=new float[object.getNumberOfVertices()*3];
	const ModelOBJ::Vertex * neutral = object.getVertexBuffer();
	const ModelOBJ::Vertex * head0 = object.getVertexBuffer();
}


// Display the face according to view and the control points ps

void face_disp(int view, double * ps)
{

	int angle = 0;

	glEnable(GL_BLEND); //enable the blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	//gluLookAt( 0,0,300, 0,0,0, 0,1,0);
	/*if(view==0)
	gluLookAt( -200,0,500, 0,0,0, 0,1,0);
	else if(view==1)*/
	gluLookAt( 00,0,-500, 0,0,0, 0,-1,0);
	/*else if(view==2)
	gluLookAt( 200,0,500, 0,0,0, 0,1,0);*/
	//glPushMatrix();
	/*if(view==0)
	glTranslatef(-200,0,0);
	else if(view==1)
	glTranslatef(00,0,0);
	else if(view==2)
	glTranslatef(200,0,0);*/
	glRotatef(angle,0,1,0);

	float color = 10;


	const int * v, * o; v=object.getIndexBuffer();


	// compute the composited
	// face using the ps control points

	glBegin(GL_TRIANGLES);
	for(int i=0;i<object.getMesh(0).triangleCount*3;i++)
	{
		/*vertices[v[i]*3]=object.getVertexBuffer()[v[i]].position[0];
		vertices[v[i]*3+1]=object.getVertexBuffer()[v[i]].position[1];
		vertices[v[i]*3+2]=object.getVertexBuffer()[v[i]].position[2];
		normals[v[i]*3]=object.getVertexBuffer()[v[i]].normal[0];
		normals[v[i]*3+1]=object.getVertexBuffer()[v[i]].normal[1];
		normals[v[i]*3+2]=object.getVertexBuffer()[v[i]].normal[2];*/

		linearcomb(ps,&vertices[v[i]*3],v[i]);
		normals[v[i]*3]=object.getVertexBuffer()[v[i]].normal[0];
		normals[v[i]*3+1]=object.getVertexBuffer()[v[i]].normal[1];
		normals[v[i]*3+2]=object.getVertexBuffer()[v[i]].normal[2];



	}
	glEnd();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0,
		vertices);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0,
		normals);

	glDrawElements(GL_TRIANGLES, object.getMesh(0).triangleCount * 3, GL_UNSIGNED_INT,
		object.getIndexBuffer() + object.getMesh(0).startIndex);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);




	glPopMatrix();


}


// compute the linear combination for a given polygon / triangle index j
// using control points p, buffer being u
float linearcomb(double * p, float * u, int j)
{
	*u=object.getVertexBuffer()[j].position[0];//vertices[j];
	*(u+1)=object.getVertexBuffer()[j].position[1];
	*(u+2)=object.getVertexBuffer()[j].position[2];
	for(int i=0;i<d;i++)
	{
		//*u+=p*h
		*u+=heads_v[i][j*3]*p[i];
		*(u+1)+=heads_v[i][j*3+1]*p[i];
		*(u+2)+=heads_v[i][j*3+2]*p[i];
	}
	return 1;
}