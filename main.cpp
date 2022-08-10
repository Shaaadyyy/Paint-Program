#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stack>
#include <list>
#include <limits.h>
#include <wingdi.h>




using namespace std;

/* Colors */
#define BLACK   RGB(0,0,0)
#define WHITE   RGB(255,255,255)
#define RED     RGB(255,0,0)
#define ORANGE  RGB(255,128,0)
#define YELLOW  RGB(255,255,0)
#define GREEN   RGB(0,255,0)
#define BLUE    RGB(0,0,255)
#define INDIGO  RGB(75,0,130)
#define VIOLET  RGB(143,0,255)

/* Menu handles */
#define MI_CLEAR 1001
#define MI_SAVE 1002
#define MI_LOAD 1003

#define MI_BLACK_COLOR 1004
#define MI_WHITE_COLOR 1005
#define MI_RED_COLOR 1006
#define MI_ORANGE_COLOR 1007
#define MI_YELLOW_COLOR 1008
#define MI_GREEN_COLOR 1009
#define MI_BLUE_COLOR 1010
#define MI_INDIGO_COLOR 1011
#define MI_VIOLET_COLOR 1012

#define MI_LINE_DDA 1013
#define MI_LINE_MID 1014
#define MI_LINE_PARA 1015

#define MI_CIRCLE_DIRECT 1016
#define MI_CIRCLE_POLAR 1017
#define MI_CIRCLE_ITERATIVE 1018
#define MI_CIRCLE_MID 1019
#define MI_CIRCLE_MOD_MID 1020

#define MI_FILL_Q1 1021
#define MI_FILL_Q2 1022
#define MI_FILL_Q3 1023
#define MI_FILL_Q4 1024
#define MI_FILL_Convex 1029
#define MI_FILL_General_Fill 1030
#define MI_FILL_RFLOODFILL 1031
#define MI_FILL_NRFLOODFILL 1032
#define MI_FILL_Square 1033
#define MI_FILL_Rectangle 1034

#define MI_ELLIPSE_DIRECT 1025
#define MI_ELLIPSE_POLAR 1026
#define MI_ELLIPSE_MIDPOINT 1027
#define MI_CLIP_POINT_CIRCLE 1045
#define MI_CLIP_LINE_CIRCLE 1046

#define MI_CLIP_POINT_RECTANGLE 1040
#define MI_CLIP_LINE_RECTANGLE 1041
#define MI_CLIP_POLYGON_RECTANGLE 1042
#define MI_CLIP_LINE_SQUARE 1043
#define MI_CLIP_POINT_SQUARE 1044
#define MI_FILL_Circle 1035
#define MI_POLYGON 1060


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_CROSS);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("2D Project"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               900,                 /* The programs width */
               700,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

struct Point
{
    int x;
    int y;
    Point(int x_=0,int y_=0)
    {

        x = x_;
        y = y_;

    }


};

int Round(double x)
{
    return (int)(x+0.5);
}



void drawHermitCurve(HDC hdc, Point p1, Point t1, Point p2, Point t2, COLORREF color)
{
    double alpha0 = p1.x;
    double alpha1 = t1.x;
    double alpha2 = (-3*p1.x) - (2*t1.x) + (3*p2.x) - (t2.x);
    double alpha3 = (2*p1.x) + (t1.x) - (2*p2.x) + (t2.x);

    double beta0 = p1.y;
    double beta1 = t1.y;
    double beta2 = -(3*p1.y) - (2*t1.y) + (3*p2.y) - (t2.y);
    double beta3 = (2*p1.y) + (t1.y) - (2*p2.y) + (t2.y);

    for(double t=0; t<=1; t+=0.001)
    {
        double x = alpha0 + (alpha1*t) + (alpha2*t*t) + (alpha3*t*t*t);
        double y = beta0 + (beta1*t) + (beta2*t*t) + (beta3*t*t*t);
        SetPixel(hdc, Round(x), Round(y), color);
    }
}

void hermitCurve(HDC hdc, Point* points, int pointsNum, COLORREF color)
{
    Point T1(3 * (points[1].x - points[0].x), 3 * (points[1].y - points[0].y));
    Point T2(3 * (points[3].x - points[2].x), 3 * (points[3].y - points[2].y));

    drawHermitCurve(hdc, points[0], T1, points[3], T2, color);
}

void bezierCurve(HDC hdc, Point* points, int pointsNum, COLORREF color)
{
    Point p1 = points[0];
    Point p2 = points[1];
    Point p3 = points[2];
    Point p4 = points[3];

    double alpha0 = p1.x;
    double alpha1 = 3*(p2.x - p1.x);
    double alpha2 = (3*p1.x) - (6*p2.x) + (3*p3.x);
    double alpha3 = (-p1.x) + (3*p2.x) - (3*p3.x) + (p4.x);

    double beta0 = p1.y;
    double beta1 = 3*(p2.y - p1.y);
    double beta2 = (3*p1.y) - (6*p2.y) + (3*p3.y);
    double beta3 = (-p1.y) + (3*p2.y) - (3*p3.y) + (p4.y);

    for(double t=0; t<=1; t+=0.001)
    {
        double x = alpha0 + (alpha1*t) + (alpha2*t*t) + (alpha3*t*t*t);
        double y = beta0 + (beta1*t) + (beta2*t*t) + (beta3*t*t*t);
        SetPixel(hdc, x, y, color);
    }
}

void fillingRectangleWithBezierCurve(HDC hdc, COLORREF color)
{


    int left = 150, top = 100;
    int right = 400, bottom = 400;
    Rectangle(hdc,left, top, right, bottom);
    int n = 4;
    POINT points[n];
    points[0]= {left,top};
    points[1]= {left,bottom};
    points[2]= {right,top};
    points[3]= {right,bottom};




    int startX = left;
    int endX = right;
    int startY = top;
    int endY = bottom;

    int h = (endX - startX)/4;

    for(int i=startX+1; i<endX; i++)
    {
        Point curvePoints[] =
        {
            Point(i, startY),
            Point(i, startY+h),
            Point(i+1, endY-h),
            Point(i, endY)
        };

        hermitCurve(hdc, curvePoints, 4, color);
    }
}


void fillingSquareWithHermitCurve(HDC hdc, COLORREF color)
{


    int left = 150, top = 150;
    int right = 250, bottom = 250;
    Rectangle(hdc,left, top, right, bottom);
    int n = 4;
    POINT points[n];
    points[0]= {left,top};
    points[1]= {left,bottom};
    points[2]= {right,top};
    points[3]= {right,bottom};

    int startX = left;
    int endX = right;
    int startY = top;
    int endY = bottom;

    int w = (endX - startX)/4;

    for(int i=startY+1; i<endY; i++)
    {
        Point curvePoints[] =
        {
            Point(startX, i),
            Point(startX+w, i),
            Point(endX-w, i+1),
            Point(endX, i)
        };

        bezierCurve(hdc, curvePoints, 4, color);
    }
}

void FloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf)
{
    COLORREF C=GetPixel(hdc,x,y);
    if(C==Cb || C==Cf)
        return;
    SetPixel(hdc,x,y,Cf);
    FloodFill(hdc,x+1,y,Cb,Cf);
    FloodFill(hdc,x-1,y,Cb,Cf);
    FloodFill(hdc,x,y+1,Cb,Cf);
    FloodFill(hdc,x,y-1,Cb,Cf);
}





struct Vertex
{
    int x,y;
    Vertex(int x,int y):x(x),y(y)
    {
    }
};
void NRFloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf)
{

    stack<Vertex> S;
    S.push(Vertex(x,y));
    while(!S.empty())
    {
        Vertex v=S.top();
        S.pop();
        COLORREF c=GetPixel(hdc,v.x,v.y);
        if(c==Cb || c==Cf)
            continue;
        SetPixel(hdc,v.x,v.y,Cf);
        S.push(Vertex(v.x+1,v.y));
        S.push(Vertex(v.x-1,v.y));
        S.push(Vertex(v.x,v.y+1));
        S.push(Vertex(v.x,v.y-1));
    }
}

struct Entry
{
    int xmin,xmax;
};

void InitEntries(Entry table[])
{
    for(int i=0; i<800; i++)
    {
        table[i].xmin=INT_MAX;
        table[i].xmax=-INT_MAX;
    }
}


void ScanEdge(POINT v1,POINT v2,Entry table[])
{
    if(v1.y==v2.y)
        return;
    if(v1.y>v2.y)
        swap(v1,v2);
    double minv=(double)(v2.x-v1.x)/(v2.y-v1.y);
    double x=v1.x;
    int y=v1.y;
    while(y<v2.y)
    {
        if(x<table[y].xmin)
            table[y].xmin=(int)ceil(x);
        if(x>table[y].xmax)
            table[y].xmax=(int)floor(x);
        y++;
        x+=minv;
    }
}

void DrawSanLines(HDC hdc,Entry table[],COLORREF color)
{
    for(int y=0; y<800; y++)
        if(table[y].xmin<table[y].xmax)
            for(int x=table[y].xmin; x<=table[y].xmax; x++)
                SetPixel(hdc,x,y,color);
}

void ConvexFill(HDC hdc,COLORREF color)
{
    int left = 150, top = 150;
    int right = 450, bottom = 450;
    Rectangle(hdc,left, top, right, bottom);
    int n = 4;
    POINT p[n];
    p[0]= {left,top};
    p[1]= {left,bottom};
    p[2]= {right,top};
    p[3]= {right,bottom};


    Entry *table=new Entry[800];
    InitEntries(table);
    POINT v1=p[n-1];
    for(int i=0; i<n; i++)
    {
        POINT v2=p[i];
        ScanEdge(v1,v2,table);
        v1=p[i];
    }
    DrawSanLines(hdc,table,color);
    delete table;
}




struct EdgeRec
{
    double x;
    double minv;
    int ymax;
    bool operator<(EdgeRec r)
    {
        return x<r.x;
    }
};
typedef list<EdgeRec> EdgeList;



EdgeRec InitEdgeRec(POINT& v1,POINT& v2)
{
    if(v1.y>v2.y)
        swap(v1,v2);
    EdgeRec rec;
    rec.x=v1.x;
    rec.ymax=v2.y;
    rec.minv=(double)(v2.x-v1.x)/(v2.y-v1.y);
    return rec;
}

void InitEdgeTable(POINT *polygon,int n,EdgeList table[])
{
    POINT v1=polygon[n-1];
    for(int i=0; i<n; i++)
    {
        POINT v2=polygon[i];
        if(v1.y==v2.y)
        {
            v1=v2;
            continue;
        }
        EdgeRec rec=InitEdgeRec(v1, v2);
        table[v1.y].push_back(rec);
        v1=polygon[i];
    }
}


void GeneralPolygonFill(HDC hdc,COLORREF c)

{

    int left = 150, top = 150;
    int right = 450, bottom = 450;
    Rectangle(hdc,left, top, right, bottom);
    int n = 4;
    POINT p[n];
    p[1]= {left,top};
    p[0]= {left,bottom};
    p[2]= {right,top};
    p[3]= {right,bottom};

    EdgeList *table=new EdgeList [800];
    InitEdgeTable(p,n,table);
    int y=0;
    while(y<800 && table[y].size()==0)
        y++;

    if(y==800)
        return;
    EdgeList ActiveList=table[y];
    while (ActiveList.size()>0)
    {
        ActiveList.sort();
        for(EdgeList::iterator it=ActiveList.begin(); it!=ActiveList.end(); it++)
        {
            int x1=(int)ceil(it->x);
            it++;
            int x2=(int)floor(it->x);
            for(int x=x1; x<=x2; x++)
                SetPixel(hdc,x,y,c);
        }
        y++;
        EdgeList::iterator it=ActiveList.begin();
        while(it!=ActiveList.end())
            if(y==it->ymax)
                it=ActiveList.erase(it);
            else
                it++;
        for(EdgeList::iterator it=ActiveList.begin(); it!=ActiveList.end(); it++)
            it->x+=it->minv;
        ActiveList.insert(ActiveList.end(),table[y].begin(),table[y].end());
    }
    delete[] table;
}
/* Global variables */
COLORREF color = BLACK;
enum algorithms {line_DDA, line_mid, line_para,
                 circle_dir, circle_polar, circle_iter, circle_mid, circle_mod_mid,
                 fill_q1, fill_q2, fill_q3, fill_q4,fill_convex,general_fill,R_FloodFill,NR_FloodFill,
                 ellipse_dir, PolygonAlg,Circle_Fill,ellipse_polar,ellipse_midpt,clip_point, clip_line, c_point, c_line,Square_Fill,Rectangle_Fill,clip_Polygon,sLine,sPoint,rLine,rPoint,cLine,cPoint
};
int alg = line_DDA;
int click_count = 0;
struct point
{
    int x;
    int y;
};

struct shape
{
    int type;
    vector<point> data;
    COLORREF color;
};

point current_point;
shape current_shape;
vector<shape> shapes;

int r;
int x_1,y_1,x_2,y_2;
int X_left, Y_top, X_right, Y_bottom;
int X_start, Y_start, X_end, Y_end;

/// Circle.

void Swap(int& x,int& y)
{
    int tmp=x;
    x=y;
    y=tmp;
}

void polygon(HDC hdc, Point& p1, Point& T1, Point& p2, Point& T2,COLORREF c)
{
	double alpha0 = p1.x, alpha1 = T1.x,
		alpha2 = -3 * p1.x - 2 * T1.x + 3 * p2.x - T2.x,
		alpha3 = 2 * p1.x + T1.x - 2 * p2.x + T2.x;
	double beta0 = p1.y, beta1 = T1.y,
		beta2 = -3 * p1.y - 2 * T1.y + 3 * p2.y - T2.y,
		beta3 = 2 * p1.y + T1.y - 2 * p2.y + T2.y;
	for (double t = 0; t <= 1; t += 0.001)
	{
		double t2 = t * t, t3 = t2 * t;
		double x = alpha0 + alpha1 * t + alpha2 * t2 + alpha3 * t3;
		double y = beta0 + beta1 * t + beta2 * t2 + beta3 * t3;
		SetPixel(hdc,x,y,c);
	}
}


void Draw8points(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc+y, color);
    SetPixel(hdc, xc-x, yc+y, color);
    SetPixel(hdc, xc+x, yc-y, color);
    SetPixel(hdc, xc-x, yc-y, color);
    SetPixel(hdc, xc-y, yc+x, color);
    SetPixel(hdc, xc+y, yc-x, color);
    SetPixel(hdc, xc+y, yc+x, color);
    SetPixel(hdc, xc-y, yc-x, color);
}
void Draw4points(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc+x,yc+y,color);
}


int getRadius(int x1, int y1, int x2, int y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}


/// (1) MidPoint.
void midpoint(HDC hdc,int xc,int yc,int r,COLORREF color)
{
    int x=0;
    int y=r;
    double d = 0;
    int r2 = r*r;
    Draw8points(hdc,x,y,xc,yc,color);
    while(x<y)
    {
        d = (((x+1)*(x+1)) + ((y-0.5)*(y-0.5)) - r2);
        if(d<0)
        {
            x++;
        }
        else
        {
            x++;
            y--;
        }
        Draw8points(hdc,x,y,xc,yc,color);
    }

}
void Circle_MidPoint(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    r = getRadius(x1,y1,x2,y2);
    midpoint(hdc, x1, y1, r,color);
}



/// (2) Iterative Polar

void IterativePolar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    double x=R,y=0;
    double dtheta=1.0/R;
    double cdtheta=cos(dtheta);
    double sdtheta=sin(dtheta);
    Draw8points(hdc,xc,yc,R,0,color);

    while(x>y)
    {
        double x1 = x*cdtheta-y*sdtheta;
        y = x*sdtheta+y*cdtheta;
        x = x1;
        Draw8points(hdc,Round(x),Round(y),xc,yc,color);
    }
}

void Circle_IterativePolar(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    r =  getRadius(x1,y1,x2,y2);
    IterativePolar(hdc,x1,y1,r,color);

}


/// (3) Direct.
void Direct(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int R2=R*R;
    Draw8points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        x++;
        y=round(sqrt((double)(R2-x*x)));
        Draw8points(hdc,x,y,xc,yc,color);
    }
}

void Circle_Direct(HDC hdc, int x1,int y1,int x2,int y2,COLORREF color)
{
    int r = sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
    Direct(hdc,x1,y1,r,color);

}



/// (4) Polar
void Polar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=R,y=0;
    double theta=0,dtheta=1.0/R;

    while(x>y)
    {
        theta+=dtheta;
        x=R*cos(theta);
        y=R*sin(theta);
        Draw8points(hdc,round(x),round(y),xc,yc,color);
    }
}

void Circle_Polar(HDC hdc, int x1,int y1,int x2,int y2,COLORREF color)
{
    int r = sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
    Polar(hdc,x1,y1,r,color);

}



/// (5) Modified Midpoint
void modified_midpoint(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    Draw8points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
            d+=2*x+3;
        else
        {
            d+=2*(x-y)+5;
            y--;
        }
        x++;
        Draw8points(hdc,x,y,xc,yc,color);
    }
}

void Circle_modified_midpoint(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    int r = sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
    modified_midpoint(hdc,x1,y1,r,color);
}
///-----------------------------------------------------------------------------------------------------------------------------------


/// Line.

void Swap3(int& xs, int& ys, int& xe, int& ye)
{
    int temp = xs;
    xs = xe;
    xe = temp;
    temp = ys;
    ys = ye;
    ye = temp;
}


/// (1) DDA.
void DDA(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
{
    int dx=xe-xs;
    int dy=ye-ys;

    if(abs(dx)>=abs(dy))
    {
        if(xs > xe)
            Swap3(xs,ys,xe,ye);
        SetPixel(hdc,xs,ys,color);
        int x=xs;

        double y=ys;
        double m = (double) dy / dx;

        while(x!=xe)
        {
            x++;
            y = ys + m;
            SetPixel(hdc,x,round(y),color);
        }

    }

    else
    {

        if (ys > ye)
            Swap3(xs, ys, xe, ye);
        SetPixel(hdc,xs,ys,color);
        int y=ys;
        double x=xs;
        double minv = (double) dx / dy;
        while(y!=ye)
        {
            x+=minv;
            y++;
            SetPixel(hdc,round(x),y,color);
        }
    }
}


/// (2) Parametric .
void parametric(HDC hdc, double xs, double ys, double xe, double ye, COLORREF color)
{
    double dx = xe - xs;
    double dy = ye - ys;
    for (double t = 0; t < 1; t += 0.001)
    {
        int x = xs + (dx * t);
        int y = ys + (dy * t);
        SetPixel(hdc, x, y, color);
    }
}


/// (3) MidPoint.
void Swap2(int& xs, int& ys, int& xe, int& ye)
{
    int temp = xs;
    xs = xe;
    xe = temp;
    temp = ys;
    ys = ye;
    ye = temp;
}
void LineMidpoint (HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d, d1, d2;

    if(dx == 0 && dy == 0)
    {
        SetPixel(hdc, x1, y1, c);
        return;
    }
    if (abs(dy) < abs(dx))
    {
        if (x1 > x2)
        {
            Swap2(x1, y1, x2, y2);
            dy *= -1;
            dx *= -1;
        }
        int x = x1, y = y1;
        SetPixel(hdc, x, y, c);

        if (dy > 0) //f(x+1,y+0.5)
        {
            d = dx - 2 * dy;
            d1 = -2 * dy;
            d2 = 2 * (dx - dy);
        }
        else //f(x-1,y+0.5)
        {
            d = dx + 2 * dy;
            d1 = 2 * dy;
            d2 = 2 * (dx + dy);
        }

        while (x < x2)
        {
            x++;
            if (d > 0)
            {
                d += d1;
            }
            else
            {
                d += d2;

                if (dy > 0)
                    y++;
                else
                    y--;
            }
            SetPixel(hdc, x, y, c);
        }
    }
    else if (abs(dx) < abs(dy))
    {
        if (y1 > y2)
        {
            Swap2(x1, y1, x2, y2);
            dy *= -1;
            dx *= -1;
        }
        int x = x1, y = y1;
        SetPixel(hdc, x, y, c);

        if (dx > 0)		//f(x+0.5,y+1)
        {
            d = dy - 2 * dx;
            d1 = -2 * dx;
            d2 = 2 * (dy - dx);
        }
        else //f(x-0.5,y+1)
        {
            d = dy + 2 * dx;
            d1 = 2 * dx;
            d2 = 2 * (dy + dx);
        }

        while (y < y2)
        {
            y++;
            if (d > 0)
            {
                d += d1;
            }
            else
            {
                d += d2;
                if (dx > 0)
                    x++;
                else
                    x--;
            }
            SetPixel(hdc, x, y, c);
        }
    }
}



/// Ellipse
/// (1) Polar
void polarEllipse(HDC hdc, int x1, int y1, int x_2, int y_2, COLORREF c)
{

    for(double theta = 0 ; theta<6.28 ; theta += 0.001)
    {
        int x = Round(x1 + x_1 * cos(theta));
        int y = Round(y1 + y_2 * sin(theta));
        SetPixel(hdc, x, y, c);
    }
}

/// (2) Direct
void DirectEllipse(HDC hdc,int x_1, int y_1, int A, int B, COLORREF c)
{
    float x = 0;
    float y = B;
    int B2 = B*B;
    int A2 = A*A;
    Draw4points(hdc,A,B,x,y,color);
    while(x*B2 < y*A2)
    {
        x++;
        y=B*sqrt(1-((x*x)/A2));
        Draw4points(hdc,A,B,x,y,color);
    }
    x = A;
    y = 0;
    Draw4points(hdc,A,B,x,y,color);
    while(x*B2 > y*A2)
    {
        y++;
        x=A*sqrt(1-((y*y)/B2));
        Draw4points(hdc,A,B,x,y,color);
    }
}

/// (3) Midpoint
void midptellipse(HDC hdc,int rx, int ry,int xc, int yc, COLORREF col)
{
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;

    // Initial decision parameter of region 1
    d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    // For region 1
    while (dx < dy)
    {
        // Checking and updating value of
        // decision parameter based on algorithm
        if (d1 < 0)
        {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        }
        else
        {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }

    // Decision parameter of region 2
    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
         ((rx * rx) * ((y - 1) * (y - 1))) -
         (rx * rx * ry * ry);

    // Plotting points of region 2
    while (y >= 0)
    {



        // Checking and updating parameter
        // value based on algorithm
        if (d2 > 0)
        {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        }
        else
        {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
}




void fillQ1(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc-y, color);
    parametric(hdc, xc, yc, xc+x, yc-y,  color);
    SetPixel(hdc, xc+y, yc-x, color);
    parametric(hdc, xc, yc, xc+y, yc-x,  color);
}

void fillQ2(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc-x, yc-y, color);
    parametric(hdc, xc, yc, xc-x, yc-y,  color);
    SetPixel(hdc, xc-y, yc-x, color);
    parametric(hdc, xc, yc, xc-y, yc-x,  color);
}

void fillQ3(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc-x, yc+y, color);
    parametric(hdc, xc, yc, xc-x, yc+y,  color);
    SetPixel(hdc, xc-y, yc+x, color);
    parametric(hdc, xc, yc, xc-y, yc+x,  color);
}

void fillQ4(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc+y, color);
    parametric(hdc, xc, yc, xc+x, yc+y,  color);
    SetPixel(hdc, xc+y, yc+x, color);
    parametric(hdc, xc, yc, xc+y, yc+x,  color);
}

void filling_algo(HDC hdc,int xc,int yc,int x1, int y1,int q, COLORREF color)
{
    int r = getRadius(x1, y1, xc, yc);
    int x=0;
    int y=r;
    double d=1-r;
    while(x<y)
    {
        if(d<=0)
        {
            d=d+2*x+3;
            x++;
        }
        else
        {
            d=d+2*(x-y)+5;
            x++;
            y--;
        }
        if(q == 1)
            fillQ1(hdc,x,y,xc,yc,color);
        else if(q == 2)
            fillQ2(hdc,x,y,xc,yc,color);
        else if(q == 3)
            fillQ3(hdc,x,y,xc,yc,color);
        else
            fillQ4(hdc,x,y,xc,yc,color);
    }
}
void fillingCircleWithOtherCircles(HDC hdc, COLORREF color)
{
    Circle_Polar(hdc,250,250,150,150,color);
     int r = sqrt(pow((250-150), 2) + pow((250-150), 2));
    for(r ; r > 0  ;r-- ){

         midpoint(hdc,250,250,r,color);

    }


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawLine74(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    double t = 0.0;
    while (t <= 1.0)
    {
        double x = x1 + (t * (x2 - x1));
        double y = y1 + (t * (y2 - y1));
        SetPixel(hdc, Round(x), Round(y), c);
        t = t + 0.0001;
    }
}

void drawRectangle(HDC hdc, Point p1, int length, int width, COLORREF c)
{
    Point points[4];
    points[0].x = p1.x;
    points[0].y = p1.y;
    points[1].x = p1.x;
    points[1].y = p1.y + length;
    points[2].x = p1.x + width;
    points[2].y = p1.y + length;
    points[3].x = p1.x + width;
    points[3].y = p1.y;
    Point v1 = points[3];
    for (int i = 0; i < 4; i++)
    {
        Point v2 = points[i];
        DrawLine74(hdc, v1.x, v1.y, v2.x, v2.y, c);
        v1 = v2;
    }
}
void rectangleClippingPoint(HDC hdc, Point p, int length, int width, Point testPoint)
{
    if (testPoint.x >= p.x && testPoint.x <= p.x + width)
    {
        if ((testPoint.y >= p.y) && (testPoint.y <= p.y + length))
        {
            SetPixel(hdc, testPoint.x, testPoint.y, RGB(0, 0, 255));
        }
    }
}
void rectangleClippingLine(HDC hdc, Point p, int length,int width, Point v1, Point v2)
{
    double t = 0.0;
    while (t <= 1.0)
    {
        double x = v1.x + (t * (v2.x - v1.x));
        double y = v1.y + (t * (v2.y - v1.y));
        Point v = Point(round(x), round(y));
        rectangleClippingPoint(hdc, p, length,width, v);
        t = t + 0.00001;
    }
}

void polygonClipping(HDC hdc, Point& p1, Point& T1, Point& p2, Point& T2, Point rectanglePoint, int length, int width)
{
    double alpha0 = p1.x, alpha1 = T1.x,         alpha2 = -3 * p1.x - 2 * T1.x + 3 * p2.x - T2.x,         alpha3 = 2 * p1.x + T1.x - 2 * p2.x + T2.x;
    double beta0 = p1.y, beta1 = T1.y,         beta2 = -3 * p1.y - 2 * T1.y + 3 * p2.y - T2.y,         beta3 = 2 * p1.y + T1.y - 2 * p2.y + T2.y;
    for (double t = 0; t <= 1; t += 0.001)
    {
        double t2 = t * t, t3 = t2 * t;
        double x = alpha0 + alpha1 * t + alpha2 * t2 + alpha3 * t3;
        double y = beta0 + beta1 * t + beta2 * t2 + beta3 * t3;
        Point v = Point(round(x), round(y));
        rectangleClippingPoint(hdc, rectanglePoint, length, width, v);
    }
}
void drawSquare(HDC hdc, Point p1, int length, COLORREF c)
{
    Point points[4];
    points[0].x = p1.x;
    points[0].y = p1.y;
    points[1].x = p1.x;
    points[1].y = p1.y - length;
    points[2].x = p1.x + length;
    points[2].y = p1.y - length;
    points[3].x = p1.x + length;
    points[3].y = p1.y;
    Point v1 = points[3];
    for (int i = 0; i < 4; i++)
    {
        Point v2 = points[i];
        DrawLine74(hdc, v1.x, v1.y, v2.x, v2.y, c);
        v1 = v2;
    }


}
void draw8Points74(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + y, yc + x, c);
    SetPixel(hdc, xc + y, yc - x, c);
    SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc - y, yc - x, c);
}
void polarCircurcleAlg74(HDC hdc, int xc, int yc, double r, COLORREF c)
{
    double x = r;
    double y = 0.0;
    double dtheta = 1.0 / r;
    double theta = 0 + dtheta;
    draw8Points74(hdc, xc, yc, (int)x, (int)y, c);
    while (x > y)
    {
        x = r * cos(theta);
        y = r * sin(theta);
        theta += dtheta;
        draw8Points74(hdc, xc, yc, Round(x), Round(y), c);
    }
}
bool isINCircule(int xc, int yc, int x, int y, double r)
{

    double result = pow((x - xc), 2.0) + pow((y - yc), 2.0) - (r * r);
    return result <= 0;
}
void circleClippingPoint(HDC hdc, int xc, int yc, double r, Point p)
{
    if(isINCircule( xc,  yc,  p.x,  p.y,  r) )
    {
        SetPixel(hdc, p.x, p.y, RGB(0,0, 0));


    }
}
void circleClippingLine(HDC hdc, int xc, int yc, double r,Point v1,Point v2)
{
    double t = 0.0;
    while (t <= 1.0)
    {
        double x = v1.x + (t * (v2.x - v1.x));
        double y = v1.y + (t * (v2.y - v1.y));
        if (isINCircule(xc, yc, x, y, r))
        {
            SetPixel(hdc, Round(x), Round(y), RGB(0,0,255));
        }

        t = t + 0.00001;
    }
}

void squareClippingPoint(HDC hdc, Point p, int length, Point testPoint)
{

    if (testPoint.x >= p.x && testPoint.x <= p.x + length)
    {
        if ((testPoint.y >= p.y - length) && (testPoint.y <= p.y))
        {
            SetPixel(hdc, testPoint.x, testPoint.y, RGB(0, 0, 255));
        }
    }

}
void squareClippingLine(HDC hdc, Point p, int length, Point v1, Point v2)
{
    double t = 0.0;
    while (t <= 1.0)
    {
        double x = v1.x + (t * (v2.x - v1.x));
        double y = v1.y + (t * (v2.y - v1.y));
        Point v=Point(round(x),round(y));
        squareClippingPoint(hdc, p, length, v);
        t = t + 0.00001;
    }
}


void add_point(int x, int y)
{
    current_point.x = x;
    current_point.y = y;
    current_shape.data.push_back(current_point);
}

void add_shape(int shape_type, COLORREF shape_color)
{
    current_shape.type = shape_type;
    current_shape.color = shape_color;
    shapes.push_back(current_shape);
    current_shape.data.clear(); // clear points
}

bool get_last_circle_data()
{
    for (int i = shapes.size() - 1; i >= 0; --i)
    {
        if (shapes[i].type == circle_mod_mid || shapes[i].type == circle_mid
                || shapes[i].type == circle_iter || shapes[i].type == circle_polar
                || shapes[i].type == circle_dir)
        {
            x_1 = shapes[i].data[0].x;
            y_1 = shapes[i].data[0].y;
            x_2 = shapes[i].data[1].x;
            y_2 = shapes[i].data[1].y;

            return true;
        }
    }

    return false;

}

void get_RGB_values(COLORREF color, int& R, int& G, int& B)
{
    R = color & 0xff;
    G = (color >> 8) & 0xff;
    B = (color >> 16) & 0xff;
}

void save_data()
{
    char filename[100];
    ofstream save_file;
    int r, g, b;



    cout << "Enter filename to save: ";
    cin >> filename;
    save_file.open(filename, ios::out | ios::trunc);

    // saving shapes' entries
    for (int i = 0; i < shapes.size(); ++i)
    {
        save_file << shapes[i].type << " ";
        for (int j = 0; j < shapes[i].data.size(); ++j)
        {
            save_file << shapes[i].data[j].x << " " << shapes[i].data[j].y;
            save_file << " ";
        }
        get_RGB_values(shapes[i].color, r, g, b);
        save_file << r << " ";
        save_file << g << " ";
        save_file << b << '\n';

    }

    save_file.close();
}

void load_data()
{
    char filename[100];
    char num[100];
    string line;
    ifstream load_file;
    int r, g, b;

    cout << "Enter filename to load: ";
    cin >> filename;
    load_file.open(filename, ios::in);

    bool start_entry = true;

    string space_delimiter = " ";
    vector<string> words;

    // loading shapes' entries
    while(getline(load_file, line))
    {
        words.clear();
        size_t pos = 0;
        while ((pos = line.find(space_delimiter)) != string::npos)
        {
            words.push_back(line.substr(0, pos));
            line.erase(0, pos + space_delimiter.length());
        }
        words.push_back(line.substr(0, pos));

        for (int i=0; i < words.size(); ++i)
        {

            strcpy(num, words[i].c_str());

            if (i == 0)
                current_shape.type = atoi(num);
            else if (i >= words.size() - 3)
            {
                r = atoi(num);
                i++;
                strcpy(num, words[i].c_str());
                g = atoi(num);
                i++;
                strcpy(num, words[i].c_str());
                b = atoi(num);
                current_shape.color = RGB(r,g,b);
            }
            else
            {
                strcpy(num, words[i++].c_str());
                current_point.x = atoi(num);
                strcpy(num, words[i].c_str());
                current_point.y = atoi(num);
                current_shape.data.push_back(current_point);
            }
        }
        shapes.push_back(current_shape);
        current_shape.data.clear();
    }

    load_file.close();


}

void draw_data(HDC hdc)
{
    for (int i = 0; i < shapes.size(); ++i)
    {
        switch(shapes[i].type)
        {
        case line_DDA:
        {
            DDA(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }

        case line_mid:
        {
            LineMidpoint(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }

        case line_para:
        {
            parametric(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }

        case circle_dir:
        {
            Circle_Direct(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }

        case circle_polar:
        {
            Circle_Polar(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }

        case circle_iter:
        {
            Circle_IterativePolar(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }

        case circle_mid:
        {
            Circle_MidPoint(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }

        case circle_mod_mid:
        {
            Circle_modified_midpoint(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }

        case fill_q1:
        {
            // assign axes to the last circle in the shapes dynamic array
            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                filling_algo(hdc, x_1, y_1, x_2, y_2,1,shapes[i].color);
            }

            break;
        }
        case fill_q2:
        {
            // assign axes to the last circle in the shapes dynamic array
            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                filling_algo(hdc, x_1, y_1, x_2, y_2,2,shapes[i].color);
            }

            break;
        }

        case fill_q3:
        {
            // assign axes to the last circle in the shapes dynamic array
            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                filling_algo(hdc, x_1, y_1, x_2, y_2, 3, shapes[i].color);
            }

            break;
        }

        case fill_q4:
        {
            // assign axes to the last circle in the shapes dynamic array
            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                filling_algo(hdc, x_1, y_1, x_2, y_2, 4, shapes[i].color);
            }

            break;
        }
        case fill_convex:
        {
            ConvexFill(hdc,shapes[i].color);
            break;
        }
        case Circle_Fill:
        {
            fillingCircleWithOtherCircles(hdc,color);
            break;
        }

        case general_fill:
        {
            GeneralPolygonFill(hdc,shapes[i].color);
            break;
        }
        case R_FloodFill:
        {
            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                FloodFill(hdc, x_1, y_1,shapes[i].color, color);
            }

            break;

        }
        case NR_FloodFill:
        {


            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                NRFloodFill(hdc, x_1, y_1,shapes[i].color, color);
            }


            break;
        }

        case Square_Fill:
        {

            fillingSquareWithHermitCurve(hdc,color);


            break;
        }
        case Rectangle_Fill:
        {



            fillingRectangleWithBezierCurve(hdc,color);
            break;
        }




        case ellipse_dir:
        {
            DirectEllipse(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }

        case ellipse_polar:
        {
            polarEllipse(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }
        case ellipse_midpt:
        {
            midptellipse(hdc,shapes[i].data[0].x,shapes[i].data[0].y,shapes[i].data[1].x,shapes[i].data[1].y,shapes[i].color);
            break;
        }
        default:
            break;
        }

    }
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);

    switch (message)                  /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;

    case WM_CREATE:                        /* Create the menu bar */
    {
        HMENU hMenubar = CreateMenu();
        HMENU hFile = CreateMenu();
        HMENU hColor = CreateMenu();
        HMENU hLine = CreateMenu();
        HMENU hCircle = CreateMenu();
        HMENU hFill = CreateMenu();
        HMENU hEllipse = CreateMenu();
        HMENU hClip = CreateMenu();
        HMENU hPolygon = CreateMenu();
        /* Menus */
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, "File");
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hColor, "Color");
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hLine, "Line");
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hCircle, "Circle");
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFill, "Fill");
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hEllipse, "Ellipse");
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hClip, "Clip");
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hPolygon, "CardinalSplines");

        /* File sub-menus */
        AppendMenu(hFile, MF_STRING, MI_CLEAR, "Clear");
        AppendMenu(hFile, MF_STRING, MI_SAVE, "Save");
        AppendMenu(hFile, MF_STRING, MI_LOAD, "Load");

        /* Color sub-menus */
        AppendMenu(hColor, MF_STRING, MI_BLACK_COLOR, "Black");
        AppendMenu(hColor, MF_STRING, MI_WHITE_COLOR, "White");
        AppendMenu(hColor, MF_STRING, MI_RED_COLOR, "Red");
        AppendMenu(hColor, MF_STRING, MI_ORANGE_COLOR, "Orange");
        AppendMenu(hColor, MF_STRING, MI_YELLOW_COLOR, "Yellow");
        AppendMenu(hColor, MF_STRING, MI_GREEN_COLOR, "Green");
        AppendMenu(hColor, MF_STRING, MI_BLUE_COLOR, "Blue");
        AppendMenu(hColor, MF_STRING, MI_INDIGO_COLOR, "Indigo");
        AppendMenu(hColor, MF_STRING, MI_VIOLET_COLOR, "Violet");

        /* Line sub-menus */
        AppendMenu(hLine, MF_STRING, MI_LINE_DDA, "DDA");
        AppendMenu(hLine, MF_STRING, MI_LINE_MID, "Midpoint");
        AppendMenu(hLine, MF_STRING, MI_LINE_PARA, "parametric");

        /* Circle sub-menus */
        AppendMenu(hCircle, MF_STRING, MI_CIRCLE_DIRECT, "Direct");
        AppendMenu(hCircle, MF_STRING, MI_CIRCLE_POLAR, "Polar");
        AppendMenu(hCircle, MF_STRING, MI_CIRCLE_ITERATIVE, "Iterative Polar");
        AppendMenu(hCircle, MF_STRING, MI_CIRCLE_MID, "Midpoint");
        AppendMenu(hCircle, MF_STRING, MI_CIRCLE_MOD_MID, "Modified Midpoint");

        /* Fill sub-menus */
        AppendMenu(hFill, MF_STRING, MI_FILL_Q1, "Quarter 1");
        AppendMenu(hFill, MF_STRING, MI_FILL_Q2, "Quarter 2");
        AppendMenu(hFill, MF_STRING, MI_FILL_Q3, "Quarter 3");
        AppendMenu(hFill, MF_STRING, MI_FILL_Q4, "Quarter 4");
        AppendMenu(hFill, MF_STRING, MI_FILL_Convex, "Convex Fill");
        AppendMenu(hFill, MF_STRING, MI_FILL_General_Fill, "General Fill");
        AppendMenu(hFill, MF_STRING, MI_FILL_RFLOODFILL, "Recursive Flood Fill");
        AppendMenu(hFill, MF_STRING, MI_FILL_NRFLOODFILL, "Non-Recursive Flood Fill");
        AppendMenu(hFill, MF_STRING, MI_FILL_Square, "Square Fill");
        AppendMenu(hFill, MF_STRING, MI_FILL_Rectangle, "Rectangle Fill");
        AppendMenu(hFill, MF_STRING, MI_FILL_Circle, "Circle Fill");
        /* Ellipse sub-menus */
        AppendMenu(hEllipse, MF_STRING, MI_ELLIPSE_DIRECT, "Direct");
        AppendMenu(hEllipse, MF_STRING, MI_ELLIPSE_POLAR, "Polar");
        AppendMenu(hEllipse, MF_STRING, MI_ELLIPSE_MIDPOINT, "Midpoint");

        /* Clip sub-menus */

        AppendMenu(hClip, MF_STRING, MI_CLIP_POINT_CIRCLE, "Clipping-Point Circle Window ");
        AppendMenu(hClip, MF_STRING, MI_CLIP_LINE_CIRCLE, "Clipping-Line circle Window");
        AppendMenu(hClip, MF_STRING, MI_CLIP_POINT_RECTANGLE, "Clipping-Point Rectangle Window ");
        AppendMenu(hClip, MF_STRING, MI_CLIP_LINE_RECTANGLE, "Clipping-Line Rectangle Window");
        AppendMenu(hClip, MF_STRING, MI_CLIP_POLYGON_RECTANGLE, "Clipping-Polygon Rectangle Window ");
        AppendMenu(hClip, MF_STRING, MI_CLIP_LINE_SQUARE, "Clipping-Line Square Window");
        AppendMenu(hClip, MF_STRING, MI_CLIP_POINT_SQUARE, "Clipping-Point Square Window ");

        /* Cardinal Splines menu */

        AppendMenu(hPolygon, MF_STRING, MI_POLYGON, "CardinalSplines");
        SetMenu(hwnd, hMenubar);

        break;
    }

    case WM_LBUTTONUP:              /* actions when left click is pressed */
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);



        switch (alg)
        {
        case line_DDA:
        {

            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);

                DDA(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);
                click_count = 0; // reset click count
            }
            break;
        }

        case line_mid:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);
                LineMidpoint(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);
                click_count = 0; // reset click count

            }
            break;
        }

        case line_para:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);
                parametric(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);
                click_count = 0; // reset click count

            }
            break;
        }

        case circle_dir:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);
                Circle_Direct(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);
                click_count = 0; // reset click count

            }
            break;
        }

        case circle_polar:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);

                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);
                Circle_Polar(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);
                click_count = 0; // reset click count

            }
            break;
        }

        case circle_iter:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);

                Circle_IterativePolar(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);
                click_count = 0; // reset click count

            }
            break;
        }

        case circle_mid:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);
                Circle_MidPoint(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);

                click_count = 0; // reset click count

            }
            break;
        }

        case circle_mod_mid:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);
                Circle_modified_midpoint(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);

                click_count = 0; // reset click count

            }
            break;
        }

        case fill_q1:
        {
            // assign axes to the last circle in the shapes dynamic array
            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                filling_algo(hdc, x_1, y_1, x_2, y_2,1,color);
                add_shape(alg, color);
            }

            break;
        }
        case Circle_Fill:
        {
            fillingCircleWithOtherCircles(hdc,color);
            break;
        }
        case fill_q2:
        {
            // assign axes to the last circle in the shapes dynamic array
            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                filling_algo(hdc, x_1, y_1, x_2, y_2,2,color);
                add_shape(alg, color);
            }

            break;
        }

        case fill_q3:
        {
            // assign axes to the last circle in the shapes dynamic array
            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                filling_algo(hdc, x_1, y_1, x_2, y_2, 3, color);
                add_shape(alg, color);
            }

            break;
        }

        case fill_q4:
        {
            // assign axes to the last circle in the shapes dynamic array
            bool circle_exist;
            circle_exist = get_last_circle_data();

            if (circle_exist)
            {
                filling_algo(hdc, x_1, y_1, x_2, y_2, 4, color);
                add_shape(alg, color);
            }

            break;
        }
        case fill_convex:
        {



            ConvexFill(hdc,color);
            break;

        }
        case general_fill:
        {



            GeneralPolygonFill(hdc,color);
            break;

        }

        case R_FloodFill:
        {

            FloodFill(hdc,x,y,shapes.front().color,color);
            break;

        }

        case NR_FloodFill:
        {
            NRFloodFill(hdc,x,y,shapes.front().color,color);
            break;

        }
        case Square_Fill:
        {
            fillingSquareWithHermitCurve(hdc,color);
            break;

        }
        case Rectangle_Fill:
        {
            fillingRectangleWithBezierCurve(hdc,color);
            break;

        }

        case ellipse_polar:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);
                polarEllipse(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);

                click_count = 0; // reset click count

            }
            break;
        }


        case ellipse_dir:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);
                polarEllipse(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);

                click_count = 0; // reset click count

            }
            break;
        }



        case ellipse_midpt:
        {
            if (click_count == 0)
            {
                x_1 = x;
                y_1 = y;
                add_point(x_1, y_1);
                click_count++;
            }
            else
            {
                x_2 = x;
                y_2 = y;
                add_point(x_2, y_2);
                polarEllipse(hdc,x_1,y_1,x_2,y_2,color);
                add_shape(alg, color);

                click_count = 0; // reset click count
            }
            break;
        }

        case PolygonAlg:
        {
            static Point p[4];
            static int index = 0;
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index==3)
            {
                Point T1(6* (p[1].x - p[0].x), 6* (p[1].y - p[0].y));
                Point T2(6 * (p[3].x - p[2].x), 6 * (p[3].y - p[2].y));
                hdc = GetDC(hwnd);
                polygon(hdc, p[0], T1, p[3], T2,RGB(255,0,0));
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
                index++;
            break;
        }
        case clip_Polygon:
        {
            static Point p[4];
            static int index = 0;
            static int length,width;
            static bool rectangle = false;
            static Point rectangleP;
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 2 &&!rectangle)
            {
                length = round(sqrt((double)pow(p[1].x - p[0].x, 2.0) + (double)pow(p[1].y - p[0].y, 2.0)));
                width = round(sqrt((double)pow(p[2].x - p[1].x, 2.0) + (double)pow(p[2].y - p[1].y, 2.0)));
                hdc = GetDC(hwnd);
                drawRectangle(hdc, p[0], length, width, RGB(0, 0, 0));
                ReleaseDC(hwnd, hdc);
                rectangleP = p[0];
                index = 0;
                rectangle = true;
            }
            else if (index==3)
            {
                Point T1(6* (p[1].x - p[0].x), 6* (p[1].y - p[0].y));
                Point T2(6 * (p[3].x - p[2].x), 6 * (p[3].y - p[2].y));
                hdc = GetDC(hwnd);
                polygonClipping(hdc, p[0], T1, p[3], T2, rectangleP, length, width);
                ReleaseDC(hwnd, hdc);

                index = 0;
            }
            else
                index++;
            break;
        }
        /////////
        case rPoint:
        {
            static Point p[3];
            static int index = 0;
            static int length,width;
            static bool rectangle = false;
            static Point rectangleP;
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 2 &&!rectangle)
            {
                length = round(sqrt((double)pow(p[1].x - p[0].x, 2.0) + (double)pow(p[1].y - p[0].y, 2.0)));
                width = round(sqrt((double)pow(p[2].x - p[1].x, 2.0) + (double)pow(p[2].y - p[1].y, 2.0)));
                hdc = GetDC(hwnd);
                drawRectangle(hdc, p[0], length, width, RGB(0, 0, 0));
                ReleaseDC(hwnd, hdc);
                rectangleP = p[0];
                index = 0;
                rectangle = true;
            }
            else if (rectangle)
            {

                rectangleClippingPoint(hdc, rectangleP, length, width, p[0]);
                ReleaseDC(hwnd, hdc);
                index=0;
            }
            else
                index++;
            break;
        }
        case rLine:
        {
            static Point p[3];
            static int index = 0;
            static int length,width;
            static bool rectangle = false;
            static Point rectangleP;
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 2 &&!rectangle)
            {
                length = round(sqrt((double)pow(p[1].x - p[0].x, 2.0) + (double)pow(p[1].y - p[0].y, 2.0)));
                width = round(sqrt((double)pow(p[2].x - p[1].x, 2.0) + (double)pow(p[2].y - p[1].y, 2.0)));
                hdc = GetDC(hwnd);
                drawRectangle(hdc, p[0], length, width, RGB(0, 0, 0));
                ReleaseDC(hwnd, hdc);
                rectangleP = p[0];
                index = 0;
                rectangle = true;
            }
            else if (index==1&&rectangle)
            {
                hdc = GetDC(hwnd);
                rectangleClippingLine( hdc, rectangleP,  length, width, p[0], p[1]);
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
                index++;
            break;
        }
        case cPoint:
        {
            static Point p[3];
            static int index = 0;
            static double r;
            static Point center;
            static bool circle=false;
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 1&&!circle)
            {

                r = sqrt((double)pow(p[1].x - p[0].x, 2.0) + (double)pow(p[1].y - p[0].y, 2.0));
                hdc = GetDC(hwnd);
                polarCircurcleAlg74( hdc, p[0].x,p[0].y, r, RGB(0,0,0));
                ReleaseDC(hwnd, hdc);
                center = p[0];
                index = 0;
                circle = true;
            }
            else if (circle)
            {
                hdc = GetDC(hwnd);
                circleClippingPoint( hdc, center.x, center.y,  r, p[0]);
                ReleaseDC(hwnd, hdc);

                index = 0;
            }
            else
                index++;
            break;
        }
        case cLine:
        {
            static Point p[3];
            static int index = 0;
            static double r;
            static Point center;
            static bool circle=false;
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 1&&!circle)
            {

                r = sqrt((double)pow(p[1].x - p[0].x, 2.0) + (double)pow(p[1].y - p[0].y, 2.0));
                hdc = GetDC(hwnd);
                polarCircurcleAlg74( hdc, p[0].x,p[0].y, r, RGB(0,0,0));
                ReleaseDC(hwnd, hdc);
                center = p[0];
                index = 0;
                circle = true;
            }
            else if (index==1&&circle)
            {
                hdc = GetDC(hwnd);
                circleClippingLine( hdc, center.x, center.y,  r, p[0],p[1]);
                ReleaseDC(hwnd, hdc);

                index = 0;
            }
            else
                index++;
            break;
        }
        case sPoint:
        {
            static Point p[2];
            static int index = 0;
            static int length;
            static bool square = false;
            static Point squareP;
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 1 &&!square)
            {
                length = round(sqrt((double)pow(p[1].x - p[0].x, 2.0) + (double)pow(p[1].y - p[0].y, 2.0)));
                hdc = GetDC(hwnd);
                drawSquare(hdc, p[0], length, RGB(0, 0, 0));
                ReleaseDC(hwnd, hdc);
                squareP = p[0];
                index = 0;
                square = true;
            }
            else if (square)
            {
                hdc = GetDC(hwnd);
                squareClippingPoint( hdc,squareP,length, p[0]);
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
                index++;
            break;
        }
        case sLine:
        {
            static Point p[2];
            static int index = 0;
            static int length;
            static bool square = false;
            static Point squareP;
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 1&&!square)
            {
                length = round(sqrt((double)pow(p[1].x - p[0].x, 2.0) + (double)pow(p[1].y - p[0].y, 2.0)));
                hdc = GetDC(hwnd);
                drawSquare(hdc, p[0], length, RGB(0, 0, 0));
                ReleaseDC(hwnd, hdc);
                squareP = p[0];
                index = 0;
                square = true;
            }
            else if (index==1&&square)
            {
                hdc = GetDC(hwnd);
                squareClippingLine( hdc, squareP, length, p[0], p[1]);
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
                index++;
            break;
        }

        default:
            break;
        }
    }

    case WM_COMMAND:                /* bind menus to actions */
    {

        switch (LOWORD(wParam))
        {
        /************** File sub-menus **************/
        case MI_CLEAR:
        {
            InvalidateRect(hwnd, NULL, TRUE);
            current_shape.data.clear(); // clear points
            shapes.clear(); // clear all shapes
            std::cout << "Data is cleared!\n\n";
            break;
        }

        case MI_SAVE:
        {
            std::cout << "Saving the data...\n";
            save_data();
            std::cout << "Data is saved!\n\n";
            break;
        }

        case MI_LOAD:
        {
            std::cout << "Loading the data...\n";
            load_data();
            draw_data(hdc);
            std::cout << "Data is loaded!\n\n";
            break;
        }

        /************** Color sub-menus **************/
        case MI_BLACK_COLOR:
        {
            color = BLACK;
            std::cout << "Color changed to black!\n\n";
            break;
        }
        case MI_WHITE_COLOR:
        {
            color = WHITE;
            std::cout << "Color changed to white!\n\n";
            break;
        }
        case MI_RED_COLOR:
        {
            color = RED;
            std::cout << "Color changed to red!\n\n";
            break;
        }
        case MI_ORANGE_COLOR:
        {
            color = ORANGE;
            std::cout << "Color changed to orange!\n\n";
            break;
        }
        case MI_YELLOW_COLOR:
        {
            color = YELLOW;
            std::cout << "Color changed to yellow!\n\n";
            break;
        }
        case MI_GREEN_COLOR:
        {
            color = GREEN;
            std::cout << "Color changed to green!\n\n";
            break;
        }
        case MI_BLUE_COLOR:
        {
            color = BLUE;
            std::cout << "Color changed to blue!\n\n";
            break;
        }
        case MI_INDIGO_COLOR:
        {
            color = INDIGO;
            std::cout << "Color changed to indigo!\n\n";
            break;
        }
        case MI_VIOLET_COLOR:
        {
            color = VIOLET;
            std::cout << "Color changed to violet!\n\n";
            break;
        }

        /************** Line sub-menus **************/
        case MI_LINE_DDA:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = line_DDA;
            std::cout << "algorithm changed to line DDA!\n\n";
            break;
        }

        case MI_LINE_MID:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = line_mid;
            std::cout << "algorithm changed to line midpoint!\n\n";
            break;
        }

        case MI_LINE_PARA:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = line_para;
            std::cout << "algorithm changed to line parametric!\n\n";
            break;
        }

        /************** Circle sub-menus **************/
        case MI_CIRCLE_DIRECT:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = circle_dir;
            std::cout << "algorithm changed to circle direct!\n\n";
            break;
        }

        case MI_CIRCLE_POLAR:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = circle_polar;
            std::cout << "algorithm changed to circle polar!\n\n";
            break;
        }

        case MI_CIRCLE_ITERATIVE:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = circle_iter;
            std::cout << "algorithm changed to circle iterative!\n\n";
            break;
        }

        case MI_CIRCLE_MID:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = circle_mid;
            std::cout << "algorithm changed to circle midpoint!\n\n";
            break;
        }

        case MI_CIRCLE_MOD_MID:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = circle_mod_mid;
            std::cout << "algorithm changed to circle modified midpoint!\n\n";
            break;
        }
        /************** cardinal **************/
        case MI_POLYGON:
        {
            alg = PolygonAlg;
            std::cout << "algorithm changed to cardinalSplines!\n\n";
            break;
        }

        /************** Fill sub-menus **************/
        case MI_FILL_Q1:
        {
            alg = fill_q1;
            std::cout << "algorithm changed to fill circle quarter 1!\n\n";
            break;
        }

        case MI_FILL_Q2:
        {
            alg = fill_q2;
            std::cout << "algorithm changed to fill circle quarter 2!\n\n";
            break;
        }

        case MI_FILL_Q3:
        {
            alg = fill_q3;
            std::cout << "algorithm changed to fill circle quarter 3!\n\n";
            break;
        }

        case MI_FILL_Q4:
        {
            alg = fill_q4;
            std::cout << "algorithm changed to fill circle quarter 4!\n\n";
            break;
        }

        case MI_FILL_Convex:
        {
            alg = fill_convex;
            std::cout << "algorithm changed to Convex Fill!\n\n";
            break;
        }

        case MI_FILL_General_Fill:
        {
            alg = general_fill;
            std::cout << "algorithm changed to General Fill!\n\n";
            break;
        }
        case MI_FILL_RFLOODFILL:
        {
            alg = R_FloodFill;
            std::cout << "algorithm changed to Recursive Flood Fill!\n\n";
            break;
        }
        case MI_FILL_NRFLOODFILL:
        {
            alg = NR_FloodFill;
            std::cout << "algorithm changed to Non-Recursive Flood Fill!\n\n";
            break;
        }

        case MI_FILL_Square:
        {
            alg = Square_Fill;
            std::cout << "algorithm changed to Square Fill with Hermit Curve!\n\n";
            break;
        }

        case MI_FILL_Rectangle:
        {
            alg = Rectangle_Fill;
            std::cout << "algorithm changed to Rectangle Fill with Bezier Curve!\n\n";
            break;
        }
        case MI_FILL_Circle:
        {
            alg = Circle_Fill;
            std::cout << "algorithm changed to Circle Fill with Other Circles!\n\n";
            break;
        }

        /************** Ellipse sub-menus **************/
        case MI_ELLIPSE_DIRECT:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = ellipse_dir;
            std::cout << "algorithm changed to ellipse direct!\n\n";
            break;
        }

        case MI_ELLIPSE_POLAR:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = ellipse_polar;
            std::cout << "algorithm changed to ellipse polar!\n\n";
            break;
        }


        /************** Clip sub-menus **************/

        /** Clip sub-menus **/

        case MI_CLIP_POINT_CIRCLE:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = cPoint;
            std::cout << "algorithm changed to clip point!\n\n";
            break;
        }

        case MI_CLIP_LINE_CIRCLE:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = cLine;
            std::cout << "algorithm changed to clip line!\n\n";
            break;
        }

        case   MI_CLIP_POINT_RECTANGLE:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = rPoint;
            std::cout << "algorithm changed to clip point!\n\n";
            break;
        }
        case  MI_CLIP_LINE_RECTANGLE:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = rLine;
            std::cout << "algorithm changed to clip line!\n\n";
            break;
        }

        case MI_CLIP_POLYGON_RECTANGLE:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = clip_Polygon;
            std::cout << "algorithm changed to clip polygon!\n\n";
            break;

        }
        case  MI_CLIP_LINE_SQUARE:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = sLine;
            std::cout << "algorithm changed to clip line!\n\n";
            break;
        }
        case  MI_CLIP_POINT_SQUARE:
        {
            click_count = 0;
            current_shape.data.clear(); // clear points
            alg = sPoint;
            std::cout << "algorithm changed to clip line!\n\n";
            break;
        }

        default:
            break;
        }
    }
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
