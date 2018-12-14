
// OpenglExampleView.h : COpenglExampleView ��Ľӿ�
//

#pragma once

#include "gl\gl.h"
#include "gl\glu.h"
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
class COpenglExampleView : public CView
{
protected: // �������л�����
	COpenglExampleView();
	DECLARE_DYNCREATE(COpenglExampleView)

// ����
public:
	COpenglExampleDoc* GetDocument() const;

// ����
public:
	HGLRC  m_hRC;
	HDC    m_hDC;
	int   m_angle;

	enum{ XX, YY, ZZ,  CYLINDER, NOROTATE }  m_rotate_axis;

	void draw_cylinder(GLfloat bottomx, GLfloat bottomy, GLfloat  bottomz,  GLfloat   r, GLfloat height);
	void draw_sphere(GLfloat  x, GLfloat y, GLfloat z, GLfloat r);
	void draw_rectangle(double x, double y, double z, double x1, double y1, double z1);
	void draw_bezier(GLfloat bottomx, GLfloat bottomy, GLfloat bottomz);
	void displaySence(void);
	void draw_cube();
	void RenderScene();
	void GLInit();
	void GLRelease();
	void GLResize(int cx, int cy);
	void GLSetupRC(HDC hdc);

	void ChangeMaterial(CString &string,
		bool update = true);
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~COpenglExampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRotate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStop();
	afx_msg void OnRotateX();
	afx_msg void OnRotateZ();
	afx_msg void On32780();
};

#ifndef _DEBUG  // OpenglExampleView.cpp �еĵ��԰汾
inline COpenglExampleDoc* COpenglExampleView::GetDocument() const
   { return reinterpret_cast<COpenglExampleDoc*>(m_pDocument); }
#endif

