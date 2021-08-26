#include "WorkerThread.h"

/// <summary>
/// WorkerThread constructor.
/// </summary>
WorkerThread::WorkerThread()
{

}

/// <summary>
/// WorkerThread destructor.
/// </summary>
WorkerThread::~WorkerThread()
{
	
}

/// <summary>
/// Start.
/// </summary>
/// <param name="t_pixTL">Pixel top left coordinate.</param>
/// <param name="t_pixBR">Pixel top right coordinate.</param>
/// <param name="t_fracTL">Fractal top left coordinate.</param>
/// <param name="t_fracBR">Fractal top right coordinate.</param>
/// <param name="t_iterations">The number of iterations.</param>
void WorkerThread::start(const Vector2 &t_pixTL, const Vector2 &t_pixBR, const Vector2 &t_fracTL, const Vector2 &t_fracBR, const int t_iterations)
{
	m_pixTL = t_pixTL;
	m_pixBR = t_pixBR;
	m_fracTL = t_fracTL;
	m_fracBR = t_fracBR;
	m_iterations = t_iterations;
	std::unique_lock<std::mutex> f_lockMutex(m_mutex);
	m_cvStart.notify_one();
}

/// <summary>
/// Create fractal using Advanced Vector Extensions.
/// https://software.intel.com/sites/landingpage/IntrinsicsGuide/
/// </summary>
void WorkerThread::createFractal()
{
	while (m_alive)
	{
		std::unique_lock<std::mutex> f_lockMutex(m_mutex);
		m_cvStart.wait(f_lockMutex);

		double f_scaleX = (m_fracBR.x - m_fracTL.x) / (double(m_pixBR.x) - double(m_pixTL.x));
		double f_scaleY = (m_fracBR.y - m_fracTL.y) / (double(m_pixBR.y) - double(m_pixTL.y));

		double f_posY = m_fracTL.y;

		int f_x;
		int f_y;

		int f_offsetY = 0;
		int f_rowSize = m_screenWidth;

		__m256i __f_one;
		__m256d __f_two;
		__m256d __f_four;
		__m256d __f_mask1;
		__m256i __f_mask2;

		__m256d __f_A;
		__m256d __f_B;
		__m256i __f_C;
		__m256i __f_N;
		__m256d __f_ZR;
		__m256d __f_ZI;
		__m256d __f_ZR2; 
		__m256d __f_ZI2;
		__m256d __f_CR;
		__m256d __f_CI;

		__m256d __f_xPosOffsets; 
		__m256d __f_posX;
		__m256d __f_scaleX; 
		__m256d __f_jumpX;
		__m256i __f_iterations;		

		__f_one = _mm256_set1_epi64x(1);
		__f_two = _mm256_set1_pd(2.0);
		__f_four = _mm256_set1_pd(4.0);
		__f_iterations = _mm256_set1_epi64x(m_iterations);

		__f_scaleX = _mm256_set1_pd(f_scaleX);
		__f_jumpX = _mm256_set1_pd(f_scaleX * 4);
		__f_xPosOffsets = _mm256_set_pd(0, 1, 2, 3);
		__f_xPosOffsets = _mm256_mul_pd(__f_xPosOffsets, __f_scaleX);

		for (f_y = m_pixTL.y; f_y < m_pixBR.y; f_y++)
		{
			// Reset X position
			__f_A = _mm256_set1_pd(m_fracTL.x);
			__f_posX = _mm256_add_pd(__f_A, __f_xPosOffsets);

			__f_CI = _mm256_set1_pd(f_posY);

			for (f_x = m_pixTL.x; f_x < m_pixBR.x; f_x += 4)
			{
				__f_CR = __f_posX;
				__f_ZR = _mm256_setzero_pd();
				__f_ZI = _mm256_setzero_pd();
				__f_N = _mm256_setzero_si256();

			repeat:
				__f_ZR2 = _mm256_mul_pd(__f_ZR, __f_ZR);
				__f_ZI2 = _mm256_mul_pd(__f_ZI, __f_ZI);
				__f_A = _mm256_sub_pd(__f_ZR2, __f_ZI2);
				__f_A = _mm256_add_pd(__f_A, __f_CR);
				__f_B = _mm256_mul_pd(__f_ZR, __f_ZI);
				__f_B = _mm256_fmadd_pd(__f_B, __f_two, __f_CI);
				__f_ZR = __f_A;
				__f_ZI = __f_B;
				__f_A = _mm256_add_pd(__f_ZR2, __f_ZI2);
				__f_mask1 = _mm256_cmp_pd(__f_A, __f_four, _CMP_LT_OQ);
				__f_mask2 = _mm256_cmpgt_epi64(__f_iterations, __f_N);
				__f_mask2 = _mm256_and_si256(__f_mask2, _mm256_castpd_si256(__f_mask1));
				__f_C = _mm256_and_si256(__f_one, __f_mask2);											
				__f_N = _mm256_add_epi64(__f_N, __f_C);

				if (_mm256_movemask_pd(_mm256_castsi256_pd(__f_mask2)) > 0)
				{
					goto repeat;
				}					

				m_fractal[f_offsetY + f_x + 0] = int(__f_N.m256i_i64[3]);
				m_fractal[f_offsetY + f_x + 1] = int(__f_N.m256i_i64[2]);
				m_fractal[f_offsetY + f_x + 2] = int(__f_N.m256i_i64[1]);
				m_fractal[f_offsetY + f_x + 3] = int(__f_N.m256i_i64[0]);

				__f_posX = _mm256_add_pd(__f_posX, __f_jumpX);
			}

			f_posY += f_scaleY;
			f_offsetY += f_rowSize;
		}

		Globals::WORKER_COMPLETE++;
	}
}