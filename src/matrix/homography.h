/* -*-c++-*- StereoV3D - Copyright (C) 2021.
* Author	: Ethan Li<ethan.li.whu@gmail.com>
* https://github.com/ethan-li-coding/StereoV3DCode
*/

#ifndef SV3D_MATRIX_HOMOGRAPHY_H
#define SV3D_MATRIX_HOMOGRAPHY_H

#include "../eigen_defs.h"


namespace  sv3d
{

	class Homography
	{
	public:
		Homography() = default;
		~Homography() = default;
		
		/**
		 * \brief ��Ӧ�Ծ������
		 * \param p1 ��ͼ1�����ص��������
		 * \param p2 ��ͼ2�����ص��������
		 */
		void Solve(const Mat3X p1, const Mat3X p2);

		/**
		 * \brief ��ȡ��Ӧ�Ծ���
		 * \return ��Ӧ�Ծ���
		 */
		Mat3 Value();
	private:
		/**
		 * \brief �ĵ㷨��ⵥӦ�Ծ���
		 * \param p1 ��ͼ1�ϵ����ص��������
		 * \param p2 ��ͼ2�ϵ����ص��������
		 */
		void Solve_FourPoints(const Mat3X p1, const Mat3X p2);

		/* ��Ӧ�Ծ������� */
		Mat3 data_;
	};

}

#endif