//OSHER THE KING//
#include <iostream>
#include "anomaly_detection_util.h"
// Yossi the lion.
using namespace std;

bool wrong(float val, float expected){
	return val<expected-0.001 || val>expected+0.001;
}

// this is a simple test to put you on the right track
int main(){
	const int N=10;
	float x[]={1,2,3,4,5,6,7,8,9,10};
	float y[]={2.1,4.2,6.1,8.1,10.3,12.2,14.4,16.1,18.2,20.3};

	Point* ps[N];
	for(int i=0;i<N;i++)
		ps[i]=new Point(x[i],y[i]);

	Line l=linear_reg(ps,N);
	Point p(4,8);

	float v[]={var(x,N),cov(x,y,N),pearson(x,y,N),l.a,l.b,l.f(4),dev(p,l)};
	float e[]={8.25,16.63,0.999,2.015,0.113,8.176,0.176};


	for(int i=0;i<7;i++)
		if(wrong(v[i],e[i]))
			cout<<"error for check "<<i<<" (-14)"<<endl;


	for(int i=0;i<N;i++)
		delete ps[i];

	cout<<"done"<<endl;
	return 0;
}
