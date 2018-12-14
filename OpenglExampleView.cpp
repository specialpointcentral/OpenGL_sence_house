
// OpenglExampleView.cpp : COpenglExampleView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenglExample.h"
#endif


#include "OpenglExampleDoc.h"
#include "OpenglExampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>
using namespace std;
// COpenglExampleView

IMPLEMENT_DYNCREATE(COpenglExampleView, CView)

BEGIN_MESSAGE_MAP(COpenglExampleView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_ROTATE, &COpenglExampleView::OnRotate)
	ON_WM_TIMER()
	ON_COMMAND(ID_Stop, &COpenglExampleView::OnStop)
	ON_COMMAND(ID_ROTATE_X, &COpenglExampleView::OnRotateX)
	ON_COMMAND(ID_ROTATE_Z, &COpenglExampleView::OnRotateZ)
	ON_COMMAND(ID_32780, &COpenglExampleView::On32780)
END_MESSAGE_MAP()
#define _USE_MATH_DEFINES

double rotate_x = 25.0;
double rotate_y = -30.0;
# define  white		1.0,			1.0,			1.0   // 对颜色的宏定义
# define  green		0.0,			0.502,			0.0
# define  red		1.0,			0.0,			0.0
# define  gray		190.0/255.0,	190.0/255.0,	190.0/255.0
# define  hgray		0.117,			0.180,			0.227
# define  blue		0.0,			0.0,			1.0
# define  gold		1.0,			215.0/255.0,	0.0
# define  hgreen	0.0,			100.0/255.0,	0.0
# define  brown		210.0/255.0,	105.0/255.0,	30.0/255.0
# define  men		244.0/255.0,	164.0/255.0,	96.0/255.0
# define  menba		139.0/255.0,	69.0/255.0,		19.0/255.0
# define  wall		220.0/255.0,	220.0/255.0,	220.0/255.0
# define  yello		220.0/255.0,	156.0/255.0,	83.0/255.0


// COpenglExampleView 构造/析构

COpenglExampleView::COpenglExampleView()
{
	// TODO:  在此处添加构造代码
	m_angle = 0;
	m_rotate_axis = NOROTATE;
}

COpenglExampleView::~COpenglExampleView()
{
}

BOOL COpenglExampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COpenglExampleView 绘制

void COpenglExampleView::OnDraw(CDC* /*pDC*/)
{
	COpenglExampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//wglMakeCurrent(m_hDC, m_hRC);
	RenderScene();
	SwapBuffers(m_hDC);
}

void COpenglExampleView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COpenglExampleView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COpenglExampleView 诊断

#ifdef _DEBUG
void COpenglExampleView::AssertValid() const
{
	CView::AssertValid();
}

void COpenglExampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenglExampleDoc* COpenglExampleView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenglExampleDoc)));
	return (COpenglExampleDoc*)m_pDocument;
}
#endif //_DEBUG

void COpenglExampleView::draw_cylinder(GLfloat bottomx, GLfloat bottomy, GLfloat  bottomz,
	GLfloat   r, GLfloat height)
{
	vector<double>  zlist;
	vector<double>  xlist;
	const double PI = 3.14159;

	glPushMatrix();

	glTranslatef(bottomx, bottomy, bottomz);
	//double height = 2; // height of the cylinder
	//double r = 1;   // radius of the cylinder
	int  count = 34;  // segmentation number 

	glLineWidth(4.0f);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);
	glEnd();

	// compute points on circle
	for (int i = 0; i < count; i++)
	{
		double a = i * 2 * PI / count;
		zlist.push_back(r * cos(a));
		xlist.push_back(r * sin(a));
	}

	// draw side faces of cylinder
	for (int i = 0; i < count; i++)
	{
		// compute normal 
		double nx = xlist[i] * 0.5 + xlist[(i + 1) % count] * 0.5;
		double ny = 0;
		double nz = zlist[i] * 0.5 + zlist[(i + 1) % count] * 0.5;
		double nlen = sqrt(nx * nx + ny * ny + nz * nz);
		nx /= nlen;
		ny /= nlen;
		nz /= nlen;
		// draw a polygon 
		glBegin(GL_POLYGON);
		glNormal3f(nx, ny, nz);
		glVertex3f(xlist[i], height, zlist[i]);
		glVertex3f(xlist[i], 0, zlist[i]);
		glVertex3f(xlist[(i + 1) % count], 0, zlist[(i + 1) % count]);
		glVertex3f(xlist[(i + 1) % count], height, zlist[(i + 1) % count]);
		glEnd();

	}

	// draw bottom face of cylinder
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0.6);
	glNormal3f(0, -1, 0);
	for (int i = 0; i < count; i++)
	{
		glVertex3f(xlist[i], 0, zlist[i]);
	}
	glEnd();


	// draw top face of cylinder
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0.6);
	glNormal3f(0, 1, 0);
	for (int i = 0; i < count; i++)
	{
		glVertex3f(xlist[i], height, zlist[i]);
	}
	glEnd();


	glPopMatrix();
}

void COpenglExampleView::draw_sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r)
{
	GLUquadricObj *m_pObj;    //保存绘图模式、法线模式、法线朝向、纹理等信息 
	//创建二次方程状态对象 ，并初始化二次方程状态 
	m_pObj = gluNewQuadric();
	gluQuadricDrawStyle(m_pObj, GLU_FILL);
	glPushMatrix();
	glTranslatef(x, y, z);
	gluSphere(m_pObj, r, 32, 32);
	glPopMatrix();
	gluDeleteQuadric(m_pObj);
}

void COpenglExampleView::draw_rectangle(double x, double y, double z, double x1, double y1, double z1)
{
	double dump_p[8][3];			// 定义长方体的8个顶点

	dump_p[0][0] = x;
	dump_p[0][1] = y;
	dump_p[0][2] = z;				// 第0个点

	dump_p[1][0] = x;
	dump_p[1][1] = y;
	dump_p[1][2] = z + z1;			// 第1个点

	dump_p[2][0] = x + x1;
	dump_p[2][1] = y;
	dump_p[2][2] = z + z1;			// 第2个点

	dump_p[3][0] = x + x1;
	dump_p[3][1] = y;
	dump_p[3][2] = z;				// 第3个点
	for (int i = 0; i < 4; i++) {	// 其余的四个点
		for (int j = 0; j < 3; j++) {
			if (j == 1)
				dump_p[i + 4][j] = dump_p[i][j] + y1;
			else
				dump_p[i + 4][j] = dump_p[i][j];
		}
	}

	// 开始绘制
	glBegin(GL_POLYGON);
	//glColor3f(red);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(dump_p[0][0], dump_p[0][1], dump_p[0][2]);
	glVertex3f(dump_p[1][0], dump_p[1][1], dump_p[1][2]);
	glVertex3f(dump_p[2][0], dump_p[2][1], dump_p[2][2]);
	glVertex3f(dump_p[3][0], dump_p[3][1], dump_p[3][2]);
	glEnd();   // 下底

	glBegin(GL_POLYGON);
	//glColor3f(green);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(dump_p[1][0], dump_p[1][1], dump_p[1][2]);
	glVertex3f(dump_p[0][0], dump_p[0][1], dump_p[0][2]);
	glVertex3f(dump_p[4][0], dump_p[4][1], dump_p[4][2]);
	glVertex3f(dump_p[5][0], dump_p[5][1], dump_p[5][2]);
	glEnd();   // 左面

	glBegin(GL_POLYGON);
	//glColor3f(green);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(dump_p[7][0], dump_p[7][1], dump_p[7][2]);
	glVertex3f(dump_p[6][0], dump_p[6][1], dump_p[6][2]);
	glVertex3f(dump_p[2][0], dump_p[2][1], dump_p[2][2]);
	glVertex3f(dump_p[3][0], dump_p[3][1], dump_p[3][2]);
	glEnd();   // 右面

	glBegin(GL_POLYGON);
	//glColor3f(green);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(dump_p[5][0], dump_p[5][1], dump_p[5][2]);
	glVertex3f(dump_p[6][0], dump_p[6][1], dump_p[6][2]);
	glVertex3f(dump_p[2][0], dump_p[2][1], dump_p[2][2]);
	glVertex3f(dump_p[1][0], dump_p[1][1], dump_p[1][2]);
	glEnd();   // 前面

	glBegin(GL_POLYGON);
	//glColor3f(green);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(dump_p[0][0], dump_p[0][1], dump_p[0][2]);
	glVertex3f(dump_p[3][0], dump_p[3][1], dump_p[3][2]);
	glVertex3f(dump_p[7][0], dump_p[7][1], dump_p[7][2]);
	glVertex3f(dump_p[4][0], dump_p[4][1], dump_p[4][2]);
	glEnd();   // 后面

	glBegin(GL_POLYGON);
	//glColor3f(red);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(dump_p[4][0], dump_p[4][1], dump_p[4][2]);
	glVertex3f(dump_p[7][0], dump_p[7][1], dump_p[7][2]);
	glVertex3f(dump_p[6][0], dump_p[6][1], dump_p[6][2]);
	glVertex3f(dump_p[5][0], dump_p[5][1], dump_p[5][2]);
	glEnd();   // 上面
}

void COpenglExampleView::draw_bezier(GLfloat bottomx, GLfloat bottomy, GLfloat bottomz)
{
	vector<double>  zlist;
	vector<double>  xlist;
	const double PI = 3.14159;

	GLfloat ctrlpoints[4][4][3] = {
		{ { 50,160,50 },{ 183,160,50 },{ 317,160,50 },{ 450,160,50 } },
		{ { 50,160,116 },{ 183,280,116 },{ 317,280,116 },{ 450,160,116 } },
		{ { 50,160,184 },{ 183,280,184 },{ 317,280,184 },{ 450,160,184 } },
		{ { 50,160,250 },{ 183,160,250 },{ 317,160,250 },{ 450,160,250 } }
	};


	//init
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glColor3f(red);

	int i, j;
	glPushMatrix();
	glTranslatef(bottomx, bottomy, bottomz);

	for (j = 0; j <= 200; j++)
	{
		glBegin(GL_LINE_STRIP);
		for (i = 0; i <= 200; i++)
			glEvalCoord2f((GLfloat)i / 200.0, (GLfloat)j / 200.0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (i = 0; i <= 200; i++)
			glEvalCoord2f((GLfloat)j / 200.0, (GLfloat)i / 200.0);
		glEnd();
	}
	glPopMatrix();
	glFlush();
}

void COpenglExampleView::displaySence(void)
{
	glPushMatrix();

	glTranslatef(-2, 0, -2);
	glScalef(0.009, 0.009, 0.009);

	// 地面底板（颜色为 绿色）
	glColor3f(green);
	draw_rectangle(0, 0, 0, 500, 10, 500);
	// 小路（颜色  灰色）
	glColor3f(gray);
	draw_rectangle(0, 1, 500, 500, 10, 100);
	// 车库底板（颜色  灰色）
	glColor3f(gray);
	draw_rectangle(330, 1, 250, 140, 10, 250);
	// 黄实线（颜色为 白色）
	for (int i = 0; i < 5; i++) {
		glColor3f(yello);
		draw_rectangle(i * 100, 9, 545, 40, 3, 10);
	}
	// 车库线
	draw_rectangle(390, 9, 260, 10, 3, 40);
	draw_rectangle(390, 9, 360, 10, 3, 40);
	draw_rectangle(390, 9, 460, 10, 3, 40);
	// 栅栏基座（颜色为 灰白色）
	glColor3f(wall);
	draw_rectangle(10, 1, 10, 20, 30, 480);				// 对应于A点
	draw_rectangle(30, 1, 10, 440, 30, 20);				// 对应于B点
	draw_rectangle(470, 1, 10, 20, 30, 480);			// 对应于C点
	draw_rectangle(30, 1, 470, 100, 30, 20);			// 对应于D点
	draw_rectangle(230, 1, 470, 100, 30, 20);			// 对应于F点
	// 栅栏柱子（颜色为 灰白色）
	glColor3f(wall);
	draw_rectangle(10, 1, 10, 20, 80, 20);				// 对应于A点
	draw_rectangle(470, 1, 10, 20, 80, 20);				// 对应于C点
	draw_rectangle(230, 1, 470, 20, 80, 20);			// 对应于F点
	draw_rectangle(10, 1, 470, 20, 80, 20);				// 对应于E点
	draw_rectangle(470, 1, 470, 20, 80, 20);			// 对应于G点
	draw_rectangle(310, 1, 470, 20, 80, 20);			// 对应于H点
	draw_rectangle(110, 1, 470, 20, 80, 20);			// 对应于I点
	// 栅栏（颜色为 深灰色）
	glColor3f(hgray);
	for (int i = 35; i < 470; i += 15)
	{
		draw_rectangle(15, 20, i, 5, 50, 5);			// 左边竖条
	}
	draw_rectangle(16, 50, 25, 3, 4, 450);
	for (int i = 35; i < 470; i += 15)
	{
		draw_rectangle(475, 20, i, 5, 50, 5);			// 右边竖条
	}
	draw_rectangle(477, 50, 25, 3, 4, 450);
	for (int i = 35; i < 470; i += 15)
	{
		draw_rectangle(i, 20, 15, 5, 50, 5);			// 上边横条
	}
	draw_rectangle(25, 50, 16, 450, 4, 3);
	for (int i = 35; i < 110; i += 15)
	{
		draw_rectangle(i, 20, 475, 5, 50, 5);			// 下边横条1
	}
	draw_rectangle(25, 50, 477, 90, 4, 3);
	for (int i = 255; i < 310; i += 15)
	{
		draw_rectangle(i, 20, 475, 5, 50, 5);			// 下边横条2
	}
	draw_rectangle(245, 50, 477, 70, 4, 3);

	// 车库墙（颜色为 灰白色）
	glColor3f(wall);
	draw_rectangle(320, 1, 250, 10, 110, 220);
	draw_rectangle(460, 1, 250, 10, 110, 220);
	draw_rectangle(330, 1, 245, 130, 110, 10);
	// 车库顶棚（颜色为 深灰色）
	glColor3f(hgray);
	draw_rectangle(320, 110, 250, 150, 10, 220);
	// 卧室和客厅的墙壁（颜色为 灰白色）
	glColor3f(wall);
	draw_rectangle(50, 1, 50, 12, 160, 200);			// 左墙壁
	draw_rectangle(438, 1, 50, 12, 160, 200);			// 右墙壁
	draw_rectangle(62, 1, 50, 376, 160, 12);			// 后墙壁
	draw_rectangle(62, 1, 238, 376, 160, 12);			// 前墙壁

	// 卧室和客厅的玻璃（颜色为 蓝色）
	glColor3f(blue);
	draw_rectangle(112, 60, 237, 50, 50, 14);
	// 房顶 （颜色 红色）
	glColor3f(red);
	draw_rectangle(50, 160, 50, 400, 2, 200);
	draw_bezier(0,1,0);
	// 门
	glColor3f(men);
	draw_rectangle(230, 1, 250, 50, 100, 4);			// 门面
	glColor3f(menba);
	draw_rectangle(235, 60, 253, 5, 5, 4);              // 门把手
	// 小路
	for (int i = 0; i < 6; i++)
	{
		glColor3f(brown);
		draw_rectangle(235 - i * 15, 10, 250 + i * 40, 40, 2, 20);
	}
	// 太阳
	glColor3f(gold);
	draw_sphere(350, 300, 0, 25.0);
	// 树干
	glColor3f(brown);
	draw_cylinder(60, 2, 390, 5, 70);
	// 树叶
	glColor3f(hgreen);
	draw_sphere(60, 80, 390, 25.0);  //球
	glPopMatrix();
}

void  COpenglExampleView::draw_cube()
{
	glBegin(GL_QUADS);
	// back
	glNormal3f(0, 0, -1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 0, 0);

	// bottom
	glNormal3f(0, -1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 0, 1);
	glVertex3f(0, 0, 1);

	//right
	glNormal3f(1, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 0, 1);

	//left
	glNormal3f(-1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glVertex3f(0, 1, 1);
	glVertex3f(0, 1, 0);

	//top
	glNormal3f(0, 1, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, 0);


	//front
	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);

	glEnd();

	//glPopMatrix();
}
// COpenglExampleView 消息处理程序
void COpenglExampleView::RenderScene()
{
	// clear color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	//draw_rectangle(1, 1, 1, 2, 2, 2);
	displaySence();
	//glColor3f(gray);
	//draw_cube();
	//draw_sphere(1,1,1,0.5);

	glFlush();
}
void COpenglExampleView::GLInit()
{
	m_hDC = ::GetDC(m_hWnd);
	static PIXELFORMATDESCRIPTOR  pfdWnd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,
		0,0,0,0,0,0,0,
		32,
		0,0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	int pixelformat;
	pixelformat = ChoosePixelFormat(m_hDC, &pfdWnd);
	SetPixelFormat(m_hDC, pixelformat, &pfdWnd);

	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);

	GLSetupRC(m_hDC);
}
void COpenglExampleView::GLRelease()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd, m_hDC);
}


// material
//GLfloat material_diffuse[] = {0.75, 0.6  ,0.22,0.5};
//GLfloat material_specular[] = { 0.9, 0.9, 0.9, 0.5};
//GLfloat material_ambient[] = {0.24, 0.2, 0.07, 1.0};
//GLfloat	emission[] = { 0.3f, 0.3f, 0.3f, 1.0f };
//glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
//glMaterialfv(GL_FRONT, GL_DIFFUSE,   material_diffuse);
//glMaterialfv(GL_FRONT, GL_AMBIENT,  material_ambient);
//glMaterialfv(GL_FRONT, GL_EMISSION, emission);
//glMateriali(GL_FRONT, GL_SHININESS, 30);


void COpenglExampleView::GLSetupRC(HDC hdc)
{
	//lighting
	GLfloat   ambientLight[] = { 0.008f, 0.008f, 0.008f, 1.0f };
	GLfloat   diffuseLight[] = { 0.008f, 0.008f, 0.008f, 1.0f };
	GLfloat   specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat   lightPos[] = { -1.0f, -1.0f, 10.0f, 0.0f };
	GLfloat	  Light_Model_Ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f }; //环境光参数
	// light 0
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);  //环境光参数
	glEnable(GL_LIGHT0);
	// color material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	// backgroud colr
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// color
	glColor3f(1, 0, 0);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

///*设置特殊效果*/
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
//glEnable(GL_BLEND);

// change material
void COpenglExampleView::ChangeMaterial(CString &string,
	bool update)
{
	float ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emission[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float shininess[] = { 0.0f };

	if (string == "Silver")
	{
		// Ambient
		ambient[0] = 0.19225f;
		ambient[1] = 0.19225f;
		ambient[2] = 0.19225f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.50754f;
		diffuse[1] = 0.50754f;
		diffuse[2] = 0.50754f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.508273f;
		specular[1] = 0.508273f;
		specular[2] = 0.508273f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 51.2f;
	}

	else if (string == "Gold")
	{
		// Ambient
		ambient[0] = 0.24725f;
		ambient[1] = 0.1995f;
		ambient[2] = 0.0745f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.75164f;
		diffuse[1] = 0.60648f;
		diffuse[2] = 0.22648f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.928281f;
		specular[1] = 0.855802f;
		specular[2] = 0.666065f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 51.2f;
	}

	else if (string == "Jade")
	{
		// Ambient
		ambient[0] = 0.135f;
		ambient[1] = 0.2225f;
		ambient[2] = 0.1575f;
		ambient[3] = 0.95f;
		// Diffuse
		diffuse[0] = 0.54f;
		diffuse[1] = 0.89f;
		diffuse[2] = 0.63f;
		diffuse[3] = 0.95f;
		// Specular
		specular[0] = 0.316228f;
		specular[1] = 0.316228f;
		specular[2] = 0.316228f;
		specular[3] = 0.95f;
		// Shininess
		shininess[0] = 12.8f;
	}

	else if (string == "Light blue")
	{
		// Ambient
		ambient[0] = 0.0f;
		ambient[1] = 0.5f;
		ambient[2] = 0.75f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.0f;
		diffuse[1] = 0.5f;
		diffuse[2] = 1.0f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.75f;
		specular[1] = 0.75f;
		specular[2] = 0.75f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 64.0f;
	}

	else if (string == "Emerald")
	{
		// Ambient
		ambient[0] = 0.0215f;
		ambient[1] = 0.1745f;
		ambient[2] = 0.0215f;
		ambient[3] = 0.55f;
		// Diffuse
		diffuse[0] = 0.07568f;
		diffuse[1] = 0.61424f;
		diffuse[2] = 0.07568f;
		diffuse[3] = 0.55f;
		// Specular
		specular[0] = 0.633f;
		specular[1] = 0.727811f;
		specular[2] = 0.633f;
		specular[3] = 0.55f;
		// Shininess
		shininess[0] = 76.8f;
	}

	else if (string == "Polished silver")
	{
		// Ambient
		ambient[0] = 0.23125f;
		ambient[1] = 0.23125f;
		ambient[2] = 0.23125f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.2775f;
		diffuse[1] = 0.2775f;
		diffuse[2] = 0.2775f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.773911f;
		specular[1] = 0.773911f;
		specular[2] = 0.773911f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 89.6f;
	}

	else if (string == "Chrome")
	{
		// Ambient
		ambient[0] = 0.25f;
		ambient[1] = 0.25f;
		ambient[2] = 0.25f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.4f;
		diffuse[1] = 0.4f;
		diffuse[2] = 0.4f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.774597f;
		specular[1] = 0.774597f;
		specular[2] = 0.774597f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 76.8f;
	}

	else if (string == "Copper")
	{
		// Ambient
		ambient[0] = 0.19125f;
		ambient[1] = 0.0735f;
		ambient[2] = 0.0225f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.7038f;
		diffuse[1] = 0.27048f;
		diffuse[2] = 0.0828f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.256777f;
		specular[1] = 0.137622f;
		specular[2] = 0.086014f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 12.8f;
	}

	else if (string == "Polished gold")
	{
		// Ambient
		ambient[0] = 0.24725f;
		ambient[1] = 0.2245f;
		ambient[2] = 0.0645f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.34615f;
		diffuse[1] = 0.3143f;
		diffuse[2] = 0.0903f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.797357f;
		specular[1] = 0.723991f;
		specular[2] = 0.208006f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 83.2f;
	}

	else if (string == "Pewter")
	{
		// Ambient
		ambient[0] = 0.105882f;
		ambient[1] = 0.058824f;
		ambient[2] = 0.113725f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.427451f;
		diffuse[1] = 0.470588f;
		diffuse[2] = 0.541176f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.333333f;
		specular[1] = 0.333333f;
		specular[2] = 0.521569f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 9.84615f;
	}

	else if (string == "Obsidian")
	{
		// Ambient
		ambient[0] = 0.05375f;
		ambient[1] = 0.05f;
		ambient[2] = 0.06625f;
		ambient[3] = 0.82f;
		// Diffuse
		diffuse[0] = 0.18275f;
		diffuse[1] = 0.17f;
		diffuse[2] = 0.22525f;
		diffuse[3] = 0.82f;
		// Specular
		specular[0] = 0.332741f;
		specular[1] = 0.328634f;
		specular[2] = 0.346435f;
		specular[3] = 0.82f;
		// Shininess
		shininess[0] = 38.4f;
	}

	else if (string == "Black plastic")
	{
		// Ambient
		ambient[0] = 0.0f;
		ambient[1] = 0.0f;
		ambient[2] = 0.0f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.01f;
		diffuse[1] = 0.01f;
		diffuse[2] = 0.01f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.5f;
		specular[1] = 0.5f;
		specular[2] = 0.5f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 32.0f;
	}

	else if (string == "Polished bronze")
	{
		// Ambient
		ambient[0] = 0.25f;
		ambient[1] = 0.148f;
		ambient[2] = 0.006475f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.4f;
		diffuse[1] = 0.2368f;
		diffuse[2] = 0.1036f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.774597f;
		specular[1] = 0.458561f;
		specular[2] = 0.200621f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 76.8f;
	}


	else if (string == "Polished copper")
	{
		// Ambient
		ambient[0] = 0.2295f;
		ambient[1] = 0.08825f;
		ambient[2] = 0.0275f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.5508f;
		diffuse[1] = 0.2118f;
		diffuse[2] = 0.066f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.580594f;
		specular[1] = 0.223257f;
		specular[2] = 0.0695701f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 51.2f;
	}

	else if (string == "Pearl")
	{
		// Ambient
		ambient[0] = 0.25f;
		ambient[1] = 0.20725f;
		ambient[2] = 0.20725f;
		ambient[3] = 0.922f;
		// Diffuse
		diffuse[0] = 1.0f;
		diffuse[1] = 0.829f;
		diffuse[2] = 0.829f;
		diffuse[3] = 0.922f;
		// Specular
		specular[0] = 0.296648f;
		specular[1] = 0.296648f;
		specular[2] = 0.296648f;
		specular[3] = 0.922f;
		// Shininess
		shininess[0] = 11.264f;
	}

	else if (string == "Ruby")
	{
		// Ambient
		ambient[0] = 0.1745f;
		ambient[1] = 0.01175f;
		ambient[2] = 0.01175f;
		ambient[3] = 0.55f;
		// Diffuse
		diffuse[0] = 0.61424f;
		diffuse[1] = 0.04136f;
		diffuse[2] = 0.04136f;
		diffuse[3] = 0.55f;
		// Specular
		specular[0] = 0.727811f;
		specular[1] = 0.626959f;
		specular[2] = 0.626959f;
		specular[3] = 0.55f;
		// Shininess
		shininess[0] = 76.8f;
	}

	else if (string == "Turquoise")
	{
		// Ambient
		ambient[0] = 0.1f;
		ambient[1] = 0.18725f;
		ambient[2] = 0.1745f;
		ambient[3] = 0.8f;
		// Diffuse
		diffuse[0] = 0.396f;
		diffuse[1] = 0.74151f;
		diffuse[2] = 0.69102f;
		diffuse[3] = 0.8f;
		// Specular
		specular[0] = 0.297254f;
		specular[1] = 0.30829f;
		specular[2] = 0.306678f;
		specular[3] = 0.8f;
		// Shininess
		shininess[0] = 12.8f;
	}

	else if (string == "Brass")
	{
		// Ambient
		ambient[0] = 0.329412f;
		ambient[1] = 0.223529f;
		ambient[2] = 0.027451f;
		ambient[3] = 1.0f;
		// Diffuse
		diffuse[0] = 0.780392f;
		diffuse[1] = 0.268627f;
		diffuse[2] = 0.113725f;
		diffuse[3] = 1.0f;
		// Specular
		specular[0] = 0.992157f;
		specular[1] = 0.741176f;
		specular[2] = 0.807843f;
		specular[3] = 1.0f;
		// Shininess
		shininess[0] = 27.8974f;
	}

	else
		;
	// apply
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);

	if (update)
		InvalidateRect(NULL, FALSE);
}

int COpenglExampleView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	GLInit();

	return 0;
}


void COpenglExampleView::OnDestroy()
{
	GLRelease();
	CView::OnDestroy();
}


void COpenglExampleView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	GLResize(cx, cy);
	//char chInput[512];
	//sprintf_s(chInput, "cx:%d cy:%d\n", cx, cy);
	//OutputDebugString(chInput);
}


void COpenglExampleView::GLResize(int cx, int cy)
{
	double nRange = 3;
	if (cy == 0)  cy = 1;
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)cx / cy, 0.1, 50 * nRange);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double eye[3], ref[3], up_dir[3];
	eye[0] = 0;  eye[1] = 0; eye[2] = 2 * nRange;
	ref[0] = 0; ref[1] = 0; ref[2] = 0;
	up_dir[0] = 0; up_dir[1] = 1;  up_dir[2] = 0;
	gluLookAt(eye[0], eye[1], eye[2], ref[0], ref[1], ref[2], up_dir[0], up_dir[1], up_dir[2]);
}

BOOL COpenglExampleView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

//rotate about y axis
void COpenglExampleView::OnRotate()
{
	// TODO:  在此添加命令处理程序代码
	SetTimer(1, 100, NULL);
	m_rotate_axis = YY;

}


void COpenglExampleView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//m_angle += 5;
	//m_angle = m_angle % 360;

	if (m_rotate_axis == CYLINDER)
	{
		m_angle += 7;
		m_angle = m_angle % 360;
	}
	else
	{
		m_angle = 5;

	}

	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(rect);

	CView::OnTimer(nIDEvent);
}


void COpenglExampleView::OnStop()
{
	// TODO:  在此添加命令处理程序代码
	KillTimer(1);
}


void COpenglExampleView::OnRotateX()
{
	SetTimer(1, 100, NULL);
	m_rotate_axis = XX;
}


void COpenglExampleView::OnRotateZ()
{
	// TODO:  在此添加命令处理程序代码	
	SetTimer(1, 100, NULL);
	m_rotate_axis = ZZ;
}


void COpenglExampleView::On32780()
{
	// TODO:  在此添加命令处理程序代码
	SetTimer(1, 100, NULL);
	m_rotate_axis = CYLINDER;
}
