#ifndef SV3D_RELATIVE_ORIENTATION_MATRIX_H
#define SV3D_RELATIVE_ORIENTATION_MATRIX_H

#include "../eigen_defs.h"

namespace sv3d {
	
	/**
	 * \brief �ӱ��ʾ���ָ���Ե����R/t���������ǰ��������Ľ�
	 * \param p1[in]		ͬ�����������ͼ�ĵ㼯 
	 * \param p2[in]		ͬ�����������ͼ�ĵ㼯
	 * \param k1_mat[in]	����ͼ�ڲξ���
	 * \param k2_mat[in]	����ͼ�ڲξ���
	 * \param E[in]			���ʾ���
	 * \param R[out]		R����
	 * \param t[out]		tʸ�� 
	 * \param uniqueness_ratio[in]	Ψһ�Ա���������ѽ����ѽ����ǰ�ĵ���������ֵ��С����ֵ�򷵻�false��
	 * \return true:���ɹ� false:���ʧ��
	 */
	bool OrientationFormEssential(const Mat3X & p1, const Mat3X & p2,
		const RMat3& k1_mat, const RMat3& k2_mat,
		const Mat3& E,
		Mat3& R, Vec3& t,
		const double uniqueness_ratio = 0.7);
}

#endif