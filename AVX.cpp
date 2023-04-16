#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <immintrin.h> //AVX、AVX2、AVX-512


using namespace std;
using namespace std;
long long head, tail, freq;        // timers
const int n = 2048;
int t = 20;
int T = 1;
int nm[20] = { 10,20,30,40,50,100,150,200,250,300,400,500,600,700,800,1000,1200,1400,1700,2000 };
float m[n][n];
void m_reset(int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < i; j++)
            m[i][j] = 0;
        m[i][i] = 1.0;
        for (int j = i + 1; j < N; j++)
            m[i][j] = rand();
    }
    for (int k = 0; k < N; k++)
        for (int i = k + 1; i < N; i++)
            for (int j = 0; j < N; j++)
                m[i][j] += m[k][j];
}
void timestart()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
}
void timestop()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);	// end time
    cout << ((tail - head) * 1000.0 / freq) / 1000 << endl;


}
void normal(int N)
{
    __m256 t1, t2, t3, t4;

    for (int k = 0; k < N; k++)

    {

        float tmp[8] = { m[k][k], m[k][k], m[k][k], m[k][k],m[k][k], m[k][k], m[k][k], m[k][k] };

        t1 = _mm256_loadu_ps(tmp);
        int j;
        for (j = k + 1; j <= N - 8; j += 8) //从后向前每次取四个

        {

            t2 = _mm256_loadu_ps(m[k] + j);

            t3 = _mm256_div_ps(t2, t1);//除法

            _mm256_storeu_ps(m[k] + j, t3);

        }


        for (; j < N; j++)

        {

            m[k][j] = m[k][j] / tmp[0];

        }

        for (int i = k + 1; i < N; i++)

        {

            float tmp[8] = { m[i][k], m[i][k], m[i][k], m[i][k],m[i][k], m[i][k], m[i][k], m[i][k] };

            t1 = _mm256_loadu_ps(tmp);
            int j;
            for (j = k + 1; j <= N - 8; j += 8)

            {

                t2 = _mm256_loadu_ps(m[i] + j);

                t3 = _mm256_loadu_ps(m[k] + j);

                t4 = _mm256_sub_ps(t2, _mm256_mul_ps(t1, t3)); //减法

                _mm256_storeu_ps(m[i] + j, t4);

            }

            for (; j < N; j++)

            {

                m[i][j] = m[i][j] - m[i][k] * m[k][j];

            }

            m[i][k] = 0;

        }

    }
}
int main()
{
    for (int j = 0; j < T; j++) {
        for (int i = 0; i < t; i++) {
            m_reset(nm[i]);
            timestart();
            normal(nm[i]);
            timestop();
        }
    }
}

Gitee — 基于 Git 的代码托管和研发协作平台
