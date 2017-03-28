// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ �������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI �����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ���ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// OGLGUIView.cpp : COGLGUIView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "OGLGUI.h"
#endif

#include "OGLGUIDoc.h"
#include "OGLGUIView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COGLGUIView

IMPLEMENT_DYNCREATE(COGLGUIView, CView)

BEGIN_MESSAGE_MAP(COGLGUIView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// COGLGUIView ����/����

COGLGUIView::COGLGUIView()
{
	// TODO:  �ڴ˴����ӹ������

}

COGLGUIView::~COGLGUIView()
{
}

BOOL COGLGUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// COGLGUIView ����

void COGLGUIView::OnDraw(CDC* /*pDC*/)
{
	COGLGUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_OGL->DrawScene();
	// TODO:  �ڴ˴�Ϊ�����������ӻ��ƴ���
}

void COGLGUIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COGLGUIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COGLGUIView ���

#ifdef _DEBUG
void COGLGUIView::AssertValid() const
{
	CView::AssertValid();
}

void COGLGUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COGLGUIDoc* COGLGUIView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COGLGUIDoc)));
	return (COGLGUIDoc*)m_pDocument;
}
#endif //_DEBUG


// COGLGUIView ��Ϣ��������


int COGLGUIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ�������ר�õĴ�������
	m_OGL = new OpenGLObject;
	if (m_OGL->InitGlew(this))
	{
		m_OGL->m_pDC = new CClientDC(this);
		m_OGL->InitializeOpenGL(m_OGL->m_pDC);
		m_OGL->CreateSceneData();
		GetClientRect(&m_OGL->m_oldRect);
	}
	SetTimer(1, 20, NULL);
	return 0;
}


void COGLGUIView::OnDestroy()
{
	CView::OnDestroy();
	// TODO:  �ڴ˴�������Ϣ�����������
	m_OGL->DestroyOGL();
}


BOOL COGLGUIView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ�������Ϣ������������/�����Ĭ��ֵ

	return true;
}


void COGLGUIView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (cy > 0)
	{
		if (m_OGL->m_oldRect.right > cx || m_OGL->m_oldRect.bottom > cy)
			RedrawWindow();
		m_OGL->m_oldRect.right = cx;
		m_OGL->m_oldRect.bottom = cy;
		m_OGL->ResetViewPort(0, 0, cx, cy);
		m_OGL->m_projection = glm::perspective(45.0f, (float)cx / (float)cy, 0.01f, 100.0f);
	}
	// TODO:  �ڴ˴�������Ϣ�����������
}


void COGLGUIView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ�������Ϣ������������/�����Ĭ��ֵ
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}