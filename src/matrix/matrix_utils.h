#ifndef SV3D_MATRIX_UTILS_H
#define SV3D_MATRIX_UTILS_H

#include "../eigen_defs.h"
#include <vector>

namespace sv3d
{
	/**
	 * \brief �ӱ��ʾ������R,t
	 * \param[in] E		���ʾ���
	 * \param[out] R_vec R����Ŀ���ֵ����
	 * \param[out] t_vec t����Ŀ���ֵ����
	 */
	void SolveRtFromEssential(const Mat3& E, std::vector<Mat3>& R_vec, std::vector<Vec3>& t_vec);

	/**
	 * \brief ˫Ŀǰ�����������������ϵ�µĿռ������
	 * \param[in] p1 ͬ����������ͼ����������
	 * \param[in] K1 ����ͼ�ڲξ���
	 * \param[in] R1 ����ͼ���R����
	 * \param[in] t1 ����ͼ���t����
	 * \param[in] p2 ͬ����������ͼ����������
	 * \param[in] K2 ����ͼ�ڲξ���
	 * \param[in] R2 ����ͼ���R����
	 * \param[in] t2 ����ͼ���t����
	 * \param[out] X �ռ������
	 * return true: �ռ����˫���ǰ�� 
	 */
	bool Triangulate2View(const Vec3& p1, const RMat3& K1, const Mat3& R1, const Vec3& t1,
						  const Vec3& p2, const RMat3& K2, const Mat3& R2, const Vec3& t2,
						  Vec3& X);
}

#endif