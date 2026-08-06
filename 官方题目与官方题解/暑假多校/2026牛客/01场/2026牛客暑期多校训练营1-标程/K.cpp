#include<bits/stdc++.h>
using namespace std;
typedef double db;
const db eps=1e-9;
int sgn(db x)
{
    if(x>eps)return 1;
    if(x<-eps)return -1;
    return 0;
}
struct Point
{
    db x,y;
    Point():x(0),y(0) {}
    Point(db _x,db _y):x(_x),y(_y) {}
    Point operator + (const Point& t)const
    {
        return Point(x+t.x,y+t.y);
    }
    Point operator - (const Point& t)const
    {
        return Point(x-t.x,y-t.y);
    }
    Point operator * (const db& t)const
    {
        return Point(x*t,y*t);
    }
    Point operator / (const db& t)const
    {
        return Point(x/t,y/t);
    }
    db operator * (const Point& t)const
    {
        return x*t.y-y*t.x;
    }
    db operator | (const Point& t)const
    {
        return x*t.x+y*t.y;
    }
    db len()const
    {
        return sqrt(x*x+y*y);
    }
    Point unit()const
    {
        return *this/len();
    }
    bool operator < (const Point &t)const
    {
        return sgn(y-t.y)==0 ? sgn(x-t.x)<0 : y<t.y;
    }
    bool operator == (const Point &t)const
    {
        return !(*this<t) && !(t<*this);
    }
    void read()
    {
        int tx,ty;
        scanf("%d%d",&tx,&ty);
        x=tx,y=ty;
    }
};
bool cmp(const Point& p,const Point& q)
{
    bool up[2]={sgn(p.y)>0 || (sgn(p.y)==0 && sgn(p.x)>0),
                sgn(q.y)>0 || (sgn(q.y)==0 && sgn(q.x)>0)};
    if(up[0]^up[1])return up[0];
    return sgn(p*q)>0;
}
struct Line
{
    Point s,e;
    Line() {}
    Line(const Point& _s,const Point& _e):s(_s),e(_e) {}
    pair<bool,Point> operator & (const Line &t)const
    {
        db c=(e-s)*(t.e-t.s);
        if(sgn(c)==0)return {0,{}};
        return {1,s+(e-s)*((t.s-s)*(t.e-t.s)/c)};
    }
};
bool equ(const Line& s,const Line& t)
{
    return sgn((s.e-s.s)*(t.e-t.s))==0 && sgn((t.s-s.s)*(s.e-s.s))==0;
}
bool point_strictly_in_segment(const Point& p,const Line& t)
{
    return sgn((p-t.s)*(t.s-t.e))==0 && sgn((p-t.s)|(t.e-t.s))>0 && sgn((p-t.e)|(t.s-t.e))>0;
}
bool segment_strictly_intersect(const Line& a,const Line& b)
{
    return sgn((b.s-a.s)*(a.e-a.s))*sgn((b.e-a.s)*(a.e-a.s))<0
            && sgn((a.s-b.s)*(b.e-b.s))*sgn((a.e-b.s)*(b.e-b.s))<0;
}
int point_in_polygon(const Point& p,const vector<Point>& poly)
{
    int n=poly.size(),cnt=0;
    for(int i=0; i<n; i++)
    {
        if(p==poly[i])return 0;
        Line t=Line(poly[i],poly[(i+1)%n]);
        if(point_strictly_in_segment(p,t))return 0;
        if(sgn(t.s.y-t.e.y)==0)continue;
        if(sgn(t.s.y-t.e.y)>0)swap(t.s,t.e);
        if(sgn(t.s.y-p.y)*sgn(t.e.y-p.y)<0 && sgn((p-t.s)*(t.e-t.s))<0)cnt++;
        else if(sgn(t.s.y-p.y)==0 && sgn(t.s.x-p.x)>0)cnt++;
    }
    return (cnt%2==0 ? 1 : -1);
}
int point_in_convex(const Point& p,const vector<Point>& poly)
{
    int n=poly.size();
    bool on_edge=false;
    for(int i=0;i<n;i++)
    {
        Point a=poly[i], b=poly[(i+1)%n];
        if(p==a) return 0;
        db c=(b-a)*(p-a);
        if(sgn(c)<0) return 1;
        if(sgn(c)==0 && sgn((p-a)|(b-a))>=0 && sgn((p-b)|(a-b))>=0)
            on_edge=true;
    }
    return on_edge ? 0 : -1;
}
vector<vector<Point>> triangulation(vector<Point> p)
{
    auto is_ear=[&](const Line& cut)
    {
        int n=p.size();
        for(int i=0; i<n; i++)
            if(point_strictly_in_segment(p[i],cut))
                return false;
        for(int i=0; i<n; i++)
            if(segment_strictly_intersect(Line(p[i],p[(i+1)%n]),cut))
                return false;
        return point_in_polygon((cut.s+cut.e)/2,p)<0;
    };
    vector<vector<Point>> res;
    while((int)p.size()>3)
    {
        int n=p.size();
        for(int i=0; i<n; i++)
            if(is_ear(Line(p[i],p[(i+2)%n])))
            {
                res.push_back({p[i],p[(i+1)%n],p[(i+2)%n]});
                p.erase(p.begin()+(i+1)%n);
                break;
            }
    }
    res.push_back({p[0],p[1],p[2]});
    return res;
}
vector<Point> convex_convolution(vector<Point> p,vector<Point> q)
{
    rotate(p.begin(),min_element(p.begin(),p.end()),p.end());
    rotate(q.begin(),min_element(q.begin(),q.end()),q.end());
    vector<Point> r;
    for(int i=0;i<(int)p.size();i++)
        r.push_back(p[(i+1)%p.size()]-p[i]);
    for(int i=0;i<(int)q.size();i++)
        r.push_back(q[(i+1)%q.size()]-q[i]);
    inplace_merge(r.begin(),r.begin()+p.size(),r.end(),cmp);
    r.insert(r.begin(),p[0]+q[0]);
    r.pop_back();
    for(int i=1;i<(int)r.size();i++)
        r[i]=r[i]+r[i-1];
    return r;
}
int solve()
{
    int n,m;
    scanf("%d%d",&n,&m);
    vector<Point> p(n),q(m);
    for(int i=0; i<n; i++)
        p[i].read();
    for(int i=0; i<m; i++)
        q[i].read();
    auto tp=triangulation(p);
    auto tq=triangulation(q);
    vector<vector<Point>> poly;
    for(auto& sp : tp)
        for(auto& sq : tq)
            poly.push_back(convex_convolution(sp,sq));
    db res=0;
    for(int i=0;i<(int)poly.size();i++)
        for(int x=0;x<(int)poly[i].size();x++)
        {
            Line ei(poly[i][x],poly[i][(x+1)%poly[i].size()]);
            vector<pair<Point,int>> events;
            for(int j=0;j<(int)poly.size();j++)
            {
                if(j==i)continue;
                vector<Point> cut{ei.s,ei.e};
                for(int y=0;y<(int)poly[j].size();y++)
                {
                    Line ej(poly[j][y],poly[j][(y+1)%poly[j].size()]);
                    auto [t,p]=ei&ej;
                    if(t && point_strictly_in_segment(p,ei) && (p==ej.s || p==ej.e || point_strictly_in_segment(p,ej)))
                        cut.push_back(p);
                }
                sort(cut.begin(),cut.end(),[&](const Point& lhs,const Point& rhs)
                {
                    return ((lhs-ei.s)|(ei.e-ei.s))<((rhs-ei.s)|(ei.e-ei.s));
                });
                cut.erase(unique(cut.begin(),cut.end()),cut.end());
                for(size_t k=0;k+1<cut.size();k++)
                {
                    int t=point_in_convex((cut[k]+cut[k+1])/2,poly[j]);
                    if(t<0)
                    {
                        events.emplace_back(cut[k],-1);
                        events.emplace_back(cut[k+1],1);
                    }
                    else if(t==0 && j<i)
                        for(int y=0;y<(int)poly[j].size();y++)
                        {
                            Line ej(poly[j][y],poly[j][(y+1)%poly[j].size()]);
                            if(equ(ei,ej) && sgn((ei.e-ei.s)|(ej.e-ej.s))>0)
                            {
                                events.emplace_back(cut[k],-1);
                                events.emplace_back(cut[k+1],1);
                                break;
                            }
                        }
                }
            }
            sort(events.begin(),events.end(),[&](const pair<Point,int>& lhs,const pair<Point,int>& rhs)
            {
                return ((lhs.first-ei.s)|(ei.e-ei.s))<((rhs.first-ei.s)|(ei.e-ei.s));
            });
            res+=ei.s*ei.e/2;
            Point las=ei.s;
            for(int j=0,cur=0;j<(int)events.size();j++)
            {
                if(cur>0)res-=las*events[j].first/2;
                cur-=events[j].second;
                las=events[j].first;
            }
        }
    return 0*printf("%.12f\n",res);
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--)solve();
    return 0;
}

