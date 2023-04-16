#include <stdio.h>
#include <time.h>
#include <iostream>
#include<ctime>
#include<arm_neon.h>
using namespace std;
float32x4_t test1;
struct timespec sts,ets;
const int n=2048;
float m[n][n];
const int t=20;
const int T=10;
int gm[t]{10,20,30,40,50,100,150,200,250,300,400,500,600,700,800,1000,1200,1400,1700,2000};
void m_reset()
{
         for (int i = 0; i < n; i++)
                 {  for (int j = 0; j < i; j++)
                 m[i][j] = 0;
         m[i][i] = 1.0;
         for (int j = i + 1; j < n; j++)
                 m[i][j] = rand();
         }
         for (int k = 0; k < n; k++)
                 for (int i = k + 1; i < n; i++)
                 for (int j = 0; j < n; j++)
                m[i][j] += m[k][j];
         }
void timestart()
{
timespec_get(&sts, TIME_UTC);
}
void timestop()
{
 timespec_get(&ets, TIME_UTC);
 time_t dsec=ets.tv_sec-sts.tv_sec;
long dnsec=ets.tv_nsec-sts.tv_nsec;
if(dnsec<0)
{
dsec--;
dnsec+=1000000000ll;
}
 printf ("%llu.%09llu \n",dsec,dnsec);

}
void normal(int N)
{               int k;
        for ( k = 0; k < N; k++)
        {
                                for (int j = k + 1; j < N; j++)
                {
                        m[k][j] = m[k][j] / m[k][k];

                }
                m[k][k] = 0;


///////////////////////////////////////////////////////////////

                for (int i = k + 1; i < N; i++)
                {int j;
                        int off=((unsigned long int)(&m[i][k+1]))%16/4;
                        for(j=k+1;j<k+1+off&&j<N;j++)
                        {
                        m[i][j]-=m[i][k]*m[k][j];
                        }
                        for (; j <= N-4; j+=4)
                        {       test1=vld1q_f32((&m[i][j]));
                                float32x4_t temp1=vmovq_n_f32(m[i][k]);
                                float32x4_t temp2=vld1q_f32(&(m[k][j]));
                                temp1=vmulq_f32(temp1,temp2);
                                test1=vsubq_f32(test1,temp1);


                        }
                        for(;j<N;j++)
                                {
                                m[i][j]-=m[i][k]*m[k][j];
                                }
                        m[i][k] = 0;
                }

        }
}
int main()
{for(int j=0;j<T;j++){
for(int i=0;i<t;i++){
        m_reset();
        timestart();
        normal(gm[i]);
        timestop();}
cout<<endl<<endl<<endl;

}
}